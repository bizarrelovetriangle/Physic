#pragma once
#include <vector>
#include "box_block.h"
#include "gjk_functions.h"

class collider_resolver
{
public:
	gjk_functions gjk;
	collider_resolver(primitives_drawer* drawer);
	void resolve_vector(std::vector<box_block>& box_blocks);
};