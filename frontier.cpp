#include "frontier.h"

using namespace std;


/* FrontierBFS */

void FrontierBFS::add(AgentState* state) {
	this->queue.push_front(state);
	this->set.insert(state);
}


AgentState* FrontierBFS::pop() {
	AgentState* state = this->queue.back();
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

bool FrontierBFS::contains(AgentState* state) {
	return this->set.count(state);
}

string FrontierBFS::getName() {
	return "breadth-first search";
}

/* FrontierBestFS */

void FrontierBestFS::add(AgentState* state) {
	this->pq.push(state);
	this->set.insert(state);
}

AgentState* FrontierBestFS::pop() {
	AgentState* state = this->pq.top();
	this->pq.pop();
	this->set.erase(state);
	return state;
}

bool FrontierBestFS::is_empty() {
	return this->pq.empty();
}

int FrontierBestFS::size() {
	return this->pq.size();
}

bool FrontierBestFS::contains(AgentState* state) {
	return this->set.count(state);
}

string FrontierBestFS::getName() {
	return "best-first search";
}
