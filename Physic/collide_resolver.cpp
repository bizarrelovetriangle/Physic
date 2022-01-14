#include "collide_resolver.h"

collide_resolver::collide_resolver(primitives_drawer& drawer)
	: gjk(drawer), drawer(drawer)
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
	
	vector2 object_a_point_velosity = point_velocity(
		object_a, clipping_res.collision_point, clipping_res.collision_normal);
	vector2 object_b_point_velosity = point_velocity(
		object_b, clipping_res.collision_point, clipping_res.collision_normal);
	vector2 objects_point_velosity_diff = object_a_point_velosity - object_b_point_velosity;

	if (objects_point_velosity_diff.dot_product(clipping_res.collision_normal) > 0 ==
		clipping_res.is_object_a_normal) {
		return;
	}
	
	collide_count++;

	vector2 penetration_from_a_to_b = clipping_res.is_object_a_normal
		? -clipping_res.collision_penetration_line
		: clipping_res.collision_penetration_line;
	penetration_from_a_to_b /= 4;

	double mass_ratio = object_a.is_infiniti_mass
		? 0 : object_b.is_infiniti_mass
		? 1 : object_a.mass / (object_a.mass + object_b.mass);

	apply_impulse(object_a, clipping_res.collision_point, -penetration_from_a_to_b * mass_ratio);
	apply_impulse(object_b, clipping_res.collision_point, penetration_from_a_to_b * (1 - mass_ratio));

	double moment_of_mass = 0;

	if (!object_a.is_infiniti_mass) {
		vector2 object_a_sholder_vector = clipping_res.collision_point - object_a.position;
		moment_of_mass += (1 / object_a.mass) +
			(pow(object_a_sholder_vector.cross_product(clipping_res.collision_normal), 2) /
				object_a.moment_of_inetia);
	}

	if (!object_b.is_infiniti_mass) {
		vector2 object_b_sholder_vector = clipping_res.collision_point - object_b.position;
		moment_of_mass += (1 / object_b.mass) +
			(pow(object_b_sholder_vector.cross_product(clipping_res.collision_normal), 2) /
				object_b.moment_of_inetia);
	}

	vector2 j = -clipping_res.collision_normal *
		objects_point_velosity_diff.dot_product(clipping_res.collision_normal) * (1 + e) /
		moment_of_mass;

	apply_impulse(object_a, clipping_res.collision_point, j);
	apply_impulse(object_b, clipping_res.collision_point, -j);
}

void collide_resolver::apply_impulse(
	physic_object& object, const vector2& impulse_point, const vector2& impulse_vector)
{
	if (object.is_infiniti_mass) return;
	vector2 sholder_vector = impulse_point - object.position;
	object.radians_velocity += sholder_vector.cross_product(impulse_vector) / object.moment_of_inetia;
	object.velocity += impulse_vector / object.mass;
}

void collide_resolver::set_velocity_in_point(
	physic_object& object, const vector2& impulse_point, const vector2& velocity)
{
	drawer.draw_cross(impulse_point, sf::Color::White);
	drawer.draw_line(object.position, impulse_point, sf::Color::White);
	vector2 object_point_velocity = point_velocity(object, impulse_point, velocity.normalize());
	vector2 impulse_vector = (velocity - object_point_velocity) * object.mass / 5;
	apply_impulse(object, impulse_point, impulse_vector);
}

vector2 collide_resolver::point_velocity(
	const physic_object& object, const vector2& impulse_point, const vector2& normal)
{
	vector2 sholder_vector = impulse_point - object.position;
	auto angle_point_velosity = sholder_vector.clockwise_perpendicular() * object.radians_velocity;
	auto radians_point_velosity_by_normal = normal * angle_point_velosity.dot_product(normal);
	return object.velocity + radians_point_velosity_by_normal;
}