#include "box_block.h"
#include "gjk_functions.h"
#include "collider_resolver.h"
#include "mouse_provider.h"
#include "wall_block.h"

#define screen_width 800
#define screen_height 800

vector2 center_point(0);

void draw_coordinates(sf::RenderWindow& window);

int main()
{
    std::vector<phisic_object*> phisic_objects;

    box_block* _box = new box_block();
    _box->size = vector2(100., 100.);
    _box->position = vector2(-100, -100);
    _box->angle = 90;
    _box->velocity.x = 1;

    wall_block* _wall = new wall_block(vector2(100, -100), (100, 100));

    phisic_objects.emplace_back(_wall);
    phisic_objects.emplace_back(_box);

    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "SFML works!");

    sf::View view;
    sf::Vector2f center(-screen_width / 2, -screen_height / 2);
    sf::Vector2f size(screen_width, screen_height); 
    view.reset(sf::FloatRect(center, size));

    window.setView(view);
    
    primitives_drawer drawer(window);
    gjk_functions gjk(&drawer);
    collider_resolver collider_resolver(&drawer);

    mouse_provider mouse_provider(200);
    
    phisic_object* selected = NULL;
    vector2 shoulder;

    while (window.isOpen())
    {
        auto pos = sf::Mouse::getPosition(window);
        vector2 mouse_position = vector2(pos.x - (screen_width / 2), pos.y - (screen_height / 2));
        mouse_provider.taking_new_position(mouse_position);
        auto mouse_speed = mouse_provider.filtered_speed();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                for (auto& box : phisic_objects) {
                    if (gjk.contains_point(box->points, mouse_position)) {
                        selected = box;
                        //selected->velocity = vector2(0, 0);
                        //selected->angle_velocity = 0;
                        shoulder = (mouse_position - box->position).rotate(-selected->angle);
                        break;
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    collider_resolver.apply_impulse(*_box, _box->points[0], vector2(0, -5), true);
                }

                if (selected != NULL) {
                    if (event.key.code == sf::Keyboard::Q) {
                        selected->angle -= 11.25;
                    }
                    if (event.key.code == sf::Keyboard::E) {
                        selected->angle += 11.25;
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (selected != NULL) {
                    //selected->velocity = mouse_provider.filtered_speed();
                }
                selected = NULL;
            }
        }

        window.clear();

        for (auto& object : phisic_objects) {
            object->update_form();
        }

        for (auto& object : phisic_objects) {
             object->draw(window);
        }

        collider_resolver.resolve_vector(phisic_objects);

        if (selected != NULL) {
            vector2 new_shoulder = shoulder.rotate(selected->angle) + _box->position;
            vector2 impulse_vector = mouse_position - new_shoulder;
            collider_resolver.apply_impulse(*_box, new_shoulder, impulse_vector, true);
        }

        if (_box->position.y > screen_height / 2) {
            _box->velocity.y = 0;
            _box->position = vector2(_box->position.x, screen_height / 2 - 0.3);
        }

        draw_coordinates(window);

        window.display();
        sf::sleep(sf::seconds(0.01f));
    }

    return 0;
}

void draw_coordinates(sf::RenderWindow& window) {
    sf::Vertex line[4] =
    {
        sf::Vertex(vector2(-100, 0), sf::Color::White),
        sf::Vertex(vector2(100, 0), sf::Color::White),
        sf::Vertex(vector2(0, 100), sf::Color::White),
        sf::Vertex(vector2(0, -100), sf::Color::White)
    };

    window.draw(line, 4, sf::Lines);
}