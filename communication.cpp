#include <assert.h>

#include "communication.h"

using namespace std;


pthread_mutex_t Communication::mtx = PTHREAD_MUTEX_INITIALIZER;

void Communication() {}

void Communication::update_postion(int time, int old_row, int old_col, int row,
	int col)
{
	assert(pthread_mutex_lock(&this->mtx));
	while (!(time < this->level_map.size())) {
		umap_t aux;
		if (this->level_map.size() > 0)
			aux= this->level_map.back();

		this->level_map.push_back(aux);
	}
	char obj = this->level_map[time][coordinates_t {old_row, old_col}];
	this->level_map[time].erase(coordinates_t {old_row, old_col});
	this->level_map[time][coordinates_t {row, col}];


	for (int i = time+1; i < this->level_map.size(); i++) {
		this->level_map[time].erase(coordinates_t {old_row, old_col});
		this->level_map[time][coordinates_t {row, col}];
	}
	assert(pthread_mutex_unlock(&this->mtx));
}

umap_t Communication::get_positions(int time)
{
	assert(pthread_mutex_lock(&this->mtx));
	while (!(time < this->level_map.size())) {
		umap_t aux;
		this->level_map.push_back(aux);
	}
	return this->level_map[time];
	assert(pthread_mutex_unlock(&this->mtx));
}
