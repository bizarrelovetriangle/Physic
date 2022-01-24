#pragma once
#include <vector>
#include "physic_object.h"
#include "gjk_functions.h"

class main_scene;

class collide_resolver
{
public:
	const primitives_drawer& drawer;
	gjk_functions gjk;
	const main_scene& scene;
	
	int collide_count = 0;

	collide_resolver(const primitives_drawer& drawer, const main_scene& scene);
	void resolve_collisions(std::vector<physic_object*>& physic_objects);
	void narrow_phase_detection(physic_object& object_a, physic_object& object_b,
		std::vector<std::tuple<physic_object*, physic_object*, clipping_result>>& penetrationMap);
	void resolve_collision(physic_object& object_a, physic_object& object_b, clipping_result& clipping_res);
	void resolve_penetration(physic_object& object_a, physic_object& object_b, clipping_result& clipping_res);
	void apply_impulse(physic_object& object, const vector2& point, const vector2& impulse);
	void apply_velocity(physic_object& object, const vector2& point, const vector2& velocity);
	void set_velocity_in_point(physic_object& object, const vector2& point, const vector2& velocity);
	vector2 point_velocity(const physic_object& object, const vector2& point);
};