#include "minkowski_differens.h"

minkowski_differens::minkowski_differens()
{
}

minkowski_differens::minkowski_differens(
	vector2* point_a, vector2* point_b, vector2 minkowski_different)
	: point_a(point_a), point_b(point_b), differens(minkowski_different)
{
}

bool minkowski_differens::operator==(minkowski_differens& result)
{
	return this->differens == result.differens;
}