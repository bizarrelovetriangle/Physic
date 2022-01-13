#pragma once
#include "vector2.h"

class minkowski_differens {
public:
	minkowski_differens();

	minkowski_differens(
		vector2* point_a, vector2* point_b, vector2 minkowski_different);

	vector2* point_a;
	vector2* point_b;
	vector2 distance;

	bool operator==(minkowski_differens& result);
};