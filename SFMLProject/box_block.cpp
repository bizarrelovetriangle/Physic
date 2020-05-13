#include "box_block.h"

box_block::box_block(vector2 pos, vector2 size)
{
	this->size = size;
	this->position = pos;

	mass = size.x * size.y * 5 / 10000;
	moment_of_inetia = 1. / 12 * mass * (pow(size.x, 2) + pow(size.y, 2));

	original_points = std::vector<vector2>{ 0, 0, 0, 0 };
	points = std::vector<vector2>{ 0, 0, 0, 0 };

	original_edges.emplace_back(original_points[0], original_points[1]);

	sfml_shape = sf::ConvexShape(4);


	original_points[0] = vector2(-size.x / 2, -size.y / 2);
	original_points[1] = vector2(size.x / 2, -size.y / 2);
	original_points[2] = vector2(size.x / 2, size.y / 2);
	original_points[3] = vector2(-size.x / 2, size.y / 2);

	original_edges.emplace_back(original_points[0], original_points[1]);
	original_edges.emplace_back(original_points[1], original_points[2]);
	original_edges.emplace_back(original_points[2], original_points[3]);
	original_edges.emplace_back(original_points[3], original_points[0]);

	update_form();

	sfml_shape.setFillColor(sf::Color::Green);
}

void box_block::update_form()
{
	velocity += acceleration;
	position += velocity;
	radians += radians_velocity;

	for (int i = 0; i < original_points.size(); i++)
	{
		points[i] = original_points[i].rotate(radians) + position;
	}

	edges.clear();
	edges.emplace_back(points[0], points[1]);
	edges.emplace_back(points[1], points[2]);
	edges.emplace_back(points[2], points[3]);
	edges.emplace_back(points[3], points[0]);
}

void box_block::draw(sf::RenderWindow& window) 
{
	//for (int i = 0; i < points.size(); i++) {
	//	sfml_shape.setPoint(i, points[i]);
	//}

	//window.draw(sfml_shape);


	sf::Vertex line[8] =
	{
		sf::Vertex(points[0], sf::Color::Yellow),
		sf::Vertex(points[1], sf::Color::Yellow),
		sf::Vertex(points[1], sf::Color::Yellow),
		sf::Vertex(points[2], sf::Color::Yellow),
		sf::Vertex(points[2], sf::Color::Yellow),
		sf::Vertex(points[3], sf::Color::Yellow),
		sf::Vertex(points[3], sf::Color::Yellow),
		sf::Vertex(points[0], sf::Color::Yellow)
	};

	window.draw(line, 8, sf::Lines);
}