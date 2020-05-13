#include "collide_resolver.h"

collide_resolver::collide_resolver(primitives_drawer* drawer)
	: gjk(drawer), drawer(drawer)
{
}

void collide_resolver::resolve_collision_vector(std::vector<phisic_object*>& phisic_objects)
{
	for (int i = 0; i < phisic_objects.size(); i++) {
		for (int i2 = i + 1; i2 < phisic_objects.size(); i2++) {
			phisic_object* object_1 = phisic_objects[i];
			phisic_object* object_2 = phisic_objects[i2];

			if (object_1->is_infiniti_mass && object_2->is_infiniti_mass) {
				continue;
			}

			auto gjk_result = gjk.GJK(object_1->points, object_2->points);

			if (gjk_result.is_collide) {
				auto epa_res = gjk.EPA(object_1->points, object_2->points, gjk_result);

				if (object_2->position.length() < 0.5)
				{
					int length = 1;
				}

				gjk.clipping(object_1->points, object_2->points, 
					object_1->edges, object_2->edges, epa_res);
				//continue;
				resolve_collision(object_1, object_2, epa_res);
			}
		}
	}
}

void collide_resolver::resolve_collision(phisic_object* object_1, phisic_object* object_2,
	epa_result& epa_res) 
{
	double e = 0.4; // 1 - absolutely inelastic

	vector2 collision_normal = epa_res.collision_penetration_line.normalize();

	vector2 object_1_sholder_vector = epa_res.collision_point - object_1->position;

	vector2 object_1_point_velosity = point_velosity(
		*object_1, epa_res.collision_point, epa_res.collision_normal);

	vector2 object_2_sholder_vector = epa_res.collision_point - object_2->position;

	vector2 object_2_point_velosity = point_velosity(
		*object_2, epa_res.collision_point, epa_res.collision_normal);

	vector2 objects_point_velosity_diff = object_1_point_velosity - object_2_point_velosity;

	vector2 objects_pos_vector = object_2->position - object_1->position;

	// ������ ��� ��������� ����� ���� ���������� ������ ����� ������ � �����
	if (objects_point_velosity_diff.dot_product(epa_res.collision_normal) > 0 == epa_res.flip) {
		return;
	}

	if (object_1->name.compare("observable") == 0 ||
		object_2->name.compare("observable") == 0)
	{
		int ttyyt = 0;
	}

	if (object_1->is_infiniti_mass) {
		auto j = collision_normal * ((-(1 + e) * objects_point_velosity_diff.dot_product(collision_normal)) /
			((1 / object_2->mass) + 
				(pow(object_2_sholder_vector.cross_product(collision_normal), 2) / object_2->moment_of_inetia)));

		collide_count++;

		vector2 penetration_vector = !epa_res.flip
			? -epa_res.collision_penetration_line
			: epa_res.collision_penetration_line;

		object_2->position -= penetration_vector;

		apply_impulse(*object_2, epa_res.collision_point, -j);
		
		return;
	}

	if (object_2->is_infiniti_mass) {
		auto j = collision_normal * ((-(1 + e) * objects_point_velosity_diff.dot_product(collision_normal)) /
			((1 / object_1->mass) +
				(pow(object_1_sholder_vector.cross_product(collision_normal), 2) / object_1->moment_of_inetia)));

		collide_count++;

		vector2 penetration_vector = epa_res.flip
			? -epa_res.collision_penetration_line
			: epa_res.collision_penetration_line;

		object_2->position -= penetration_vector;

		apply_impulse(*object_1, epa_res.collision_point, -j);

		return;
	}

	vector2 j = collision_normal * (-(1 + e) * objects_point_velosity_diff.dot_product(collision_normal)) /
		((1 / object_1->mass) + (1 / object_2->mass) +
			(pow(object_1_sholder_vector.cross_product(collision_normal), 2) / object_1->moment_of_inetia) +
			(pow(object_2_sholder_vector.cross_product(collision_normal), 2) / object_2->moment_of_inetia));

	vector2 penetration_vector = epa_res.flip 
		? - epa_res.collision_penetration_line
		: epa_res.collision_penetration_line;

	if (object_2->name.compare("observable") == 0) 
	{
		if (penetration_vector.y > 0.03) {
			int ttyyt = 0;
		}
	}

	object_1->position -= penetration_vector / 2;
	object_2->position += penetration_vector / 2;

	apply_impulse(*object_1, epa_res.collision_point, j);
	apply_impulse(*object_2, epa_res.collision_point, -j);
}

vector2 collide_resolver::point_velosity(
	phisic_object& object, vector2 impulse_point, vector2 normal)
{
	vector2 sholder_vector = impulse_point - object.position;
	auto angle_point_velosity = sholder_vector.clockwise_perpendicular() * object.radians_velocity;
	auto radians_point_velosity_by_normal = normal * angle_point_velosity.dot_product(normal);
	return object.velocity + radians_point_velosity_by_normal;
}

void collide_resolver::apply_impulse(
	phisic_object& object, vector2 impulse_point, vector2 impulse_vector) 
{
	vector2 sholder_vector = impulse_point - object.position;
	object.radians_velocity += sholder_vector.cross_product(impulse_vector) / object.moment_of_inetia;
	object.velocity += impulse_vector / object.mass;
}

void collide_resolver::apply_impulse_by_mouse(
	phisic_object& object, vector2 impulse_point, vector2 impulse_vector)
{
	drawer->draw_cross(impulse_point, sf::Color::White);

	drawer->draw_line(object.position, impulse_point, sf::Color::White);

	vector2 object_point_velosity = point_velosity(
		object, impulse_point, impulse_vector.normalize());

	drawer->draw_line(impulse_point, impulse_point + object_point_velosity, sf::Color::Blue);

	apply_impulse(object, impulse_point, impulse_vector - object_point_velosity);
}