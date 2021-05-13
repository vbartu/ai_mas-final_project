#include <iostream>
#include <cstdio>
#include <typeinfo>

#include "global.h"
#include "bdi_agent_state.h"

using namespace std;

AgentState::AgentState(int agent_id, int agent_row, int agent_col,
		vector<vector<char>> boxes, vector<vector<char>> goal) {
    this->agent_id = agent_id;
    this->agent_row = agent_row;
    this->agent_col = agent_col;
    this->boxes = boxes;
	this->goal = goal;
	this->g = 0;
}

bool AgentState::operator==(const AgentState &other) const {
  return this->agent_row == other.agent_row
         && this->agent_col == other.agent_col
         && this->boxes == other.boxes
		 && this->goal == other.goal;
}

vector<AgentState*> AgentState::get_expanded_states() {
    vector<AgentState*> expanded_states;
	for (int act = 0; act < actions.size(); act++) {
		Action action = actions[act];
		if (this->is_applicable(action)) {
			expanded_states.push_back(this->apply_action(action));
		}
	}
	return expanded_states;
}

bool AgentState::is_applicable(Action action) {
    if (action.type == ActionType::NOOP) {
      return true;
    }

    else if (action.type == ActionType::MOVE) {
      int destination_row = this->agent_row + action.ard;
      int destination_col = this->agent_col + action.acd;
      return is_free(destination_row, destination_col);
    }

    else if (action.type == ActionType::PUSH) {
      int box_row = this->agent_row + action.ard;
      int box_col = this->agent_col + action.acd;
      char box = this->boxes[box_row][box_col];

      // Check if there is a box in the given direction,
      // and if the box belongs to this agent (same color)
      if (!is_box(box)) {
        return false;
      }
      if (get_color(box) != get_color(this->agent_id)) {
        return false;
      }
      // Check if the future box cell is occupied
      // The agent will occupy the current box cell
      int box_dst_row = box_row + action.brd;
      int box_dst_col = box_col + action.bcd;
      return is_free(box_dst_row, box_dst_col);
    }

    else if (action.type == ActionType::PULL) {
      int box_row = this->agent_row - action.brd;
      int box_col = this->agent_col - action.bcd;
      char box = this->boxes[box_row][box_col];

      // Check if there is a box in the given direction,
      // and if the box belongs to this agent (same color)
      if (!is_box(box)) {
        return false;
      }
      if (get_color(box) != get_color(this->agent_id)) {
        return false;
      }
      // Check if the future box cell is occupied
      // The agent will occupy the current box cell
      int agent_dst_row = this->agent_row + action.ard;
      int agent_dst_col = this->agent_col + action.acd;
      return is_free(agent_dst_row, agent_dst_col);
    }
    return false;
}

AgentState* AgentState::apply_action(Action action) {
    int copy_agent_row = this->agent_row;
    int copy_agent_col = this->agent_col;
	CAction completed_action;
    vector<vector<char>> copy_boxes(this->boxes);

	if (action.type == ActionType::NOOP) {
		completed_action = CAction(action, {this->agent_row, this->agent_col});
	}
	else if (action.type == ActionType::MOVE) {
		copy_agent_row += action.ard;
		copy_agent_col += action.acd;
		completed_action = CAction(action, {this->agent_row, this->agent_col});
	} else if (action.type == ActionType::PUSH) {
		copy_agent_row += action.ard;
		copy_agent_col += action.acd;
		// Box current and future position
		int box_row = this->agent_row + action.ard;
		int box_col = this->agent_col + action.acd;
		int box_dst_row = box_row + action.brd;
		int box_dst_col = box_col + action.bcd;
		// Copy box to new cell, the delete it from the old one
		char box = copy_boxes[box_row][box_col];
		copy_boxes[box_dst_row][box_dst_col] = box;
		copy_boxes[box_row][box_col] = ' ';
		completed_action = CAction(action, {this->agent_row, this->agent_col},
			{box_row, box_col}, box);
	} else if (action.type == ActionType::PULL) {
		copy_agent_row += action.ard;
		copy_agent_col += action.acd;
		// Box current and future position
		int box_row = this->agent_row - action.brd;
		int box_col = this->agent_col - action.bcd;
		int box_dst_row = box_row + action.brd;
		int box_dst_col = box_col + action.bcd;
		// Copy box to new cell, the delete it from the old one
		char box = copy_boxes[box_row][box_col];
		copy_boxes[box_dst_row][box_dst_col] = box;
		copy_boxes[box_row][box_col] = ' ';
		completed_action = CAction(action, {this->agent_row, this->agent_col},
			{box_row, box_col}, box);
	}

	AgentState* copy_state;
	copy_state = new AgentState(this->agent_id, copy_agent_row, copy_agent_col,
		copy_boxes, this->goal);
    copy_state->parent = this;
    copy_state->action = completed_action;
    copy_state->g = this->g + 1;
    return  copy_state;
}


bool AgentState::is_goal_state() {
	char goal;
	for (int row = 0; row < this->goal.size(); row++) {
		for (int col = 0; col < this->goal[row].size(); col++) {
			goal = this->goal[row][col];
			if ('A' <= goal && goal <= 'Z'
					&& this->boxes[row][col] != goal) {
				return false;
			} else if ('0' <= goal && goal <= '9'
					&& goal == (char)(this->agent_id) + '0'
					&& (this->agent_row != row || this->agent_col != col)) {
				return false;
			}
		}
	}
	return true;
};

bool AgentState::is_free(int row, int col) {
	return !walls[row][col] && this->boxes[row][col] == ' ';
}

vector<CAction> AgentState::extract_plan() {
    vector<CAction> plan(this->g);
    AgentState* state = this;
    while (state->g != 0) {
		plan[state->g - 1] = state->action;
		state = state->parent;
    }
    return plan;
};

int AgentState::hashCode() const {
	int prime = 31;
	int result = 1;
	for (int row = 0; row < this->boxes.size(); ++row) {
		for (int col = 0; col < this->boxes[row].size(); ++col) {
			char c = this->boxes[row][col];
			if (c!=0) {
				result = prime * result + (row * this->boxes[row].size() + col) * c;
			}
		}
	}
	result = prime * result + this->agent_row * this->boxes[0].size()
		+ this->agent_col;
	return result;
};

string AgentState::repr()
{
    string lines;
	lines += "State: ";
	lines += this->action.name;
	lines += "\n";
    for (int row = 0; row < this->boxes.size(); row++)
    {
      string line;
      for (int col = 0; col < this->boxes[row].size(); col++)
      {
        if (this->boxes[row][col] != ' ')
        {
          line += this->boxes[row][col];
        }
        else if (walls[row][col])
        {
          line += "+";
        }
        else if (this->agent_row == row && this->agent_col == col)
        {
          line += (char)(this->agent_id + '0');
        }
        else
        {
          line += " ";
        }
      }
		line += "     ";
      for (int col = 0; col < this->boxes[row].size(); col++)
      {
        if (this->goal[row][col] != ' ')
        {
          line += this->goal[row][col];
        }
        else if (walls[row][col])
        {
          line += "+";
        }
        else
        {
          line += " ";
        }
      }
	  if (row == 0) {
		line += "     ";
		line += to_string(this->g);
	  }
      lines = lines + line + "\n";
    }
    return lines;
};
