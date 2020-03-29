#pragma once
#include "vector2.h"
#include "minkowski_differens.h"

class gjk_result {
public:
	gjk_result(bool is_collide);

	gjk_result(
		bool is_collide,
		minkowski_differens mink_a, minkowski_differens mink_b, minkowski_differens mink_c);

	minkowski_differens mink_a;
	minkowski_differens mink_b;
	minkowski_differens mink_c;

	bool is_collide = false;
};