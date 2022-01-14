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
	void resolve_collisions(std::vector<physic_object*>& physic_objects);
	void narrow_phase_detection(physic_object& object_a, physic_object& object_b);
	void resolve_collision(physic_object& object_a, physic_object& object_b, clipping_result& clipping_res);
	void apply_impulse(physic_object& object, const vector2& impulse_point, const vector2& impulse_vector);
	void set_velosity_in_point(physic_object& object, const vector2& impulse_point, const vector2& impulse_vector);
	vector2 point_velosity(const physic_object& object, const vector2& impulse_point, const vector2& normal);
};