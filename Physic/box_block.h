#pragma once
#include "vector2.h"
#include "physic_object.h"
#include <vector>

class box_block : public physic_object {
public:
	box_block(vector2 pos = {}, vector2 size = { 100., 100. });

	sf::ConvexShape sfml_shape;

	void update_form(double frame_interval) override;
	void draw(sf::RenderWindow& window) override;

	sf::ConvexShape create_sfml_shape();
};