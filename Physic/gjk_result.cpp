#include "gjk_result.h"

gjk_result::gjk_result(bool is_collide) : is_collide(is_collide)
{
}

gjk_result::gjk_result(
	bool is_collide,
	minkowski_difference mink_a, minkowski_difference mink_b, minkowski_difference mink_c)
	: is_collide(is_collide), mink_a(mink_a), mink_b(mink_b), mink_c(mink_c)
{
}