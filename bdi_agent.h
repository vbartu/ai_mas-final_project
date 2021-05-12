#ifndef __BDI_AGENT_H
#define __BDI_AGENT_H

#include <vector>

#include "bdi_agent_state.h"
#include "global.h"
#include "action.h"
#include "conflict_state.h"

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


typedef struct conflict_t {
	bool is_conlfict;
	coordinates_t conflict_loc;
	char object;
	int agent_id;
} conflitc_t;


class BdiAgent {
	public:
		static vector<umap_t> world;
		static int current_time;
		static pthread_mutex_t world_mtx;




		static vector<bool> conflicts;
		static bool no_more_conflicts;
		static pthread_mutex_t conflicts_mtx;

		static vector<bool> finished;
		static pthread_mutex_t finished_mtx;


		int agent_id;
		int time;
		vector<Action> final_plan;

		BdiAgent(int agent_id);


		goal_t get_next_goal(umap_t believes);
		AgentState* intention_to_state(umap_t believes, goal_t intention);
		ConflictState* conflict_to_state(umap_t believes, char other_id,
				vector<CAction> actions, vector<CAction> other_actions);

		umap_t get_current_map();
		void set_next_action(CAction action);
		bool check_conflict(CAction next_action);
		void update_position(CAction action);

		void run();
};

#endif // __BDI_AGENT_H
