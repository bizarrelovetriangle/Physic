#pragma once
#include <vector>
#include "physic_object.h"
#include "gjk_functions.h"

class collide_resolver
{
public:
	primitives_drawer drawer;
	gjk_functions gjk;
	
	int collide_count = 0;

	collide_resolver(primitives_drawer& drawer);
	void resolve_collision_vector(std::vector<physic_object*>& physic_objects);
	void resolve_collision(physic_object* object_1, physic_object* object_2, clipping_result& clipping_res);
	vector2 point_velosity(physic_object& object, vector2 impulse_point, vector2 normal);
	void apply_impulse(physic_object& object, vector2 impulse_point, vector2 impulse_vector);
	void set_velosity_in_point(physic_object& object, vector2 impulse_point, vector2 impulse_vector);
};