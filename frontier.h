#ifndef __FRONTIER_H
#define __FRONTIER_H

#include <string>
#include <deque>
#include <unordered_set>
#include <queue>

#include "bdi_agent_state.h"
#include "heuristic.h"

using namespace std;

class Frontier {
  public:
    virtual void add(AgentState* state) = 0;
    virtual AgentState* pop() = 0;
    virtual bool is_empty() = 0;
    virtual int size() = 0;
    virtual bool contains(AgentState* state) = 0;
    virtual string getName() = 0;
};

class FrontierBFS: public Frontier {
  public:
    deque<AgentState*> queue;
    unordered_set<AgentState*, HashHelper, EqualHelper> set;

    void add(AgentState* state);
    AgentState* pop();
    bool is_empty();
    int size();
    bool contains(AgentState* state);
    string getName();
};

class FrontierBestFS: public Frontier {
  public:
    unordered_set<AgentState*, HashHelper, EqualHelper> set;
    priority_queue<AgentState*, vector<AgentState*>, HeuristicHelper> pq;

    void add(AgentState* state);
    AgentState* pop();
    bool is_empty();
    int size();
    bool contains(AgentState* state);
    string getName();
};

#endif
