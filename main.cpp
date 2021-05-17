#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>

#include "global.h"
#include "color.h"
#include "action.h"
#include "bdi_agent.h"

using namespace std;

#define CLIENT_NAME "Final project client"

/** Global variables */
static vector<vector<Action>> global_result;
static pthread_mutex_t global_result_mtx = PTHREAD_MUTEX_INITIALIZER;

/** Function prototypes */
static void* agent_thread(void* args);
static void split_level(umap_t initial_map);

/** Function definitions */
static void* agent_thread(void* args) {
	int agent_id = *((int*)args);
	BdiAgent agent = BdiAgent(agent_id);
	agent.run();

	pthread_mutex_lock(&global_result_mtx);
	while (global_result.size() <= agent_id) {
		global_result.push_back(vector<Action>(0));
	}
	//global_result[agent_id] = agent.final_plan;
	vector<Action> aux;
	for (CAction a : agent.final_plan) {
		Action b = a;
		aux.push_back(b);
	}
	global_result[agent_id] = aux;

	for (int i = agent.final_plan.size(); i < global_result.size(); i++) {
		global_result[i].push_back(actions[0]);
	}
	pthread_mutex_unlock(&global_result_mtx);

	delete((int*)args);
	return NULL;
}

static void split_level(umap_t initial_map)
{
	cerr << "Initial map size: " << initial_map.size() << endl;
	cerr << "Splitted into " << n_agents << " agents" << endl;

	BdiAgent::world.push_back(initial_map);
	//BdiAgent::current_time;

	// Create and join threads
	pthread_t threads[n_agents];
	for (int i = 0; i < n_agents; i++) {
		pthread_t thread;
		int* agent_id = new int(i);
		pthread_create(&thread, NULL, agent_thread, agent_id);
		threads[i] = thread;
	}
	cerr << "Joining threads" << endl;
	for (int i = 0; i < n_agents; i++) {
		pthread_join(threads[i], NULL);
	}
	cerr << "Threads joined" << endl;

	int max_depth = 0;
	for (int i = 0; i < global_result.size(); i++) {
		if (global_result[i].size() > max_depth)
			max_depth = global_result[i].size();
	}
	for (int i = 0; i < global_result.size(); i++) {
		while (global_result[i].size() < max_depth) {
			global_result[i].push_back(actions[0]);
		}
	}
}

/** Main function */
int main () {
	cout << CLIENT_NAME << endl;

	string line;
	getline(cin, line); // #domain
	getline(cin, line); // hospital
	getline(cin, line); // #levelname
	getline(cin, line); // level name

	// Parse colors
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
	n_rows = 0;
	n_cols = 0;

	getline(cin, line);
	while (line[0] != '#') {
		initial_state_lines[n_rows++] = line;

		if (line.length() > n_cols)
			n_cols = line.length() -1;
		getline(cin, line);
	}


	// Parse initial state
	vector<vector<char>> boxes;
	umap_t initial_map;

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
			coordinates_t coord;
			coord.x = i;
			coord.y = j;
			if (c == ' ') {
				continue;
			} else if (c == '+') {
				walls[i][j] = true;
			} else if ('0' <= c && c <= '9') {
				initial_map[coord] = c;
				agent_rows.push_back(i);
				agent_cols.push_back(j);
			} else if ('A' <= c && c <= 'Z') {
				initial_map[coord] = c;
				boxes[i][j] = c;
			}
		}
	}

	n_agents = agent_rows.size();

	getline(cin, line);
	for (int i = 0; line[0] != '#'; i++) {
		for (int j = 0; j < line.length(); j++) {
			char c = line[j];
			if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')) {
				cerr << "Goal " << c << " in " << i << ", " << j << endl;
				goals[i][j] = c;
				goals_map[(coordinates_t){i,j}] = c;
			}
		}
		getline(cin, line);
	}

	calculate_distances();

	split_level(initial_map);
	cerr << "Final Result length: " << global_result[0].size() << endl;

	// Send final plan to server
	int n_agents = global_result.size();
	for(int i = 0; i < global_result[0].size(); i++) {
		string join_action = "";
		for (int j = 0; j < global_result.size(); j++) {
			join_action += global_result[j][i].name;
			if (j != global_result.size()-1) {
				join_action += "|";
			}
		}
		cout << join_action << endl;
		getline(cin, line);
	}

	return 0;
}
