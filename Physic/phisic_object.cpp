#include "physic_object.h"

void physic_object::update_form(double frame_interval)
{
	velocity += acceleration * frame_interval;
	position += velocity * frame_interval;
	radians += radians_velocity * frame_interval;

	model_matrix = matrix3x3();
	model_matrix.rotate(radians);
	model_matrix.transfer(position);

	for (int i = 0; i < original_vertices.size(); i++)
	{
		vertices[i] = model_matrix.multiply(original_vertices[i], 1);
	}
}