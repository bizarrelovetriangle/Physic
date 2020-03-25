#include "vector2.h"

#ifndef PRIMITIVES_DRAWER
#define PRIMITIVES_DRAWER
class primitives_drawer {
public:
	primitives_drawer(sf::RenderWindow& window);

	sf::RenderWindow* window;

	void draw_line(
		vector2& a, vector2& b, sf::Color color);

	void draw_number(
		vector2& pos, sf::RenderWindow& window, int number);

	void draw_text(
		vector2& pos, std::string str);
};
#endif