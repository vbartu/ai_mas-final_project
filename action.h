#include <vector>
#include <string>

using namespace std;

#define lenActionList 29

enum ActionType {
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

		Action(int x) {

			switch (x) {

				case 0:
					name = "NoOp";
					type = NOOP;
					ard = 0;
					acd = 0;
					brd = 0;
					bcd = 0;
					break;

				case 1:
					name = "Move(N)";
					type = MOVE;
					ard = -1;
					acd = 0;
					brd = 0;
					bcd = 0;
					break;

				case 2:
					name = "Move(S)";
					type = MOVE;
					ard = 1;
					acd = 0;
					brd = 0;
					bcd = 0;
					break;

				case 3:
					name = "Move(E)";
					type = MOVE;
					ard = 0;
					acd = 1;
					brd = 0;
					bcd = 0;
					break;

				case 4:
					name = "Move(W)";
					type = MOVE;
					ard = 0;
					acd = -1;
					brd = 0;
					bcd = 0;
					break;

				case 5:
					name = "Push(N,N)";
					type = PUSH;
					ard = -1;
					acd = 0;
					brd = -1;
					bcd = 0;
					break;

				case 6:
					name = "Push(N,E)";
					type = PUSH;
					ard = -1;
					acd = 0;
					brd = 0;
					bcd = 1;
					break;

				case 7:
					name = "Push(N,W)";
					type = PUSH;
					ard = -1;
					acd = 0;
					brd = 0;
					bcd = -1;
					break;

				case 8:
					name = "Push(S,S)";
					type = PUSH;
					ard = 1;
					acd = 0;
					brd = 1;
					bcd = 0;
					break;

				case 9:
					name = "Push(S,E)";
					type = PUSH;
					ard = 1;
					acd = 0;
					brd = 0;
					bcd = 1;
					break;

				case 10:
					name = "Push(S,W)";
					type = PUSH;
					ard = 1;
					acd = 0;
					brd = 0;
					bcd = -1;
					break;

				case 11:
					name = "Push(E,N)";
					type = PUSH;
					ard = 1;
					acd = 0;
					brd = 0;
					bcd = -1;
					break;

				case 12:
					name = "Push(E,S)";
					type = PUSH;
					ard = 0;
					acd = 1;
					brd = 1;
					bcd = 0;
					break;

				case 13:
					name = "Push(E,E)";
					type = PUSH;
					ard = 0;
					acd = 1;
					brd = 0;
					bcd = 1;
					break;

				case 14:
					name = "Push(W,N)";
					type = PUSH;
					ard = 0;
					acd = -1;
					brd = -1;
					bcd = 0;
					break;

				case 15:
					name = "Push(W,S)";
					type = PUSH;
					ard = 0;
					acd = -1;
					brd = 1;
					bcd = 0;
					break;

				case 16:
					name = "Push(W,W)";
					type = PUSH;
					ard = 0;
					acd = -1;
					brd = 0;
					bcd = -1;
					break;

				case 17:
					name = "Pull(N,N)";
					type = PULL;
					ard = -1;
					acd = 0;
					brd = -1;
					bcd = 0;
					break;

				case 18:
					name = "Pull(N,E)";
					type = PULL;
					ard = -1;
					acd = 0;
					brd = 0;
					bcd = 1;
					break;

				case 19:
					name = "Pull(N,W)";
					type = PULL;
					ard = -1;
					acd = 0;
					brd = 0;
					bcd = -1;
					break;

				case 20:
					name = "Pull(S,S)";
					type = PULL;
					ard = 1;
					acd = 0;
					brd = 1;
					bcd = 0;
					break;

				case 21:
					name = "Pull(S,E)";
					type = PULL;
					ard = 1;
					acd = 0;
					brd = 0;
					bcd = 1;
					break;

				case 22:
					name = "Pull(S,W)";
					type = PULL;
					ard = 1;
					acd = 0;
					brd = 0;
					bcd = -1;
					break;

				case 23:
					name = "Pull(E,N)";
					type = PULL;
					ard = 1;
					acd = 0;
					brd = 0;
					bcd = -1;
					break;

				case 24:
					name = "Pull(E,S)";
					type = PULL;
					ard = 0;
					acd = 1;
					brd = 1;
					bcd = 0;
					break;

				case 25:
					name = "Pull(E,E)";
					type = PULL;
					ard = 0;
					acd = 1;
					brd = 0;
					bcd = 1;
					break;

				case 26:
					name = "Pull(W,N)";
					type = PULL;
					ard = 0;
					acd = -1;
					brd = -1;
					bcd = 0;
					break;

				case 27:
					name = "Pull(W,S)";
					type = PULL;
					ard = 0;
					acd = -1;
					brd = 1;
					bcd = 0;
					break;

				case 28:
					name = "Pull(W,W)";
					type = PULL;
					ard = 0;
					acd = -1;
					brd = 0;
					bcd = -1;
					break;
			}
		}
};
