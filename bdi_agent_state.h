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
    CAction action;
    AgentState *parent;
    int g; // depth
	bool on_box;
	bool allow_others;

    AgentState(int agent_id, int agent_row, int agent_col,
		vector<vector<char>> boxes, vector<vector<char>> goal);

	AgentState(int agent_id, int agent_row, int agent_col,
		vector<vector<char>> boxes, vector<vector<char>> goal, bool allow_others);

    vector<AgentState*> get_expanded_states();

    bool is_applicable(Action action);

    AgentState* apply_action(Action action);

    bool is_goal_state();

    bool is_free(int row, int col);
	bool is_other_box(int row, int col);

    vector<CAction> extract_plan();

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
