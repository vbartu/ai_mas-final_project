#include <iostream>
#include <assert.h>

#include "communication.h"

using namespace std;

pthread_mutex_t Communication::mtx = PTHREAD_MUTEX_INITIALIZER;


Communication::Communication(umap_t intial_map)
{
	this->level_map.push_back(intial_map);
}

void Communication::update_postion(int time, int old_row, int old_col, int row,
	int col)
{
	pthread_mutex_lock(&this->mtx);
	while (this->level_map.size() <= time) {
		umap_t aux= this->level_map.back();
		this->level_map.push_back(aux);
	}
	char obj = this->level_map[time][coordinates_t {old_row, old_col}];
	//cerr << "obj: " << (int)obj << endl;
	this->level_map[time].erase(coordinates_t {old_row, old_col});
	this->level_map[time][coordinates_t {row, col}] = obj;


	for (int i = time+1; i < this->level_map.size(); i++) {
		this->level_map[time].erase(coordinates_t {old_row, old_col});
		this->level_map[time][coordinates_t {row, col}] = obj;
	}
	pthread_mutex_unlock(&this->mtx);
}

umap_t Communication::get_positions(int time)
{
	pthread_mutex_lock(&this->mtx);
	while (!(time < this->level_map.size())) {
		umap_t aux = this->level_map.back();
		this->level_map.push_back(aux);
	}
	umap_t aux = this->level_map[time];
	pthread_mutex_unlock(&this->mtx);
	return aux;
}
