#include "main_scene.h"
#include "star.h"

main_scene::main_scene() :
	window(sf::VideoMode(global::screen_width, global::screen_height), "Physic"),
	drawer(window),
	gjk(drawer),
	collider_resolver(drawer, *this),
	mouse_picker(window, physic_objects, gjk, collider_resolver, drawer)
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
		scene_update();
		scene_draw();

		int fps = 60;
		double time = clock.getElapsedTime().asSeconds();
		if (1. / fps > time) sf::sleep(sf::seconds(1. / fps - time));

		frame_interval = clock.restart().asSeconds();
		//frame_interval = 1. / 60;

		frame_speed = frame_interval * 50;
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

	for (auto& object : physic_objects) {
		object->apply_acceleration(frame_speed);
	}

	collider_resolver.resolve_collisions(physic_objects);

	for (auto& object : physic_objects) {
		object->apply_velocity(frame_speed);
		object->update_form();
	}
}

void main_scene::scene_draw()
{
	for (auto& object : physic_objects) {
		object->draw(window);
	}

	draw_info();
	draw_coordinates();
	window.display();
	window.clear();
}

void main_scene::create_blocks()
{
	//auto box1 = new box_block({ 0, -120 }, { 200 });
	//auto box2 = new box_block({ 0, 100 }, 200);
	//auto box3 = new box_block({ 0, 320 }, 200);
	////box1->acceleration.y = 0.5;
	////box2->acceleration.y = 1.0;
	////box3->acceleration.y = 1.0;
	//box1->name = "box1";
	//box2->name = "box2";
	//box3->name = "box3";
	////physic_objects.emplace_back(box3);
	//physic_objects.emplace_back(box2);
	//physic_objects.emplace_back(box1);

	for (int i = 0; i < 7; i++) {
		for (int i2 = 0; i2 < 2; i2++) {
			star* star_block = new star(
				vector2(-550. + 171. * i2, -400 + 131. * i),
				i * i2 % 2 ? 100 : 70);
	
			star_block->acceleration.y = 0.5;
	
			//physic_objects.emplace_back(star_block);
		}
	}
	
	for (int i = 0; i < 9; i++) {
		for (int i2 = 0; i2 < 2; i2++) {
			box_block* _box = new box_block(
				vector2(+131. * i2, -400 + 101. * i),
				i * i2 % 2 ? 100 : 70);
	
			_box->acceleration.y = 0.5;
	
			physic_objects.emplace_back(_box);
		}
	}
	
	for (int i = 0; i < 20; i++) {
		for (int i2 = 0; i2 < 1; i2++) {
			box_block* _box = new box_block(
				vector2(300 + 131. * i2, -300 + 35. * i),
				vector2(i % 2 ? 120 : 60, 30));
	
			_box->acceleration.y = 0.5;
	
			physic_objects.emplace_back(_box);
		}
	}
}

void main_scene::create_walls()
{
	int wall_width = 1300;
	int wall_height = 950;

	vector2 a(-wall_width / 2, wall_height / 2);
	vector2 b(-wall_width / 2, -wall_height / 2);
	vector2 c(wall_width / 2, -wall_height / 2);
	vector2 d(wall_width / 2, wall_height / 2);

	wall_block* left_wall = new wall_block(a, b);
	wall_block* top_wall = new wall_block(b, c);
	wall_block* right_wall = new wall_block(c, d);
	wall_block* bottom_wall = new wall_block(d, a);

	physic_objects.emplace_back(left_wall);
	physic_objects.emplace_back(top_wall);
	physic_objects.emplace_back(right_wall);
	physic_objects.emplace_back(bottom_wall);
}

void main_scene::draw_info()
{
	static double average_interval = 0;
	static double update_interval_seconds = 0.5;
	static double seconds_to_update = update_interval_seconds;
	static std::vector<double> intervals;
	intervals.push_back(1 / frame_interval);
	seconds_to_update -= frame_interval;

	if (seconds_to_update <= 0) {
		average_interval = std::reduce(intervals.cbegin(), intervals.cend()) / (double)intervals.size();
		intervals.clear();
		seconds_to_update = update_interval_seconds;
	}

	drawer.draw_text(vector2(-global::screen_width / 2, -global::screen_height / 2),
		std::string("fps: ") + std::to_string(average_interval) + std::string("\n") +
		std::string("collisions: ") + std::to_string(collider_resolver.collide_count) + std::string("\n")
	);
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