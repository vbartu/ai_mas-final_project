#include "heuristic.h"

static int goal_count(const State* state);

bool HeuristicHelper::operator()(const State* state1, const State* state2) const
{
	return this->h(state1) > this->h(state2);
}

int HeuristicHelper::h(const State* state) const
{
	return goal_count(state);
}

static int goal_count(const State* state)
{
	int missing = 0;
	for (int row = 0; row < state->simplified_goals.size(); row++) {
		for (int col = 0; col < state->simplified_goals.size(); col++) {
			char goal = state->simplified_goals[row][col];
			if (goal >= 'A' && goal <= 'Z'
					&& state->boxes[row][col] != goal) {
				missing++;
			} else if (goal >= '0' && goal <= '9'
					&& state->agent_at(row, col) != goal) {
				missing++;
			}
		}
	}
	return missing;
}


