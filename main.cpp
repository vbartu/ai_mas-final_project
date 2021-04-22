#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "color.h"
#include "graphsearch.h"
#include "state.h"
#include "split_level.h"

using namespace std;

#define CLIENT_NAME "Final project client"


int main () {
	cout << CLIENT_NAME << endl;
	////cout << "#" << "Starting" << endl;

	string line;
	getline(cin, line); // #domain
	getline(cin, line); // hospital
	getline(cin, line); // #levelname
	getline(cin, line); // level name

	// Parse colors

	vector<int> agent_colors(10);
	vector<int> box_colors(26);

	getline(cin, line); // #colors
	getline(cin, line);
	while (line[0] != '#') {
		int color_end = line.find(":");
		string color = line.substr(0, color_end);

		for (int i = color_end+2; i < line.length(); i += 3) {
			char obj = line[i];
			if (obj >= '0' and obj <= '9') {
				agent_colors[obj - '0'] = get_color_value(color);
			} else {
				box_colors[obj - 'A'] = get_color_value(color);
			}
		}
		getline(cin, line);
	}



	// Parse level size
	string initial_state_lines[100] = {};
	int n_rows = 0;
	int n_cols = 0;

	getline(cin, line);
	while (line[0] != '#') {
		initial_state_lines[n_rows++] = line;

		if (line.length() > n_cols)
			n_cols = line.length() -1;
		getline(cin, line);
	}


	// Parse initial state
	vector<vector<bool>> walls;
	vector<vector<char>> boxes;
	vector<vector<char>> goals;

	for (int i= 0; i < n_rows; i++) {
		vector<bool> n_wall;
		walls.push_back(n_wall);
		vector<char> n_box;
		boxes.push_back(n_box);
		vector<char> n_goal;
		goals.push_back(n_goal);

		for (int j = 0; j < n_cols; j++) {
			walls[i].push_back(false);
			boxes[i].push_back(' ');
			goals[i].push_back(' ');
		}
	}

	vector<int> agent_rows;
	vector<int> agent_cols;

	for (int i= 0; i < n_rows; i++) {
		line = initial_state_lines[i];
		for (int j = 0; j < line.length(); j++) {
			char c = line[j];
			if (c == ' ') {
				continue;
			} else if (c == '+') {
				walls[i][j] = true;
			} else if ('0' <= c && c <= '9') {
				agent_rows.push_back(i);
				agent_cols.push_back(j);
			} else {
				boxes[i][j] = c;
			}
		}
	}

	getline(cin, line);
	for (int i = 0; line[0] != '#'; i++) {
		for (int j = 0; j < line.length(); j++) {
			char c = line[j];
			if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')) {
				cerr << "Goal " << c << " in " << i << ", " << j << endl;
				goals[i][j] = c;
			}
		}
		getline(cin, line);
	}

	State::agent_colors = agent_colors;
	State::walls = walls;
	State::goals = goals;
	State::box_colors = box_colors;
	State* initial_state = new State(boxes, agent_rows, agent_cols);

	vector<vector<Action>> result = split_level(initial_state);
	cerr << "Final Result length: " << result[0].size() << endl;

	int n_agents = result.size();
	for(int i = 0; i < result[0].size(); i++) {
		string join_action = "";
		for (int j = 0; j < result.size(); j++) {
			join_action += result[j][i].name;
			if (j != result.size()-1) {
				join_action += "|";
			}
		}
		cout << join_action << endl;
		getline(cin, line);
	}

	return 0;

}
