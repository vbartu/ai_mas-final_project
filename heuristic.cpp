#include <iostream>
#include "heuristic.h"
#include "global.h"

#define OTHER_BOXES_PENALTY 10

static int precomputed_distance(const AgentState* state);
static int precomputed_distance(const ConflictState* state);
static int keep_near_box(const AgentState* state);
static int avoid_other_boxes(const AgentState* state);
static int avoid_moving_boxes(const AgentState* state);

bool HeuristicHelper::operator()(const AgentState* state1, const AgentState* state2) const
{
	return this->h(state1) > this->h(state2);
}

int HeuristicHelper::h(const AgentState* state) const
{
	return precomputed_distance(state) + keep_near_box(state) + avoid_other_boxes(state) + avoid_moving_boxes(state);
}

static int avoid_other_boxes(const AgentState* state)
{
	if (state->allow_others && state->on_box) {
		return OTHER_BOXES_PENALTY;
	}
	return 0;
}

static int precomputed_distance(const AgentState* state)
{
	int distance = 0;
	coordinates_t agent_pos = {state->agent_row, state->agent_col};
	for (int row = 0; row < n_rows; row++) {
		for (int col = 0; col < n_cols; col++) {
			char goal = state->goal[row][col];
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

static int keep_near_box(const AgentState* state)
{
	int distance = 0;
	coordinates_t agent_pos = {state->agent_row, state->agent_col};
	for (int row = 0; row < n_rows; row++) {
		for (int col = 0; col < n_cols; col++) {
			char box = state->boxes[row][col];
			if (is_box(box) && box == state->carry_box) {
				distance += distance_map[{row, col}][agent_pos] - 1;
			}
		}
	}
	return distance;
}

static int avoid_moving_boxes(const AgentState* state)
{
	//if (state->agent_id == 3) cerr << "VAMMMOOOSS" << endl;
	if (state->action.type == PULL || state->action.type == PUSH) {
		if (state->action.box != state->carry_box) {
			return 3;
		}
	}
	return 0;
}






bool ConflictHeuristicHelper::operator()(const ConflictState* state1, const ConflictState* state2) const
{
	return this->h(state1) > this->h(state2);
}

int ConflictHeuristicHelper::h(const ConflictState* state) const
{
	return precomputed_distance(state);
}

static int precomputed_distance(const ConflictState* state)
{
	int distance = 0;
	//coordinates_t agent_pos = {state->agent_row, state->agent_col};
	for (int row = 0; row < n_rows; row++) {
		for (int col = 0; col < n_cols; col++) {
			char goal = state->goals[row][col];
			if (is_box(goal) && state->boxes[row][col] != goal) {
				for (int box_row = 0; box_row < n_rows; box_row++) {
					for (int box_col = 0; box_col < n_cols; box_col++) {
						char box = state->boxes[box_row][box_col];
						if (box == goal){
							distance += distance_map[{box_row,box_col}][{row, col}];
						}
					}
				}
			} else if (goal >= '0' && goal <= '9') {
				coordinates_t agent_pos = {
					state->agent_rows[goal-'0'],
					state->agent_cols[goal-'0'],
				};
				distance += distance_map[agent_pos][{row, col}];
			}
		}
	}
	return distance;
}
