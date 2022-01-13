#include "minkowski_difference.h"

minkowski_difference::minkowski_difference()
{
}

minkowski_difference::minkowski_difference(
	vector2* point_a, vector2* point_b, vector2 minkowski_different)
	: point_a(point_a), point_b(point_b), distance(minkowski_different)
{
}

bool minkowski_difference::operator==(minkowski_difference& result)
{
	return this->distance == result.distance;
}