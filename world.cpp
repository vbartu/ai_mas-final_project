#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>

#include "world.h"

using namespace std;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


World::World(umap_t intial_map)
{
	this->current_time = 1;
	this->agent_times = vector<int>(n_agents, 0);
	this->level_map.push_back(intial_map);
}

bool World::is_conflict(int time, coordinates_t next_pos)
{
	if (this->level_map[time-1].count(next_pos)) {
		cerr << "Conflict1: " << this->level_map[time-1][next_pos] << endl;
		return true;
	}

	if (this->level_map[time].count(next_pos)) {
		cerr << "Conflict2: " << this->level_map[time][next_pos] << endl;
		return true;
	}
	return false;
}

bool World::update_position(int agent_id, int time, Action action,
		AgentState* state)
{
	assert(!pthread_mutex_lock(&mtx));
	while (time > this->current_time) {
		cerr << "Stopped " << agent_id << endl;
		assert(!pthread_cond_wait(&cond, &mtx));
	}

	if (time == this->level_map.size()) {
		this->level_map.push_back(this->level_map.back());
	}

	bool conflicts = false;
	coordinates_t agent_pos = {state->agent_row, state->agent_col};
	if (action.type == ActionType::MOVE) {
		coordinates_t agent_next = add(agent_pos, {action.ard, action.acd});
		conflicts = is_conflict(time, agent_next);
		if (!conflicts) {
			char agent = this->level_map[time][agent_pos];
			this->level_map[time].erase(agent_pos);
			this->level_map[time][agent_next] = agent;
		}

	} else if (action.type == ActionType::PUSH) {
		coordinates_t box_pos = add(agent_pos, {action.ard, action.acd});
		coordinates_t box_next = add(box_pos, {action.brd, action.bcd});
		conflicts = is_conflict(time, box_next);
		if (!conflicts) {
			char box = this->level_map[time][box_pos];
			this->level_map[time][box_next] = box;
			char agent = this->level_map[time][agent_pos];
			this->level_map[time][box_pos] = agent;
			this->level_map[time].erase(agent_pos);
		}

	} else if (action.type == ActionType::PULL) {
		coordinates_t agent_next = add(agent_pos, {action.ard, action.acd});
		coordinates_t box_pos = sub(agent_pos, {action.brd, action.bcd});
		conflicts = is_conflict(time, agent_next);
		if (!conflicts) {
			char agent = this->level_map[time][agent_pos];
			this->level_map[time][agent_next] = agent;
			char box = this->level_map[time][box_pos];
			this->level_map[time][agent_pos] = box;
			this->level_map[time].erase(box_pos);
		}
	}

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

	assert(!pthread_mutex_unlock(&mtx));
	return !conflicts;
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

string World::repr()
{
	string lines;
	for (int i = 0; i < n_rows; i++) {
		string line;
		for (int j = 0; j < n_cols; j++) {
			if (this->level_map[this->current_time-1].count((coordinates_t){i,j})) {
				line += this->level_map[this->current_time-1][(coordinates_t){i,j}];
			} else {
				line += " ";
			}
		}
		lines += line;
		lines += "\n";
	}
	return lines;
}

