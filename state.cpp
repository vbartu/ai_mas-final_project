#include <iostream>
#include <cstdio>
#include <random>
#include <typeinfo>

#include "action.h"
#include "color.h"
#include "state.h"

using namespace std;

vector<vector<bool>> State::walls;
vector<vector<char>> State::goals;
vector<int> State::agent_colors;
vector<int> State::box_colors;

State::State() {};

State::State(vector<vector<char>> boxes, vector<int> &agent_rows,
		vector<int> &agent_cols) {
    this->boxes = boxes;
    this->agent_rows = agent_rows;
    this->agent_cols = agent_cols;
	this->g = 0;
};


State::State(const State &state) {
    agent_rows = state.agent_rows;
    agent_cols = state.agent_cols;
    boxes = state.boxes;
    walls = state.walls;
    agent_colors = state.agent_colors;
    box_colors = state.box_colors;
    joint_action = state.joint_action;
    g = state.g;
    parent = state.parent;
};

bool State::operator==(const State &other) const {
  return this->agent_rows == other.agent_rows
         && this->agent_cols == other.agent_cols
         && this->boxes == other.boxes;
};

State* State::apply_action(vector<Action> joint_action) {
    /*
    Returns the state resulting from applying joint_action in this state.
    Precondition: Joint action must be applicable and non-conflicting in this state.
    */

    // Copy this state-
    vector<int> copy_agents_rows(this->agent_rows);
    vector<int> copy_agents_cols(this->agent_cols);
    vector<vector<char>> copy_boxes(this->boxes);

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
        int box_row = this->agent_rows[agent] + joint_action[agent].ard;
        int box_col = this->agent_cols[agent] + joint_action[agent].acd;
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
        int box_row = this->agent_rows[agent] - joint_action[agent].ard;
        int box_col = this->agent_cols[agent] - joint_action[agent].acd;
        int box_dst_row = box_row + joint_action[agent].brd;
        int box_dst_col = box_col + joint_action[agent].bcd;
        // Copy box to new cell, the delete it from the old one
        copy_boxes[box_dst_row][box_dst_col] = copy_boxes[box_row][box_col];
        copy_boxes[box_row][box_col] = ' ';
      }
    }

	State* copy_state;
	copy_state = new State(copy_boxes, copy_agents_rows, copy_agents_cols);
    copy_state->parent = this;
    copy_state->joint_action = joint_action;
    copy_state->g = this->g + 1;

	//cerr << "Parent: " << this << ", Child(" << g << "): " << copy_state << endl;

    return  copy_state;
};


bool State::is_goal_state() {
    char goal;
    for (int row = 0; row < this->goals.size(); row++) {
      for (int col = 0; col < this->goals[row].size(); col++) {
        goal = this->goals[row][col];
        if ('A' <= goal && goal <= 'Z' && this->boxes[row][col] != goal) {
          return false;
        }
        else if ('0' <= goal && goal <= '9' && (!(this->agent_rows[int(goal) - int('0')] == row && this->agent_cols[int(goal) - int('0')] == col))) {
          return false;
        }
      }
    }
    return true;
};


vector<State*> State::get_expanded_states() {
    int num_agents = this->agent_rows.size();
    // Determine list of applicable action for each individual agent
    vector<vector<Action>> applicable_actions;
    for (int agent = 0; agent < num_agents; agent++) {
		vector<Action> aux;
		applicable_actions.push_back(aux);
      for (int act = 0; act < actions.size(); act++) {
        Action action = actions[act];
        if (is_applicable(agent, action)) {
          applicable_actions[agent].push_back(action);
        }
      }
    }
    // Iterate over joint actions, check conflict and generate child states.
    vector<Action> joint_action(num_agents, actions[0]);
    int actions_permutation[num_agents] = {};
    vector<State*> expanded_states;
    while (true) {
      for (int agent = 0; agent < num_agents; agent++) {
        joint_action[agent] = applicable_actions[agent][actions_permutation[agent]];
      }
      // if (! is_conflicting(joint_action)) {
	  State* aux_state = apply_action(joint_action);
      expanded_states.push_back(aux_state);
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
    int agent_row = this->agent_rows[agent];
    int agent_col = this->agent_cols[agent];
    int agent_color = this->agent_colors[agent];

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
      char box = this->boxes[box_row][box_col];

      // Check if there is a box in the given direction,
      // and if the box belongs to this agent (same color)
      if (box == ' ') {
        return false;
      }
      int box_color = this->box_colors[int(box) - int('A')];
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
      char box = this->boxes[box_row][box_col];

      // Check if there is a box in the given direction,
      // and if the box belongs to this agent (same color)
      if (box == ' ') {
        return false;
      }
      int box_color = this->box_colors[int(box) - int('A')];
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
    return (! this->walls[row][col] && this->boxes[row][col] == ' ' && agent_at(row,col) == false);
}


char State::agent_at(int row, int col) {
    for (int agent = 0; agent < this->agent_rows.size(); agent++) {
      if (this->agent_rows[agent] == row && this->agent_cols[agent] == col) {
        return char(agent + int('0'));
      }
    }
    return false;
};


vector<vector<Action>> State::extract_plan() {
    vector<vector<Action>> plan(this->g);
    for (int i= 0; i < this->g; i++) {
      plan.push_back(vector<Action>(this->joint_action.size()));
    }
    State* state = this;
    while (! state->joint_action.empty()) {
      plan[state->g - 1] = state->joint_action;
      state = state->parent;
    }
    return plan;
};

int State::hashCode() const {
      int prime = 31;
      int result = 1;
      //result = prime * result + hashCode(this->box_colors);
      //result = prime * result + hashCode(this->agent_colors);
      //result = prime * result + hashCode(this->walls);
      //result = prime * result + hashCode(this->goals);
      //result = prime * result + hashCode(this->agent_rows);
      //result = prime * result + hashCode(this->agent_cols);
      for (int row = 0; row < this->boxes.size(); ++row) {
        for (int col = 0; col < this->boxes[row].size(); ++col) {
          char c = this->boxes[row][col];
          if (c!=0) {
            result = prime * result + (row * this->boxes[row].size() + col) * c;
          }
        }
      }
	return result;
};


bool State::equals(State other)  {
    State state = *this; // maybe there is a better way to pass *this directly
    if (state == other) {
      return true;
    }
    if (typeid(other).name() == typeid(state).name() ) {
      return false;
    }
    if (this->agent_rows != other.agent_rows) {
      return false;
    }
    if (this->agent_cols != other.agent_cols) {
      return false;
    }
    if (this->agent_colors != other.agent_colors) {
      return false;
    }
    if (this->walls != other.walls) {
      return false;
    }
    if (this->boxes != other.boxes) {
      return false;
    }
    if (this->box_colors != other.box_colors) {
      return false;
    }
    if (this->goals != other.goals) {
      return false;
    }
    return true;
};


string State::repr()
{
    string lines;
    for (int row = 0; row < this->boxes.size(); row++)
    {
      string line;
      for (int col = 0; col < this->boxes[row].size(); col++)
      {
        if (this->boxes[row][col] != ' ')
        {
          line += this->boxes[row][col];
        }
        else if (this->walls[row][col] != false)
        {
          line += "+";
        }
        else if (agent_at(row,col) != false)
        {
          line += agent_at(row,col);
        }
        else
        {
          line += " ";
        }
      }
      lines = lines + line + "\n";
    }
    return lines;
};
