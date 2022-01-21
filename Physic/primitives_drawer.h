#pragma once
#include "vector2.h"

class primitives_drawer {
public:
	primitives_drawer(sf::RenderWindow& window);

	sf::RenderWindow* window;

	void draw_line(
		const vector2& a, const vector2& b, const sf::Color& color) const;

	void draw_cross(
		const vector2& point, const sf::Color& color) const;

	void draw_number(
		const vector2& pos, double number) const;

	void draw_text(
		const vector2& pos, std::string str) const;
};