#pragma once
#include "vector2.h"

class collision_result {
public:
	vector2 collision_point;
	vector2 collision_normal;
	double collision_penetration;
	vector2 collision_penetration_line;
};