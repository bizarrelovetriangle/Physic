#include "epa_result.h"

epa_result::epa_result()
{
}

epa_result::epa_result(const vector2& collision_normal, bool is_object_1_normal)
	: collision_normal(collision_normal), is_object_1_normal(is_object_1_normal)
{
}
