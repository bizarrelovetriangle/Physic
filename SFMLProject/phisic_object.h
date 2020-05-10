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

	double mass = 1;
	bool is_infiniti_mass;

	std::vector<vector2> original_points;
	std::vector<std::tuple<vector2&, vector2&>> original_edges;

	std::vector<vector2> points;
	std::vector<std::tuple<vector2&, vector2&>> edges;

	virtual void update_form() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
};