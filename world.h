#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <string>
#include <vector>
#include <unordered_map>
#include <pthread.h>

#include "action.h"
#include "bdi_agent_state.h"

using namespace std;

typedef struct coordinates_t {
	int x;
	int y;
} coordinates_t;

coordinates_t add(coordinates_t a, coordinates_t b);
coordinates_t sub(coordinates_t a, coordinates_t b);


class MapHashHelper {
	public:
		int operator()(const coordinates_t c) const {
			return c.x + 50*c.y;
		}
};

class MapEqualHelper {
	public:
		int operator()(const coordinates_t c1, const coordinates_t c2) const {
			return c1.x == c2.x && c1.y == c2.y;
		}
};

typedef unordered_map<coordinates_t, char, MapHashHelper, MapEqualHelper> umap_t;

class World {
	public:
		int current_time;
		vector<int> agent_times;
		vector<umap_t> level_map;

		World() {};
		World(umap_t initial_map, int number_agents);

		bool update_position(int agent_id, int time, Action action,
			AgentState* state);

		void finished(int agent_id);

		umap_t get_positions(int time);

		bool is_conflict(int time, coordinates_t next_pos);

		string repr();
};

#endif // __COMMUNICATION_H
