#include "collider_resolver.h"

collider_resolver::collider_resolver(primitives_drawer* drawer)
    : gjk(drawer), drawer(drawer)
{
}

void collider_resolver::resolve_vector(std::vector<box_block>& box_blocks)
{
    for (int i = 0; i < box_blocks.size(); i++) {
        for (int i2 = i + 1; i2 < box_blocks.size(); i2++) {
            box_block& _box = box_blocks[i];
            box_block& _box2 = box_blocks[i2];

            auto gjk_result = gjk.GJK(_box.points, _box2.points);
            
            if (gjk_result.is_collide) {
                gjk.EPA(_box.points, _box2.points, gjk_result);
            }
        }
    }
}

void collider_resolver::apply_impulse(box_block& box, vector2 impulse_point, vector2 impulse_vector, bool appling)
{
    vector2 impulse_vector_normal = impulse_vector.normalize();

    drawer->draw_cross(impulse_point, sf::Color::White);

    drawer->draw_line(box.position, impulse_point, sf::Color::White);

    vector2 sholder_vector = impulse_point - box.position;

    auto impulse_point_velocity = box.velocity;

    vector2 impulse_vector_old = impulse_vector;

    impulse_vector -= impulse_point_velocity;

    vector2 rotate_velocity_normal = vector2(-sholder_vector.y, sholder_vector.x).normalize();
    
    if (box.angle_velocity < 0) {
        rotate_velocity_normal = -rotate_velocity_normal;
    }
    
    auto impulse_point_rotate_velocity = rotate_velocity_normal *
        (2 * 3.1415 * sholder_vector.length() * box.angle_velocity / 360);
    
    auto impulse_point_rotate_velocity2 = impulse_vector_normal *
        impulse_vector_normal.dot_product(impulse_point_rotate_velocity);
    
    impulse_vector -= impulse_point_rotate_velocity2;
    
    drawer->draw_line(impulse_point, impulse_point + impulse_vector, sf::Color::Blue);
    
    if (appling) {
        box.angle_velocity += sholder_vector.cross_product(impulse_vector) * box.moment_of_inetia;
        box.velocity += impulse_vector;
    }
}