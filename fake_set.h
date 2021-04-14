#ifndef __FAKE_SET_H
#define __FAKE_SET_H

#include <vector>

#include "state.h"

using namespace std;

class FakeSet {
public:
	vector<State*> storage;

	FakeSet();

	void insert(State* state);

	int count(State* state);

	void erase(State* state);

	int size();
};

#endif
