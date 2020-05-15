#pragma once
#include "vector2.h"
#include "phisic_object.h"
#include "gjk_functions.h"
#include "mouse_filter.h"
#include "collide_resolver.h"
#include "global.h"

class mouse_picker {
public:
	std::vector<phisic_object*>& phisic_objects;
	void update_object();
	void control_object(sf::Event event);
	mouse_picker(sf::RenderWindow& window, 
		std::vector<phisic_object*>& phisic_objects, gjk_functions& gjk, collide_resolver& collider_resolver);

private:
	phisic_object* selected = nullptr;
	vector2 shoulder;
	sf::RenderWindow& window;
	gjk_functions& gjk;
	collide_resolver& collider_resolver;
	mouse_filter mouse_filter;
	void take_object();
	void release_object();
	void key_control(sf::Event::KeyEvent key_event);
	vector2 mouse_position(sf::Event::MouseMoveEvent& mouseMoveEvent);
};

