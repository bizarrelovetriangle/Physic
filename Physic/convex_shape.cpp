#include "convex_shape.h"

void convex_shape::update(const matrix3x3& model_matrix)
{
	auto th = this;
	auto& edges2 = edges;

	for (int i = 0; i < original_vertices.size(); i++)
	{
		vertices[i] = model_matrix.multiply(original_vertices[i], 1);
	}
}
