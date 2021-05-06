#include <iostream>
#include <assert.h>
#include <stdio.h>

#include "global.h"
#include "bdi_agent.h"
#include "action.h"
#include "graphsearch.h"
#include "color.h"

using namespace std;


static umap_t world2;

World BdiAgent::world;

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
		} else if (is_box(obj) && get_color(this->agent_id == get_color(obj))) {
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

void BdiAgent::run()
{
	umap_t believes;
	goal_t intention;

	while (true) {
		believes = this->world.get_positions(this->time);
		intention = this->get_next_goal(believes);
		cerr << "Next goal " << intention.type << " " << intention.row << " " << intention.col << endl;
		if (intention.type == NO_GOAL) {
			this->world.finished(this->agent_id);
			return;
		}

		AgentState* state = this->intention_to_state(believes, intention);
		cerr << state->repr();
		vector<Action> plan = search(state);
		cerr << "Plan result size: " << plan.size() << endl;
		for (int i = 0; i < plan.size();) {
			Action next_action = plan[i];
			fprintf(stderr, "Next %d action(%d): %s\n", agent_id, time,
				next_action.name.c_str());

			this->time++;
			if (world.update_position(agent_id, time, next_action, state)) {
				i++;
				this->final_plan.push_back(next_action);
				state = state->apply_action(next_action);
				//cerr << state->repr();
			}
			else {
				cerr << "Conflict!!!" << endl;
				//assert(0);
				this->final_plan.push_back(actions[0]);
			}
		}
	}
}
