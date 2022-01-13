#include "mouse_picker.h"

mouse_picker::mouse_picker(
	sf::RenderWindow& window, 
	std::vector<physic_object*>& physic_objects, gjk_functions& gjk, collide_resolver& collider_resolver)
	: window(window), 
	physic_objects(physic_objects), gjk(gjk), collider_resolver(collider_resolver), mouse_filter(2)
{
}

void mouse_picker::take_object()
{
	for (auto& physic_object : physic_objects) {
		if (gjk.contains_point(physic_object->vertices, mouse_filter.position)) {
			selected = physic_object;
			shoulder = (mouse_filter.filtered_position() - physic_object->position).rotate(-selected->radians);
			break;
		}
	}
}

void mouse_picker::update_object()
{
	if (selected == NULL) {
		return;
	}

	vector2 new_shoulder = shoulder.rotate(selected->radians) + selected->position;
	vector2 impulse_vector = mouse_filter.filtered_position() - new_shoulder;
	collider_resolver.set_velosity_in_point(*selected, new_shoulder, impulse_vector);
}

void mouse_picker::control_object(sf::Event event) 
{
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		take_object();
		break;
	case sf::Event::MouseMoved:
		mouse_filter.taking_new_position(mouse_position(event.mouseMove));
		break;
	case sf::Event::MouseButtonReleased:
		release_object();
		break;
	case sf::Event::KeyPressed:
		key_control(event.key);
		break;
	}
}

void mouse_picker::key_control(sf::Event::KeyEvent key_event)
{
	if (selected == NULL) {
		return;
	}

	switch (key_event.code) {
	case sf::Keyboard::Q:
		selected->radians_velocity -= global::to_radians(1.25);
		break;
	case sf::Keyboard::E:
		selected->radians_velocity += global::to_radians(1.25);
		break;
	case sf::Keyboard::Z:
		selected->radians -= global::to_radians(2);
		break;
	case sf::Keyboard::X:
		selected->radians += global::to_radians(2);
		break;
	case sf::Keyboard::R:
		selected->position = mouse_filter.position;
		selected->velocity = vector2::zero_vector;
		selected->radians = 0;
		selected->radians_velocity = 0;
		shoulder = vector2::zero_vector;
		break;
	case sf::Keyboard::Space:
		collider_resolver.apply_impulse(*selected, selected->vertices[0], vector2(0, -5));
		break;
	}
}

void mouse_picker::release_object()
{
	selected = NULL;
}

vector2 mouse_picker::mouse_position(sf::Event::MouseMoveEvent& mouseMoveEvent)
{
	return vector2(mouseMoveEvent.x - (global::screen_width / 2.), 
		mouseMoveEvent.y - (global::screen_height / 2.));
}