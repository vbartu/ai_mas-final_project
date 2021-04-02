#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "action.h"

using namespace std;

class State {
  public:

    vector<char> Magent_rows;
    vector<char> Magent_cols;
    vector<vector<char>> Mboxes;
    static vector<vector <char>> Mwalls;
    static vector<vector <char>> Mgoals;

    // Constructor
    State(){};

    State(vector<char> &Aagent_rows, vector<char> &Aagent_cols, vector<vector<char>> &Aboxes) {
      Magent_rows = Aagent_rows;
      Magent_cols = Aagent_cols;
      Mboxes = Aboxes;
    };
  };

    State apply_action(vector<int> joint_action) {
      /*
      Returns the state resulting from applying joint_action in this state.
      Precondition: Joint action must be applicable and non-conflicting in this state.
      */

      // Copy this state-
      vector<char> copy_agents_rows(State.Magent_rows);
      vector<char> copy_agents_cols(State.Magent_cols);
      vector<vector<char>> copy_boxes(State.Mboxes);

      // Appy each action.
      for (int agent = 0; agent < joint_action.size(); agent++) {

        if (joint_action[agent].type == action_type::NOOP){
          ;
        }

        else if (joint_action[agent].type == action_type::MOVE) {
          copy_agents_rows[agent] += action.agent_row_delta;
          copy_agents_cols[agent] += action.agent_col_delta;
        }

        else if (joint_action[agent] == Push) {

        }

        else if (joint_action[agent] == Pull) {

        }
      }
    }



int main() {

  vector<vector<char>> boxes{ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9, 4 } };
  vector<char> rows{ {4, 5, 6} };
  vector<char> cols{ {1, 2, 3} };

  State state1(rows, cols, boxes);

  return 0;
}
