#include "collider_resolver.h"

collider_resolver::collider_resolver(primitives_drawer* drawer)
    : gjk(drawer)
{
}

void collider_resolver::resolve_vector(std::vector<box_block>& box_blocks)
{
    for (int i = 0; i < box_blocks.size(); i++) {
        for (int i2 = i + 1; i2 < box_blocks.size(); i2++) {
            box_block& _box2 = box_blocks[i];
            box_block& _box = box_blocks[i2];

            auto gjk_result = gjk.GJK(_box.points, _box2.points);
            
            if (gjk_result.is_collide) {
                gjk.EPA(_box.points, _box2.points, gjk_result);
            }
        }
    }
}
