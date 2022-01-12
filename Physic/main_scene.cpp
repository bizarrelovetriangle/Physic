#include "main_scene.h"

main_scene::main_scene() : 
	window(sf::VideoMode(global::screen_width, global::screen_height), "PhysicsProject"),
	drawer(window),
	gjk(drawer),
	collider_resolver(drawer),
	mouse_picker(window, phisic_objects, gjk, collider_resolver)
{
	sf::View view;
	vector2 center(-global::screen_width / 2, -global::screen_height / 2);
	vector2 size(global::screen_width, global::screen_height);
	view.reset(sf::FloatRect(center, size));
	window.setView(view);

	create_walls();
	create_blocks();
}

void main_scene::start() 
{
	sf::Clock clock;
	while (window.isOpen())
	{
		frame_interval = clock.restart().asSeconds();
		
		scene_update();
		scene_draw();
	}
}

void main_scene::scene_update()
{
	mouse_picker.update_object();

	sf::Event event;
	while (window.pollEvent(event))
	{
		mouse_picker.control_object(event);

		if (event.type == sf::Event::Closed) {
			window.close();
		}
	}

	collider_resolver.resolve_collision_vector(phisic_objects);

	for (auto& object : phisic_objects) {
		object->update_form(frame_interval * 50);
	}
}

void main_scene::scene_draw()
{
	window.clear();

	for (auto& object : phisic_objects) {
		object->draw(window);
	}

	drawer.draw_text(vector2(-220), "fps");
	drawer.draw_number(vector2(-200), 1 / frame_interval);
	static double average_interval = 0;
	static std::vector<double> intervals;
	intervals.push_back(1 / frame_interval);
	if (intervals.size() == 1000) {
		average_interval = std::reduce(intervals.begin(), intervals.end()) / (double)intervals.size();
		intervals.clear();
	}
	drawer.draw_number(vector2(-180), average_interval);

	draw_coordinates();
	window.display();
}

void main_scene::create_blocks()
{
	//for (int i = 0; i < 4; i++) {
	//	for (int i2 = 0; i2 < 5; i2++) {
	//		box_block* _box = new box_block(
	//			vector2(-200. + 101. * i2, -100 + 101. * i),
	//			vector2(100., (i * i2 % 2 ? 100. : 70.)));
	//
	//		_box->acceleration.y = 0.1;
	//
	//		phisic_objects.emplace_back(_box);
	//	}
	//}

	for (int i = 0; i < 7; i++) {
		for (int i2 = 0; i2 < 11; i2++) {
			box_block* _box = new box_block(
				vector2(-500. + 101. * i2, -200 + 101. * i),
				vector2(100., (i * i2 % 2 ? 100. : 70.)));

			_box->acceleration.y = 0.1;

			phisic_objects.emplace_back(_box);
		}
	}

	//box_block* _box = new box_block(
	//	vector2(0., 200),
	//	vector2(100., 100.));
	////_box->acceleration.y = 0.1;
	////_box->acceleration.x = 0.1;
	//phisic_objects.emplace_back(_box);

	//box_block* _box2 = new box_block(
	//	vector2(100., 200.),
	//	vector2(100., 100.));
	////_box2->acceleration.y = 0.1;
	//_box2->name = "observable";
	//phisic_objects.emplace_back(_box2);
}

void main_scene::create_walls()
{
	int wall_width = 1300;
	int wall_height = 980;

	vector2 a(-wall_width / 2, wall_height / 2);
	vector2 b(-wall_width / 2, -wall_height / 2);
	vector2 c(wall_width / 2, -wall_height / 2);
	vector2 d(wall_width / 2, wall_height / 2);

	wall_block* left_wall = new wall_block(a, b);
	wall_block* top_wall = new wall_block(b, c);
	wall_block* right_wall = new wall_block(c, d);
	wall_block* borrom_wall = new wall_block(d, a);

	phisic_objects.emplace_back(left_wall);
	phisic_objects.emplace_back(top_wall);
	phisic_objects.emplace_back(right_wall);
	phisic_objects.emplace_back(borrom_wall);
}

void main_scene::draw_coordinates()
{
	sf::Vertex line[4] =
	{
		sf::Vertex(vector2(-10, 0), sf::Color::White),
		sf::Vertex(vector2(10, 0), sf::Color::White),
		sf::Vertex(vector2(0, 10), sf::Color::White),
		sf::Vertex(vector2(0, -10), sf::Color::White)
	};

	window.draw(line, 4, sf::Lines);
}