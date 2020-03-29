#include "box_block.h"
#include "gjk_functions.h"
#include "collider_resolver.h"

#define screen_width 800
#define screen_height 800

vector2 center_point(0);

int main()
{
    std::vector<box_block> box_blocks;

    box_blocks.emplace_back();
    box_blocks.emplace_back();

    box_block& _box2 = box_blocks[0];
    _box2.size = vector2(100., 200.);
    _box2.position = vector2(-55, 80);

    box_block& _box = box_blocks[1];
    _box.size = vector2(100., 100.);
    _box.angle = 20;

    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "SFML works!");

    sf::View view;
    sf::Vector2f center(-screen_width / 2, -screen_height / 2);
    sf::Vector2f size(screen_width, screen_height);
    view.reset(sf::FloatRect(center, size));

    window.setView(view);

    primitives_drawer drawer(window);
    gjk_functions gjk(&drawer);
    collider_resolver collider_resolver(&drawer);

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
                for (auto& box : box_blocks) {
                    if (gjk.contains_point(box.points, position)) {
                        selected = &box;
                        shoulder = box.position - position;
                        break;
                    }
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
            selected->position = position + shoulder;
        }

        window.clear();

        for (auto& box : box_blocks) {
            box.update_form();
        }

        for (auto& box : box_blocks) {
            window.draw(box.sfml_shape);
        }

        collider_resolver.resolve_vector(box_blocks);
        
        sf::Vertex line[4] =
        {
            sf::Vertex(vector2(-100, 0), sf::Color::White),
            sf::Vertex(vector2(100, 0), sf::Color::White),
            sf::Vertex(vector2(0, 100), sf::Color::White),
            sf::Vertex(vector2(0, -100), sf::Color::White)
        };

        window.draw(line, 4, sf::Lines);

        window.display();
    }

    return 0;
}