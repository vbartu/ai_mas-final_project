#include <iostream>
#include <cstdio>
#include <typeinfo>

#include "action.h"
#include "global.h"
#include "conflict_state.h"

using namespace std;

ConflictState::ConflictState(vector<int> agent_ids, vector<int> agent_rows,
		vector<int> agent_cols, vector<vector<char>> boxes,
		vector<vector<char>> goal)
{
	this->agent_ids = agent_ids;
	this->agent_rows = agent_rows;
	this->agent_cols = agent_cols;
	this->boxes = boxes;
	this->goals = goals;
	this->n_agents = this->agent_rows.size();
	this->g = 0;
}

bool ConflictState::operator==(const ConflictState &other) const {
	return this->agent_rows == other.agent_rows
		 && this->agent_cols == other.agent_cols
		 && this->boxes == other.boxes
		 && this->goals == other.goals;
}

vector<ConflictState*> ConflictState::get_expanded_states() {
	vector<vector<Action>> applicable_actions(n_agents);
	for (int agent = 0; agent < num_agents; agent++) {
		for (int act = 0; act < actions.size(); act++) {
			Action action = actions[act];
			if (is_applicable(agent, action)) {
				applicable_actions[agent].push_back(action);
			}
		}
	}

	vector<Action> joint_action(n_agents);
	int actions_permutation[n_agents] = {};
	vector<ConflictState*> expanded_states;

	while (true) {
		for (int agent = 0; agent < n_agents; agent++) {
			joint_action[agent] = applicable_actions[agent][actions_permutation[agent]];
		}

		if (!is_conflicting(joint_action)) {
			expanded_states.push_back(apply_action(joint_action));
		}

		bool done = false;
		for (int agent = 0; agent < n_agents; agent++) {
			if (actions_permutation[agent] < applicable_actions[agent].size() - 1) {
				actions_permutation[agent] += 1;
				break;
			} else {
				actions_permutation[agent] = 0;
				if (agent == n_agents - 1)
					done = true;
			}
		}
		if (done)
			break;
	}
	return expanded_states;
}

