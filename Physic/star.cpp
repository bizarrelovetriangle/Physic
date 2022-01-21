#include "star.h"

star::star(vector2 pos, double size)
{
	this->size = size;
	this->position = pos;

	mass = size * size * 3 / 10000;
	moment_of_inertia = mass * 2000;

	convex_shape& convex_shape_a = convex_shapes.emplace_back();

	convex_shape_a.original_vertices = std::vector<vector2>{
		vector2(0, 1) * size,
		vector2(0.587785, -0.809016) * size,
		vector2(-0.356344, -0.115783) * size
	};

	convex_shape_a.vertices = std::vector<vector2>{ 0, 0, 0 };

	convex_shape_a.original_edges = std::vector<edge>{
		{convex_shape_a.original_vertices[0], convex_shape_a.original_vertices[1]},
		{convex_shape_a.original_vertices[1], convex_shape_a.original_vertices[2]},
		{convex_shape_a.original_vertices[2], convex_shape_a.original_vertices[0]}
	};

	convex_shape_a.edges = std::vector<edge>{
		{convex_shape_a.vertices[0], convex_shape_a.vertices[1]},
		{convex_shape_a.vertices[1], convex_shape_a.vertices[2]},
		{convex_shape_a.vertices[2], convex_shape_a.vertices[0]}
	};

	convex_shape& convex_shape_b = convex_shapes.emplace_back();
	
	convex_shape_b.original_vertices = std::vector<vector2>{
		vector2(-0.951057, 0.309017) * size,
		vector2(0.951056, 0.309017) * size,
		vector2(0, -0.374682) * size
	};
	
	convex_shape_b.vertices = std::vector<vector2>{ 0, 0, 0 };
	
	convex_shape_b.original_edges = std::vector<edge>{
		{convex_shape_b.original_vertices[0], convex_shape_b.original_vertices[1]},
		{convex_shape_b.original_vertices[1], convex_shape_b.original_vertices[2]},
		{convex_shape_b.original_vertices[2], convex_shape_b.original_vertices[0]}
	};
	
	convex_shape_b.edges = std::vector<edge>{
		{convex_shape_b.vertices[0], convex_shape_b.vertices[1]},
		{convex_shape_b.vertices[1], convex_shape_b.vertices[2]},
		{convex_shape_b.vertices[2], convex_shape_b.vertices[0]}
	};
	
	convex_shape& convex_shape_c = convex_shapes.emplace_back();
	
	convex_shape_c.original_vertices = std::vector<vector2>{
		vector2(0, 1) * size,
		vector2(0.3648, -0.118531) * size,
		vector2(-0.587785, -0.809017) * size,
	};

	convex_shape_c.vertices = std::vector<vector2>{ 0, 0, 0 };
	
	convex_shape_c.original_edges = std::vector<edge>{
		{convex_shape_c.original_vertices[0], convex_shape_c.original_vertices[1]},
		{convex_shape_c.original_vertices[1], convex_shape_c.original_vertices[2]},
		{convex_shape_c.original_vertices[2], convex_shape_c.original_vertices[0]}
	};
	
	convex_shape_c.edges = std::vector<edge>{
		{convex_shape_c.vertices[0], convex_shape_c.vertices[1]},
		{convex_shape_c.vertices[1], convex_shape_c.vertices[2]},
		{convex_shape_c.vertices[2], convex_shape_c.vertices[0]}
	};

	for (auto& convex_shape : convex_shapes) {
		auto& sfml_shape = sfml_shapes.emplace_back(convex_shape.vertices.size());
		sfml_shape.setFillColor(sf::Color::Transparent);
		sfml_shape.setOutlineColor(sf::Color::Yellow);
		sfml_shape.setOutlineThickness(1);
	}
}

void star::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < convex_shapes.size(); i++) {
		for (int j = 0; j < convex_shapes[i].vertices.size(); j++) {
			sfml_shapes[i].setPoint(j, convex_shapes[i].vertices[j]);
		}

		window.draw(sfml_shapes[i]);
	}
}