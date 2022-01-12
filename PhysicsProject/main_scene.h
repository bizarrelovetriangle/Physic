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
#include "global.h"

class main_scene {
public:
	main_scene();
	void start();

protected:
	sf::RenderWindow window;
	primitives_drawer drawer;
	gjk_functions gjk;
	collide_resolver collider_resolver;
	mouse_picker mouse_picker;
	std::vector<phisic_object*> phisic_objects;
	double frame_interval;
	void scene_update();
	void scene_draw();
private:
	void create_walls();
	void create_blocks();
	void draw_coordinates();
};