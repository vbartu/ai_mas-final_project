#ifndef __HEURISTIC_H
#define __HEURISTIC_H

#include "bdi_agent_state.h"
#include "conflict_state.h"

class HeuristicHelper {
	public:
		bool operator()(const AgentState* state1, const AgentState* state2) const;
		int h(const AgentState* state) const;
};

class ConflictHeuristicHelper {
	public:
		bool operator()(const ConflictState* state1, const ConflictState* state2) const;
		int h(const ConflictState* state) const;
};

#endif // __HEURISTIC_H
