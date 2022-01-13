#pragma once
#include "vector2.h"

class minkowski_difference {
public:
	minkowski_difference();

	minkowski_difference(
		vector2* point_a, vector2* point_b, vector2 minkowski_different);

	vector2* point_a;
	vector2* point_b;
	vector2 distance;

	bool operator==(minkowski_difference& result);
};