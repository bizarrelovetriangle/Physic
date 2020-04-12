#pragma once
#include "vector2.h"
#include "phisic_object.h"
#include <vector>

class box_block : public phisic_object {
public:
	box_block(vector2 pos = {}, vector2 size = { 100., 100. });

	sf::ConvexShape sfml_shape;

	void update_form() override;
	void draw(sf::RenderWindow& window) override;

	void viscosity_value(double& value);

	void viscosity_vector(vector2& vector);

	sf::ConvexShape create_sfml_shape();
};