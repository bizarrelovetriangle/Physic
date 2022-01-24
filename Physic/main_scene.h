#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <vector>
#include <numeric>
#include "wall_block.h"
#include "physic_object.h"
#include "box_block.h"
#include "primitives_drawer.h"
#include "gjk_functions.h"
#include "collition_resolver.h"
#include "mouse_picker.h"
#include "global.h"

class main_scene {
public:
	main_scene();
	void start();
	double frame_interval;
	double delta;

protected:
	sf::RenderWindow window;
	primitives_drawer drawer;
	gjk_functions gjk;
	collition_resolver collider_resolver;
	mouse_picker mouse_picker;
	std::vector<physic_object*> physic_objects;
	void scene_update();
	void scene_draw();

private:
	void create_walls();
	void draw_info();
	void create_blocks();
	void draw_coordinates();
};