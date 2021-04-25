#ifndef __HEURISTIC_H
#define __HEURISTIC_H

#include <vector>
#include <array>
#include <map>
#include <algorithm>

#include "bdi_agent_state.h"

using namespace std;

class Heuristic {
  public:
    int max_row;
    int max_col;
    map<char, map<array<int,2>,vector<vector<int>>>> goals;
    map<char, map<array<int,2>,vector<vector<int>>>> initial_boxes;
    map<int, map<char, map<array<int,2>,int>>> nearest_box_to_goal;
    vector<int> colors_left;

    Heuristic();
    Heuristic(AgentState initial_state);
    vector<vector<int>> calculate_global_map(AgentState initial_state);
    vector<array<int,2>> get_neighbours(AgentState &state, vector<vector<int>> &distance_map, int row, int col);
    void set_adjacent(AgentState &state, vector<vector<int>> &distance_map, int current_dist, vector<array<int,2>> &neighbours);
    void set_adjacent_for_agent(AgentState &state, vector<vector<int>> &distance_map, int current_dist, vector<array<int,2>> &neighbours, int agent_color);
    void precompute_map(AgentState &initial_state);
    int precomputed_distance(AgentState &state);
    vector<vector<int>> compute_box_to_map(AgentState state, int agent_color, char box, int box_row, int box_col);
    int agent_to_box(AgentState state);
};


#endif
