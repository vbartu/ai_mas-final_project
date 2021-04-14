#include "fake_set.h"

using namespace std;

FakeSet::FakeSet(){};

void FakeSet::insert(State* state)
{
	for (State* other: this->storage) {
		if (*state == *other) {
			return;
		}
	}
	this->storage.push_back(state);
}

int FakeSet::count(State* state)
{
	for (State* other: this->storage) {
		if (*state == *other) {
			return 1;
		}
	}
	return 0;
}

void FakeSet::erase(State* state)
{
	for (int i = 0; i < this->storage.size(); i++) {
		if (*state == *(storage[i])) {
			this->storage.erase(this->storage.begin()+i);
			return;
		}
	}
}

int FakeSet::size()
{
	return this->storage.size();
}
