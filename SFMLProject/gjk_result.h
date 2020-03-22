#include "vector2.h"
#include "support_function_result.h"

class gjk_result {
public:
	gjk_result(bool is_collide) : is_collide(is_collide)
	{
	}

	gjk_result(
		bool is_collide,
		support_function_result a, support_function_result b, support_function_result c)
		: is_collide(is_collide), a(a), b(b), c(c)
	{
	}

	support_function_result a;
	support_function_result b;
	support_function_result c;

	bool is_collide = false;
};