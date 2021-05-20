#ifndef __GRAPHSEARCH_H
#define __GRAPHSEARCH_H

#include <vector>

#include "bdi_agent_state.h"
#include "conflict_state.h"

using namespace std;

vector<CAction> search(AgentState* initial_state, int limit);

vector<vector<CAction>> conflict_search(ConflictState* initial_state);

#endif // __GRAPHSEARCH_H
