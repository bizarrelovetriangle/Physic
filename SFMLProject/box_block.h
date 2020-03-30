#pragma once
#include "vector2.h"
#include <vector>

class box_block {
public:
	vector2 size;
	vector2 position;
	vector2 velocity;
	vector2 acceleration;

	double angle = 0;
	double angle_velocity = 0;

	std::vector<vector2> original_points { 0, 0, 0, 0 };

	std::vector<vector2> points { 0, 0, 0, 0 };

	sf::ConvexShape sfml_shape;

	box_block(vector2 pos = {}, vector2 size = { 100., 100. });

	void update_form();

	void viscosity_value(double& value);

	void viscosity_vector(vector2& vector);

	sf::ConvexShape create_sfml_shape();
};