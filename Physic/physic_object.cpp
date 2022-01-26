#include "physic_object.h"

void physic_object::apply_acceleration(double frame_interval)
{
	velocity += acceleration * frame_interval;
}

void physic_object::apply_velocity(double frame_interval)
{
	position += velocity * frame_interval;
	radians += radians_velocity * frame_interval;
}

void physic_object::update_form() {
	model_matrix = matrix3x3();
	model_matrix.rotate(radians);
	model_matrix.transfer(position);

	for (auto& convex_shape : convex_shapes) {
		convex_shape.update(model_matrix);
	}
}