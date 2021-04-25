#include <iostream>

#include "bdi_agent.h"
#include "action.h"
#include "graphsearch.h"

using namespace std;

Communication BdiAgent::communication;
vector<vector<char>> BdiAgent::goals;
umap_t BdiAgent::goals_map;
vector<int> BdiAgent::box_color;
vector<int> BdiAgent::agent_color;

BdiAgent::BdiAgent(int agent_id)
{
	this->time = 0;
	this->agent_id = agent_id;
}

goal_t BdiAgent::get_next_goal(umap_t believes) {
	goal_t g;
	g.type = -1;
	for (int row = 0; row < this->goals.size(); row++) {
		for (int col = 0; col < this->goals[row].size(); col++) {
			char goal = this->goals[row][col];
			if (goal >= '0' && goal <= '9' && goal - '0' == this->agent_id) {
				try{
					if (believes.at((coordinates_t){row,col}) == goal) {
						continue;
					}
				} catch (...) {
					;
				}

				g = {2, row, col};
				break;
			}
		}
	}
	return g;
}

AgentState* BdiAgent::intention_to_state(umap_t believes, goal_t intention)
{
	int n_rows = this->goals.size(), n_cols = this->goals[0].size();
	int agent_row, agent_col;
	vector<vector<char>> boxes(n_rows, vector<char>(n_cols, ' '));
	for (auto& it : believes) {
		coordinates_t c = it.first;
		char obj = believes[c];
		if (obj - '0' == this->agent_id) {
			agent_row = c.x;
			agent_col = c.y;
		} else {
			boxes[c.x][c.y] = obj;
		}
	}

	vector<vector<char>> goal(n_rows, vector<char>(n_cols, ' '));
	if (intention.type == 2) {
		goal[intention.row][intention.col] = (char)(this->agent_id) + '0';
	} else {
		cerr << "Not yet implented!!!" << endl;
	}

	return new AgentState(this->agent_id, agent_row, agent_col, boxes, goal);
}

bool BdiAgent::is_conflict(Action action)
{
	return false;
}

void BdiAgent::run()
{
	umap_t believes;
	goal_t intention;

	while (true) {
		believes = this->communication.get_positions(this->time);
		intention = this->get_next_goal(believes);
		if (intention.type == -1) {
			cerr << "Agent " << this->agent_id << " finished" << endl;
			return;
		}
		AgentState* state = this->intention_to_state(believes, intention);
		vector<Action> plan = search(state);
		cerr << "Result: " << plan.size() << endl;
		for (auto& it : plan) {
			Action next_action = it;
			//cerr << "Next action (" << agent_id << "): " << next_action.name << endl;
			AgentState* next_state = state->apply_action(next_action);
			believes = communication.get_positions(this->time);

			if (!this->is_conflict(next_action)) {
				this->final_plan.push_back(next_action);
				this->time++;
				communication.update_postion(this->time,
						state->agent_row, state->agent_col,
						next_state->agent_row, next_state->agent_col);
				state = next_state;
			} else {
				break;
			}
		}
	}
}
