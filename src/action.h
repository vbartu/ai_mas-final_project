#ifndef __ACTION_H
#define __ACTION_H

#include <string>
#include <vector>

#include "global.h"

using namespace std;

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
	int ard; // agent row delta
	int acd; // agent column delta
	int brd; // box row delta
	int bcd; // box column delta

	Action();

	Action(const Action& action);

	Action(string name, int type, int ard, int acd, int brd, int bcd);

	bool operator==(const Action &action) const;


};

class CAction : public Action {
	public:
		coordinates_t agent_pos;
		coordinates_t box_pos;
		coordinates_t agent_final;
		coordinates_t box_final;
		char box;

		CAction();
		CAction(Action action, coordinates_t agent_pos);
		CAction(Action action, coordinates_t agent_pos, char box);
		CAction(Action action, coordinates_t agent_pos, coordinates_t box_pos,
			char box);

		bool conflicts(CAction other);
		bool conflicts_goal(CAction other);
};


extern vector<Action> actions;

#define ACTION_NOOP (actions[0])

#endif
