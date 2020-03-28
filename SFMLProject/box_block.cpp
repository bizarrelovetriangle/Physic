#include "box_block.h"

box_block::box_block(vector2 pos, vector2 size)
	: sfml_shape(4)
{
	this->size.width = size.width;
	this->size.height = size.height;
	this->position.x = position.x;
	this->position.y = position.y;

	update_form();

	sfml_shape.setFillColor(sf::Color::Green);
}

void box_block::update_form()
{
	a.x = -size.width / 2;
	a.y = size.height / 2;

	b.x = size.width / 2;
	b.y = size.height / 2;

	c.x = size.width / 2;
	c.y = -size.height / 2;

	d.x = -size.width / 2;
	d.y = -size.height / 2;
		
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