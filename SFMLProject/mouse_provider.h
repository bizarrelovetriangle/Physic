#pragma once
#include "vector2.h"

class mouse_provider {
public:
	vector2 position;
	vector2 current_speed;

	mouse_provider(int filter_iterations);

	void taking_new_position(vector2 new_mouse_position);

	vector2 filtered_speed();

private:
	vector2 prev_position;
	int filter_iterations;
	std::vector<vector2> speed_vector;
};