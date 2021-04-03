#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "action.h"
#include "color.h"

using namespace std;

class State {
  public:

    vector<char> Magent_rows;
    vector<char> Magent_cols;
    vector<vector<char>> Mboxes;
    static vector<vector <char>> Mwalls;
    static vector<vector <char>> Mgoals;
    static vector<int> Magent_color;
    vector<Action> Mjoint_action;

    final State Mparent;
    final int Mg; // depth

    // Constructor
    State(){}

    State(vector<char> &Aagent_rows, vector<char> &Aagent_cols, vector<vector<char>> &Aboxes) {
        Magent_rows = Aagent_rows;
        Magent_cols = Aagent_cols;
        Mboxes = Aboxes;
    }

    State apply_action(vector<Action> joint_action) {
        /*
        Returns the state resulting from applying joint_action in this state.
        Precondition: Joint action must be applicable and non-conflicting in this state.
        */

        // Copy this state-
        vector<char> copy_agents_rows(this->Magent_rows);
        vector<char> copy_agents_cols(this->Magent_cols);
        vector<vector<char>> copy_boxes(this->Mboxes);

        // Appy each action.
        for (int agent = 0; agent < joint_action.size(); agent++) {

          if (joint_action[agent].type == action_type::NOOP){
            ;
          }

          else if (joint_action[agent].type == action_type::MOVE) {
            copy_agents_rows[agent] += joint_action[agent].ard;
            copy_agents_cols[agent] += joint_action[agent].acd;
          }

          else if (joint_action[agent].type == action_type::PUSH) {

            copy_agents_rows[agent] += joint_action[agent].ard;
            copy_agents_cols[agent] += joint_action[agent].acd;
            // Box current and future position
            int box_row = this->Magent_rows[agent] + joint_action[agent].ard;
            int box_col = this->Magent_cols[agent] + joint_action[agent].acd;
            int box_dst_row = box_row + joint_action[agent].brd;
            int box_dst_col = box_col + joint_action[agent].bcd;
            // Copy box to new cell, the delete it from the old one
            copy_boxes[box_dst_row][box_dst_col] = copy_boxes[box_row][box_col];
            copy_boxes[box_row][box_col] = ' ';
          }

          else if (joint_action[agent].type == action_type::PULL) {

            copy_agents_rows[agent] += joint_action[agent].ard;
            copy_agents_cols[agent] += joint_action[agent].acd;
            // Box current and future position
            int box_row = this->Magent_rows[agent] - joint_action[agent].ard;
            int box_col = this->Magent_cols[agent] - joint_action[agent].acd;
            int box_dst_row = box_row + joint_action[agent].brd;
            int box_dst_col = box_col + joint_action[agent].bcd;
            // Copy box to new cell, the delete it from the old one
            copy_boxes[box_dst_row][box_dst_col] = copy_boxes[box_row][box_col];
            copy_boxes[box_row][box_col] = ' ';
          }
        }
        State copy_state(copy_agents_rows, copy_agents_cols, copy_boxes);
        copy_state.parent = this;
        copy_state.Mjoint_action = joint_action;
        copy_state.g = this->Mg + 1;
        return copy_state;
    }

    bool is_goal_state() {
        char goal;
        for (int row = 0; row < State.Mgoals.size(); row++) {
          for (int col = 0; col < State.Mgoals[i].size; col++) {
            goal = State.Mgoals[row][col];

            if ('A' <= goal && goal <= 'Z' && this->Mboxes[row][col] != goal) {
              return false;
            }
            else if ('0' <= goal && goal <= '9' && (!(this->Magent_rows[int(goal) - int('0')] == row && this->Magent_cols[int(goal) - int('0')] == col))) {
              return false;
            }
          }
        }
        return true;
    }

    vector<State> get_expanded_states() {
        int num_agents = this->Magent_rows.size();

        // Determine list of applicable action for each individual agent
        vector<vector<Action>> applicable_actions;
        for (int agent = 0; agent < num_agents; agent++) {
          // for (action in Action) {
            if (this->is_applicable(agent, action)) {
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
        int agent_row = this->Magent_rows[agent];
        int agent_col = this->Magent_cols[agent];
        color agent_color = State.Magent_color[agent];

        if (action.type == action_type::NOOP) {
          return true;
        }

        else if (action.type == action_type::MOVE) {
          int destination_row = agent_row + action.ard;
          int destination_col = agent_col + action.acd;
          return is_free(destination_row,destination_col);
        }

        else if (action.type == action_type::PUSH) {
          int box_row = agent_row + action.ard;
          int box_col = agent_col + action.acd;
          char box = State.Mboxes[box_row][box_col];

          // Check if there is a box in the given direction,
          // and if the box belongs to this agent (same color)
          if (box == ' ') {
            return false;
          }
          color box_color = State.box_colors[int(box) - int('A')];
          if (box_color != agent_color) {
            return false;
          }
          // Check if the future box cell is occupied
          // The agent will occupy the current box cell
          int box_dst_row = box_row + action.brd;
          int box_dst_col = box_col + action.bcd;
          return is_free(box_dst_row, box_dst_col);
        }

        else if (action.type == action_type::PULL) {
          int box_row = agent_row - action.brd;
          int box_col = agent_col - action.bcd;
          char box State.Mboxes[box_row][box_col];

          // Check if there is a box in the given direction,
          // and if the box belongs to this agent (same color)
          if (box == ' ') {
            return false;
          }
          color box_color = State.box_colors[int(box) - ord('A')];
          if (box_color != agent_color) {
            return false;
          }
          // Check if the future box cell is occupied
          // The agent will occupy the current box cell
          int box_dst_row = box_row + action.ard;
          int box_dst_col = box_col + action.acd;
          return is_free(agent_dst_row, agent_dst_col);
        }
    }

    bool is_free(int row, int col) {
        return (!State.Mwalls[row][col] && this->Mboxes[row][col] == ' ' && agent_at(row,col)) == false;
    }

    char agent_at(int row, int col) {
        for (int agent = 0; agent < this->Magent_rows.size(); agent++) {
          if (this->Magent_rows[agent] == row && this->Magent_cols[agent] == col) {
            return char(agent + int('0'));
        return false
          }
        }
    }

    vector<Action> extract_plan() {
        vector<Action> plan(this->Mg);
        State state = this;
        while (state.joint_action) {
          plan[state.g - 1] = state.joint_action;
          state = state.parent
        }
        return plan;
    }
  }


int main() {

  vector<vector<char>> boxes{ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9, 4 } };
  vector<char> rows{ {4, 5, 6} };
  vector<char> cols{ {1, 2, 3} };

  State state1(rows, cols, boxes);

  return 0;
}
