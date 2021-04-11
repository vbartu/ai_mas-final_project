#include "frontier.h"
#include "state.h"

using namespace std;


/* FrontierBFS */

void FrontierBFS::add(State* state) {
  this->queue.push_front(state);
  this->set.insert(state);
}


State* FrontierBFS::pop() {
	State* state = this->queue.back();
	this->queue.pop_back();
	this->set.erase(state);
  return state;
}


bool FrontierBFS::is_empty() {
  return this->queue.empty();
}

int FrontierBFS::size() {
  return this->queue.size();
}

bool FrontierBFS::contains(State* state) {
	return this->set.count(state);
}

string FrontierBFS::getName() {
  return "breadth-first search";
}
