#pragma once
#include <vector>
#include "phisic_object.h"
#include "gjk_functions.h"

class collide_resolver
{
public:
	primitives_drawer* drawer;
	gjk_functions gjk;
	
	int collide_count = 0;

	collide_resolver(primitives_drawer* drawer);
	void resolve_collision_vector(std::vector<phisic_object*>& phisic_objects);
	void resolve_collision(phisic_object* object_1, phisic_object* object_2, clipping_result& clipping_res);
	vector2 point_velosity(phisic_object& object, vector2 impulse_point, vector2 normal);
	void apply_impulse(phisic_object& object, vector2 impulse_point, vector2 impulse_vector);
	void set_velosity_in_point(phisic_object& object, vector2 impulse_point, vector2 impulse_vector);
};