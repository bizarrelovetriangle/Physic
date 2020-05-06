#pragma once
#include "vector2.h"
#include <vector>

class phisic_object {
public:
	vector2 size;
	vector2 position;
	vector2 velocity;
	vector2 acceleration;

	double angle = 0;
	double angle_velocity = 0;
	double moment_of_inetia = 1;

	double mass = 1; // now is not use
	bool is_infiniti_mass;

	std::vector<vector2> original_points;

	std::vector<vector2> points;

	virtual void update_form() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
};