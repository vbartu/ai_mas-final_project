#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <deque>

#include "bdi_agent.h"
#include "graphsearch.h"
#include "communication.h"

using namespace std;

vector<umap_t> BdiAgent::world;
int BdiAgent::current_time = 0;
pthread_mutex_t BdiAgent::world_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t BdiAgent::world_cond = PTHREAD_COND_INITIALIZER;
static vector<bool> advance_control(10, false);


BdiAgent::BdiAgent(int agent_id)
{
	this->time = 0;
	this->agent_id = agent_id;
	this->waiting_for_agent = false;
}

coordinates_t get_nearest_adjacent(umap_t believes, coordinates_t box_pos, coordinates_t agent_pos)
{
	int row_delta[4] = {-1, 0, 0, 1};
	int col_delta[4] = {0, -1, 1, 0};
	coordinates_t result = {-1, -1};
	int dist = 10000;
	for (int i = 0; i < 4; i++) {
		coordinates_t adj_pos = {
			box_pos.x + row_delta[i],
			box_pos.y + col_delta[i]
		};
		if (walls[adj_pos.x][adj_pos.y] || (believes.count(adj_pos) && is_box(believes[adj_pos]))) {
			continue;
		} else {
			if (distance_map[adj_pos][agent_pos] < dist) {
				result = adj_pos;
				dist = distance_map[adj_pos][agent_pos];
			}
		}
	}
	return result;
}

vector<coordinates_t> find_neighbours(coordinates_t pos)
{
	int row_delta[4] = {-1, 0, 0, 1};
	int col_delta[4] = {0, -1, 1, 0};
	vector<coordinates_t> result;
	for (int i = 0; i < 4; i++) {
		coordinates_t ne = {pos.x + row_delta[i], pos.y + col_delta[i]};
		if (!(ne.x < 0) && !(ne.y < 0) && !(ne.x >= n_rows) && !(ne.y >= n_cols)
				&& !walls[ne.x][ne.y]) {
			result.push_back(ne);
		}
	}
	return result;
}

coordinates_t BdiAgent::nearest_help_goal_cell(umap_t believes, coordinates_t box_pos,
	vector<CAction> other_actions)
{
	vector<coordinates_t> neighbours = find_neighbours(box_pos);
	for (int j = 0; j < neighbours.size(); j++)
	{
		bool cell_free = true;
		for (int i = 0; i < other_actions.size(); i++) {
			if (believes.count(neighbours[j])
						|| equal(neighbours[j], box_pos)
						|| equal(neighbours[j], other_actions[i].agent_pos)
						|| equal(neighbours[j], other_actions[i].box_pos)
						|| equal(neighbours[j], other_actions[i].agent_final)
						|| equal(neighbours[j], other_actions[i].box_final)) {
				cell_free = false;
				break;
			}
		}

		if (cell_free)
			return neighbours[j];

		vector<coordinates_t> neigh_children = find_neighbours(neighbours[j]);
		for (int k = 0; k < neigh_children.size(); k++) {
			bool already_stored = false;
			for (coordinates_t old_ne :neighbours) {
				if (neigh_children[k].x == old_ne.x && neigh_children[k].y == old_ne.y) {
					already_stored = true;
					break;
				}
			}
			if (!already_stored) {
				neighbours.push_back(neigh_children[k]);
			}
		}
	}
	return (coordinates_t) {-1,-1};
}

goal_t BdiAgent::get_next_goal(umap_t believes)
{

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
					return (goal_t) {CARRY_BOX_TO_GOAL, goal_coord};
				}
			}
		}
	}

