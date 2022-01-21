#include "collide_resolver.h"
#include "main_scene.h"

collide_resolver::collide_resolver(const primitives_drawer& drawer, const main_scene& scene)
	: gjk(drawer), drawer(drawer), scene(scene)
{
}

void collide_resolver::resolve_collisions(std::vector<physic_object*>& physic_objects)
{
	for (int i = 0; i < physic_objects.size(); i++) {
		for (int i2 = i + 1; i2 < physic_objects.size(); i2++) {
			physic_object& object_a = *physic_objects[i];
			physic_object& object_b = *physic_objects[i2];
			if (object_a.is_infiniti_mass && object_b.is_infiniti_mass) continue;
			narrow_phase_detection(object_a, object_b);
		}
	}
}

void collide_resolver::narrow_phase_detection(
	physic_object& object_a, physic_object& object_b)
{
	for (auto& convex_shape_a : object_a.convex_shapes) {
		for (auto& convex_shape_b : object_b.convex_shapes) {
			auto gjk_result = gjk.GJK(convex_shape_a.vertices, convex_shape_b.vertices);

			if (gjk_result.is_collide) {
				auto epa_res = gjk.EPA(convex_shape_a.vertices, convex_shape_b.vertices, gjk_result);
				auto clipping_res = gjk.clipping(convex_shape_a.vertices, convex_shape_b.vertices,
					convex_shape_a.edges, convex_shape_b.edges, epa_res);
				resolve_collision(object_a, object_b, clipping_res);
			}
		}
	}
}

void collide_resolver::resolve_collision(
	physic_object& object_a, physic_object& object_b, clipping_result& clipping_res)
{
	double e = 0.4; // 1 - absolutely inelastic

	vector2 object_a_point_velosity = point_velocity(object_a, clipping_res.collision_point);
	vector2 object_b_point_velosity = point_velocity(object_b, clipping_res.collision_point);
	vector2 objects_point_velosity_diff = object_a_point_velosity - object_b_point_velosity;

	if (objects_point_velosity_diff.dot_product(clipping_res.collision_normal) > 0 ==
		clipping_res.is_object_a_normal) {
		return;
	}

	collide_count++;

	vector2 penetration_from_a_to_b = clipping_res.is_object_a_normal
		? -clipping_res.collision_penetration_line
		: clipping_res.collision_penetration_line;

	if (!object_a.is_infiniti_mass && !object_b.is_infiniti_mass) {
		double mass_ratio = object_a.mass / (object_a.mass + object_b.mass);
		apply_velocity(object_a, clipping_res.collision_point, -penetration_from_a_to_b * mass_ratio / 3);
		apply_velocity(object_b, clipping_res.collision_point, penetration_from_a_to_b * (1 - mass_ratio) / 3);
	}
	
	if (object_a.is_infiniti_mass) object_b.position += penetration_from_a_to_b;
	if (object_b.is_infiniti_mass) object_a.position -= penetration_from_a_to_b;

	double moment_of_mass = 0;

	if (!object_a.is_infiniti_mass) {
		vector2 object_a_sholder_vector = clipping_res.collision_point - object_a.position;
		moment_of_mass += (1 / object_a.mass) +
			(pow(object_a_sholder_vector.cross_product(clipping_res.collision_normal), 2) /
				object_a.moment_of_inertia);
	}

	if (!object_b.is_infiniti_mass) {
		vector2 object_b_sholder_vector = clipping_res.collision_point - object_b.position;
		moment_of_mass += (1 / object_b.mass) +
			(pow(object_b_sholder_vector.cross_product(clipping_res.collision_normal), 2) /
				object_b.moment_of_inertia);
	}

	vector2 j = -clipping_res.collision_normal *
		objects_point_velosity_diff.dot_product(clipping_res.collision_normal) * (1 + e) /
		moment_of_mass;

	apply_impulse(object_a, clipping_res.collision_point, j);
	apply_impulse(object_b, clipping_res.collision_point, -j);
}

void collide_resolver::apply_impulse(
	physic_object& object, const vector2& point, const vector2& impulse)
{
	if (object.is_infiniti_mass) return;
	vector2 sholder_vector = point - object.position;
	object.radians_velocity += sholder_vector.cross_product(impulse) / object.moment_of_inertia;
	object.velocity += impulse / object.mass;
}


void collide_resolver::apply_velocity(
	physic_object& object, const vector2& point, const vector2& velocity)
{
	if (object.is_infiniti_mass) return;
	vector2 sholder_vector = point - object.position;
	vector2 sholder_perpendicular = -sholder_vector.clockwise_perpendicular();

	// The linear algebra magic. Here we able to find impulse required to get a certain velocity in point
	double ratio = pow(sholder_perpendicular.length(), 2) * object.mass / object.moment_of_inertia;
	vector2 velocity_change_ratio = velocity * object.mass * ratio / (1 + (ratio - 1) / 2);
	vector2 impulse = velocity * object.mass - (velocity_change_ratio / 2).projection_to(sholder_perpendicular);
	
	// Should be equal to velocity_change
	//vector2 actual_velocity_impact = impulse / object.mass +
	// 	-sholder_vector.clockwise_perpendicular() * sholder_vector.cross_product(impulse) / object.moment_of_inertia;

	apply_impulse(object, point, impulse);
}

void collide_resolver::set_velocity_in_point(
	physic_object& object, const vector2& point, const vector2& velocity)
{
	vector2 object_point_velocity = point_velocity(object, point);
	vector2 velocity_change = velocity - object_point_velocity;
	apply_velocity(object, point, velocity_change / 3);
}

vector2 collide_resolver::point_velocity(
	const physic_object& object, const vector2& point)
{
	vector2 sholder_vector = point - object.position;
	auto angle_point_velosity = -sholder_vector.clockwise_perpendicular() * object.radians_velocity;
	return object.velocity + angle_point_velosity;
}