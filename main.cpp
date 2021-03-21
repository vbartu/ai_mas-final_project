#include <iostream>
#include <cstdio>
#include <string>

using namespace std;

#define CLIENT_NAME "Final project client"

enum color_values {
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
		

int main () {
	cout << CLIENT_NAME << endl;
	cout << "#" << "starting" << endl;

	string line;
	getline(cin, line); // #domain
	getline(cin, line); // hospital
	getline(cin, line); // #levelname
	getline(cin, line); // level name

	// Read colors
	getline(cin, line); // #colors
	int agent_colors[10] = {};
	int box_colors[26] = {};
	getline(cin, line);
	while (line[0] != '#') {
		int color_end = line.find(":");
		string color = line.substr(0, color_end);

		int len = line.length();

		for (int i = color_end+2; i < line.length(); i += 3) {
			char object = line[i];
			if (object >= '0' and object <= '9') {
				agent_colors[object-'0'] = get_color_value(color);
			} else {
				box_colors[object-'A'] = get_color_value(color);
			}
		}

		getline(cin, line);
	}

	for (int i=0; i < 10; i++) {
		printf("#Agent %d: %d\n", i, agent_colors[i]);
	}
	for (int i=0; i < 26; i++) {
		printf("#Box %c: %d\n", i+(int)'A', box_colors[i]);
	}

	while (getline(cin, line)) {
		cout << "#" << line << endl;
	}
}
