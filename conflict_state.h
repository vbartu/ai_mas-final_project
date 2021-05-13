#ifndef __CONFLICT_STATE_H
#define __CONFLICT_STATE_H

#include <vector>
#include <string>
#include "action.h"

using namespace std;

class ConflictState {
	public:
	vector<int> agent_ids;
	vector<int> agent_rows;
	vector<int> agent_cols;
	vector<vector<char>> boxes;
	vector<vector<char>> goals;
	int conflict_n_agents;
	vector<CAction> joint_action;
	ConflictState *parent;
	int g; // depth

	// Constructor
	ConflictState(vector<int> agent_ids, vector<int> agent_rows, vector<int> agent_cols,
		vector<vector<char>> boxes, vector<vector<char>> goal);

	vector<ConflictState*> get_expanded_states();

	bool is_applicable(int agent, Action action);

	bool is_conflicting(vector<Action> joint_action);

	ConflictState* apply_action(vector<Action> joint_action);

	bool is_goal_state();

	char agent_at(int row, int col) const;

	bool is_free(int row, int col);

	vector<vector<CAction>> extract_plan();

	int hashCode() const;

	bool operator==(const ConflictState &other) const;

	string repr();
};

class ConflictHashHelper {
	public:
		int operator()(const ConflictState* state) const {
			return state->hashCode();
		}
};

class ConflictEqualHelper {
	public:
		int operator()(const ConflictState* state1, const ConflictState* state2) const {
			return *state1 == *state2;
		}
};

#endif // __CONFLICT_STATE_H

