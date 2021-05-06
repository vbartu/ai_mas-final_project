#include "global.h"


int n_agents;
int n_rows;
int n_cols;
vector<int> agent_colors(10);
vector<int> box_colors(26);
vector<vector<bool>> walls;
vector<vector<char>> goals;
umap_t goals_map;


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
