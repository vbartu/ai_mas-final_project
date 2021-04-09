#ifndef __FRONTIER_H
#define __FRONTIER_H

#include <cstdio>
#include <string>
#include <vector>
#include <deque>
#include "state.h"
// #include "action

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

    void add(State state);
    State pop();
    bool is_empty();
    int size();
    bool contains(State state);
    string getName();
};


class FrontierDFS: public Frontier {
  public:
    deque<State> queue;
    vector<State> set;

    void add(State state);
    State pop();
    bool is_empty();
    int size();
    bool contains(State state);
    string getName();
};


// class FrontierBestFirt(Frontier) {
//   public:
//     // heuristic heur;
//     vector<State> queue;
//     vector<State> set;
//     int last_f = -1;
// };

#endif
