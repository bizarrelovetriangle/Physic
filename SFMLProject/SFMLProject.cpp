#include "box.h"
#include "gjk_functions.h"

#define screen_width 800
#define screen_height 800

vector2 center_point(0);

int main()
{
    box_block box2;
    box2.size = vector2(100., 100.);
    box2.pos = vector2(-55, 80);


    box_block box;
    box.size = vector2(100., 100.);
    box.angle = 20;

    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "SFML works!");

    sf::View view;
    sf::Vector2f center(-screen_width / 2, -screen_height / 2);
    sf::Vector2f size(screen_width, screen_height);
    view.reset(sf::FloatRect(center, size));

    window.setView(view);

    primitives_drawer drawer(window);
    gjk_functions gjk(&drawer);

    box_block* selected = NULL;
    vector2 shoulder;

    while (window.isOpen())
    {
        auto pos = sf::Mouse::getPosition(window);
        vector2 position(pos.x - (screen_width / 2), pos.y - (screen_height / 2));

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (gjk.contains_point(box.points, position)) {
                    selected = &box;
                    shoulder = box.pos - position;
                }
                if (gjk.contains_point(box2.points, position)) {
                    selected = &box2;
                    shoulder = box2.pos - position;
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (selected != NULL) {
                    if (event.key.code == sf::Keyboard::Q) {
                        selected->angle -= 10;
                    }
                    if (event.key.code == sf::Keyboard::E) {
                        selected->angle += 10;
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                selected = NULL;
            }
        }

        if (selected != NULL) {
            selected->pos = position + shoulder;
        }

        window.clear();

        box2.update_form();
        box.update_form();
        auto shape = box.create_sfml_shape();
        auto shape2 = box2.create_sfml_shape();

        window.draw(shape);

        window.draw(shape2);


        auto gjk_result = gjk.GJK(box.points, box2.points);
        gjk.EPA(box.points, box2.points, gjk_result);
        

        sf::Vertex line[4] =
        {
            sf::Vertex(vector2(-100, 0), sf::Color::White),
            sf::Vertex(vector2(100, 0), sf::Color::White),
            sf::Vertex(vector2(0, 100), sf::Color::White),
            sf::Vertex(vector2(0, -100), sf::Color::White)
        };
        window.draw(line, 4, sf::Lines);

        //double distance = GJK::line_point_distance(box.a, box.c, position);
        //
        //sf::Font font;
        //if (!font.loadFromFile("OpenSans-Regular.ttf"))
        //{
        //    throw "error?";
        //}
        //sf::Text text;
        //text.setFont(font);
        //text.setString("x: " + std::to_string(distance));
        //text.setPosition(-200, -200);
        //text.setFillColor(sf::Color::Blue);
        //text.setCharacterSize(20);
        //window.draw(text);
        //


        window.display();
    }

    return 0;
}