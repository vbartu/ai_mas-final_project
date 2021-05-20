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

/* ConflictFrontierBFS */

void ConflictFrontierBFS::add(ConflictState* state) {
	this->queue.push_front(state);
	this->set.insert(state);
}


ConflictState* ConflictFrontierBFS::pop() {
	ConflictState* state = this->queue.back();
	this->queue.pop_back();
	this->set.erase(state);
	return state;
}


bool ConflictFrontierBFS::is_empty() {
	return this->queue.empty();
}

int ConflictFrontierBFS::size() {
	return this->queue.size();
}

bool ConflictFrontierBFS::contains(ConflictState* state) {
	return this->set.count(state);
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

/** BestFS */

void ConflictFrontierBestFS::add(ConflictState* state) {
	this->pq.push(state);
	this->set.insert(state);
}


ConflictState* ConflictFrontierBestFS::pop() {
	ConflictState* state = this->pq.top();
	this->pq.pop();
	this->set.erase(state);
	return state;
}


bool ConflictFrontierBestFS::is_empty() {
	return this->pq.empty();
}

int ConflictFrontierBestFS::size() {
	return this->pq.size();
}

bool ConflictFrontierBestFS::contains(ConflictState* state) {
	return this->set.count(state);
}
