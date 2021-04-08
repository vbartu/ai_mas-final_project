#include "action.h"
#include <iostream>


using namespace std;


Action::Action(string name, int type, int ard, int acd, int brd, int bcd)
{
	this->name = name;
	this->type = type;
	this->ard = ard;
	this->acd = acd;
	this->brd = brd;
	this->bcd = bcd;
}

bool Action::operator==(const Action &action) const
{
	return this->name == action.name;
}



const Action NoOp = Action("NoOp", NOOP, 0, 0, 0, 0);
const Action MoveN = Action("Move(N)", MOVE, -1, 0, 0, 0);
const Action MoveS = Action("Move(S)", MOVE, 1, 0, 0, 0);
const Action MoveE = Action("Move(E)", MOVE, 0, 1, 0, 0);
const Action MoveW = Action("Move(W)", MOVE, 0, -1, 0, 0);
const Action PushNN = Action("Push(N,N)", PUSH, -1, 0, -1, 0);
const Action PushNE = Action("Push(N,E)", PUSH, -1, 0, 0, 1);
const Action PushNW = Action("Push(N,W)", PUSH, -1, 0, 0, -1);
const Action PushSS = Action("Push(S,S)", PUSH, 1, 0, 1, 0);
const Action PushSE = Action("Push(S,E)", PUSH, 1, 0, 0, 1);
const Action PushSW = Action("Push(S,W)", PUSH, 1, 0, 0, -1);
const Action PushEN = Action("Push(E,N)", PUSH, 0, 1, -1, 0);
const Action PushES = Action("Push(E,S)", PUSH, 0, 1, 1, 0);
const Action PushEE = Action("Push(E,E)", PUSH, 0, 1, 0, 1);
const Action PushWN = Action("Push(W,N)", PUSH, 0, -1, -1, 0);
const Action PushWS = Action("Push(W,S)", PUSH, 0, -1, 1, 0);
const Action PushWW = Action("Push(W,W)", PUSH, 0, -1, 0, -1);
const Action PullNN = Action("Pull(N,N)", PULL, -1, 0, -1, 0);
const Action PullNE = Action("Pull(N,E)", PULL, -1, 0, 0, 1);
const Action PullNW = Action("Pull(N,W)", PULL, -1, 0, 0, -1);
const Action PullSS = Action("Pull(S,S)", PULL, 1, 0, 1, 0);
const Action PullSE = Action("Pull(S,E)", PULL, 1, 0, 0, 1);
const Action PullSW = Action("Pull(S,W)", PULL, 1, 0, 0, -1);
const Action PullEN = Action("Pull(E,N)", PULL, 0, 1, -1, 0);
const Action PullES = Action("Pull(E,S)", PULL, 0, 1, 1, 0);
const Action PullEE = Action("Pull(E,E)", PULL, 0, 1, 0, 1);
const Action PullWN = Action("Pull(W,N)", PULL, 0, -1, -1, 0);
const Action PullWS = Action("Pull(W,S)", PULL, 0, -1, 1, 0);
const Action PullWW = Action("Pull(W,W)", PULL, 0, -1, 0, -1);

Action action_list[] = {NoOp, MoveN, MoveS, MoveE, MoveW, PushNN, PushNE,
	PushNW, PushSS, PushSE, PushSW, PushEN, PushES, PushEE, PushWN, PushWS,
	PushWW, PullNN, PullNE, PullNW, PullSS, PullSE, PullSW, PullEN, PullES,
	PullEE, PullWN, PullWS, PullWW};

int main () {
	bool test = NoOp == MoveN;
}
