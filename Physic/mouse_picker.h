#pragma once
#include "vector2.h"
#include "physic_object.h"
#include "gjk_functions.h"
#include "collition_resolver.h"
#include "global.h"

class mouse_picker {
public:
	std::vector<physic_object*>& physic_objects;
	void update_object();
	void control_object(sf::Event event);
	mouse_picker(
		sf::RenderWindow& window, 
		std::vector<physic_object*>& physic_objects,
		gjk_functions& gjk,
		collition_resolver& collider_resolver,
		primitives_drawer& drawer);

private:
	physic_object* selected = nullptr;
	vector2 shoulder;
	sf::RenderWindow& window;
	gjk_functions& gjk;
	collition_resolver& collider_resolver;
	primitives_drawer& drawer;
	vector2 position;
	void take_object();
	void release_object();
	void key_control(sf::Event::KeyEvent key_event);
	vector2 mouse_position(sf::Event::MouseMoveEvent& mouseMoveEvent);
};

