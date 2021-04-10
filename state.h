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
    vector<char> Magent_rows;
    vector<char> Magent_cols;
    vector<vector<char>> Mboxes;
    vector<vector<char>> Mwalls;
    vector<vector<char>> Mgoals;
    vector<int> Magent_colors;
    vector<int> Mbox_colors;
    vector<Action> Mjoint_action;
    // final State Mparent;
    int Mg; // depth
    int Mhash = 0;

    // Constructor
    State();

    State(vector<vector<char>> &Awalls, vector<vector<char>> &Agoals, vector<int> &Abox_colors,
          vector<vector<char>> &Aboxes, vector<int> &Aagent_colors, vector<char> &Aagent_rows,
          vector<char> &Aagent_cols);

    State(vector<char> &Aagent_rows, vector<char> &Aagent_cols, vector<vector<char>> &Aboxes);

    State(const State &state);

    bool operator==(const State &other);

    State apply_action(vector<Action> joint_action);

    bool is_goal_state();

    vector<State> get_expanded_states();

    bool is_applicable(int agent, Action action);

    bool is_free(int row, int col);

    char agent_at(int row, int col);

    vector<vector<Action>> extract_plan();

    int hashCode();

    bool equals(State other);

    string repr();
};

#endif
