#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "bdi_agent.h"
#include "graphsearch.h"
#include "communication.h"

using namespace std;

vector<umap_t> BdiAgent::world;
int BdiAgent::current_time = 0;
pthread_mutex_t BdiAgent::world_mtx = PTHREAD_MUTEX_INITIALIZER;
vector<bool> BdiAgent::conflicts;
bool BdiAgent::no_more_conflicts = false;
pthread_mutex_t BdiAgent::conflicts_mtx = PTHREAD_MUTEX_INITIALIZER;
vector<bool> BdiAgent::finished;
pthread_mutex_t BdiAgent::finished_mtx = PTHREAD_MUTEX_INITIALIZER;


BdiAgent::BdiAgent(int agent_id)
{
	this->time = 0;
	this->agent_id = agent_id;
}

goal_t BdiAgent::get_next_goal(umap_t believes) {

	// Get agent position
	coordinates_t agent_coord;
	for (auto obj_it : believes) {
		if (obj_it.second - '0' == this->agent_id) {
			agent_coord = obj_it.first;
			break;
		}
	}

	// Check if agent is next to a box
	bool agent_adj = false;
	int row_delta[4] = {-1, 0, 0, 1};
	int col_delta[4] = {0, -1, 1, 0};
	for (int i = 0; i < 4; i++) {
		coordinates_t adj_coord = {
			agent_coord.x + row_delta[i],
			agent_coord.y + col_delta[i],
		};

		if (believes.count(adj_coord)) {
			char box = believes[adj_coord];
			if (get_color(box) == get_color(this->agent_id) && is_box(box)) {
				coordinates_t goal_coord;
				bool found = false;
				for (auto goal_it : goals_map) {
					if (goal_it.second == box) {
						goal_coord = goal_it.first;
						found = true;
						break;
					}
				}
				if (found && (adj_coord.x != goal_coord.x
							|| adj_coord.y != goal_coord.y)) {
					return (goal_t) {
						CARRY_BOX_TO_GOAL,
						goal_coord.x,
						goal_coord.y
					};
				}
			}
		}
	}

	// Look for boxes not yet in their goals
	goal_t agent_goal = {.type = NO_GOAL};
	for (auto goal_it : goals_map) {
		coordinates_t goal_coord = goal_it.first;
		char goal_obj = goal_it.second;

		// Skip any goal of other color
		if (get_color(goal_obj) != get_color(this->agent_id)) {
			continue;
		}

		// Save agent goal, in case no box is left
		if (goal_obj - '0' == this->agent_id
				&& (goal_coord.x != agent_coord.x
					|| goal_coord.y != agent_coord.y)) {
			agent_goal = {AGENT_GOAL, goal_coord.x, goal_coord.y};
			continue;
		}

		// Skip agent goals
		if (goal_obj < 'A' || goal_obj > 'Z') {
			continue;
		}

		// Skip if box is already in goal
		umap_t::iterator box_it = believes.find(goal_coord);
		if (box_it != believes.end() && box_it->second == goal_obj) {
			continue;
		}

		for (auto box_it : believes) {
			if (box_it.second == goal_obj) {
				for (int i = 0; i < 4; i++) {
					int adj_row = box_it.first.x + row_delta[i];
					int adj_col = box_it.first.y + col_delta[i];
					if (!walls[adj_row][adj_col]) {
						return (goal_t) {
							FIND_BOX,
							adj_row,
							adj_col,
						};
					}
				}
			}
		}
	}
	return agent_goal;
}