// Look for boxes not yet in their goals
	goal_t agent_goal = {.type = NO_GOAL, agent_coord};
	int dist = 10000;

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
			agent_goal = {AGENT_GOAL, goal_coord};
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
				int d = distance_map[box_it.first][agent_coord];
				if (d < dist) {
					dist = d;
					coordinates_t adj_pos = get_nearest_adjacent(believes, box_it.first, agent_coord);
					agent_goal = {FIND_BOX, adj_pos};
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
		} else if (is_box(obj)) {
			boxes[c.x][c.y] = obj;
			if (goals_map.count(it.first) && goals_map[it.first] == obj) {
				goal[it.first.x][it.first.y] = obj;
			}
		}

	}

	if (intention.type == AGENT_GOAL) {
		goal[intention.pos.x][intention.pos.y] = (char)(this->agent_id) + '0';
	} else if (intention.type == CARRY_BOX_TO_GOAL) {
		goal[intention.pos.x][intention.pos.y] = goals_map[{intention.pos.x, intention.pos.y}];
	} else if (intention.type == HELP_MOVE_BOX) {
		goal[intention.pos.x][intention.pos.y] = intention.helping_box;
	} else if (intention.type == FIND_BOX) {
		goal[intention.pos.x][intention.pos.y] = this->agent_id + '0';
	} else {
		cerr << "Not yet implented!!!" << endl;
	}

	return new AgentState(this->agent_id, agent_row, agent_col, boxes, goal, true);
}

conflict_t solve_conflict(umap_t believes, vector<int> agent_ids, vector<vector<CAction>> next_actions, vector<goal_t> intentions)
{
	// Add agents
	vector<int> agent_rows, agent_cols;
	agent_rows.push_back(next_actions[0][0].agent_pos.x);
	agent_cols.push_back(next_actions[0][0].agent_pos.y);
	agent_rows.push_back(next_actions[1][0].agent_pos.x);
	agent_cols.push_back(next_actions[1][0].agent_pos.y);
	// Add boxes (and their goals if they are on them)
	vector<vector<char>> boxes(n_rows, vector<char>(n_cols, ' '));
	vector<vector<char>> conflict_goals(n_rows, vector<char>(n_cols, ' '));
	for (auto& it : believes) {
		if (is_box(it.second)) {
			boxes[it.first.x][it.first.y] = it.second;
			if (goals_map.count(it.first) && goals_map[it.first] == it.second) {
				conflict_goals[it.first.x][it.first.y] = it.second;
			}
		}
	}

	// Add goals
	vector<int> skip(next_actions.size(), 0);
	int aux_i = (next_actions[0].size() > 3) ? 3 : next_actions[0].size()-1;
	int aux_j = (next_actions[1].size() > 3) ? 3 : next_actions[1].size()-1;
	bool founded = false;
	for (int i = aux_i; i >= 0; i--) {
		if (founded) break;
		for (int j = aux_j; j >= 0; j--) {
			if (!next_actions[0][i].conflicts_goal(next_actions[1][j])) {
				skip[0] = i;
				skip[1] = j;
				founded = true;
				break;
			}
		}
	}
	if (founded) {
		cerr << "Increible error con solucion, lloremos" << endl;
		for (int i = 0; i < next_actions.size(); i++) {
			conflict_goals[next_actions[i][skip[i]].box_final.x][next_actions[i][skip[i]].box_final.y] = next_actions[i][skip[i]].box;
			conflict_goals[next_actions[i][skip[i]].agent_final.x][next_actions[i][skip[i]].agent_final.y] = agent_ids[i] + '0';
		}
	} else if (intentions[0].type == NO_GOAL || intentions[1].type == NO_GOAL) {
		cerr << "Increible error maybe solucion, lloremos" << endl;
		for (int i = 0; i < next_actions.size(); i++) {
			if (intentions[i].type != NO_GOAL) {
				conflict_goals[next_actions[i][0].box_final.x][next_actions[i][0].box_final.y] = next_actions[i][0].box;
				conflict_goals[next_actions[i][0].agent_final.x][next_actions[i][0].agent_final.y] = agent_ids[i] + '0';
			}
		}
	} else {
		cerr << "Increible error sin solucion, lloremos" << endl;
		vector<vector<CAction>> conflict_plan(2);
		int first, second;
		if (intentions[1].type > intentions[0].type) {
			first = 1;
			second = 0;
		} else {
			first = 0;
			second = 1;
		}
		skip[first] = 0;
		conflict_plan[first].push_back(next_actions[first][0]);
		skip[second] = next_actions[second].size()-1;
		conflict_plan[second].push_back(CAction(ACTION_NOOP, next_actions[second][0].agent_pos));
		return {conflict_plan, skip};
	}

	// Resolve level
	ConflictState* state = new ConflictState(agent_ids, agent_rows, agent_cols, boxes, conflict_goals);
	cerr << state->repr();
	vector<vector<CAction>> conflict_plan = conflict_search(state);

	cerr << "Solved!!! " << conflict_plan[0].size() << " skip: " << skip[0] << " " << skip[1] << endl;
	for (int i = 0; i < conflict_plan[0].size(); i ++) {
		cerr << conflict_plan[0][i].name << "|" << conflict_plan[1][i].name << endl;
	}
	cerr << endl;
	for (auto& plan : conflict_plan) {
		while (plan[plan.size()-1].type == NOOP && plan.size() > 2) {
			plan.erase(plan.end()-1);
		}
	}
	return {conflict_plan, skip};
}

