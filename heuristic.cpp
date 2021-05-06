#include <iostream>
#include "heuristic.h"
#include "global.h"

static int goal_count(const AgentState* state);
static int precomputed_distance(const AgentState* state);

bool HeuristicHelper::operator()(const AgentState* state1, const AgentState* state2) const
{
	return this->h(state1) > this->h(state2);
}

int HeuristicHelper::h(const AgentState* state) const
{
	//cerr << "P: " << precomputed_distance(state);
	return precomputed_distance(state);
}

static int goal_count(const AgentState* state)
{
	int missing = 0;
	for (int row = 0; row < state->goal.size(); row++) {
		for (int col = 0; col < state->goal.size(); col++) {
			char goal = state->goal[row][col];
			if (goal >= 'A' && goal <= 'Z'
					&& state->boxes[row][col] != goal) {
				missing++;
			} else if (goal >= '0' && goal <= '9'
					&& state->agent_row == row
					&& state->agent_col == col) {
				missing++;
			}
		}
	}
	return missing;
}

static int precomputed_distance(const AgentState* state)
{
	int distance = 0;
	coordinates_t agent_pos = {state->agent_row, state->agent_col};
	for (int row = 0; row < n_rows; row++) {
		for (int col = 0; col < n_cols; col++) {
			char goal = state->goal[row][col];
			if (goal != ' ') {
				cerr << "Box: " << goal << endl;
			}
			if (is_box(goal) && state->boxes[row][col] != goal) {
				for (int box_row = 0; box_row < n_rows; box_row++) {
					for (int box_col = 0; box_col < n_cols; box_col++) {
						char box = state->boxes[box_row][box_col];
						if (box == goal){
							distance += distance_map[{box_row,box_col}][{row, col}];
						}
					}
				}
			} else if (goal >= '0' && goal <= '9'
					&& !(agent_pos.x == row && agent_pos.y == col)) {
				distance += distance_map[agent_pos][{row, col}];
			}
		}
	}
	return distance;
}
