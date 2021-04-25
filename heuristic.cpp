#include "heuristic.h"

static int goal_count(const AgentState* state);

bool HeuristicHelper::operator()(const AgentState* state1, const AgentState* state2) const
{
	return this->h(state1) > this->h(state2);
}

int HeuristicHelper::h(const AgentState* state) const
{
	return goal_count(state);
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


