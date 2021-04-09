#include <cstdio>
#include "state.h"
#include "action.h"
#include "frontier.h"

using namespace std;

class PriorityQueueNode {
    // def __init__(self, priority: 'int', state: 'State'):
    int priority;
    State state;

    PriorityQueueNode(){};

    PriorityQueueNode(int priority, State state) {
      this->priority = priority;
      this->state = state;
    }

    int __lt__( PriorityQueueNode other):
        return this->priority < other.priority

/* FrontierBFS */

void FrontierBFS::add(State state) {
  this->queue.push_front(state);
  bool found = false;
  for(int i = 0; i< set.size(); i++){
    if (this->set[i] == state) {
      found = true
      break;
    }
  if(!found) {
    this->set.push_back(state);
  }
  // this->set.insert(state);
}

State FrontierBFS::pop() {
  state = this->queue.pop_front();

  for(int i = 0; i< set.size(); i++){
    if (set[i] == state) {
      set.erase(set-begin() + i);
      break;
    }
  // this->set.erase(state);
  return state;
}

bool FrontierBFS::is_empty() {
  return this->queue.empty();
}

int FrontierBFS::size() {
  return this->queue.size();
}

bool FrontierBFS::contains(State state) {
  for(int i = 0; i< set.size(); i++){
    if (set[i] == state) {
      return true;
    }
  }
  return false;
  // return this->set.find(element) != this->set.end();
}

string FrontierBFS::getName() {
  return "breadth-first search"
}


/* FrontierDFS */

void FrontierDFS::add(State state) {
  this->queue.push_front(state);
  bool found = false;
  for(int i = 0; i< set.size(); i++){
    if (this->set[i] == state) {
      found = true
      break;
    }
  if(!found) {
    this->set.push_back(state);
  }
  // this->set.insert(state);
}

State FrontierDFS::pop() {
  state = this->queue.pop_back();
  for(int i = 0; i < set.size(); i++){
    if (set[i] == state) {
      set.erase(set-begin() + i);
      break;
    }
  // this->set.erase(state);
  return state;
}

bool FrontierBFS::is_empty() {
  return this->queue.empty();
}

int FrontierBFS::size() {
  return this->queue.size();
}

bool FrontierBFS::contains(State state) {
  for(int i = 0; i< set.size(); i++){
    if (set[i] == state) {
      return true;
    }
  }
  return false;
  // return this->set.find(element) != this->set.end();
}

string FrontierBFS::getName() {
  return "depth-first search"
}


/* FrontierBestFirst */
void FrontierDFS::add(State state) {
  this->queue.push_front(state);
  bool found = false;
  for(int i = 0; i< set.size(); i++){
    if (this->set[i] == state) {
      found = true
      break;
    }
  if(!found) {
    this->set.push_back(state);
  }
  // this->set.insert(state);
}

State FrontierDFS::pop() {
  state = this->queue.pop_back();
  for(int i = 0; i < set.size(); i++){
    if (set[i] == state) {
      set.erase(set-begin() + i);
      break;
    }
  // this->set.erase(state);
  return state;
}

bool FrontierBFS::is_empty() {
  return this->queue.empty();
}

int FrontierBFS::size() {
  return this->queue.size();
}

bool FrontierBFS::contains(State state) {
  for(int i = 0; i< set.size(); i++){
    if (set[i] == state) {
      return true;
    }
  }
  return false;
  // return this->set.find(element) != this->set.end();
}

string FrontierBFS::getName() {
  return "depth-first search"
}
