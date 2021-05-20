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
	HELP_MOVE_BOX,
	AGENT_GOAL,        // If agent has goal, move agent to goal,
} goal_type_t;


typedef struct goal_t {
	goal_type_t type;
	coordinates_t pos;
	int helping_agent;
	int helping_box;
} goal_t;


typedef struct conflict_t {
	vector<vector<CAction>> new_actions;
	vector<int> skip;
} conflitc_t;


class BdiAgent {
	public:
		static vector<umap_t> world;
		static int current_time;
		static pthread_mutex_t world_mtx;

		int agent_id;
		int time;
		vector<CAction> final_plan;
		vector<CAction> plan;
		int plan_index;

		bool waiting_for_agent;

		BdiAgent(int agent_id);


		goal_t get_next_goal(umap_t believes);
		goal_t get_next_conflict_box_goal(vector<CAction> actions);

		AgentState* intention_to_state(umap_t believes, goal_t intention);
		AgentState* help_intention_to_state(umap_t believes, goal_t intention,
				coordinates_t agent_pos, coordinates_t box_pos);

		bool try_around_box(umap_t believes, goal_t intention, coordinates_t box_pos);

		ConflictState* conflict_to_state(umap_t believes, char other_id,
				CAction action, CAction other_action);

		umap_t get_current_map();
		void set_next_action(CAction action);
		bool check_conflict(CAction next_action);
		void update_position(CAction action);

		coordinates_t nearest_help_goal_cell(umap_t believes, coordinates_t box_pos,
			vector<CAction> other_actions);

		void run();
};

#endif // __BDI_AGENT_H
