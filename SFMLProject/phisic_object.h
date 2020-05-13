#pragma once
#include "vector2.h"
#include <vector>
#include "edge.h"

class phisic_object {
public:
	vector2 size;
	vector2 position;
	vector2 velocity;
	vector2 acceleration;

	double radians = 0;
	double radians_velocity = 0;
	double moment_of_inetia = 1;

	double mass = 1;
	bool is_infiniti_mass;

	std::vector<vector2> original_points;
	std::vector<edge> original_edges;

	std::vector<vector2> points;
	std::vector<edge> edges;

	std::string name;

	virtual void update_form() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
};