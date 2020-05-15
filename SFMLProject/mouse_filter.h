#pragma once
#include "vector2.h"

class mouse_filter {
public:
	vector2 position;
	vector2 current_speed;

	mouse_filter(int filter_iterations);

	void taking_new_position(vector2 new_mouse_position);

	vector2 filtered_speed();
	vector2 filtered_position();

private:
	vector2 prev_position;
	int filter_iterations;
	std::vector<vector2> speed_vector;
	std::vector<vector2> position_vector;
};