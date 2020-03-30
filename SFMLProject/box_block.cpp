#include "box_block.h"

box_block::box_block(vector2 pos, vector2 size)
	: sfml_shape(4)
{
	this->size.x = size.x;
	this->size.y = size.y;
	this->position.x = position.x;
	this->position.y = position.y;

	update_form();

	sfml_shape.setFillColor(sf::Color::Green);
}

void box_block::update_form()
{
	original_points[0].x = -size.x / 2;
	original_points[0].y = size.y / 2;

	original_points[1].x = size.x / 2;
	original_points[1].y = size.y / 2;

	original_points[2].x = size.x / 2;
	original_points[2].y = -size.y / 2;

	original_points[3].x = -size.x / 2;
	original_points[3].y = -size.y / 2;

	velocity += acceleration;
	position += velocity;
	angle += angle_velocity;
		
	for (int i = 0; i < original_points.size(); i++)
	{
		original_points[i].rotate_it(angle);
	}

	for (int i = 0; i < points.size(); i++)
	{
		points[i] = original_points[i] + position;
	}

	for (int i = 0; i < points.size(); i++) {
		sfml_shape.setPoint(i, points[i]);
	}
}