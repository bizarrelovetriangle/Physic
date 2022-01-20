#pragma once
#include "vector2.h"
#include "matrix3x3.h"
#include <vector>
#include "edge.h"
#include "convex_shape.h"

class physic_object {
public:
	vector2 size;
	vector2 position;
	vector2 velocity;
	vector2 acceleration;

	double radians = 0;
	double radians_velocity = 0;
	double moment_of_inertia = 1;

	double mass = 1;
	bool is_infiniti_mass;

	matrix3x3 model_matrix;
	std::vector<convex_shape> convex_shapes;

	std::string name;

	void update(double frame_interval);
	virtual void draw(sf::RenderWindow& window) = 0;
};