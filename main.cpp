#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "color.h"

using namespace std;

#define CLIENT_NAME "Final project client"


int main () {
	cout << CLIENT_NAME << endl;
	cout << "#" << "Starting" << endl;

	string line;
	getline(cin, line); // #domain
	getline(cin, line); // hospital
	getline(cin, line); // #levelname
	getline(cin, line); // level name

	// Parse colors
	getline(cin, line); // #colors
	vector<int> agent_colors(10);
	vector<int> box_colors(26);
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
			n_cols = line.length();
		getline(cin, line);
	}


	// Parse initial state
	vector<vector<bool>> walls(n_rows);
	vector<vector<int>> agents(n_rows);
	vector<vector<char>> boxes(n_rows);
	vector<vector<char>> goals(n_rows);

	for (int i; i < n_rows; i++) {
		walls.push_back(vector<bool>(n_cols));
		// walls[i] = vector<bool>(n_cols);
		agents.push_back(vector<int>(n_cols));
		boxes.push_back(vector<char>(n_cols));
		goals.push_back(vector<char>(n_cols));
	}

	for (int i = 0; i < n_rows; i++) {
		line = initial_state_lines[i];
		for (int j = 0; j < line.length(); j++) {
			char c = line[j];
			if (c == ' ') {
				continue;
			} else if (c == '+') {
				walls[i][j] = true;
			} else if (c >= '0' and c <= '9') {
				agents[i][j] = c - '0';
			} else {
				boxes[i][j] = c;
			}
		}
	}

	getline(cin, line);
	for (int i = 0; line[0] != '#'; i++) {
		for (int j = 0; j < line.length(); j++) {
			char c = line[j];
			if (c == ' ' || c == '+') {
				continue;
			} else {
				goals[i][j] = c;
			}
		}
		getline(cin, line);
	}
}

// State.walls = &walls
// State.boxes = &boxes
// State.goals = &goals
