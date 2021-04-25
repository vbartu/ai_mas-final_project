#ifndef __BDI_AGENT_H
#define __BDI_AGENT_H

#include <vector>

#include "communication.h"
#include "bdi_agent_state.h"

using namespace std;

typedef struct goal_t {
	//goal_t() : type(-1), row(-1), col(-1) {}; // empty constructor
	int type; // 0 is for going to a box, 1 for carrying that box to its goal, 2 for going to agent goal,
	int row;
	int col;
} goal_t;

class BdiAgent {
	public:
		// static map<char, coordinates_t> goals;
		static Communication communication;
		static vector<vector<char>> goals;
		static umap_t goals_map;
		static vector<int> box_color;
		static vector<int> agent_color;

		int agent_id;
		int time;
		vector<Action> final_plan;

		BdiAgent(int agent_id);

		goal_t get_next_goal(umap_t believes);

		AgentState* intention_to_state(umap_t believes, goal_t intention);

		bool is_conflict(Action action);

		void run();


};

#endif // __BDI_AGENT_H
