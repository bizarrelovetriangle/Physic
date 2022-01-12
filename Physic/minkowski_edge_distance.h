#pragma once
#include "vector2.h"
#include "minkowski_differens.h"

class minkowski_edge_distance {
public:
	minkowski_edge_distance(
		double distance, minkowski_differens& mink_a, minkowski_differens& mink_b);

	minkowski_differens mink_a;
	minkowski_differens mink_b;
	double distance;
};