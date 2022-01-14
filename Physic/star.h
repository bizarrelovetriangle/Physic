#pragma once
#include "vector2.h"
#include "physic_object.h"
#include <vector>

class star : public physic_object {
public:
	star(vector2 pos = {}, double size = 100);

	sf::ConvexShape sfml_shape;

	void draw(sf::RenderWindow& window) override;

	sf::ConvexShape create_sfml_shape();
};

