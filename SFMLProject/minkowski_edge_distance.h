#include "vector2.h"
#include "minkowski_differens.h"

#ifndef MINK_DIST
#define MINK_DIST
class minkowski_edge_distance {
public:
	minkowski_edge_distance(
		double distance, minkowski_differens& mink_a, minkowski_differens& mink_b)
		: mink_a(mink_a), mink_b(mink_b), distance(distance)
	{
	}

	minkowski_differens mink_a;
	minkowski_differens mink_b;
	double distance;
};
#endif