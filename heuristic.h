#ifndef __HEURISTIC_H
#define __HEURISTIC_H

#include "state.h"

class HeuristicHelper {
	public:
		bool operator()(const State* state1, const State* state2) const;
		int h(const State* state) const;
};

#endif // __HEURISTIC_H
