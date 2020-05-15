#include "mouse_filter.h"

mouse_filter::mouse_filter(int filter_iterations)
	: filter_iterations(filter_iterations)
{
	for (int i = 0; i < filter_iterations; i++) {
		speed_vector.emplace_back();
	}

	for (int i = 0; i < filter_iterations; i++) {
		position_vector.emplace_back();
	}
}

void mouse_filter::taking_new_position(vector2 new_mouse_position)
{
	prev_position = position;
	position = new_mouse_position;

	current_speed = position - prev_position;

	for (int i = filter_iterations - 1; i > 0; i--){
		speed_vector[i] = speed_vector[i - 1];
		position_vector[i] = position_vector[i - 1];
	}

	speed_vector[0] = current_speed;
	position_vector[0] = new_mouse_position;
}

vector2 mouse_filter::filtered_speed()
{
	vector2 filtered_speed;

	for (int i = 0; i < filter_iterations; i++) {
		filtered_speed += speed_vector[i];
	}

	filtered_speed /= filter_iterations;

	return filtered_speed;
}

vector2 mouse_filter::filtered_position()
{
	vector2 filtered_position;

	for (int i = 0; i < filter_iterations; i++) {
		filtered_position += position_vector[i];
	}

	filtered_position /= filter_iterations;

	return filtered_position;
}
