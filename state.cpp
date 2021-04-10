#include <iostream>
#include <cstdio>
#include <random>
#include <typeinfo>

#include "action.h"
#include "color.h"
#include "state.h"

using namespace std;

vector<vector<bool>> State::Mwalls;
vector<vector<char>> State::Mgoals;
vector<int> State::Magent_colors;
vector<int> State::Mbox_colors;

// Constructor
State::State(){};

State::State(vector<vector<bool>> &Awalls, vector<vector<char>> &Agoals, vector<int> &Abox_colors,
             vector<vector<char>> &Aboxes, vector<int> &Aagent_colors, vector<int> &Aagent_rows,
             vector<int> &Aagent_cols) {
    Mwalls = Awalls;
    Mgoals = Agoals;
    Mbox_colors = Abox_colors;
    Mboxes = Aboxes;
    Magent_colors = Aagent_colors;
    Magent_rows = Aagent_rows;
    Magent_cols = Aagent_cols;
  };


State::State(vector<int> &Aagent_rows, vector<int> &Aagent_cols, vector<vector<char>> &Aboxes) {
    Magent_rows = Aagent_rows;
    Magent_cols = Aagent_cols;
    Mboxes = Aboxes;
};


    // Copy Constructor
State::State(const State &state) {
    Magent_rows = state.Magent_rows;
    Magent_cols = state.Magent_cols;
    Mboxes = state.Mboxes;
    Mwalls = state.Mwalls;
    Magent_colors = state.Magent_colors;
    Mbox_colors = state.Mbox_colors;
    Mjoint_action = state.Mjoint_action;
    Mg = state.Mg;
    Mhash = state.Mhash;
    // Mparent(this->Mparent);
};

    // To check if two state are equals
bool State::operator==(const State &other) {
  return this->Magent_rows == other.Magent_rows
         && this->Magent_cols == other.Magent_cols
         && this->Mboxes == other.Mboxes
         && this->Mwalls == other.Mwalls
         && this->Magent_colors == other.Magent_colors
         && this->Mbox_colors == other.Mbox_colors
         && this->Mjoint_action == other.Mjoint_action
         && this->Mg == other.Mg
         && this->Mhash == other.Mhash;
         // && this->Mparent == other.Mparent;
};

/*
bool operator==(State state1, State state2) {
  return state1.Magent_rows == state2.Magent_rows
         && state1.Magent_cols == state2.Magent_cols
         && state1.Mboxes == state2.Mboxes
         && state1.Mwalls == state2.Mwalls
         && state1.Magent_colors == state2.Magent_colors
         && state1.Mbox_colors == state2.Mbox_colors
         && state1.Mjoint_action == state2.Mjoint_action
         && state1.Mg == state2.Mg
         && Mhash == state2.Mhash;
         // && state1.Mparent == state2.Mparent;
}
*/

