#pragma once
#include "vector2.h"
#include "phisic_object.h"
#include "gjk_functions.h"
#include "mouse_filter.h"
#include "collide_resolver.h"

class mouse_picker {
public:
	std::vector<phisic_object*>& phisic_objects;
	void take_object(vector2 mouse_pos);
	void update_object(vector2 mouse_pos);
	void control_object(sf::Event event);
	void release_object();
	mouse_picker(std::vector<phisic_object*>& phisic_objects, gjk_functions& gjk, collide_resolver& collider_resolver);

private:
	phisic_object* selected = nullptr;
	vector2 shoulder;
	gjk_functions& gjk;
	collide_resolver& collider_resolver;
	mouse_filter mouse_filter;
};