AgentState* BdiAgent::intention_to_state(umap_t believes, goal_t intention)
{
	int agent_row, agent_col;
	vector<vector<char>> boxes(n_rows, vector<char>(n_cols, ' '));
	vector<vector<char>> goal(n_rows, vector<char>(n_cols, ' '));

	for (auto& it : believes) {
		coordinates_t c = it.first;
		char obj = believes[c];
		if (obj - '0' == this->agent_id) {
			agent_row = c.x;
			agent_col = c.y;
		} else if (is_box(obj) && get_color(this->agent_id) == get_color(obj)) {
			boxes[c.x][c.y] = obj;
			if (goals_map.count(it.first) && goals_map[it.first] == obj) {
				goal[it.first.x][it.first.y] = obj;
			}
		}
	}

	if (intention.type == AGENT_GOAL) {
		goal[intention.row][intention.col] = (char)(this->agent_id) + '0';
	} else if (intention.type == CARRY_BOX_TO_GOAL) {
		goal[intention.row][intention.col] = goals_map[{intention.row, intention.col}];
	} else if (intention.type == FIND_BOX) {
		goal[intention.row][intention.col] = this->agent_id + '0';
	} else {
		cerr << "Not yet implented!!!" << endl;
	}

	return new AgentState(this->agent_id, agent_row, agent_col, boxes, goal);
}


void select_no_conflict_state(vector<CAction> a1, vector<CAction> a2, int* a1_inc, int* a2_inc)
{
	for (int i = 0; i < a1.size(); i++) {
		for (int j = 0; j < a2.size() && j <= i; j++) {
			if (!a1[i].conflicts_goal(a2[j])) {
				*a1_inc = i;
				*a2_inc = j;
				return;
			}
		}
	}
}

ConflictState* BdiAgent::conflict_to_state(umap_t believes, char other_id,
		CAction action, CAction other_action) {
	vector<int> agent_ids(2);
	vector<int> agent_rows(2);
	vector<int> agent_cols(2);
	vector<vector<char>> boxes(n_rows, vector<char>(n_cols, ' '));
	vector<vector<char>> goals(n_rows, vector<char>(n_cols, ' '));

	agent_ids.push_back(this->agent_id);
	agent_ids.push_back(other_id);

	for (auto& it : believes) {
		coordinates_t c = it.first;
		char obj = believes[c];
		if (int(obj) - int('0') == this->agent_id) {
			agent_rows[0] = c.x;
			agent_cols[0] = c.y;
		} else if (int(obj) - int('0') == int(other_id)) {
			agent_rows[1] = c.x;
			agent_cols[1] = c.y;
		} else if (is_box(obj) && (get_color(this->agent_id) == get_color(obj)
					|| get_color(other_id) == get_color(obj))) {
			boxes[c.x][c.y] = obj;
			// why do we need al the other goals in this ConflictState? Is it no sufficient to have the new goals that we find later?
			// It will be easier for the search i think
			if (goals_map.count(it.first) && goals_map[it.first] == obj) {
				goals[it.first.x][it.first.y] = obj;
			}
		}
	}

	coordinates_t a1 = action.agent_final;
	goals[a1.x][a1.y] = this->agent_id + '0';
	if (action.type == ActionType::PUSH
			|| action.type == ActionType::PULL) {
		coordinates_t b1 = action.box_final;
		goals[b1.x][b1.y] = action.box;
	}
	coordinates_t a2 = other_action.agent_final;
	goals[a2.x][a2.y] = other_id;
	if (other_action.type == ActionType::PUSH
			|| other_action.type == ActionType::PULL) {
		coordinates_t b2 = other_action.box_final;
		goals[b2.x][b2.y] = other_action.box;
	}

	return new ConflictState(agent_ids, agent_rows, agent_cols, boxes, goals);
}


/** Conflicts:
 * AGent vs Agent: compute new plan
 * Agent vs box: ask agent to move it
 * Agents of same color: Inform other agents of our intentions
 * Deadlocks
 */
