#include "star.h"

star::star(vector2 pos, double size)
{
	this->size = size;
	this->position = pos;

	mass = size * size * 3 / 10000;
	moment_of_inetia = mass * 2000;

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
		vector2(-0.356344, -0.115783) * size,
		vector2(0, -0.374682) * size,
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

	sfml_shape = sf::ConvexShape(4);
	sfml_shape.setFillColor(sf::Color::Green);
}

void star::draw(sf::RenderWindow& window)
{
	//for (int i = 0; i < vertices.size(); i++) {
	//	sfml_shape.setPoint(i, vertices[i]);
	//}

	//window.draw(sfml_shape);

	for (auto& convex_shape : convex_shapes) {
		sf::Vertex line[6] =
		{
			sf::Vertex(convex_shape.vertices[0], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[1], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[1], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[2], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[2], sf::Color::Yellow),
			sf::Vertex(convex_shape.vertices[0], sf::Color::Yellow),
		};

		window.draw(line, 6, sf::Lines);
	}
}