#include <vector>
#include <string>
#include <iostream>
#include <cstdio>

using namespace std;

enum ActionType {
	NoOp = 0,
	Move,
	Push,
  Pull,
};

enum Action {

		NoOp("NoOp", ActionType.NoOp, 0, 0, 0, 0),

	  MoveN("Move(N)", ActionType.Move, -1, 0, 0, 0),
	  MoveS("Move(S)", ActionType.Move, 1, 0, 0, 0),
	  MoveE("Move(E)", ActionType.Move, 0, 1, 0, 0),
	  MoveW("Move(W)", ActionType.Move, 0, -1, 0, 0),

		PushNN("Push(N,N)", ActionType.PUSH, -1, 0, -1, 0),
		PushNE("Push(N,E)", ActionType.PUSH, -1, 0, 0, 1),
		PushNW("Push(N,W)", ActionType.PUSH, -1, 0, 0, -1),
		PushSS("Push(S,S)", ActionType.PUSH, 1, 0, 1, 0),
		PushSE("Push(S,E)", ActionType.PUSH, 1, 0, 0, 1),
		PushSW("Push(S,W)", ActionType.PUSH, 1, 0, 0, -1),
		PushEN("Push(E,N)", ActionType.PUSH, 0, 1, -1, 0),
		PushES("Push(E,S)", ActionType.PUSH, 0, 1, 1, 0),
		PushEE("Push(E,E)", ActionType.PUSH, 0, 1, 0, 1),
		PushWN("Push(W,N)", ActionType.PUSH, 0, -1, -1, 0),
		PushWS("Push(W,S)", ActionType.PUSH, 0, -1, 1, 0),
		PushWW("Push(W,W)", ActionType.PUSH, 0, -1, 0, -1),

		PullNN("Pull(N,N)", ActionType.PULL, -1, 0, -1, 0),
		PullNE("Pull(N,E)", ActionType.PULL, -1, 0, 0, 1),
		PullNW("Pull(N,W)", ActionType.PULL, -1, 0, 0, -1),
		PullSS("Pull(S,S)", ActionType.PULL, 1, 0, 1, 0),
		PullSE("Pull(S,E)", ActionType.PULL, 1, 0, 0, 1),
		PullSW("Pull(S,W)", ActionType.PULL, 1, 0, 0, -1),
		PullEN("Pull(E,N)", ActionType.PULL, 0, 1, -1, 0),
		PullES("Pull(E,S)", ActionType.PULL, 0, 1, 1, 0),
		PullEE("Pull(E,E)", ActionType.PULL, 0, 1, 0, 1),
		PullWE("Pull(W,N)", ActionType.PULL, 0,-1, -1, 0),
		PullWS("Pull(W,S)", ActionType.PULL, 0,-1, 1, 0),
		PullWW("Pull(W,W)", ActionType.PULL, 0,-1, 0, -1);

		final name;
	  final ActionType type;
	  final agentRowDelta; // vertical displacement of agent (-1,0,+1)
	  final agentColDelta; // horisontal displacement of agent (-1,0,+1)
	  final boxRowDelta; // vertical diplacement of box (-1,0,+1)
	  final boxColDelta; // horisontal displacement of box (-1,0,+1)

	  Action(String name, ActionType type, int ard, int acd, int brd, int bcd)
	  {
	      this.name = name;
	      this.type = type;
	      this.agentRowDelta = ard;
	      this.agentColDelta = acd;
	      this.boxRowDelta = brd;
	      this.boxColDelta = bcd;
	  }
}


// class Action {
//   public:
//      name;
//      type;
//      ard; // horisontal displacement agent
//      acd; // vertical displacement agent
//      brd; // horisontal displacement box
//      bcd; //vertical displacement box
//
//     Action();
//
//     Action( Aname,  Atype,  Aard,  Aacd,  Abrd,  Abcd){
//       name = Aname;
//       type = Atype;
//       ard = Aard;
//       acd = Aacd;
//       brd = Abrd;
//       bcd = Abcd;
//     };
//
//      NoOp() {
//       name = "NoOp";
//       type = NOOP;
//       ard = 0;
//       acd = 0;
//       brd = 0;
//       bcd = 0;
//     }

    //  NoOp( "NoOp",  type = NOOP,  ard = 0,  acd = 0,  brd = 0,  bcd = 0);
    //
    //  MoveN( "Move(N)",  ActionType.MOVE,  -1, , 0, 0);
    //  MoveS( "Move(S)",  ActionType.MOVE,  1, , 0, 0);
    //  MoveE( "Move(E)",  ActionType.MOVE,  0,1, 0, 0);
    //  MoveW( "Move(W)",  ActionType.MOVE,  0,-1, 0, 0);
    //
//      PushNN( "Push(N,N)",  ActionType.PUSH,  -1, , -1, 0);
//      PushNE( "Push(N,E)",  ActionType.PUSH,  -1, , 0, 1);
//      PushNW( "Push(N,W)",  ActionType.PUSH,  -1, , -0, -1);
//      PushSS( "Push(S,S)",  ActionType.PUSH,  1, , 1, 0);
//      PushSE( "Push(S,E)",  ActionType.PUSH,  1, , 0, 1);
//      PushSW( "Push(S,W)",  ActionType.PUSH,  1, , 0, -1);
//      PushEN( "Push(E,N)",  ActionType.PUSH,  0,1, -1, 0);
//      PushES( "Push(E,S)",  ActionType.PUSH,  0,1, 1, 0);
//      PushEE( "Push(E,E)",  ActionType.PUSH,  0,1, 0, 1);
//      PushWN( "Push(W,N)",  ActionType.PUSH,  0,-1, -1, 0);
//      PushWS( "Push(W,S)",  ActionType.PUSH,  0,-1, 1, 0);
//      PushWW( "Push(W,W)",  ActionType.PUSH,  0,-1, 0, -1);
//
//      PullNN( "Pull(N,N)",  ActionType.PULL,  -1, , -1, 0);
//      PullNE( "Pull(N,E)",  ActionType.PULL,  -1, , 0, 1);
//      PullNW( "Pull(N,W)",  ActionType.PULL,  -1, , 0, -1);
//      PullSS( "Pull(S,S)",  ActionType.PULL,  1, , 1, 0);
//      PullSE( "Pull(S,E)",  ActionType.PULL,  1, , 0, 1);
//      PullSW( "Pull(S,W)",  ActionType.PULL,  1, , 0, -1);
//      PullEN( "Pull(E,N)",  ActionType.PULL,  0,1, -1, 0);
//      PullES( "Pull(E,S)",  ActionType.PULL,  0,1, 1, 0);
//      PullEE( "Pull(E,E)",  ActionType.PULL,  0,1, 0, 1);
//      PullWE( "Pull(W,N)",  ActionType.PULL,  0,-1, -1, 0);
//      PullWS( "Pull(W,S)",  ActionType.PULL,  0,-1, 1, 0);
//      PullWW( "Pull(W,W)",  ActionType.PULL,  0,-1, 0,  Abcd =-1);
// };
