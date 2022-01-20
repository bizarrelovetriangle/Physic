#include "main_scene.h"
#include "star.h"

main_scene::main_scene() :
	window(sf::VideoMode(global::screen_width, global::screen_height), "PhysicsProject"),
	drawer(window),
	gjk(drawer),
	collider_resolver(drawer),
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
		frame_interval = clock.restart().asSeconds();
		
		scene_update();
		scene_draw();
		//sf::sleep(sf::seconds(0.05f));
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

	collider_resolver.resolve_collisions(physic_objects);

	for (auto& object : physic_objects) {
		object->update(frame_interval * 50);
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
	for (int i = 0; i < 8; i++) {
		for (int i2 = 0; i2 < 3; i2++) {
			star* _box = new star(
				vector2(-200. + 101. * i2, -100 + 101. * i),
				i * i2 % 2 ? 100 : 70);

			_box->acceleration.y = 0.1;

			physic_objects.emplace_back(_box);
		}
	}

	for (int i = 0; i < 8; i++) {
		for (int i2 = 0; i2 < 2; i2++) {
			box_block* _box = new box_block(
				vector2( + 101. * i2, -100 + 101. * i),
				i * i2 % 2 ? 100 : 70);

			_box->acceleration.y = 0.1;

			physic_objects.emplace_back(_box);
		}
	}

	for (int i = 0; i < 8; i++) {
		for (int i2 = 0; i2 < 1; i2++) {
			box_block* _box = new box_block(
				vector2(200 + 101. * i2, -100 + 101. * i),
				vector2(i % 2 ? 120 : 70, 30));

			_box->acceleration.y = 0.1;

			physic_objects.emplace_back(_box);
		}
	}

	//for (int i = 0; i < 7; i++) {
	//	for (int i2 = 0; i2 < 11; i2++) {
	//		star* _box = new star(
	//			vector2(-500. + 101. * i2, -200 + 101. * i), i * i2 % 2 ? 100 : 70);
	//
	//		_box->acceleration.y = -0.1;
	//
	//		physic_objects.emplace_back(_box);
	//	}
	//}

	//star* _star = new star(vector2(0., 200));
	//_star->acceleration.y = 0.1;
	//physic_objects.emplace_back(_star);
	//
	//star* _star2 = new star(vector2(200., 200));
	//_star2->acceleration.y = 0.1;
	//_star2->velocity.x = -1;
	//physic_objects.emplace_back(_star2);
	 
	box_block* _box = new box_block(
		vector2(-200., -200),
		vector2(50., 200.));
	_box->velocity = vector2(0, 2);
	_box->radians_velocity = 0.1;
	//_box->acceleration.y = 0.1;
	//_box->acceleration.x = 0.1;
	physic_objects.emplace_back(_box);
	
	//box_block* _box2 = new box_block(
	//	vector2(100., 200.),
	//	vector2(100., 100.));
	//_box2->radians = 1;
	////_box2->acceleration.y = 0.1;
	//_box2->name = "observable";
	//physic_objects.emplace_back(_box2);
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
	wall_block* borrom_wall = new wall_block(d, a);

	physic_objects.emplace_back(left_wall);
	physic_objects.emplace_back(top_wall);
	physic_objects.emplace_back(right_wall);
	physic_objects.emplace_back(borrom_wall);
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

	drawer.draw_text(vector2(-global::screen_width / 2 , -global::screen_height / 2),
		std::string("fps: ") + std::to_string(average_interval) + std::string("\n") +
		std::string("collisions: ") + std::to_string(collider_resolver.collide_count));
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