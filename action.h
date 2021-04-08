#include <vector>
#include <string>

using namespace std;

#define ACTION_LIST_SIZE 29

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

	Action(){};

	Action(string name, int type, int ard, int acd, int brd, int bcd);

	bool operator==(const Action &action) const;


};

extern Action action_list[];
