#include <iostream>
#include <assert.h>
#include <stdio.h>

#include "communication.h"

using namespace std;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


World::World(umap_t intial_map, int number_agents)
{
	this->current_time = 1;
	this->agent_times = vector<int>(number_agents, 0);
	this->level_map.push_back(intial_map);
	//this->level_map.push_back(this->level_map.back());
}

bool World::conflict(int time, int row, int col)
{
		bool result;
		//pthread_mutex_lock(&this->mtx);
		coordinates_t c = {row, col};
		result = (time < this->level_map.size()+1 && this->level_map[time+1].count(c)
			|| time < this->level_map.size() && this->level_map[time].count(c));
		for (auto& it: level_map[time]) {
			cerr << it.second << ": " << it.first.x << ", " << it.first.y << endl;
		}
		//pthread_mutex_unlock(&this->mtx);
		return result;
}

void World::update_postion(int agent_id, int time, int old_row, int old_col,
		int row, int col, bool last)
{
	assert(!pthread_mutex_lock(&mtx));
	while (time > this->current_time) {
		cerr << "Stopped " << agent_id << endl;
		assert(!pthread_cond_wait(&cond, &mtx));
	}

	if (time == this->level_map.size()) {
		this->level_map.push_back(this->level_map.back());
	}

	char obj = this->level_map[time][(coordinates_t){old_row, old_col}];
	this->level_map[time].erase((coordinates_t){old_row, old_col});
	this->level_map[time][(coordinates_t){row, col}] = obj;

	if (last) {
		this->agent_times[agent_id] = time;
		bool update = true;
		for (int agent_time : this->agent_times) {
			if (agent_time < time) {
				update = false;
				break;
			}
		}
		if (update) {
			this->current_time++;
			assert(!pthread_cond_broadcast(&cond));
		}
	}

	assert(!pthread_mutex_unlock(&mtx));
}

void World::finished(int agent_id)
{
	assert(!pthread_mutex_lock(&mtx));
	cerr << "Agent " << agent_id << " finished" << endl;
	this->agent_times[agent_id] = 999999999;
	bool update = true;
	for (int agent_time : this->agent_times) {
		if (agent_time < this->current_time) {
			update = false;
			break;
		}
	}
	if (update) {
		this->current_time++;
		assert(!pthread_cond_broadcast(&cond));
	}
	assert(!pthread_mutex_unlock(&mtx));
}

umap_t World::get_positions(int time)
{
	assert(!pthread_mutex_lock(&mtx));
	while (time > this->current_time) {
		assert(!pthread_cond_wait(&cond, &mtx));
	}
	umap_t result = this->level_map[time];
	assert(!pthread_mutex_unlock(&mtx));
	return result;
}
