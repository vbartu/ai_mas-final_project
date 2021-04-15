#ifndef __FRONTIER_H
#define __FRONTIER_H

#include <string>
#include <deque>
#include <unordered_set>

#include "state.h"

using namespace std;

class Frontier {
  public:
    virtual void add(State* state) = 0;
    virtual State* pop() = 0;
    virtual bool is_empty() = 0;
    virtual int size() = 0;
    virtual bool contains(State* state) = 0;
    virtual string getName() = 0;
};


class FrontierBFS: public Frontier {
  public:
    deque<State*> queue;
    unordered_set<State*, HashHelper, EqualHelper> set;

    void add(State* state);
    State* pop();
    bool is_empty();
    int size();
    bool contains(State* state);
    string getName();
};

#endif
