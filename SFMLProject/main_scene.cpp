#include "main_scene.h"

main_scene::main_scene() : 
	window(sf::VideoMode(screen_width, screen_height), "SFML works!"),
	drawer(window),
	gjk(drawer),
	collider_resolver(drawer),
	mouse_picker(phisic_objects, gjk, collider_resolver)
{
	sf::View view;
	vector2 center(-screen_width / 2, -screen_height / 2);
	vector2 size(screen_width, screen_height);
	view.reset(sf::FloatRect(center, size));
	window.setView(view);

	create_walls();

	for (int i = 0; i < 4; i++) {
		for (int i2 = 0; i2 < 5; i2++) {
			box_block* _box = new box_block(
				vector2(-200. + 101. * i2, -100 + 101. * i),
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

void main_scene::start() 
{
	while (window.isOpen())
	{
		auto pos = sf::Mouse::getPosition(window);
		vector2 mouse_position = vector2(pos.x - (screen_width / 2.), pos.y - (screen_height / 2.));

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				mouse_picker.take_object(mouse_position);
			}
			if (event.type == sf::Event::KeyPressed) {
				mouse_picker.control_object(event);
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				mouse_picker.release_object();
			}
		}

		window.clear();

		for (auto& object : phisic_objects) {
			object->update_form();
		}

		for (auto& object : phisic_objects) {
			object->draw(window);
		}

		collider_resolver.resolve_collision_vector(phisic_objects);

		//drawer.draw_number(vector2(-200), collider_resolver.collide_count);
		mouse_picker.update_object(mouse_position);

		draw_coordinates();

		window.display();
		sf::sleep(sf::seconds(0.01f));
	}
}

void main_scene::create_walls()
{
	int wall_width = 700;
	int wall_height = 700;

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