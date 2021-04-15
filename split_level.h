#ifndef __SPLIT_LEVEL_H
#define __SPLIT_LEVEL_H

#include <vector>

#include "state.h"
#include "action.h"

using namespace std;

vector<vector<Action>> split_level(State* initial_state);

#endif // __SPLIT_LEVEL_H
