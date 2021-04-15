#include <iostream>

#include "split_level.h"
#include "frontier.h"
#include "graphsearch.h"

using namespace std;

vector<vector<Action>> split_level(State* initial_state)
{
	cerr << "Splitted levels:" << endl;

	int number_of_levels = initial_state->agent_rows.size();
	State* splitted[number_of_levels];

	vector<vector<Action>> global_result;

	for (int i = 0; i < number_of_levels; i++) {
		vector<int> agent_rows;
		vector<int> agent_cols;
		agent_rows.push_back(initial_state->agent_rows[i]);
		agent_cols.push_back(initial_state->agent_cols[i]);

		int color = initial_state->agent_colors[i];
		vector<vector<char>> boxes = initial_state->boxes;
		vector<vector<char>> goals = initial_state->goals;

		for (int j = 0; j < boxes.size(); j++) {
			for (int k = 0; k < boxes[i].size(); k++) {
				char box = boxes[j][k];
				if (initial_state->box_colors[box - 'A'] != color) {
					boxes[j][k] = ' ';
				}
				char goal = goals[j][k];
				if (goal >= '0' && goal <= '9') {
					if ((goal - '0') == i) {
						goals[j][k] = '0';
					} else {
						goals[j][k] = ' ';
					}
				} else if (goal >= 'A' && goal <= 'Z'
						&& initial_state->box_colors[goal - 'A'] != color) {
					goals[j][k] = ' ';
				}
			}
		}

		State* state = new State(boxes, agent_rows, agent_cols, goals);
		splitted[i] = state;
		int n_goals = 0;
		for (auto col : goals) {
			for (auto goal : col) {
				if (goal != ' ') {
					n_goals++;
				}
			}
		}
		cerr << "Agent " << i << ", Goals: " << n_goals << endl;
		cerr << state->repr();
		//cerr << endl;
		//cerr << state->repr_goal();

		FrontierBFS frontier;
		vector<vector<Action>> result = search(state, frontier);
		cerr << "Result " << i << ": " << result.size() << endl;

		for (int j = 0; j < result.size(); j++) {
			if (j < global_result.size()) {
				global_result[j].push_back(result[j][0]);
			} else {
				vector<Action> new_actions(i, actions[0]);
				global_result.push_back(new_actions);
				global_result[j].push_back(result[j][0]);
			}
		}

		for (int j = result.size(); j < global_result.size(); j++) {
			global_result[j].push_back(actions[0]);
		}
	}
	return global_result;
}
