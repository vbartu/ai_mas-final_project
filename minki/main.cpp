#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>

#include "color.h"

using namespace std;

#define CLIENT_NAME "Final project client"

int main (int argc, char *argv[])
{
	cout << CLIENT_NAME << endl;
	cout << "#" << "Starting" << endl;

	// Creation of ifstream class object to read the file
	ifstream fin;

	// by default open mode = ios::in mode
	fin.open(argv[1]);

	string line;

	// Read a Line from File
	getline(fin, line); // #domain
	getline(fin, line); // hospital
	getline(fin, line); // #levelname
	getline(fin, line); // level name

	// Parse colors
	getline(fin, line); // #colors
	int agent_colors[10] = {};
	int box_colors[26] = {};
	getline(fin, line);
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

		getline(fin, line);
	}

	// Parse level size
	string initial_state_lines[100] = {};
	int n_rows = 0;
	int n_cols = 0;

	getline(fin, line);
	while (line[0] != '#') {
		initial_state_lines[n_rows++] = line;
		if (line.length() > n_cols)
			n_cols = line.length();
		getline(fin, line);
	}

	// Parse initial state
	bool walls[n_rows][n_cols] = {};
	int agents[n_rows][n_cols] = {};
	char boxes[n_rows][n_cols] = {};

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

	// Parse goal state
	char goals[n_rows][n_cols] = {};

	getline(fin, line);
	for (int i = 0; line[0] != '#'; i++) {
		for (int j = 0; j < line.length(); j++) {
			char c = line[j];
			if (c == ' ' || c == '+') {
				continue;
			} else {
				goals[i][j] = c;
			}
		}
		getline(fin, line);
	}

	// Close the file
    fin.close();

	printf("Finish\n");

	return 0;
}
