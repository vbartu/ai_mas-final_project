#include <iostream>
#include <pthread.h>
#include <stdlib.h>

#include "split_level.h"
#include "frontier.h"
#include "graphsearch.h"

using namespace std;

static vector<vector<Action>> global_result;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

typedef struct state_aux_t {
	State* state;
	int agent_id;
} state_aux_t;

void* agent_thread(void* state_aux2) {
	cerr << "Thread " << ((state_aux_t*)state_aux2)->agent_id << endl;
	state_aux_t* state_aux = (state_aux_t*) state_aux2;
	FrontierBestFS frontier;

	vector<vector<Action>> result = search(state_aux->state, frontier);

	cerr << "Result thread " << state_aux->agent_id << ": " << result.size() << endl;

	vector<Action> aux;
	for (int i = 0; i < result.size(); i++) {
		aux.push_back(result[i][0]);
	}

	pthread_mutex_lock(&mtx);
	while (!(global_result.size() > state_aux->agent_id)) {
		global_result.push_back(vector<Action>(0));
	}
	global_result[state_aux->agent_id] = aux;
	
	for (int i = result.size(); i < global_result.size(); i++) {
		global_result[i].push_back(actions[0]);
	}
	pthread_mutex_unlock(&mtx);

	free((state_aux_t*)state_aux2);
	return NULL;
}

vector<vector<Action>> split_level(State* initial_state)
{
	cerr << "Splitted levels:" << endl;

	int number_of_levels = initial_state->agent_rows.size();

	vector<pthread_t> threads;

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

		state_aux_t* state_aux = (state_aux_t*) malloc(sizeof(state_aux_t));
		state_aux->state = state;
		state_aux->agent_id = i;
		pthread_t thread;
		int err = pthread_create(&thread, NULL, agent_thread, state_aux);
		threads.push_back(thread);
	}
	cerr << "Joining threads" << endl;
	for (pthread_t thread : threads) {
		pthread_join(thread, NULL);
	}
	cerr << "Threads joined" << endl;
	int max_d = 0;
	for (int i = 0; i < global_result.size(); i++) {
		if (global_result[i].size() > max_d)
			max_d = global_result[i].size();
	}
	for (int i = 0; i < global_result.size(); i++) {
		while (global_result[i].size() < max_d) {
			global_result[i].push_back(actions[0]);
		}
	}

	return global_result;
}
