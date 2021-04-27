#ifndef __BDI_AGENT_STATE_H
#define __BDI_AGENT_STATE_H

#include <vector>
#include <string>
#include "action.h"

using namespace std;

class AgentState {
  public:
	int agent_id;
	int agent_row;
	int agent_col;
    vector<vector<char>> boxes;
    vector<vector<char>> goal;
    static vector<vector<bool>> walls;
    static vector<int> agent_colors;
    static vector<int> box_colors;
    Action action;
    AgentState *parent;
    int g; // depth

    AgentState(int agent_id, int agent_row, int agent_col,
		vector<vector<char>> boxes, vector<vector<char>> goal);

    vector<AgentState*> get_expanded_states();

    bool is_applicable(Action action);

    AgentState* apply_action(Action action);

    bool is_goal_state();

    bool is_free(int row, int col);

    vector<Action> extract_plan();

    int hashCode() const;

    bool operator==(const AgentState &other) const;

    string repr();
};

class HashHelper {
	public:
		int operator()(const AgentState* state) const {
			return state->hashCode();
		}
};

class EqualHelper {
	public:
		int operator()(const AgentState* state1, const AgentState* state2) const {
			return *state1 == *state2;
		}
};

#endif
