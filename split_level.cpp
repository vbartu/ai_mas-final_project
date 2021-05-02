#include <iostream>
#include <pthread.h>
#include <stdlib.h>

#include "split_level.h"
#include "frontier.h"
#include "graphsearch.h"
#include "bdi_agent.h"

using namespace std;

static vector<vector<Action>> global_result;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void* agent_thread(void* args) {
	int agent_id = *((int*)args);
	BdiAgent agent = BdiAgent(agent_id);
	agent.run();

	vector<Action> aux = agent.final_plan;

	pthread_mutex_lock(&mtx);
	while (global_result.size() <= agent_id) {
		global_result.push_back(vector<Action>(0));
	}
	global_result[agent_id] = aux;
	
	for (int i = aux.size(); i < global_result.size(); i++) {
		global_result[i].push_back(actions[0]);
	}
	pthread_mutex_unlock(&mtx);

	delete((int*)args);
	return NULL;
}

vector<vector<Action>> split_level(vector<vector<char>> boxes,
		vector<vector<char>> goals, vector<int> agent_rows,
		vector<int> agent_cols, umap_t initial_map, umap_t goals_map)
{

	cerr << "Initial map: " << initial_map.size() << endl;

	int number_of_levels = agent_rows.size();
	cerr << "Splitted levels(" << number_of_levels << ")" << endl;

	World world(initial_map, number_of_levels);
	BdiAgent::world = world;
	BdiAgent::goals = goals;
	BdiAgent::goals_map = goals_map;


	vector<pthread_t> threads;

	for (int i = 0; i < number_of_levels; i++) {
		pthread_t thread;
		int* agent_id = new int(i);
		int err = pthread_create(&thread, NULL, agent_thread, agent_id);
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
