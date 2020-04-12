#pragma once
#include <vector>
#include "phisic_object.h"
#include "gjk_functions.h"

class collider_resolver
{
public:
	primitives_drawer* drawer;
	gjk_functions gjk;
	collider_resolver(primitives_drawer* drawer);
	void resolve_vector(std::vector<phisic_object*>& phisic_objects);
	void apply_impulse(phisic_object& box, vector2 impulse_point, vector2 impulse_vector, bool appling);
};