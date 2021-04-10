#ifndef __GRAPHSEARCH_H
#define __GRAPHSEARCH_H

#include <vector>

#include "frontier.h"
#include "state.h"
#include "action.h"

vector<vector<Action>> search(State initial_state, Frontier &frontier);

#endif // __GRAPHSEARCH_H
