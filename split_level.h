#ifndef __SPLIT_LEVEL_H
#define __SPLIT_LEVEL_H

#include <vector>

#include "action.h"
#include "world.h"

using namespace std;

vector<vector<Action>> split_level(vector<vector<char>> boxes,
		vector<vector<char>> goals, vector<int> agent_rows,
		vector<int> agent_cols, umap_t initial_map, umap_t goals_map);

#endif // __SPLIT_LEVEL_H
