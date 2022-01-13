#pragma once
#include "vector2.h"

class minkowski_difference {
public:
	minkowski_difference();

	minkowski_difference(
		const vector2* point_a, const vector2* point_b, vector2 minkowski_different);

	const vector2* point_a;
	const vector2* point_b;
	vector2 distance;

	bool operator==(minkowski_difference& result);
};