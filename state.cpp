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
    vector<Action> Mjoint_action;

    final State Mparent;
    final int Mg = 0; // depth

    // Constructor
    State(){};

    State(vector<char> &Aagent_rows, vector<char> &Aagent_cols, vector<vector<char>> &Aboxes) {
      Magent_rows = Aagent_rows;
      Magent_cols = Aagent_cols;
      Mboxes = Aboxes;
    };
  };

    State apply_action(vector<Action> joint_action) {
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
          copy_agents_rows[agent] += joint_action[agent].agent_row_delta;
          copy_agents_cols[agent] += joint_action[agent].agent_col_delta;
        }

        else if (joint_action[agent].type == action_type::PUSH) {
          int box_row, box_col, box_dst_row, box_dst_col;

          copy_agents_rows[agent] += joint_action[agent].agent_row_delta;
          copy_agents_cols[agent] += joint_action[agent].agent_col_delta;
          // Box current and future position
          box_row = State.Magent_rows[agent] + joint_action[agent].agent_row_delta;
          box_col = State.Magent_cols[agent] + joint_action[agent].agent_col_delta;
          box_dst_row = box_row + joint_action[agent].box_row_delta;
          box_dst_col = box_col + joint_action[agent].box_col_delta;
          // Copy box to new cell, the delete it from the old one
          copy_boxes[box_dst_row][box_dst_col] = copy_boxes[box_row][box_col];
          copy_boxes[box_row][box_col] = '';
        }

        else if (joint_action[agent].type == action_type::PULL) {
          int box_row, box_col, box_dst_row, box_dst_col;

          copy_agents_rows[agent] += joint_action[agent].agent_row_delta;
          copy_agents_cols[agent] += joint_action[agent].agent_col_delta;
          // Box current and future position
          box_row = State.Magent_rows[agent] - joint_action[agent].agent_row_delta;
          box_col = State.Magent_cols[agent] - joint_action[agent].agent_col_delta;
          box_dst_row = box_row + joint_action[agent].box_row_delta;
          box_dst_col = box_col + joint_action[agent].box_col_delta;
          // Copy box to new cell, the delete it from the old one
          copy_boxes[box_dst_row][box_dst_col] = copy_boxes[box_row][box_col];
          copy_boxes[box_row][box_col] = '';
        }
      }
      State copy_state(copy_agents_rows, copy_agents_cols, copy_boxes);
      // copy_state.parent = ...;
      copy_state.Mjoint_action = joint_action;
      // copy_state.g = self.g + 1;
      return copy_state;
    }

    bool is_goal_state() {
      char goal;
      for (int row = 0; row < State.Mgoals.size(); row++) {
        for (int col = 0; col < State.Mgoals[i].size; col++) {
          goal = State.Mgoals[row][col];

          if ('A' <= goal && goal <= 'Z' && State.Mboxes[row][col] != goal) {
            return false;
          }
          else if ('0' <= goal && goal <= '9' && (!(State.Magent_rows[int(goal) - int('0')] == row && State.Magent_cols[int(goal) - int('0')] == col))) {
            return false;
          }
        }
      }
      return true;
    }

    vector<State> get_expanded_states() {
      int num_agents = State.Magent_rows.size();

      // Determine list of applicable action for each individual agent
      vector<Action> applicable_actions;
      for (int agent = 0; agent < num_agents; agent++) {
        // for (action in Action) {
          if (is_applicable(agent, action)) {
            applicable_action[agent].push_back(action);
          }
        // }
      }
      // Iterate over joint actions, check conflict and generate child states.
      vector<Action> joint_action (num_agents);
      int actions_permutation[num_agents] = {}
      vector<State> expanded_states;
      while (true) {
        for (int agent = 0; agent < num_agents; agent++) {
          joint_action[agent].push_back(applicable_action[agent][actions_permutation[agent]]);
        }
        if (! is_conflicting(joint_action)) {
          expanded_states.push_back(apply_action(joint_action));
        }
        // Advance permutation.
        bool done = false;
        for (int agent = 0; agent < num_agents; agent++) {
          if (actions_permutation[agent] < applicable_actions[agent].size() - 1) {
            actions_permutation += 1;
            break;
          }
          else {
            actions_permutation[agent] = 0;
            if (agent == num_agents -1) {
              done = True;
            }
          }
        }
        if (done) {
          break;
        }
      }
      State._RNG.shuffle(expanded_states);
      return expanded_states;
    }

    bool is_appplicable(int agent, Action action) {
      
    }






int main() {

  vector<vector<char>> boxes{ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9, 4 } };
  vector<char> rows{ {4, 5, 6} };
  vector<char> cols{ {1, 2, 3} };

  State state1(rows, cols, boxes);

  return 0;
}
