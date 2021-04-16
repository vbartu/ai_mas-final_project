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

/* FrontierBestFS */

void FrontierBestFS::add(State* state) {
  this->queue.push_front(state);
  this->pq.push(state);
  // printf("pq size: %d\n",this->pq.size());
  this->set.insert(state);
}

State* FrontierBestFS::pop() {
	State* state = this->pq.top();
  this->pq.pop();
	this->set.erase(state);
  return state;
}

bool FrontierBestFS::is_empty() {
  return this->queue.empty();
}

int FrontierBestFS::size() {
  return this->queue.size();
}

bool FrontierBestFS::contains(State* state) {
	return this->set.count(state);
}

string FrontierBestFS::getName() {
  return "best-first search";
}