State State::apply_action(vector<Action> joint_action) {
    /*
    Returns the state resulting from applying joint_action in this state.
    Precondition: Joint action must be applicable and non-conflicting in this state.
    */

    // Copy this state-
    vector<int> copy_agents_rows(this->Magent_rows);
    vector<int> copy_agents_cols(this->Magent_cols);
    vector<vector<char>> copy_boxes(this->Mboxes);

    // Appy each action.
    for (int agent = 0; agent < joint_action.size(); agent++) {

      if (joint_action[agent].type == ActionType::NOOP){
        ;
      }

      else if (joint_action[agent].type == ActionType::MOVE) {
        copy_agents_rows[agent] += joint_action[agent].ard;
        copy_agents_cols[agent] += joint_action[agent].acd;
      }

      else if (joint_action[agent].type == ActionType::PUSH) {

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

      else if (joint_action[agent].type == ActionType::PULL) {

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
    // copy_state.Mparent = this;
    copy_state.Mjoint_action = joint_action;
    copy_state.Mg = this->Mg + 1;
    return copy_state;
};


bool State::is_goal_state() {
    char goal;
    for (int row = 0; row < this->Mgoals.size(); row++) {
      for (int col = 0; col < this->Mgoals[row].size(); col++) {
        goal = this->Mgoals[row][col];
        if ('A' <= goal && goal <= 'Z' && this->Mboxes[row][col] != goal) {
          return false;
        }
        else if ('0' <= goal && goal <= '9' && (!(this->Magent_rows[int(goal) - int('0')] == row && this->Magent_cols[int(goal) - int('0')] == col))) {
          return false;
        }
      }
    }
    return true;
};


vector<State> State::get_expanded_states() {
    int num_agents = this->Magent_rows.size();
    // Determine list of applicable action for each individual agent
    vector<vector<Action>> applicable_actions;
    for (int agent = 0; agent < num_agents; agent++) {
      for (int act = 0; act < ACTION_LIST_SIZE; act++) {
        Action action = action_list[act];
        if (is_applicable(agent, action)) {
          applicable_actions[agent].push_back(action);
        }
      }
    }
    // Iterate over joint actions, check conflict and generate child states.
    vector<Action> joint_action (num_agents);
    int actions_permutation[num_agents] = {};
    vector<State> expanded_states;
    while (true) {
      for (int agent = 0; agent < num_agents; agent++) {
        joint_action.push_back(applicable_actions[agent][actions_permutation[agent]]);
      }
      // if (! is_conflicting(joint_action)) {
      //   expanded_states.push_back(apply_action(joint_action));
      // }
      // Advance permutation.
      bool done = false;
      for (int agent = 0; agent < num_agents; agent++) {
        if (actions_permutation[agent] < applicable_actions[agent].size() - 1) {
          actions_permutation[agent] += 1;
          break;
        }
        else {
          actions_permutation[agent] = 0;
          if (agent == num_agents -1) {
            done = true;
          }
        }
      }
      if (done) {
        break;
      }
    }
    // this->RNG.shuffle(expanded_states);
    return expanded_states;
};


bool State::is_applicable(int agent, Action action) {
    int agent_row = this->Magent_rows[agent];
    int agent_col = this->Magent_cols[agent];
    int agent_color = this->Magent_colors[agent];

    if (action.type == ActionType::NOOP) {
      return true;
    }

    else if (action.type == ActionType::MOVE) {
      int destination_row = agent_row + action.ard;
      int destination_col = agent_col + action.acd;
      return is_free(destination_row,destination_col);
    }

    else if (action.type == ActionType::PUSH) {
      int box_row = agent_row + action.ard;
      int box_col = agent_col + action.acd;
      char box = this->Mboxes[box_row][box_col];

      // Check if there is a box in the given direction,
      // and if the box belongs to this agent (same color)
      if (box == ' ') {
        return false;
      }
      int box_color = this->Mbox_colors[int(box) - int('A')];
      if (box_color != agent_color) {
        return false;
      }
      // Check if the future box cell is occupied
      // The agent will occupy the current box cell
      int box_dst_row = box_row + action.brd;
      int box_dst_col = box_col + action.bcd;
      return is_free(box_dst_row, box_dst_col);
    }

    else if (action.type == ActionType::PULL) {
      int box_row = agent_row - action.brd;
      int box_col = agent_col - action.bcd;
      char box = this->Mboxes[box_row][box_col];

      // Check if there is a box in the given direction,
      // and if the box belongs to this agent (same color)
      if (box == ' ') {
        return false;
      }
      int box_color = this->Mbox_colors[int(box) - int('A')];
      if (box_color != agent_color) {
        return false;
      }
      // Check if the future box cell is occupied
      // The agent will occupy the current box cell
      int agent_dst_row = agent_row + action.ard;
      int agent_dst_col = agent_col + action.acd;
      return is_free(agent_dst_row, agent_dst_col);
    }
    return false;
};


bool State::is_free(int row, int col) {
    return (! this->Mwalls[row][col] && this->Mboxes[row][col] == ' ' && agent_at(row,col)) == false;
}


char State::agent_at(int row, int col) {
    for (int agent = 0; agent < this->Magent_rows.size(); agent++) {
      if (this->Magent_rows[agent] == row && this->Magent_cols[agent] == col) {
        return char(agent + int('0'));
      }
    }
    return false;
};


vector<vector<Action>> State::extract_plan() {
    vector<vector<Action>> plan(this->Mg);
    for (int i= 0; i < this->Mg; i++) {
      plan.push_back(vector<Action>(this->Mjoint_action.size()));
    }
    State state = *this;
    while (! state.Mjoint_action.empty()) {
      plan[state.Mg - 1]= state.Mjoint_action;
      // state = state.Mparent;
    }
    return plan;
};


int State::hashCode() {
    if (this->Mhash == 0) {
      int prime = 31;
      int result = 1;
      //result = prime * result + hashCode(this->Mbox_colors);
      //result = prime * result + hashCode(this->Magent_colors);
      //result = prime * result + hashCode(this->Mwalls);
      //result = prime * result + hashCode(this->Mgoals);
      //result = prime * result + hashCode(this->Magent_rows);
      //result = prime * result + hashCode(this->Magent_cols);
      for (int row = 0; row < this->Mboxes.size(); ++row) {
        for (int col = 0; col < this->Mboxes[row].size(); ++col) {
          char c = this->Mboxes[row][col];
          if (c!=0) {
            result = prime * result + (row * this->Mboxes[row].size() + col) * c;
          }
        }
      }
      this->Mhash = result;
    }
    return this->Mhash;
};


bool State::equals(State other)  {
    State state = *this; // maybe there is a better way to pass *this directly
    if (state == other) {
      return true;
    }
    if (typeid(other).name() == "State" ) {
      return false;
    }
    if (this->Magent_rows != other.Magent_rows) {
      return false;
    }
    if (this->Magent_cols != other.Magent_cols) {
      return false;
    }
    if (this->Magent_colors != other.Magent_colors) {
      return false;
    }
    if (this->Mwalls != other.Mwalls) {
      return false;
    }
    if (this->Mboxes != other.Mboxes) {
      return false;
    }
    if (this->Mbox_colors != other.Mbox_colors) {
      return false;
    }
    if (this->Mgoals != other.Mgoals) {
      return false;
    }
    return true;
};


string State::repr()
{
    string lines;
    for (int row = 0; row < this->Mboxes.size(); row++)
    {
      string line;
      for (int col = 0; col < this->Mboxes[row].size(); col++)
      {
        if (this->Mboxes[row][col] != ' ')
        {
          line.append(to_string(this->Mboxes[row][col]));
        }
        else if (this->Mwalls[row][col] != false)
        {
          line.append("+");
        }
        else if (agent_at(row,col) != false)
        {
          line.append(to_string(agent_at(row,col)));
        }
        else
        {
          line.append(" ");
        }
      }
      for (int i = 1; i < line.length(); i += 3)
      {
        line.append(i, ' ');
      }
      lines.append(line);
      lines.append("\n");
    }
    return lines;
};
