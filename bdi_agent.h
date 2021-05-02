#ifndef __BDI_AGENT_H
#define __BDI_AGENT_H

#include <vector>

#include "communication.h"
#include "bdi_agent_state.h"

using namespace std;


typedef enum goal_type_t {
	NO_GOAL,           // There is no goals for this agent
	FIND_BOX,          // Move agent to box
	CARRY_BOX_TO_GOAL, // If agent is next to box, move box to goal
	AGENT_GOAL,        // If agent has goal, move agent to goal
} goal_type_t;


typedef struct goal_t {
	goal_type_t type;
	int row;
	int col;
} goal_t;


class BdiAgent {
	public:
		// static map<char, coordinates_t> goals;
		static World world;
		static vector<vector<char>> goals;
		static umap_t goals_map;

		int agent_id;
		int time;
		vector<Action> final_plan;

		BdiAgent(int agent_id);

		goal_t get_next_goal(umap_t believes);

		AgentState* intention_to_state(umap_t believes, goal_t intention);

		bool is_conflict(Action action, AgentState* state);

		void update_action(Action action, AgentState* state);

		void run();


};

#endif // __BDI_AGENT_H
