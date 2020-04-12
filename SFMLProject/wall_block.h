#pragma once
#include "phisic_object.h"

class wall_block : public phisic_object {
public:
	vector2 a, b;

	wall_block(vector2 a = { 0, -100. }, vector2 b = { 0, 100. });

	void update_form() override;
	void draw(sf::RenderWindow& window) override;
};