void BdiAgent::run()
{
	umap_t believes;
	goal_t intention;

	while (true) {
		believes = this->get_current_map();
		intention = this->get_next_goal(believes);
		cerr << "Next goal " << intention.type << " " << intention.row << " " << intention.col << endl;

		vector<CAction> plan;
		if (intention.type == NO_GOAL) {
			//this->world.finished(this->agent_id);
			plan.push_back(CAction(actions[0], final_plan.back().agent_final));
			//return;
		} else {
			AgentState* state = this->intention_to_state(believes, intention);
			cerr << state->repr();
			plan = search(state);
			cerr << "Plan result size: " << plan.size() << endl;
		}


		for (int i = 0; i < plan.size();) {
			CAction next_action = plan[i];
			this->time++;
			fprintf(stderr, "Next %d action(%d): %s\n", agent_id, time,
				next_action.name.c_str());
			//believes = this->get_current_map();

			msg_t msg;
			msg.agent_id = this->agent_id;
			msg.type = MSG_TYPE_NEXT_ACTION;
			msg.next_action = next_action;
			broadcast_msg(msg);
			
			vector<bool> conflicts_with_other(n_agents, true);
			conflicts_with_other[this->agent_id] = false; // CHECK boxes
			vector<bool> finished(n_agents, false);
			vector<bool> noop_ops(n_agents, false);
			if (next_action.type == ActionType::NOOP)
				noop_ops[this->agent_id] = true;

			bool comm_loop = true;
			while (comm_loop) { // Communication loop
				if (get_msg(this->agent_id, &msg)) {
					int sender = msg.agent_id;
					//cerr << "Agent " << this->agent_id << ": MSG (" << sender << ") type " << msg.type << endl;

					switch (msg.type) {

					case MSG_TYPE_NEXT_ACTION:
						if (next_action.conflicts(msg.next_action)) {
							cerr << "CONFLICT!!!!! " << sender;
							vector<CAction> next_actions;
							for (int j = i; j < plan.size() && j < i+3; j++) {
								next_actions.push_back(plan[j]);
							}
							msg.agent_id = this->agent_id;
							msg.type = MSG_TYPE_CONFLICT_AGENTS;
							msg.conflict = {
									.goal_type = intention.type,
									.next_actions = next_actions,
								};
							send_msg_to_agent(sender, msg);
						} else {
							if (msg.next_action.type == ActionType::NOOP) {
								noop_ops[sender] = true;
								bool exit_aux = true;
								for (bool noop : noop_ops) {
									if (!noop) {
										exit_aux = false;
									}
								}
								if (exit_aux) {
									return;
								}
							} else {
								noop_ops[sender] = false;
							}

							conflicts_with_other[sender] = false;
							for (bool c : conflicts_with_other) {
								if (c)
									break;
							}
							finished[this->agent_id] = true;
							msg.agent_id = this->agent_id;
							msg.type = MSG_TYPE_STEP_FINISHED;
							broadcast_msg(msg);
						}
						break;

					case MSG_TYPE_STEP_FINISHED:
						finished[sender] = true;
						break;

					case MSG_TYPE_CONFLICT_AGENTS:
						if (this->agent_id < sender) {
							// Resolve conflict
							vector<CAction> next_actions;
							for (int j = i; j < plan.size() && j < i+3; j++) {
								next_actions.push_back(plan[j]);
							}
							vector<CAction> other_actions = msg.conflict.next_actions;

							int a1_skip, a2_skip;
							select_no_conflict_state(next_actions, other_actions, &a1_skip, &a2_skip);

							ConflictState* conflict_state = conflict_to_state(believes, sender,
								next_actions[a1_skip], other_actions[a2_skip]);
							cerr << conflict_state->repr();
							vector<vector<CAction>> conflict_plan = conflict_search(conflict_state);

							msg.agent_id = this->agent_id;
							msg.type = MSG_TYPE_CONFLICT_AGENTS_RESOLVED;
							msg.conflict_resolved = {
								.skip = a2_skip,
								.new_actions = conflict_plan[1],
							};
							send_msg_to_agent(sender, msg);
							plan.erase(plan.begin()+i, plan.begin()+i+a1_skip);
							plan.insert(plan.begin()+i, conflict_plan[0].begin(), conflict_plan[0].end());
							
							msg.type = MSG_TYPE_CHECK_AGAIN;
							broadcast_msg(msg);
							conflicts_with_other = vector<bool>(n_agents, true);
							conflicts_with_other[this->agent_id] = false;
							finished = vector<bool>(n_agents, false);

							msg.type = MSG_TYPE_NEXT_ACTION;
							msg.next_action = next_action;
							broadcast_msg(msg);
						}
						break;

					case MSG_TYPE_CHECK_AGAIN:
						conflicts_with_other = vector<bool>(n_agents, true);
						conflicts_with_other[this->agent_id] = false;
						finished = vector<bool>(n_agents, false);
						break;
					}
				} else {
					sleep(0.3);
					comm_loop = false;
					for (bool f : finished) {
						if (!f) {
							comm_loop = true;
							break;
						}
					}
					//cerr << "Agent " << agent_id << " loop: " << comm_loop << endl;
				}
			} // Communication loop

			update_position(next_action);
			i++;
			this->final_plan.push_back(next_action);

			//if (world.update_position(agent_id, time, next_action, state)) {
			//	i++;
			//	this->final_plan.push_back(next_action);
			//	state = state->apply_action(next_action);
			//	//cerr << state->repr();
			//}
			//else {
			//	cerr << "Conflict!!!" << endl;
			//	//assert(0);
			//	this->final_plan.push_back(actions[0]);
			//}
		}
	}
}

