#include <cstdio>
#include <string>
#include <vector>
#include "state.h"
#include <deque>

using namespace std;

class Frontier {
  public:
    void add(State state);
    State pop();
    bool is_empty();
    int size();
    bool contains(State state);
    string getName();
};


class FrontierBFS: public Frontier {
  public:
    deque<State> queue;
    vector<State> set;
};
