#pragma once
#include "physic_object.h"

class wall_block : public physic_object {
public:
	vector2 a, b;

	wall_block(vector2 a = { 0, -100. }, vector2 b = { 0, 100. });

	void draw(sf::RenderWindow& window) override;
};