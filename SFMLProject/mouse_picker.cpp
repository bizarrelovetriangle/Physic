#include "mouse_picker.h"

mouse_picker::mouse_picker(
	std::vector<phisic_object*>& phisic_objects, gjk_functions& gjk, collide_resolver& collider_resolver)
	: phisic_objects(phisic_objects), gjk(gjk), collider_resolver(collider_resolver), mouse_filter(2)
{
}

void mouse_picker::take_object(vector2 mouse_pos)
{
	for (auto& phisic_object : phisic_objects) {
		if (gjk.contains_point(phisic_object->vertices, mouse_pos)) {
			selected = phisic_object;
			shoulder = (mouse_filter.filtered_position() - phisic_object->position).rotate(-selected->radians);
			break;
		}
	}
}

void mouse_picker::update_object(vector2 mouse_pos)
{
	mouse_filter.taking_new_position(mouse_pos);

	if (selected != NULL) {
		vector2 new_shoulder = shoulder.rotate(selected->radians) + selected->position;
		vector2 impulse_vector = mouse_filter.filtered_position() - new_shoulder;
		collider_resolver.set_velosity_in_point(*selected, new_shoulder, impulse_vector);
	}
}

void mouse_picker::control_object(sf::Event event) 
{
	if (selected != NULL) {
		if (event.key.code == sf::Keyboard::Q) {
			selected->radians_velocity -= 1.25;
		}
		if (event.key.code == sf::Keyboard::E) {
			selected->radians_velocity += 1.25;
		}
		if (event.key.code == sf::Keyboard::Z) {
			selected->radians -= 2;
		}
		if (event.key.code == sf::Keyboard::X) {
			selected->radians += 2;
		}
		if (event.key.code == sf::Keyboard::R) {
			selected->position = vector2();
			selected->radians = 0;
			selected->velocity = vector2();
			selected->radians_velocity = 0;
		}
		if (event.key.code == sf::Keyboard::Space) {
			collider_resolver.apply_impulse(*selected, selected->vertices[0], vector2(0, -5));
		}
	}
}

void mouse_picker::release_object()
{
	selected = NULL;
}