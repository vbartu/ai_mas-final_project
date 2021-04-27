#ifndef __COLOR_H
#define __COLOR_H

#include <string>

using namespace std;

enum color {
	BLUE=1,
	RED,
	CYAN,
	PURPLE,
	GREEN,
	ORANGE,
	PINK,
	GREY,
	LIGHTBLUE,
	BROWN
};

int get_color_value(string color);

int get_color(char object);

bool is_box(char object);

#endif
