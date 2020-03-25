#include "minkowski_edge_distance.h"

minkowski_edge_distance::minkowski_edge_distance(
	double distance, minkowski_differens& mink_a, minkowski_differens& mink_b)
	: mink_a(mink_a), mink_b(mink_b), distance(distance)
{
}