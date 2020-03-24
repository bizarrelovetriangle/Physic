#include "vector2.h"

#ifndef SUPPORT_RESULT
#define SUPPORT_RESULT
class minkowski_differens {
public:
	minkowski_differens() 
	{
	}

	minkowski_differens(
		primitives::vector2* point_a, primitives::vector2* point_b, primitives::vector2 minkowski_different)
		: point_a(point_a), point_b(point_b), differens(minkowski_different)
	{
	}

	primitives::vector2* point_a;
	primitives::vector2* point_b;
	primitives::vector2 differens;

	bool operator==(minkowski_differens result) 
	{
		return this->differens == result.differens;
	}
};
#endif