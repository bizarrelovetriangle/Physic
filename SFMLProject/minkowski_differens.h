#include "vector2.h"

#ifndef MINK_DIFF
#define MINK_DIFF
class minkowski_differens {
public:
	minkowski_differens() 
	{
	}

	minkowski_differens(
		vector2* point_a, vector2* point_b, vector2 minkowski_different)
		: point_a(point_a), point_b(point_b), differens(minkowski_different)
	{
	}

	vector2* point_a;
	vector2* point_b;
	vector2 differens;

	bool operator==(minkowski_differens& result) 
	{
		return this->differens == result.differens;
	}
};
#endif