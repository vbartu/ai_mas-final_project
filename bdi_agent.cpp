#include <iostream>
#include <assert.h>

#include "main.h"
#include "bdi_agent.h"
#include "action.h"
#include "graphsearch.h"
#include "color.h"

using namespace std;

Communication BdiAgent::communication;
vector<vector<char>> BdiAgent::goals;
umap_t BdiAgent::goals_map;

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
	int n_rows = this->goals.size(), n_cols = this->goals[0].size();
	int agent_row, agent_col;
	vector<vector<char>> boxes(n_rows, vector<char>(n_cols, ' '));
	vector<vector<char>> goal(n_rows, vector<char>(n_cols, ' '));

	for (auto& it : believes) {
		coordinates_t c = it.first;
		char obj = believes[c];
		//cerr << "Object: " << obj << endl;
		if (obj - '0' == this->agent_id) {
			agent_row = c.x;
			agent_col = c.y;
		} else {
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

bool BdiAgent::is_conflict(Action action)
{
	return false;
}

void BdiAgent::update_action(Action action, AgentState* state) {
	if (action.type == ActionType::MOVE) {
		int next_agent_row = state->agent_row + action.ard;
		int next_agent_col = state->agent_col + action.acd;
		this->communication.update_postion(this->time, state->agent_row,
			state->agent_col, next_agent_row, next_agent_col);

	} else if (action.type == ActionType::PUSH) {
		int box_row = state->agent_row + action.ard;
		int box_col = state->agent_col + action.acd;
		int box_dst_row = box_row + action.brd;
		int box_dst_col = box_col + action.bcd;
		this->communication.update_postion(this->time, box_row, box_col,
			box_dst_row, box_dst_col);
		int next_agent_row = state->agent_row + action.ard;
		int next_agent_col = state->agent_col + action.acd;
		this->communication.update_postion(this->time, state->agent_row,
			state->agent_col, next_agent_row, next_agent_col);

	} else if (action.type == ActionType::PULL) {
		int next_agent_row = state->agent_row + action.ard;
		int next_agent_col = state->agent_col + action.acd;
		this->communication.update_postion(this->time, state->agent_row,
			state->agent_col, next_agent_row, next_agent_col);
		int box_row = state->agent_row - action.brd;
		int box_col = state->agent_col - action.bcd;
		int box_dst_row = box_row + action.brd;
		int box_dst_col = box_col + action.bcd;
		this->communication.update_postion(this->time, box_row, box_col,
			box_dst_row, box_dst_col);
	}

}

void BdiAgent::run()
{
	umap_t believes;
	goal_t intention;

	while (true) {
		believes = this->communication.get_positions(this->time);
		intention = this->get_next_goal(believes);
		cerr << "Next goal " << intention.type << " " << intention.row << " " << intention.col << endl;
		if (intention.type == NO_GOAL) {
			cerr << "Agent " << this->agent_id << " finished" << endl;
			return;
		}

		AgentState* state = this->intention_to_state(believes, intention);
		cerr << state->repr();
		vector<Action> plan = search(state);
		cerr << "Plan result size: " << plan.size() << endl;

		for (Action next_action : plan) {
			cerr << "Next action (" << agent_id << "): " << next_action.name << endl;
			if (!this->is_conflict(next_action)) {
				this->final_plan.push_back(next_action);
				update_action(next_action, state);
				this->time++;
				state = state->apply_action(next_action);
				//cerr << state->repr();
			}
		}
	}
}
