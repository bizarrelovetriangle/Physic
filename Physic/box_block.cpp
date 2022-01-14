#include "box_block.h"

box_block::box_block(vector2 pos, vector2 size)
{
	this->size = size;
	this->position = pos;

	mass = size.x * size.y * 5 / 10000;
	moment_of_inetia = mass * 2000;

	convex_shape& convex_shape = convex_shapes.emplace_back();

	convex_shape.original_vertices = std::vector<vector2>{
		vector2(-size.x / 2, -size.y / 2),
		vector2(size.x / 2, -size.y / 2),
		vector2(size.x / 2, size.y / 2),
		vector2(-size.x / 2, size.y / 2)
	};
	
	convex_shape.vertices = std::vector<vector2>{ 0, 0, 0, 0 };

	convex_shape.original_edges = std::vector<edge>{
		{convex_shape.original_vertices[0], convex_shape.original_vertices[1]},
		{convex_shape.original_vertices[1], convex_shape.original_vertices[2]},
		{convex_shape.original_vertices[2], convex_shape.original_vertices[3]},
		{convex_shape.original_vertices[3], convex_shape.original_vertices[0]}
	};

	convex_shape.edges = std::vector<edge>{
		{convex_shape.vertices[0], convex_shape.vertices[1]},
		{convex_shape.vertices[1], convex_shape.vertices[2]},
		{convex_shape.vertices[2], convex_shape.vertices[3]},
		{convex_shape.vertices[3], convex_shape.vertices[0]}
	};

	sfml_shape = sf::ConvexShape(4);
	sfml_shape.setFillColor(sf::Color::Green);
}

void box_block::draw(sf::RenderWindow& window) 
{
	//for (int i = 0; i < vertices.size(); i++) {
	//	sfml_shape.setPoint(i, vertices[i]);
	//}

	//window.draw(sfml_shape);

	for (auto& convex_shape : convex_shapes) {
		sf::Vertex line[8] =
		{
			sf::Vertex(convex_shape.vertices[0], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[1], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[1], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[2], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[2], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[3], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[3], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[0], sf::Color::Yellow)
		};

		window.draw(line, 8, sf::Lines);
	}
}