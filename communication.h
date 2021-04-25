#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <vector>
#include <unordered_map>
#include <pthread.h>

using namespace std;

typedef struct coordinates_t {
	int x;
	int y;
} coordinates_t;


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

class Communication {
	public:
		vector<umap_t> level_map;
		static pthread_mutex_t mtx;

		Communication() {};
		Communication(umap_t initial_map);

		void update_postion(int time, int old_row, int old_col, int row,
			int col);

		umap_t get_positions(int time);
};

#endif // __COMMUNICATION_H