bool box_conflict(CAction action, umap_t believes, coordinates_t* box_pos)
{

	bool is_box_coflict = false;
 	if (action.type == ActionType::MOVE || action.type == ActionType::PULL) {
		if (believes.count(action.agent_final) && is_box(believes[action.agent_final])) {
			*box_pos = action.agent_final;
			is_box_coflict = true;
		}
	} else if (action.type == ActionType::PUSH) {
		if (believes.count(action.box_final) && is_box(believes[action.box_final])) {
			*box_pos = action.box_final;
			is_box_coflict = true;
		}
	}

	if (is_box) {
		vector<coordinates_t> neighbours = find_neighbours(*box_pos);
		for (auto n : neighbours) {
			if (believes.count(n) && believes[n] >= '0' && believes[n] <= '9'
					&& get_color(believes[n]) == get_color(believes[*box_pos])) {
				is_box_coflict = false;
				break;
			}
		}
	}

	return is_box_coflict;
}

bool BdiAgent::try_around_box(umap_t believes, goal_t intention, coordinates_t box_pos)
{
	cerr << "Agent " << agent_id << " Tryin going around" << endl;
	vector<CAction> next_actions(plan.begin()+plan_index, plan.end());
	int agent_row = next_actions[0].agent_pos.x;
	int agent_col = next_actions[0].agent_pos.y;
	vector<vector<char>> boxes(n_rows, vector<char>(n_cols, ' '));
	vector<vector<char>> goals(n_rows, vector<char>(n_cols, ' '));

	if (next_actions[0].type == ActionType::PUSH
		|| next_actions[0].type == ActionType::PULL) {
			boxes[next_actions[0].box_pos.x][next_actions[0].box_pos.y] = next_actions[0].box;
	}

	boxes[box_pos.x][box_pos.y] = believes[box_pos];
	for (auto& it : believes) {
		if (equal(it.first, {agent_row, agent_col}))
			continue;
		char obj = it.second;
		boxes[it.first.x][it.first.y] = obj;
		if (is_box(obj) && get_color(this->agent_id) == get_color(obj)
				&& goals_map.count(it.first) && goals_map[it.first] == obj) {
			goals[it.first.x][it.first.y] = obj;
		}
	}

	bool from_scratch = true;
	vector<CAction>::iterator erase_actions_until;
	for (int i = 1; i < next_actions.size(); i++) {
		if (equal(next_actions[i].agent_final, box_pos)
				|| equal(next_actions[i].box_final, box_pos)) {
			continue;
		}
		erase_actions_until = plan.begin()+plan_index+i+1;
		goals[next_actions[i].agent_final.x][next_actions[i].agent_final.y] = agent_id + '0';
		if (!equal(next_actions[i].agent_final, next_actions[i].box_final)) {
			goals[next_actions[i].box_final.x][next_actions[i].box_final.y] = next_actions[i].box;
		}
		from_scratch = false;
		break;
	}
	if (from_scratch) {
		if (intention.type == AGENT_GOAL) {
			goals[intention.pos.x][intention.pos.y] = (char)(this->agent_id) + '0';
		} else if (intention.type == CARRY_BOX_TO_GOAL) {
			goals[intention.pos.x][intention.pos.y] = goals_map[{intention.pos.x, intention.pos.y}];
		} else if (intention.type == FIND_BOX) {
			goals[intention.pos.x][intention.pos.y] = this->agent_id + '0';
		}
		erase_actions_until = plan.end();
	}

	AgentState* state = new AgentState(agent_id, agent_row, agent_col, boxes, goals);
	//cerr << state->repr();

	vector<CAction> around_plan = search(state, 30000);

	if (!around_plan.empty()) {
		cerr << "Agent " << agent_id << " Going around" << endl;
		cerr << state->repr();
		for (int a = 0; a < around_plan.size(); a++)
			cerr << around_plan[a].name << endl;
		plan.erase(plan.begin()+plan_index, erase_actions_until);
		plan.insert(plan.begin()+plan_index, around_plan.begin(), around_plan.end());
		return true;
	}
	return false;
}

