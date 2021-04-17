#include "heuristic_utils.h"

using namespace std;

Heuristic::Heuristic(State initial_state) {
  this->max_row = initial_state.goals.size();
  this->max_col = initial_state.goals[0].size();

};


vector<vector<int>> Heuristic::calculate_global_map(State initial_state) {
  int pos = 0;
  vector<vector<int>> global_map;
  for (int i = 0; i < initial_state.walls.size(); i++) {
    vector<int> n_col(initial_state.walls[i].size(), -1);
    global_map.push_back(n_col);
    for (int j = 0; j < initial_state.walls[i].size(); j++) {
      if (initial_state.walls[i][j] == false) {
        global_map[i][j] = pos++;
      }
    }
  }
  return global_map;
}

// // cell in x,y coordinate
// vector<vector<array<int,2>>> Heuristic::calculate_global_map(State initial_state) {
//   vector<vector<array<int,2>>> global_map;
//   for (int i = 0; i < initial_state.walls.size(); i++) {
//     vector<array<int,2>> n_col(initial_state.walls[i].size(), {-1,-1});
//     global_map.push_back(n_col);
//     for (int j = 0; j < initial_state.walls[i].size(); j++) {
//       if (initial_state.walls[i][j] == false) {
//         global_map[i][j] = {i,j};
//       }
//     }
//   }
//   return global_map;
// }


vector<array<int,2>> Heuristic::get_neighbours(State &state, vector<vector<int>> &distance_map, int row, int col) {
  int row_delta[4] = {-1, 0, 0, 1};
  int col_delta[4] = {0, -1, 1, 0};
  vector<array<int,2>> neighbours;

  for(int i = 0; i < 4; i++) {
    int row_neigh = row + row_delta[i];
    int col_neigh = col + col_delta[i];
    if (!state.walls[row_neigh][col_neigh] && !state.goals[row_neigh][col_neigh] && distance_map[row_neigh][col_neigh] == -1) {
      neighbours.push_back({row_neigh,col_neigh});
    }
  }
  return neighbours;
};


void Heuristic::set_adjacent(State &state, vector<vector<int>> &distance_map, int current_dist, vector<array<int,2>> &neighbours) {
  vector<array<int,2>> new_neighbours;
  while(!neighbours.empty()) {
    int adj_row = neighbours.back()[0];
    int adj_col = neighbours.back()[1];
    neighbours.pop_back();
    distance_map[adj_row][adj_col] = current_dist;
    vector<array<int,2>> neighbour_children = get_neighbours(state, distance_map, adj_row, adj_col);
    for (array<int,2> child: neighbour_children) {
      bool found = false;
      for(array<int,2> new_child: new_neighbours) {
        if (child == new_child) {
          found = true;
          break;
        }
      if (found){
        new_neighbours.push_back(child);
      }
      }
    }
  }
  if (!new_neighbours.empty()) {
    set_adjacent(state, distance_map, current_dist + 1, new_neighbours);
  }
};


void Heuristic::set_adjacent_for_agent(State &state, vector<vector<int>> &distance_map, int current_dist, vector<array<int,2>> &neighbours, int agent_color) {
  vector<array<int,2>> new_neighbours;
  while(!neighbours.empty()) {
    int adj_row = neighbours.back()[0];
    int adj_col = neighbours.back()[1];
    neighbours.pop_back();
    distance_map[adj_row][adj_col] = current_dist;
    char agent = state.agent_at(adj_row, adj_col);
    if (agent && state.agent_colors[int(agent) - int('0')] == agent_color) {
      return;
    }
    vector<array<int,2>> neighbour_children = get_neighbours(state, distance_map, adj_row, adj_col);
    for (array<int,2> child: neighbour_children) {
      bool found = false;
      for(array<int,2> new_child: new_neighbours) {
        if (child == new_child) {
          found = true;
          break;
        }
      if (found){
        new_neighbours.push_back(child);
      }
      }
    }
  }
  if (!new_neighbours.empty()) {
    set_adjacent_for_agent(state, distance_map, current_dist + 1, new_neighbours, agent_color);
  }
};


void Heuristic::precompute_map(State &initial_state) {
  for(int row = 0; row < this->max_row; row++) {
    for(int col = 0; col < this->max_col; col++) {
      char goal = initial_state.goals[row][col];

      if (goal != ' ') {
        // Initialize distance_map
        vector<vector<int>> distance_map;
          for (int i= 0; i < this->max_row; i++) {
        		vector<int> n_col(this->max_col,-1);
        		distance_map.push_back(n_col);
          }
        distance_map[row][col] = 0;
        vector<array<int,2>> neighbours = get_neighbours(initial_state, distance_map, row, col);
        // neighbours.clear();
        set_adjacent(initial_state, distance_map, 1, neighbours);
        if (!this->goals.count(goal)) { // I'd like to use contains or insert_or assign
          this->goals[goal];
        }
        map<array<int,2>, vector<vector<int>>> data;
        array<int,2> pos = {row,col};
        data[pos] = distance_map;
        this->goals[goal] = data;
        // data.clear();
      }

      char box = initial_state.boxes[row][col];
      if (box != ' ') {
        // Initialize distance_map
        vector<vector<int>> distance_map;
          for (int i= 0; i < this->max_row; i++) {
        		vector<int> n_col(this->max_col,-1);
        		distance_map.push_back(n_col);
          }
        distance_map[row][col] = 0;
        vector<array<int,2>> neighbours = get_neighbours(initial_state, distance_map, row, col);
        set_adjacent(initial_state, distance_map, 1, neighbours);
        map<array<int,2>, vector<vector<int>>> data;
        array<int,2> pos = {row,col};
        data[pos] = distance_map;
        // data.clear();
        this->initial_boxes[box] = data;
      }
    }
  }
}


