#ifndef __WORLD_H
#define __WORLD_H

#include <string>
#include <vector>

#include "action.h"
#include "bdi_agent_state.h"
#include "global.h"

using namespace std;

class World {
	public:
		int current_time;
		vector<int> agent_times;
		vector<umap_t> level_map;

		World() {};
		World(umap_t initial_map);

		bool update_position(int agent_id, int time, Action action,
			AgentState* state);

		void finished(int agent_id);

		umap_t get_positions(int time);

		bool is_conflict(int time, coordinates_t next_pos);

		string repr();
};

#endif // __WORLD_H
