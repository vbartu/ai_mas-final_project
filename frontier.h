#include <cstdio>
#include <string>
#include <vector>
#include "state.h"
#include <deque>

using namespace std;

class Frontier {
  public:
    virtual void add(State state) = 0;
    virtual State pop() = 0;
    virtual bool is_empty() = 0;
    virtual int size() = 0;
    virtual bool contains(State state) = 0;
    virtual string getName() = 0;
};


class FrontierBFS: public Frontier {
  public:
    deque<State> queue;
    vector<State> set;
};

class FrontierDFS: public Frontier {
  public:
    deque<State> queue;
    vector<State> set;
};

class FrontierBestFirt(Frontier) {
  public:
    // heuristic heur;
    vector<State> queue;
    vector<State> set;
    int last_f = -1;
};
