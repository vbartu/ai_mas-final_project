#include "action.h"

using namespace std;

Action::Action() {}

Action::Action(string name, int type, int ard, int acd, int brd, int bcd)
{
	this->name = name;
	this->type = type;
	this->ard = ard;
	this->acd = acd;
	this->brd = brd;
	this->bcd = bcd;
}

Action::Action(const Action& action)
{
	this->name = action.name;
	this->type = action.type;
	this->ard = action.ard;
	this->acd = action.acd;
	this->brd = action.brd;
	this->bcd = action.bcd;

}

bool Action::operator==(const Action& action) const
{
	return this->name == action.name;
}


CAction::CAction() {}

CAction::CAction(Action action, coordinates_t agent_pos)
		: Action(action)
{
	this->agent_pos = agent_pos;
	this->box_pos = agent_pos;
	this->agent_final = add(agent_pos, {this->ard, this->acd});
	this->box_final = this->agent_final;
	this->box = 0;
}

CAction::CAction(Action action, coordinates_t agent_pos, coordinates_t box_pos,
		char box) : Action(action)
{
	this->agent_pos = agent_pos;
	this->box_pos = box_pos;
	this->agent_final = add(agent_pos, {this->ard, this->acd});
	this->box_final = add(box_pos, {this->brd, this->bcd});
	this->box = box;
}

bool CAction::conflicts(CAction other)
{
	// maybe the first 4 where we check only the current position doesn't make really sense
	// since in each state we check for conflict in the next state, s basically you will never end up
	// in a state where your current position is in conflict with the current positio of something else -> you are right, but there is no harm in checking
	return equal(this->agent_pos, other.agent_pos)
			|| equal(this->agent_pos, other.box_pos)
			|| equal(this->box_pos, other.agent_pos)
			|| equal(this->box_pos, other.box_pos)
			|| equal(this->agent_pos, other.agent_final)
			|| equal(this->agent_pos, other.box_final)
			|| equal(this->agent_final, other.agent_pos)
			|| equal(this->agent_final, other.agent_final)
			|| equal(this->box_final, other.agent_pos)
			|| equal(this->box_final, other.agent_final)
			|| equal(this->box_final, other.box_pos)
			|| equal(this->box_final, other.box_final);
}

bool CAction::conflicts_goal(CAction other)
{
	return equal(this->agent_final, other.agent_final)
		|| equal(this->agent_final, other.box_final)
		|| equal(this->box_final, other.agent_final)
		|| equal(this->box_final, other.agent_final);
}


const Action NoOp   = Action("NoOp", NOOP, 0, 0, 0, 0);
const Action MoveN  = Action("Move(N)", MOVE, -1, 0, 0, 0);
const Action MoveS  = Action("Move(S)", MOVE, 1, 0, 0, 0);
const Action MoveE  = Action("Move(E)", MOVE, 0, 1, 0, 0);
const Action MoveW  = Action("Move(W)", MOVE, 0, -1, 0, 0);
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

vector<Action> actions = {NoOp, MoveN, MoveS, MoveE, MoveW, PushNN, PushNE,
	PushNW, PushSS, PushSE, PushSW, PushEN, PushES, PushEE, PushWN, PushWS,
	PushWW, PullNN, PullNE, PullNW, PullSS, PullSE, PullSW, PullEN, PullES,
	PullEE, PullWN, PullWS, PullWW};
