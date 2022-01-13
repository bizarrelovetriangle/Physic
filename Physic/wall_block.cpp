#include "wall_block.h"

wall_block::wall_block(vector2 a, vector2 b)
{
	position = (a + b) / 2;
	
	a -= position;
	b -= position;

	this->a = a;
	this->b = b;

	is_infiniti_mass = true;

	original_vertices = std::vector<vector2>{ a, b };
	vertices = std::vector<vector2>{ 0, 0 };
	original_edges = std::vector<edge>{ {this->a, this->b} };
	edges = std::vector<edge>{ {vertices[0], vertices[1]} };
}

void wall_block::update_form(double frame_interval)
{
	velocity += acceleration * frame_interval;
	position += velocity * frame_interval;
	radians += radians_velocity * frame_interval;

	for (int i = 0; i < original_vertices.size(); i++)
	{
		vertices[i] = original_vertices[i].rotate(radians) + position;
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