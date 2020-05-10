#include "box_block.h"

box_block::box_block(vector2 pos, vector2 size)
{
	this->size = size;
	this->position = pos;

	mass = size.x * size.y * 5 / 10000;
	moment_of_inetia = 1. / 12 * mass * (pow(size.x, 2) + pow(size.y, 2));

	original_points = std::vector<vector2>{ 0, 0, 0, 0 };
	points = std::vector<vector2>{ 0, 0, 0, 0 };

	original_edges.push_back(std::forward_as_tuple(original_points[0], original_points[1]));

	sfml_shape = sf::ConvexShape(4);

	
	original_points[0] = vector2(-size.x / 2, size.y / 2);
	original_points[1] = vector2(size.x / 2, size.y / 2);
	original_points[2] = vector2(size.x / 2, -size.y / 2);
	original_points[3] = vector2(-size.x / 2, -size.y / 2);
	
	original_edges.push_back(std::forward_as_tuple(original_points[0], original_points[1]));
	original_edges.push_back(std::forward_as_tuple(original_points[1], original_points[2]));
	original_edges.push_back(std::forward_as_tuple(original_points[2], original_points[3]));
	original_edges.push_back(std::forward_as_tuple(original_points[3], original_points[0]));

	update_form();

	sfml_shape.setFillColor(sf::Color::Green);
}

void box_block::update_form()
{
	velocity += acceleration;
	position += velocity;
	angle += angle_velocity;
		
	viscosity_vector(velocity);
	viscosity_value(angle_velocity);

	for (int i = 0; i < original_points.size(); i++)
	{
		points[i] = original_points[i].rotate(angle) + position;
	}

	edges.push_back(std::forward_as_tuple(points[0], points[1]));
	edges.push_back(std::forward_as_tuple(points[1], points[2]));
	edges.push_back(std::forward_as_tuple(points[2], points[3]));
	edges.push_back(std::forward_as_tuple(points[3], points[0]));
}

void box_block::draw(sf::RenderWindow& window) 
{
	for (int i = 0; i < points.size(); i++) {
		sfml_shape.setPoint(i, points[i]);
	}

	window.draw(sfml_shape);
}

void box_block::viscosity_value(double& value)
{
	double viscosity_factor = 0.;

	if (std::abs(value) <= viscosity_factor) {
		value = 0;
	}
	else if (value > 0) {
		value -= viscosity_factor;
	}
	else {
		value += viscosity_factor;
	}
}


void box_block::viscosity_vector(vector2& vector)
{
	vector2 viscosity_vectro_factor = vector.normalize() * 0.;

	if (vector.length() <= viscosity_vectro_factor.length()) {
		vector = vector2(0, 0);
	}
	else if (vector.length() > 0) {
		vector -= viscosity_vectro_factor;
	}
	else {
		vector += viscosity_vectro_factor;
	}
}