bool ConflictState::is_applicable(int agent, Action action) {
	int agent_row = this->agent_rows[agent];
	int agent_col = this->agent_cols[agent];
	int agent_color = get_color(this->agent_ids[agent]);

	if (action.type == ActionType::NOOP) {
		return true;
	}

	else if (action.type == ActionType::MOVE) {
		int destination_row = agent_row + action.ard;
		int destination_col = agent_col + action.acd;
		return is_free(destination_row, destination_col);
	}

	else if (action.type == ActionType::PUSH) {
		int box_row = agent_row + action.ard;
		int box_col = agent_col + action.acd;
		char box = this->boxes[box_row][box_col];

		// Check if there is a box in the given direction,
		// and if the box belongs to this agent (same color)
		if (!is_box(box)) {
			return false;
		}
		if (get_color(box) != agent_color) {
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
		if (!is_box(box)) {
			return false;
		}
		if (get_color(box) != agent_color) {
			return false;
		}
		// Check if the future box cell is occupied
		// The agent will occupy the current box cell
		int agent_dst_row = agent_row + action.ard;
		int agent_dst_col = agent_col + action.acd;
		return is_free(agent_dst_row, agent_dst_col);
	}
	return false;
}

bool ConflictState::is_conflicting(vector<Action> joint_action)
{
	vector<int> dst_rows(n_agents);
	vector<int> dst_cols(n_agents);
	vector<int> box_rows(n_agents);
	vector<int> box_cols(n_agents);

	for (int agent = 0; agent < n_agents; agent++) {
		Action action = joint_action[agent];
		int agent_row = this->agent_rows[agent];
		int agent_col = this->agent_cols[agent];

		if (action.type == ActionType::MOVE) {
			dst_rows[agent] = agent_row + action.ard;
			dst_cols[agent] = agent_col + action.acd;
			box_rows[agent] = agent_row;
			box_cols[agent] = agent_col;
		} else if (action.type == ActionType::PUSH) {
			dst_rows[agent] = agent_row + action.ard;
			dst_cols[agent] = agent_col + action.acd;
			box_rows[agent] = agent_row + action.ard + action.brd;
			box_cols[agent] = agent_col + action.acd + action.bcd;
		} else if (action.type == ActionType::PULL) {
			dst_rows[agent] = agent_row + action.ard;
			dst_cols[agent] = agent_col + action.acd;
			box_rows[agent] = agent_row;
			box_cols[agent] = agent_col;
		}
	}

	for (int a1 = 0; a1 < n_agents; a1++) {
		for (int a2 = a1+1; a2 < n_agents; a2++) {
			if ((dst_rows[a1] == dst_rows[a2] && dst_cols[a1] == dst_cols[a2])
					|| (dst_rows[a1] == box_rows[a2] && dst_cols[a1] == box_cols[a2])
					|| (box_rows[a1] == dst_rows[a2] && box_cols[a1] == dst_cols[a2])
					|| (box_rows[a1] == box_rows[a2] && box_cols[a1] == box_cols[a2])) {
				return true;
			}
		}
	}

	return false;
}

ConflictState* ConflictState::apply_action(vector<Action> joint_action) {
	vector<int> copy_agents_rows(this->agent_rows);
	vector<int> copy_agents_cols(this->agent_cols);
	vector<CAction> completed_actions(n_agents);
	vector<vector<char>> copy_boxes(this->boxes);

	for (int agent = 0; agent < joint_action.size(); agent++) {
		Action action = joint_action[agent]
		if (joint_action[agent].type == ActionType::MOVE) {
			copy_agents_rows[agent] += action.ard;
			copy_agents_cols[agent] += action.acd;
			completed_actions[agent] = CAction(action,
				{agent_rows[agent], agent_cols[agent]}, {-1, -1});
		}
		else if (joint_action[agent].type == ActionType::PUSH) {
			copy_agents_rows[agent] += action.ard;
			copy_agents_cols[agent] += action.acd;
			// Box current and future position
			int box_row = this->agent_rows[agent] + action.ard;
			int box_col = this->agent_cols[agent] + action.acd;
			int box_dst_row = box_row + action.brd;
			int box_dst_col = box_col + action.bcd;
			// Copy box to new cell, the delete it from the old one
			copy_boxes[box_dst_row][box_dst_col] = copy_boxes[box_row][box_col];
			copy_boxes[box_row][box_col] = ' ';
			completed_actions[agent] = CAction(action,
				{agent_rows[agent], agent_cols[agent]}, {box_row, box_col});
		}
		else if (joint_action[agent].type == ActionType::PULL) {
			copy_agents_rows[agent] += joint_action[agent].ard;
			copy_agents_cols[agent] += joint_action[agent].acd;
			// Box current and future position
			int box_row = this->agent_rows[agent] - action.brd;
			int box_col = this->agent_cols[agent] - action.bcd;
			int box_dst_row = box_row + action.brd;
			int box_dst_col = box_col + action.bcd;
			// Copy box to new cell, the delete it from the old one
			copy_boxes[box_dst_row][box_dst_col] = copy_boxes[box_row][box_col];
			copy_boxes[box_row][box_col] = ' ';
			completed_actions[agent] = CAction(action,
				{agent_rows[agent], agent_cols[agent]}, {box_row, box_col});
		}
	}

	ConflictState* copy_state;
	copy_state = new ConflictState(this->agent_ids, copy_agents_rows,
		copy_agents_cols, copy_boxes, this->goals);
	copy_state->parent = this;
	copy_state->joint_action = joint_action;
	copy_state->g = this->g + 1;
	return	copy_state;
}

bool ConflictState::is_goal_state() {
	char goal;
	for (int row = 0; row < this->goals.size(); row++) {
		for (int col = 0; col < this->goals[row].size(); col++) {
			goal = this->goals[row][col];
			if ('0' <= goal && goal <= '9' && agent_at(row, col) != goal) {
				return false;
			} else if (is_box(goal) && this->boxes[row][col] != goal) {
				return false;
			}
		}
	}
	return true;
}

char ConflictState::agent_at(int row, int col) const {
	for (int agent = 0; agent < this->n_agents; agent++) {
		if (this->agent_rows[agent] == row && this->agent_cols[agent] == col) {
			return this->agent_ids[agent] + '0';
		}
	}
	return 0;
}

bool ConflictState::is_free(int row, int col) {
	return !walls[row][col] && this->boxes[row][col] == ' ' && !agent_at(row, col);
}

vector<vector<CAction>> ConflictState::extract_plan() {
	vector<vector<CAction>> plan(this->g);
	ConflictState* state = this;
	while (state->g != 0) {
		plan[state->g - 1] = state->joint_action;
		state = state->parent
	}
	return plan;
}

int ConflictState::hashCode() const {
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
	for (int agent = 0; agent < n_agents; agent++) {
		result = prime * result + this->agent_rows[agent] * this->boxes[0].size()
			+ this->agent_cols[agent];
	}
	return result;
}

string ConflictState::repr()
{
	string lines;
	for (int row = 0; row < this->boxes.size(); row++) {
		string line;
		for (int col = 0; col < this->boxes[row].size(); col++) {
			if (this->boxes[row][col] != ' ') {
				line += this->boxes[row][col];
			} else if (this->walls[row][col]) {
				line += "+";
			} else if (agent_at(row, col)) {
				line += agent_at(row, col);
			} else {
				line += " ";
			}
		}
		line += "     ";
		for (int col = 0; col < this->boxes[row].size(); col++) {
			if (this->goals[row][col] != ' ') {
				line += this->goals[row][col];
			} else if (this->walls[row][col]) {
				line += "+";
			} else {
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
}
