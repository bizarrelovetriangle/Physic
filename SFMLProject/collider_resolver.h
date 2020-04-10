#pragma once
#include <vector>
#include "box_block.h"
#include "gjk_functions.h"

class collider_resolver
{
public:
	primitives_drawer* drawer;
	gjk_functions gjk;
	collider_resolver(primitives_drawer* drawer);
	void resolve_vector(std::vector<box_block>& box_blocks);
	void apply_impulse(box_block& box, vector2 impulse_point, vector2 impulse_vector, bool appling);
};