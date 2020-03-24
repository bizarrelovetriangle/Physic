#include "vector2.h"
#include "array_extentions.h"

#ifndef BOX_BLOCK
#define BOX_BLOCK
class box_block {
public:
	vector2 size;
	vector2 pos;

	double angle = 0;

	vector2 original_points[4];
	vector2& a = original_points[0];
	vector2& b = original_points[1];
	vector2& c = original_points[2];
	vector2& d = original_points[3];

	vector2 points[4];

	box_block(vector2 pos = {}, vector2 size = {})
	{
		this->size.width = size.width;
		this->size.height = size.height;
		this->pos.x = pos.x;
		this->pos.y = pos.y;

		update_form();
	}

	void update_form()
	{
		a.x = -size.width / 2;
		a.y = size.height / 2;

		b.x = size.width / 2;
		b.y = size.height / 2;

		c.x = size.width / 2;
		c.y = -size.height / 2;

		d.x = -size.width / 2;
		d.y = -size.height / 2;

		for (int i = 0; i < array_extentions::array_size(original_points); i++)
		{
			original_points[i].rotate_it(angle);
		}

		for (int i = 0; i < array_extentions::array_size(points); i++)
		{
			points[i] = original_points[i] + pos;
		}
	}

	sf::ConvexShape create_sfml_shape()
	{
		sf::ConvexShape shape(4);

		for (int i = 0; i < array_extentions::array_size(points); i++) {
			shape.setPoint(i, points[i]);
		}

		shape.setFillColor(sf::Color::Green);

		return shape;
	}
};
#endif