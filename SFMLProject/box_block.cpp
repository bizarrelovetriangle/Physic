#include "box_block.h"

box_block::box_block(vector2 pos, vector2 size)
{
	this->size = size;
	this->position = pos;

	mass = size.x * size.y * 5 / 10000;
	moment_of_inetia = 1. / 12 * mass * (pow(size.x, 2) + pow(size.y, 2));

	original_vertices = std::vector<vector2>{ 0, 0, 0, 0 };
	vertices = std::vector<vector2>{ 0, 0, 0, 0 };

	original_edges.emplace_back(original_vertices[0], original_vertices[1]);

	sfml_shape = sf::ConvexShape(4);


	original_vertices[0] = vector2(-size.x / 2, -size.y / 2);
	original_vertices[1] = vector2(size.x / 2, -size.y / 2);
	original_vertices[2] = vector2(size.x / 2, size.y / 2);
	original_vertices[3] = vector2(-size.x / 2, size.y / 2);

	original_edges.emplace_back(original_vertices[0], original_vertices[1]);
	original_edges.emplace_back(original_vertices[1], original_vertices[2]);
	original_edges.emplace_back(original_vertices[2], original_vertices[3]);
	original_edges.emplace_back(original_vertices[3], original_vertices[0]);

	sfml_shape.setFillColor(sf::Color::Green);
}

void box_block::update_form()
{
	velocity += acceleration;
	position += velocity;
	radians += radians_velocity;

	for (int i = 0; i < original_vertices.size(); i++)
	{
		vertices[i] = original_vertices[i].rotate(radians) + position;
	}

	edges.clear();
	edges.emplace_back(vertices[0], vertices[1]);
	edges.emplace_back(vertices[1], vertices[2]);
	edges.emplace_back(vertices[2], vertices[3]);
	edges.emplace_back(vertices[3], vertices[0]);
}

void box_block::draw(sf::RenderWindow& window) 
{
	//for (int i = 0; i < vertices.size(); i++) {
	//	sfml_shape.setPoint(i, vertices[i]);
	//}

	//window.draw(sfml_shape);


	sf::Vertex line[8] =
	{
		sf::Vertex(vertices[0], sf::Color::Yellow),
		sf::Vertex(vertices[1], sf::Color::Yellow),
		sf::Vertex(vertices[1], sf::Color::Yellow),
		sf::Vertex(vertices[2], sf::Color::Yellow),
		sf::Vertex(vertices[2], sf::Color::Yellow),
		sf::Vertex(vertices[3], sf::Color::Yellow),
		sf::Vertex(vertices[3], sf::Color::Yellow),
		sf::Vertex(vertices[0], sf::Color::Yellow)
	};

	window.draw(line, 8, sf::Lines);
}