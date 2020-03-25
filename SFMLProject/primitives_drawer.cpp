#include "primitives_drawer.h"

primitives_drawer::primitives_drawer(sf::RenderWindow& window)
	: window(&window)
{

}

sf::RenderWindow* window;

void primitives_drawer::draw_line(
	vector2& a, vector2& b, sf::Color color)
{
	sf::Vertex line[2] =
	{
		sf::Vertex(a, color),
		sf::Vertex(b, color)
	};
	window->draw(line, 2, sf::Lines);
}

void primitives_drawer::draw_number(
	vector2& pos, sf::RenderWindow& window, int number)
{
	draw_text(pos, std::to_string(number));
}

void primitives_drawer::draw_text(
	vector2& pos, std::string str)
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