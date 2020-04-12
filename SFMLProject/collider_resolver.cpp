#include "collider_resolver.h"

collider_resolver::collider_resolver(primitives_drawer* drawer)
    : gjk(drawer), drawer(drawer)
{
}

void collider_resolver::resolve_vector(std::vector<phisic_object*>& phisic_objects)
{
    for (int i = 0; i < phisic_objects.size(); i++) {
        for (int i2 = i + 1; i2 < phisic_objects.size(); i2++) {
            phisic_object* object_1 = phisic_objects[i];
            phisic_object* object_2 = phisic_objects[i2];

            auto gjk_result = gjk.GJK(object_1->points, object_2->points);

            if (gjk_result.is_collide) {
                gjk.EPA(object_1->points, object_2->points, gjk_result);
            }
        }
    }
}

void collider_resolver::apply_impulse(phisic_object& object, vector2 impulse_point, vector2 impulse_vector, bool appling)
{
    vector2 impulse_vector_normal = impulse_vector.normalize();

    drawer->draw_cross(impulse_point, sf::Color::White);

    drawer->draw_line(object.position, impulse_point, sf::Color::White);

    vector2 sholder_vector = impulse_point - object.position;

    auto impulse_point_velocity = object.velocity;

    vector2 impulse_vector_old = impulse_vector;

    impulse_vector -= impulse_point_velocity;

    vector2 rotate_velocity_normal = vector2(-sholder_vector.y, sholder_vector.x).normalize();
    
    if (object.angle_velocity < 0) {
        rotate_velocity_normal = -rotate_velocity_normal;
    }
    
    auto impulse_point_rotate_velocity = rotate_velocity_normal *
        (2 * 3.1415 * sholder_vector.length() * object.angle_velocity / 360);
    
    auto impulse_point_rotate_velocity2 = impulse_vector_normal *
        impulse_vector_normal.dot_product(impulse_point_rotate_velocity);
    
    impulse_vector -= impulse_point_rotate_velocity2;
    
    drawer->draw_line(impulse_point, impulse_point + impulse_vector, sf::Color::Blue);
    
    if (appling) {
        object.angle_velocity += sholder_vector.cross_product(impulse_vector) * object.moment_of_inetia;
        object.velocity += impulse_vector;
    }
}