#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <vector>
#include <unordered_map>

using namespace std;


typedef struct coordinates_t {
	int x;
	int y;
} coordinates_t;

coordinates_t add(coordinates_t a, coordinates_t b);
coordinates_t sub(coordinates_t a, coordinates_t b);


class MapHashHelper {
	public:
		int operator()(const coordinates_t c) const;
};

class MapEqualHelper {
	public:
		int operator()(const coordinates_t c1, const coordinates_t c2) const;
};

typedef unordered_map<coordinates_t, char, MapHashHelper, MapEqualHelper> umap_t;


bool is_box(char object);
int get_color(char object);


extern int n_agents;
extern int n_rows;
extern int n_cols;
extern vector<int> agent_colors;
extern vector<int> box_colors;
extern vector<vector<bool>> walls;
extern vector<vector<char>> goals;
extern umap_t goals_map;

#endif // __GLOBAL_H
