#pragma once
#include <vector>
#include "vector2.h"
#include "edge.h"
#include "matrix3x3.h"

class convex_shape
{
public:
	std::vector<vector2> original_vertices;
	std::vector<edge> original_edges;
	std::vector<vector2> vertices;
	std::vector<edge> edges;
	void update(const matrix3x3& model_matrix);
};
