#ifndef __ACTION_H
#define __ACTION_H

#include <string>
#include <vector>

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

	Action(string name, int type, int ard, int acd, int brd, int bcd);

	bool operator==(const Action &action) const;


};

extern vector<Action> actions;

#endif
