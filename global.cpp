#include <iostream>

#include "global.h"


int n_agents;
int n_rows;
int n_cols;
vector<int> agent_colors(10);
vector<int> box_colors(26);
vector<vector<bool>> walls;
vector<vector<char>> goals;
umap_t goals_map;
dmap_t distance_map;


Coordinates::Coordinates(int x, int y)
{
	this->x = x;
	this->y = y;
}

bool Coordinates::operator==(const Coordinates& other) const
{
	return this->x == other.x && this->y == other.y;
}

Coordinates Coordinates::operator+(const Coordinates& other)
{
	return Coordinates(this->x+other.x, this->y+other.y);
}

Coordinates Coordinates::operator-(const Coordinates& other)
{
	return Coordinates(this->x-other.x, this->y-other.y);
}

int MapHashHelper::operator()(const coordinates_t c) const
{
	return c.x + 50*c.y;
}


int MapEqualHelper::operator()(const coordinates_t c1, const coordinates_t c2) const
{
	return c1.x == c2.x && c1.y == c2.y;
}


coordinates_t add(coordinates_t a, coordinates_t b)
{
	return (coordinates_t) {a.x + b.x, a.y + b.y};
}

coordinates_t sub(coordinates_t a, coordinates_t b)
{
	return (coordinates_t) {a.x - b.x, a.y - b.y};
}


bool is_box(char object) {
	if (object >= 'A' && object <= 'Z') {
		return true;
	}
	return false;
}

int get_color(char object)
{
	if (object >= 0 && object <= 9) {
		return agent_colors[object];
	} else if (object >= '0' && object <= '9') {
		return agent_colors[object - '0'];
	} else if (object >= 'A' && object <= 'Z') {
		return box_colors[object - 'A'];
	} else {
		return -1;
	}
}

static vector<coordinates_t> get_neighbours(coordinates_t coord,
		dmap_aux_t& distances);
static void set_adjacent(vector<coordinates_t> neighbours,
		dmap_aux_t& distances, int current_dist);

static vector<coordinates_t> get_neighbours(coordinates_t coord,
		dmap_aux_t& distances)
{
	int row_delta[4] = {-1, 0, 0, 1};
	int col_delta[4] = {0, -1, 1, 0};
	vector<coordinates_t> result;
	for (int i = 0; i < 4; i++) {
		coordinates_t ne = {coord.x + row_delta[i], coord.y + col_delta[i]};
		if (!(ne.x < 0) && !(ne.y < 0) && !(ne.x >= n_rows) && !(ne.y >= n_cols)
				&& !walls[ne.x][ne.y] && !distances.count(ne)) {
			result.push_back(ne);
		}
	}
	return result;
}

static void set_adjacent(vector<coordinates_t> neighbours,
		dmap_aux_t& distances, int current_dist)
{
	vector<coordinates_t> new_neighbours;
	for (coordinates_t ne : neighbours) {
		distances[ne] = current_dist;

		vector<coordinates_t> neighbour_children = get_neighbours(ne, distances);
		for (coordinates_t child : neighbour_children) {
			bool already_stored = false;
			for (coordinates_t new_child : new_neighbours) {
				if (child.x == new_child.x && child.y == new_child.y) {
					already_stored = true;
					break;
				}
			}
			if (!already_stored) {
				new_neighbours.push_back(child);
			}
		}
	}
	if (new_neighbours.size()) {
		set_adjacent(new_neighbours, distances, current_dist+1);
	}
}

void calculate_distances()
{
	for (int row = 0; row < n_rows; row++) {
		for (int col = 0; col < n_cols; col++) {
			if (walls[row][col]) {
				continue;
			}

			dmap_aux_t distances;
			distances[{row, col}] = 0;
			vector<coordinates_t> neighbours = get_neighbours({row, col},
				distances);
			set_adjacent(neighbours, distances, 1);
			distance_map[{row, col}] = distances;
		}
	}
}
