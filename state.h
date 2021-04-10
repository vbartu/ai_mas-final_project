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
    vector<int> Magent_rows;
    vector<int> Magent_cols;
    vector<vector<char>> Mboxes;
    static vector<vector<bool>> Mwalls;
    static vector<vector<char>> Mgoals;
    static vector<int> Magent_colors;
    static vector<int> Mbox_colors;
    vector<Action> Mjoint_action;
    State *Mparent;
    int Mg; // depth
    int Mhash = 0;

    // Constructor
    State();

    State(vector<vector<bool>> &Awalls, vector<vector<char>> &Agoals, vector<int> &Abox_colors,
          vector<vector<char>> &Aboxes, vector<int> &Aagent_colors, vector<int> &Aagent_rows,
          vector<int> &Aagent_cols);

    State(vector<int> &Aagent_rows, vector<int> &Aagent_cols, vector<vector<char>> &Aboxes);

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