int Heuristic::precomputed_distance(State &state) {
  int distance = 0;
  for(int row = 0; row < state.boxes.size(); row++) {
    for (int col = 0; col < state.boxes[row].size(); col++) {

      char obj = state.boxes[row][col];
      if (obj == ' ') {
        obj = state.agent_at(row,col);
      }
      if (obj == ' ' || state.goals[row][col] == obj) {
        continue;
      }

      if ('A' <= obj && obj <= 'Z') {
        int box_color = state.box_colors[int(obj) - int('A')];
        bool found = false;
        for (int color: state.box_colors) {
          if (box_color == color) {
            found = true;
            break;
          }
        }
        if(!found) {
          colors_left.push_back(box_color);
        }
      }

      int min_distance = -1;
      if (this->goals.count(obj)) {
        for (auto goal: this->goals[obj]) {
          int goal_row = goal.first[0];
          int goal_col = goal.first[1];
          if (state.boxes[goal_row][goal_col] == obj || state.agent_at(goal_row, goal_col) == obj) {
            continue; // goal cell already occupied
          }
          int dst = goal.second[row][col];
          if (dst == -1) {
            dst = 100000;
          }
          if (min_distance == -1) {
            min_distance = dst;
          }
          else {
            min_distance = min(min_distance,dst);
          }
        }
        if (min_distance != -1) {
          distance += min_distance;
          if ('A' <= obj && obj <= 'Z') {
            int box_color = state.box_colors[int(obj) - int('A')];
            if(!this->nearest_box_to_goal.count(box_color)) {
              map<char,map<array<int,2>,int>> box_type;
              map<array<int,2>, int> data;
              array<int,2> pos = {row,col};
              data[pos] = min_distance;
              box_type[obj] = data;
              this->nearest_box_to_goal[box_color] = box_type;
            }
            else if(min_distance < this->nearest_box_to_goal[box_color][obj][{row,col}]) {
              map<char,map<array<int,2>,int>> box_type;
              map<array<int,2>, int> data;
              array<int,2> pos = {row,col};
              data[pos] = min_distance;
              box_type[obj] = data;
              this->nearest_box_to_goal[box_color] = box_type;
            }
          }
        }
      }
    }
  }
  return distance;
}

vector<vector<int>> Heuristic::compute_box_to_map(State state, int agent_color, char box, int box_row, int box_col) {
  if (this->initial_boxes[box].begin()->first[0] == box_row && this->initial_boxes[box].begin()->first[1] == box_col) {
    return this->initial_boxes[box][{box_row, box_col}];
  }
  vector<vector<int>> distance_map;
    for (int i= 0; i < this->max_row; i++) {
      vector<int> n_col(this->max_col,-1);
      distance_map.push_back(n_col);
    }
  distance_map[box_row][box_col] = 0; // in python = 1 ???
  vector<array<int,2>> neighbours = get_neighbours(state, distance_map, box_row, box_col);
  // neighbours.clear();
  set_adjacent_for_agent(state, distance_map, 1, neighbours, agent_color);
  return distance_map;
}


int Heuristic::agent_to_box(State state) {
  int distance = 0;
  for (int agent = 0; agent < state.agent_rows.size(); agent++) {
    int agent_row = state.agent_rows[agent];
    int agent_col = state.agent_cols[agent];
    int agent_color = state.agent_colors[agent];
    bool found = false;
    for (int i = 0; i < this->nearest_box_to_goal.size(); i++) {
      if (this->nearest_box_to_goal.count(agent_color)) {
        found = true;
      }
    }
    if (found){
      char box = this->nearest_box_to_goal[agent_color].begin()->first;
      int box_row = this->nearest_box_to_goal[agent_color][box].begin()->first[0];
      int box_col = this->nearest_box_to_goal[agent_color][box].begin()->first[1];
      vector<vector<int>> distance_map;
        for (int i= 0; i < this->max_row; i++) {
          vector<int> n_col(this->max_col,-1);
          distance_map.push_back(n_col);
        }
      distance_map = compute_box_to_map(state, agent_color, box, box_row, box_col);
      int dst = distance_map[agent_row][agent_col];
      if (dst == -1) {
        dst = 100000;
      }
      distance += dst;
    }
  }
  return distance;
}
