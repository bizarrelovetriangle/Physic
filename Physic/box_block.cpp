#include "box_block.h"

box_block::box_block(vector2 pos, vector2 size)
{
	this->size = size;
	this->position = pos;

	mass = size.x * size.y * 5 / 10000;
	moment_of_inertia = mass * 2000;

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

	for (auto& convex_shape : convex_shapes) {
		auto& sfml_shape = sfml_shapes.emplace_back(convex_shape.vertices.size());
		sfml_shape.setFillColor(sf::Color::Transparent);
		sfml_shape.setOutlineColor(sf::Color::Green);
		sfml_shape.setOutlineThickness(1);
	}
}

void box_block::draw(sf::RenderWindow& window) 
{
	for (int i = 0; i < convex_shapes.size(); i++) {
		for (int j = 0; j < convex_shapes[i].vertices.size(); j++) {
			sfml_shapes[i].setPoint(j, convex_shapes[i].vertices[j]);
		}

		window.draw(sfml_shapes[i]);
	}
}