umap_t BdiAgent::get_current_map()
{
	umap_t result;
	assert(!pthread_mutex_lock(&world_mtx));
	result = world[current_time];
	assert(!pthread_mutex_unlock(&world_mtx));
	return result;
}

// bool BdiAgent::check_conflict(CAction next_action)
// {
// 	vector<CAction> actions;
// 	assert(!pthread_mutex_lock(&next_actions_mtx));
// 	actions = next_actions;
// 	assert(!pthread_mutex_unlock(&next_actions_mtx));
//
// 	for (int i = 0; i < actions.size(); i++) {
// 		if (i == this->agent_id)
// 			continue;
//
// 		if (next_action.conflicts(actions[i])) {
// 			assert(!pthread_mutex_lock(&conflicts_mtx));
// 			conflicts[this->agent_id] = true;
// 			assert(!pthread_mutex_unlock(&conflicts_mtx));
// 			return true;
// 		}
// 	}
// 	assert(!pthread_mutex_lock(&conflicts_mtx));
// 	if (conflicts[this->agent_id]) {
// 		conflicts[this->agent_id] = false;
// 	}
// 	no_more_conflicts = true;
// 	for(bool conflict : conflicts) {
// 		if (conflict) {
// 			no_more_conflicts = false;
// 			break;
// 		}
// 	}
// 	assert(!pthread_mutex_unlock(&conflicts_mtx));
// 	return false;
// }

void BdiAgent::update_position(CAction action)
{
	assert(!pthread_mutex_lock(&world_mtx));
	if (current_time == world.size()) {
		world.push_back(world.back());
	}
	assert(!pthread_mutex_unlock(&world_mtx));

	if (action.type == ActionType::MOVE) {
		assert(!pthread_mutex_lock(&world_mtx));
		char agent = world[current_time][action.agent_pos];
		world[current_time].erase(action.agent_pos);
		world[current_time][action.agent_final] = agent;
		assert(!pthread_mutex_unlock(&world_mtx));

	} else if (action.type == ActionType::PUSH) {
		assert(!pthread_mutex_lock(&world_mtx));
		char box = world[current_time][action.box_pos];
		world[current_time][action.box_final] = box;
		char agent = world[current_time][action.agent_pos];
		world[current_time][action.box_pos] = agent;
		world[current_time].erase(action.agent_pos);
		assert(!pthread_mutex_unlock(&world_mtx));

	} else if (action.type == ActionType::PULL) {
		assert(!pthread_mutex_lock(&world_mtx));
		char agent = world[current_time][action.agent_pos];
		world[current_time][action.agent_final] = agent;
		char box = world[current_time][action.box_pos];
		world[current_time][action.agent_pos] = box;
		world[current_time].erase(action.box_pos);
		assert(!pthread_mutex_unlock(&world_mtx));
	}
}
