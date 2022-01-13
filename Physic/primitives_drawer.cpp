#include "primitives_drawer.h"

primitives_drawer::primitives_drawer(sf::RenderWindow& window)
	: window(&window)
{

}

sf::RenderWindow* window;

void primitives_drawer::draw_line(
	const vector2& a, const vector2& b, const sf::Color& color)
{
	sf::Vertex line[2] =
	{
		sf::Vertex(a, color),
		sf::Vertex(b, color)
	};
	window->draw(line, 2, sf::Lines);
}

void primitives_drawer::draw_cross(
	const vector2& point, const sf::Color& color)
{
	double cross_size = 3;

	vector2 a(point.x - cross_size, point.y - cross_size);
	vector2 b(point.x + cross_size, point.y + cross_size);
	draw_line(a, b, color);

	vector2 c(point.x - cross_size, point.y + cross_size);
	vector2 d(point.x + cross_size, point.y - cross_size);
	draw_line(c, d, color);
}

void primitives_drawer::draw_number(
	const vector2& pos, double number)
{
	draw_text(pos, std::to_string(number));
}

void primitives_drawer::draw_text(
	const vector2& pos, std::string str)
{
	static int i = 1;
	static sf::Font font;
	if (i == 1 && !font.loadFromFile("arial.ttf"))
	{
		throw "error?";
	}
	i = 0;

	sf::Text text;
	text.setFont(font);
	text.setString(str);
	text.setPosition(pos);
	text.setFillColor(sf::Color::Blue);
	text.setCharacterSize(20);
	window->draw(text);
}