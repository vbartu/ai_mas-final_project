#include "color.h"
#include "main.h"

int get_color_value(string color)
{
	if (color.compare("blue") == 0) {
		return BLUE;
	} else if (color.compare("red") == 0) {
		return RED;
	} else if (color.compare("cyan") == 0) {
		return CYAN;
	} else if (color.compare("purple") == 0) {
		return PURPLE;
	} else if (color.compare("green") == 0) {
		return GREEN;
	} else if (color.compare("orange") == 0) {
		return ORANGE;
	} else if (color.compare("pink") == 0) {
		return PINK;
	} else if (color.compare("grey") == 0) {
		return GREY;
	} else if (color.compare("lightblue") == 0) {
		return LIGHTBLUE;
	} else if (color.compare("brown") == 0) {
		return BROWN;
	}
	return -1;
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

bool is_box(char object) {
	if (object >= 'A' && object <= 'Z') {
		return true;
	}
	return false;
}
