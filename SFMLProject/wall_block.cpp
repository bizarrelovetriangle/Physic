#include "wall_block.h"

wall_block::wall_block(vector2 a, vector2 b)
{
	position = (a + b) / 2;
	
	a -= position;
	b -= position;

	this->a = a;
	this->b = b;

	moment_of_inetia = 0.008;
	is_infiniti_mass = true;

	original_points = std::vector<vector2>{ 0, 0 };
	points = std::vector<vector2>{ 0, 0 };

	original_points[0] = a;
	original_points[1] = b;
	original_edges.push_back(std::forward_as_tuple(this->a, this->b));

	update_form();
}

void wall_block::update_form()
{
	original_points[0] = a;
	original_points[1] = b;

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
}

void wall_block::draw(sf::RenderWindow& window) {
	sf::Vertex line[2] =
	{
		sf::Vertex(a + position, sf::Color::White),
		sf::Vertex(b + position, sf::Color::White)
	};

	window.draw(line, 2, sf::Lines);
}