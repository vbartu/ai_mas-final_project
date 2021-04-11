#ifndef __STATE_H
#define __STATE_H

#include <vector>
#include <string>
#include "action.h"

using namespace std;

class State {
  // private:
    // const  int RNG = rand()%1;
  public:
    vector<int> agent_rows;
    vector<int> agent_cols;
    vector<vector<char>> boxes;
    static vector<vector<bool>> walls;
    static vector<vector<char>> goals;
    static vector<int> agent_colors;
    static vector<int> box_colors;
    vector<Action> joint_action;
    State *parent;
    int g; // depth

    // Constructor
    State();

	State(vector<vector<char>> boxes, vector<int> &Aagent_rows,
		vector<int> &Aagent_cols);

    State(const State &state);

    bool operator==(const State &other) const;

    State apply_action(vector<Action> joint_action);

    bool is_goal_state();

    vector<State> get_expanded_states();

    bool is_applicable(int agent, Action action);

    bool is_free(int row, int col);

    char agent_at(int row, int col);

    vector<vector<Action>> extract_plan();

    int hashCode() const;

    bool equals(State other);

    string repr();
};

#endif
