#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <vector>
#include "wall_block.h"
#include "phisic_object.h"
#include "box_block.h"
#include "primitives_drawer.h"
#include "gjk_functions.h"
#include "collide_resolver.h"
#include "mouse_picker.h"

class main_scene {
public:
	double screen_width = 1400;
	double screen_height = 1000;
	main_scene();
	void start();

protected:
	sf::RenderWindow window;
	primitives_drawer drawer;
	gjk_functions gjk;
	collide_resolver collider_resolver;
	std::vector<phisic_object*> phisic_objects;

private:
	void create_walls();
	void draw_coordinates();
	mouse_picker mouse_picker;
};