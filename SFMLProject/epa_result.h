#pragma once
#include "vector2.h"

class epa_result {
public:
	vector2 collision_point;
	vector2 collision_normal;
	double collision_penetration = 0;
	vector2 collision_penetration_line;
	bool flip = false;
};