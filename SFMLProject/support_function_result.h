#include "vector2.h"

#ifndef SUPPORT_RESULT
#define SUPPORT_RESULT
class support_function_result {
public:
	support_function_result() 
	{
	}

	support_function_result(
		primitives::vector2* point_a, primitives::vector2* point_b, primitives::vector2 minkowski_different)
		: point_a(point_a), point_b(point_b), minkowski_different(minkowski_different)
	{
	}

	primitives::vector2* point_a;
	primitives::vector2* point_b;
	primitives::vector2 minkowski_different;

	bool operator==(support_function_result result) 
	{
		return this->minkowski_different == result.minkowski_different;
	}
};
#endif