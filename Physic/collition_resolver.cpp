#include "collition_resolver.h"
#include "main_scene.h"

collition_resolver::collition_resolver(const primitives_drawer& drawer, const main_scene& scene)
	: gjk(drawer), drawer(drawer), scene(scene)
{
}

// todo: accumulate all impulses before applying
//		 resolve penetration only for top lying object
void collition_resolver::resolve_collisions(std::vector<physic_object*>& physic_objects)
{
	std::vector<std::tuple<physic_object*, physic_object*, clipping_result>> penetrationMap;

	for (int i = 0; i < physic_objects.size(); i++) {
		for (int i2 = i + 1; i2 < physic_objects.size(); i2++) {
			physic_object& object_a = *physic_objects[i];
			physic_object& object_b = *physic_objects[i2];
			if (object_a.is_infiniti_mass && object_b.is_infiniti_mass) continue;
			narrow_phase_detection(object_a, object_b, penetrationMap);
		}
	}

	// resolve_collision_iterations * 20 ??
	for (int i = 0; i < resolve_collision_iterations; i++) {
		for (auto& penetration : penetrationMap) {
			resolve_collision(*std::get<0>(penetration), *std::get<1>(penetration), std::get<2>(penetration));
		}
		// apply impulses
	}

	for (auto& penetration : penetrationMap) {
		resolve_penetration(*std::get<0>(penetration), *std::get<1>(penetration), std::get<2>(penetration));
	}
}

void collition_resolver::narrow_phase_detection(
	physic_object& object_a, physic_object& object_b,
	std::vector<std::tuple<physic_object*, physic_object*, clipping_result>>& penetrationMap)
{
	std::vector<clipping_result> clipping_results;
	for (auto& convex_shape_a : object_a.convex_shapes) {
		for (auto& convex_shape_b : object_b.convex_shapes) {
			auto gjk_result = gjk.GJK(convex_shape_a.vertices, convex_shape_b.vertices);

			if (gjk_result.is_collide) {
				auto epa_res = gjk.EPA(convex_shape_a.vertices, convex_shape_b.vertices, gjk_result);
				auto clipping_res = gjk.clipping(convex_shape_a.vertices, convex_shape_b.vertices,
					convex_shape_a.edges, convex_shape_b.edges, epa_res);
				clipping_results.emplace_back(clipping_res);
			}
		}
	}

	if (!clipping_results.empty()) {
		auto clipping_res = std::reduce(clipping_results.cbegin(), clipping_results.cend()) / clipping_results.size();
		penetrationMap.emplace_back(std::make_tuple(&object_a, &object_b, clipping_res));
	}
}

void collition_resolver::resolve_collision(
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

	double moment_of_mass = 0;

	if (!object_a.is_infiniti_mass) {
		vector2 object_a_shoulder_vector = clipping_res.collision_point - object_a.position;
		moment_of_mass += (1 / object_a.mass) +
			(pow(object_a_shoulder_vector.cross_product(clipping_res.collision_normal), 2) /
				object_a.moment_of_inertia);
	}

	if (!object_b.is_infiniti_mass) {
		vector2 object_b_shoulder_vector = clipping_res.collision_point - object_b.position;
		moment_of_mass += (1 / object_b.mass) +
			(pow(object_b_shoulder_vector.cross_product(clipping_res.collision_normal), 2) /
				object_b.moment_of_inertia);
	}

	vector2 j = -clipping_res.collision_normal *
		objects_point_velosity_diff.dot_product(clipping_res.collision_normal) * (1 + e) /
		moment_of_mass;

	apply_impulse(object_a, clipping_res.collision_point, j);
	apply_impulse(object_b, clipping_res.collision_point, -j);
}


void collition_resolver::resolve_penetration(
	physic_object& object_a, physic_object& object_b, clipping_result& clipping_res)
{
	vector2 penetration_from_a_to_b = clipping_res.is_object_a_normal
		? -clipping_res.collision_penetration_line
		: clipping_res.collision_penetration_line;

	if (!object_a.is_infiniti_mass && !object_b.is_infiniti_mass) {
		double mass_ratio = object_a.mass / (object_a.mass + object_b.mass);
		penetration_from_a_to_b *= 0.5;
		object_a.position -= penetration_from_a_to_b * mass_ratio;
		object_b.position += penetration_from_a_to_b * (1 - mass_ratio);
	}

	if (object_a.is_infiniti_mass) object_b.position += penetration_from_a_to_b;
	if (object_b.is_infiniti_mass) object_a.position -= penetration_from_a_to_b;
}

void collition_resolver::apply_impulse(
	physic_object& object, const vector2& point, const vector2& impulse)
{
	if (object.is_infiniti_mass) return;
	vector2 shoulder_vector = point - object.position;
	object.radians_velocity += shoulder_vector.cross_product(impulse) / object.moment_of_inertia;
	object.velocity += impulse / object.mass;
}


void collition_resolver::apply_velocity(
	physic_object& object, const vector2& point, const vector2& velocity)
{
	if (object.is_infiniti_mass) return;
	vector2 shoulder_vector = point - object.position;
	vector2 shoulder_perpendicular = -shoulder_vector.clockwise_perpendicular();

	// The linear algebra magic. Here we able to find impulse required to get a certain velocity in point
	double ratio = pow(shoulder_perpendicular.length(), 2) * object.mass / object.moment_of_inertia;
	vector2 velocity_change_ratio = velocity * object.mass * ratio / (ratio + 1);
	vector2 impulse = velocity * object.mass - velocity_change_ratio.projection_to(shoulder_perpendicular);

	// Should be equal to velocity_change
	//vector2 actual_velocity_impact = impulse / object.mass +
	//	shoulder_perpendicular * shoulder_perpendicular.dot_product(impulse) / object.moment_of_inertia;

	apply_impulse(object, point, impulse);
}

void collition_resolver::set_velocity_in_point(
	physic_object& object, const vector2& point, const vector2& velocity)
{
	vector2 object_point_velocity = point_velocity(object, point);
	vector2 velocity_change = velocity - object_point_velocity;
	apply_velocity(object, point, velocity_change / 3);
}

vector2 collition_resolver::point_velocity(
	const physic_object& object, const vector2& point)
{
	vector2 shoulder_vector = point - object.position;
	auto angle_point_velosity = -shoulder_vector.clockwise_perpendicular() * object.radians_velocity;
	return object.velocity + angle_point_velosity;
}