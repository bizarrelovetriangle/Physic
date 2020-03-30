#include "mouse_provider.h"

mouse_provider::mouse_provider(int filter_iterations)
	: filter_iterations(filter_iterations)
{
	for (int i = 0; i < filter_iterations; i++) {
		speed_vector.emplace_back();
	}
}

void mouse_provider::taking_new_position(vector2 new_mouse_position)
{
	prev_position = position;
	position = new_mouse_position;

	current_speed = position - prev_position;

	for (int i = filter_iterations - 1; i < 1; i--){
		speed_vector[i - 1] = speed_vector[i];
	}

	speed_vector[filter_iterations - 1] = current_speed;
}

vector2 mouse_provider::filtered_speed()
{
	vector2 filtered_speed;

	for (int i = 0; i < filter_iterations; i++){
		filtered_speed += speed_vector[i];
	}

	filtered_speed /= filter_iterations;

	return speed_vector[filter_iterations - 1];
}
