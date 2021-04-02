#include <vector>
#include <string>
#include <iostream>
#include <cstdio>

using namespace std;

enum class action_type {
	NOOP = 0,
	MOVE,
	PUSH,
  PULL
};

class Action {
  public:
    string name;
    int type;
    int ard; // horisontal displacement agent
    int acd; // vertical displacement agent
    int brd; // horisontal displacement box
    int bcd; //vertical displacement box

    Action() {};

    // Action(string Aname, int Atype, int Aard, int Aacd, int Abrd, int Abcd){
    //   name = Aname;
    //   type = Atype;
    //   ard = Aard;
    //   acd = Aacd;
    //   brd = Abrd;
    //   bcd = Abcd;
    // };

    void NoOp() {
      name = "NoOp";
      type = NOOP;
      ard = 0;
      acd = 0;
      brd = 0;
      bcd = 0;
      cout << "ok ciao" << endl;
    }
    // void NoOp(string "NoOp", int type = NOOP, int ard = 0, int acd = 0, int brd = 0, int bcd = 0) {};
    //
    // void MoveN(string "Move(N)", int Atype = MOVE, int Aard = -1, int Aacd = 0, int Abrd = 0, int Abcd = 0) {};
    // void MoveS(string "Move(S)", int Atype = MOVE, int Aard = 1, int Aacd = 0, int Abrd = 0, int Abcd = 0) {};
    // void MoveE(string "Move(E)", int Atype = MOVE, int Aard = 0, int Aacd = 1, int Abrd = 0, int Abcd = 0) {};
    // void MoveW(string "Move(W)", int Atype = MOVE, int Aard = 0, int Aacd = -1, int Abrd = 0, int Abcd = 0) {};
    //
    // void PushNN(string "Push(N,N)", int Atype = PUSH, int Aard = -1, int Aacd = 0, int Abrd = -1, int Abcd = 0) {};
    // void PushNE(string "Push(N,E)", int Atype = PUSH, int Aard = -1, int Aacd = 0, int Abrd = 0, int Abcd = 1) {};
    // void PushNW(string "Push(N,W)", int Atype = PUSH, int Aard = -1, int Aacd = 0, int Abrd = -0, int Abcd = -1) {};
    // void PushSS(string "Push(S,S)", int Atype = PUSH, int Aard = 1, int Aacd = 0, int Abrd = 1, int Abcd = 0) {};
    // void PushSE(string "Push(S,E)", int Atype = PUSH, int Aard = 1, int Aacd = 0, int Abrd = 0, int Abcd = 1) {};
    // void PushSW(string "Push(S,W)", int Atype = PUSH, int Aard = 1, int Aacd = 0, int Abrd = 0, int Abcd = -1) {};
    // void PushEN(string "Push(E,N)", int Atype = PUSH, int Aard = 0, int Aacd = 1, int Abrd = -1, int Abcd = 0) {};
    // void PushES(string "Push(E,S)", int Atype = PUSH, int Aard = 0, int Aacd = 1, int Abrd = 1, int Abcd = 0) {};
    // void PushEE(string "Push(E,E)", int Atype = PUSH, int Aard = 0, int Aacd = 1, int Abrd = 0, int Abcd = 1) {};
    // void PushWN(string "Push(W,N)", int Atype = PUSH, int Aard = 0, int Aacd = -1, int Abrd = -1, int Abcd = 0) {};
    // void PushWS(string "Push(W,S)", int Atype = PUSH, int Aard = 0, int Aacd = -1, int Abrd = 1, int Abcd = 0) {};
    // void PushWW(string "Push(W,W)", int Atype = PUSH, int Aard = 0, int Aacd = -1, int Abrd = 0, int Abcd = -1) {};
    //
    // void PullNN(string "Pull(N,N)", int Atype = PULL, int Aard = -1, int Aacd = 0, int Abrd = -1, int Abcd = 0) {};
    // void PullNE(string "Pull(N,E)", int Atype = PULL, int Aard = -1, int Aacd = 0, int Abrd = 0, int Abcd = 1) {};
    // void PullNW(string "Pull(N,W)", int Atype = PULL, int Aard = -1, int Aacd = 0, int Abrd = 0, int Abcd = -1) {};
    // void PullSS(string "Pull(S,S)", int Atype = PULL, int Aard = 1, int Aacd = 0, int Abrd = 1, int Abcd = 0) {};
    // void PullSE(string "Pull(S,E)", int Atype = PULL, int Aard = 1, int Aacd = 0, int Abrd = 0, int Abcd = 1) {};
    // void PullSW(string "Pull(S,W)", int Atype = PULL, int Aard = 1, int Aacd = 0, int Abrd = 0, int Abcd = -1) {};
    // void PullEN(string "Pull(E,N)", int Atype = PULL, int Aard = 0, int Aacd = 1, int Abrd = -1, int Abcd = 0) {};
    // void PullES(string "Pull(E,S)", int Atype = PULL, int Aard = 0, int Aacd = 1, int Abrd = 1, int Abcd = 0) {};
    // void PullEE(string "Pull(E,E)", int Atype = PULL, int Aard = 0, int Aacd = 1, int Abrd = 0, int Abcd = 1) {};
    // void PullWE(string "Pull(W,N)", int Atype = PULL, int Aard = 0, int Aacd = -1, int Abrd = -1, int Abcd = 0) {};
    // void PullWS(string "Pull(W,S)", int Atype = PULL, int Aard = 0, int Aacd = -1, int Abrd = 1, int Abcd = 0) {};
    // void PullWW(string "Pull(W,W)", int Atype = PULL, int Aard = 0, int Aacd = -1, int Abrd = 0, int Abcd =-1) {};
};

// int main(){
//
//   Action action;
//   action.NoOp();
//
//   vector<int> act = {{0}, {1}};
//
//   for (int i = 0; i < act.size(); i++) {
//     if (act[i] == NOOP) {
//       cout << "ok"<< endl;
//     }
//   }
//
//   return 0;
// }
