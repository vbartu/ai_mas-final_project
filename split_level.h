#ifndef __SPLIT_LEVEL_H
#define __SPLIT_LEVEL_H

#include <vector>

#include "action.h"
#include "communication.h"

using namespace std;

vector<vector<Action>> split_level(vector<vector<char>> boxes,
		vector<vector<char>> goals, vector<int> agent_rows,
		vector<int> agent_cols, umap_t initial_map, vector<int> box_color,
		vector<int> agent_colors);

#endif // __SPLIT_LEVEL_H
