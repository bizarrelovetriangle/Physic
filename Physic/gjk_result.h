#pragma once
#include "vector2.h"
#include "minkowski_difference.h"

class gjk_result {
public:
	gjk_result(bool is_collide);

	gjk_result(
		bool is_collide,
		minkowski_difference mink_a, minkowski_difference mink_b, minkowski_difference mink_c);

	minkowski_difference mink_a;
	minkowski_difference mink_b;
	minkowski_difference mink_c;

	bool is_collide = false;
};