void BdiAgent::run()
{
	umap_t believes;
	deque<goal_t> intentions(1, {NO_GOAL, {-1,-1}});
	bool waiting_for_help = false;

	while (true) {
		believes = this->get_current_map();

		if (intentions.front().type == HELP_MOVE_BOX) {
			msg_t msg;
			msg.type = MSG_TYPE_CONFLICT_BOX_RESOLVED;
			msg.agent_id = this->agent_id;
			send_msg_to_agent(this->time, intentions.front().helping_agent, msg);
		}
		intentions.pop_front();
		if (!intentions.size()) {
			intentions.push_back(get_next_goal(believes));
		}
		goal_t intention = intentions.front();

		if (intention.type != NO_GOAL)
			cerr << "Intention agent " << agent_id << ": " << intention.type << " (" << intention.pos.x << ", " << intention.pos.y << ")" << endl;

		plan.clear();
		if (intention.type == NO_GOAL) {
			if (n_agents == 1) return;
			plan.push_back(CAction(ACTION_NOOP, {intention.pos.x, intention.pos.y}));
		} else {
			AgentState* state = this->intention_to_state(believes, intention);
			cerr << state->repr();
			plan = search(state, -1);
			cerr << "Plan result size: " << plan.size() << endl;
		}
		for (plan_index = 0; plan_index < plan.size(); plan_index++) {
plan_loop:
			CAction next_action = plan[plan_index];
			this->time;
			//if (time > 100) return;

			vector<bool> conflicts_with_other(n_agents, true);
			vector<bool> finished(n_agents, false);
			vector<bool> noop_ops(n_agents, false);

			if (next_action.type == ActionType::NOOP)
				noop_ops[this->agent_id] = true;

			msg_t msg;
			coordinates_t box_pos;
			believes = this->get_current_map();

			// Box conflict
			if (box_conflict(next_action, believes, &box_pos) && !waiting_for_agent) {
				if (try_around_box(believes, intention, box_pos)) {
					goto plan_loop;
				}
				else {
					cerr << "Box conflict agent " << agent_id << endl;
					vector<CAction> next_actions(plan.begin()+plan_index, plan.end());
					msg.agent_id = this->agent_id;
					msg.type = MSG_TYPE_CONFLICT_BOX;
					msg.conflict_box = {box_pos, next_actions};
					for (int agent = 0; agent < n_agents; agent++) {
						if (get_color(agent) == get_color(believes[box_pos])) {
								send_msg_to_agent(this->time, agent, msg);
								break;
						}
					}
					this->waiting_for_agent = true;
				}
			}

			if (this->waiting_for_agent) {
				plan.insert(plan.begin()+plan_index, CAction(ACTION_NOOP, {next_action.agent_pos.x, next_action.agent_pos.y}));
				next_action = plan[plan_index];
			}

			fprintf(stderr, "Action agent %d (time %d): %s, (%d, %d)\t%d\n", agent_id, time,
				next_action.name.c_str(), next_action.agent_pos.x, next_action.agent_pos.y,
				plan.size());

			conflicts_with_other[this->agent_id] = false;
			msg.agent_id = this->agent_id;
			msg.type = MSG_TYPE_NEXT_ACTION;
			msg.next_action = next_action;
			broadcast_msg(this->time, msg);

			bool step_finished = false;
			int debug = 0;
			while (!step_finished && n_agents > 1)
			{ // Communication loop
				if (get_msg(this->time, this->agent_id, &msg)) {
					int sender = msg.agent_id;

					switch (msg.type) {

					case MSG_TYPE_CONFLICT_BOX:
						{
							// Go to box intention
							if (intention.type == AGENT_GOAL || intention.type == FIND_BOX) {
								plan.erase(plan.begin()+plan_index+1, plan.end());
							}
							believes = get_current_map();
							coordinates_t adjacent_pos = get_nearest_adjacent(believes, msg.conflict_box.box_pos, plan[plan.size()-1].agent_final);
							intentions.push_back({FIND_BOX, adjacent_pos});
							// Move box intention
							coordinates_t goal_box_pos = nearest_help_goal_cell(believes, msg.conflict_box.box_pos,
								msg.conflict_box.next_actions);
							intentions.push_back({HELP_MOVE_BOX, goal_box_pos, sender, believes[msg.conflict_box.box_pos]});
							cerr << "Agent " << agent_id <<" solved box conflict for agent " << sender << endl;
							break;
						}

					case MSG_TYPE_CONFLICT_BOX_RESOLVED:
							this->waiting_for_agent = false;
							break;

					case MSG_TYPE_NEXT_ACTION:
						if (next_action.conflicts(msg.next_action)) {
							cerr << "Conflict " << agent_id << endl;
							msg.agent_id = this->agent_id;
							msg.type = MSG_TYPE_CONFLICT_AGENTS;
							msg.conflict.next_actions = vector<CAction>(plan.begin()+plan_index, plan.end());
							msg.conflict.intention = intention;
							send_msg_to_agent(this->time, sender, msg);

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
									bool every_one_finished = true;
									believes = get_current_map();
									for (auto& it : goals_map) {
										if (believes.count(it.first) && believes[it.first] == it.second) {
											continue;
										}
										every_one_finished = false;
										break;
									}
									if (every_one_finished) return;
								}
							} else {
								noop_ops[sender] = false;
							}
							conflicts_with_other[sender] = false;
							bool no_conflicts = true;
							for (bool c : conflicts_with_other) {
								if (c) {
									no_conflicts = false;
									break;
								}
							}
							if (no_conflicts) {
								finished[this->agent_id] = true;
								msg.agent_id = this->agent_id;
								msg.type = MSG_TYPE_STEP_FINISHED;
								broadcast_msg(this->time, msg);
							}
						}
						break;

					case MSG_TYPE_CONFLICT_AGENTS:
						if (this->agent_id < sender) {
							// Solve conflict
							vector<int> agent_ids({this->agent_id, sender});
							vector<goal_t> intentions({intention, msg.conflict.intention});
							vector<vector<CAction>> next_actions;
							next_actions.push_back(vector<CAction>(plan.begin()+plan_index, plan.end()));
							next_actions.push_back(vector<CAction>(msg.conflict.next_actions.begin(), msg.conflict.next_actions.end()));
							believes = get_current_map();
							conflict_t conflict = solve_conflict(believes, agent_ids, next_actions, intentions);
							// Send solution
							msg.agent_id = this->agent_id;
							msg.type = MSG_TYPE_CONFLICT_AGENTS_RESOLVED;
							msg.conflict_resolved = {
								.skip = conflict.skip[1],
								.new_actions = conflict.new_actions[1],
							};
							send_msg_to_agent(this->time, sender, msg);
							// Update solution locally
							plan.erase(plan.begin()+plan_index, plan.begin()+plan_index+conflict.skip[0]+1);
							plan.insert(plan.begin()+plan_index, conflict.new_actions[0].begin(), conflict.new_actions[0].end());
							next_action = plan[plan_index];
							this->waiting_for_agent = false;
			fprintf(stderr, "Action agent %d (time %d): %s, (%d, %d)\t%d\n", agent_id, time,
				next_action.name.c_str(), next_action.agent_pos.x, next_action.agent_pos.y,
				plan.size());
							// Tell everyone to check again
							msg.type = MSG_TYPE_CHECK_AGAIN;
							broadcast_msg_me(this->time, msg);
						}
						else if (!next_action.conflicts(msg.conflict.next_actions[0])) {
							cerr << "Conflict by yout part " << agent_id << endl;
							msg.agent_id = this->agent_id;
							msg.type = MSG_TYPE_CONFLICT_AGENTS;
							msg.conflict.next_actions = vector<CAction>(plan.begin()+plan_index, plan.end());
							msg.conflict.intention = intention;
							send_msg_to_agent(this->time, sender, msg);
						}
						break;

					case MSG_TYPE_CONFLICT_AGENTS_RESOLVED:
						this->waiting_for_agent = false;
						plan.erase(plan.begin()+plan_index, plan.begin()+plan_index+msg.conflict_resolved.skip+1);
						plan.insert(plan.begin()+plan_index, msg.conflict_resolved.new_actions.begin(), msg.conflict_resolved.new_actions.end());
						next_action = plan[plan_index];
						this->waiting_for_agent = false;
			fprintf(stderr, "Action agent %d (time %d): %s, (%d, %d)\t%d\n", agent_id, time,
				next_action.name.c_str(), next_action.agent_pos.x, next_action.agent_pos.y,
				plan.size());
						break;

					case MSG_TYPE_STEP_FINISHED:
						finished[sender] = true;
						break;

					case MSG_TYPE_CHECK_AGAIN:
						conflicts_with_other = vector<bool>(n_agents, true);
						conflicts_with_other[this->agent_id] = false;
						finished = vector<bool>(n_agents, false);

						msg.agent_id = this->agent_id;
						msg.type = MSG_TYPE_NEXT_ACTION;
						msg.next_action = next_action;
						broadcast_msg(this->time, msg);
						break;
					}
				} else { // No messages
						step_finished = true;
						for (bool f : finished) {
							if (!f) {
								step_finished = false;
								sleep(0.3);
								//cerr << agent_id << endl;
								break;
						}
					}
				}
				debug++;
				//if (debug >= 300000) {
				if (false) {
					return;
				}
			} // Communication loop

			update_position(next_action);
			this->time++;
			this->final_plan.push_back(next_action);
			this->advance();
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

void BdiAgent::update_position(CAction action)
{
	assert(!pthread_mutex_lock(&world_mtx));
	if (this->time > current_time)
		current_time++;
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

void BdiAgent::advance()
{
	assert(!pthread_mutex_lock(&world_mtx));
	advance_control[this->agent_id] = true;

	bool advance = true;
	for (int i = 0; i < n_agents; i++) {
		if (!advance_control[i]) {
			advance = false;
			break;
		}
	}
	if (advance) {
		advance_control = vector<bool>(10, false);
		assert(!pthread_cond_broadcast(&world_cond));
	} else {
		assert(!pthread_cond_wait(&world_cond, &world_mtx));	
	}
	assert(!pthread_mutex_unlock(&world_mtx));
}

