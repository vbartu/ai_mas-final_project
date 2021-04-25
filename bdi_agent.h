#ifndef __BDI_AGENT_H
#define __BDI_AGENT_H


typedef struct goal_t {
	goal_t() : type(-1), row(-1), col(-1) {}; // empty constructor
	int type; // 0 is for going to a box, 1 for carrying that box to its goal, 2 for going to agent goal,
	int row;
	int col;
};

class BdiAgent {
	public:
		// static map<char, coordinates_t> goals;
		static vector<vector<char>> goals;
		static umap_t goals_map;
		static vector<int> box_color;
		static vector<int> agent_color;
		static vector<int> box_color;
		vector<vector<char>> global_map;

		int agent_id;

		BdiAgent();

		void run();

};

#endif // __BDI_AGENT_H
