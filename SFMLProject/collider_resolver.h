#include <vector>
#include "box_block.h"
#include "gjk_functions.h"

#ifndef COLLIDER_RESOLVER
#define COLLIDER_RESOLVER
class collider_resolver
{
public:
	gjk_functions gjk;
	collider_resolver(primitives_drawer* drawer);
	void resolve_vector(std::vector<box_block>& box_blocks);
};
#endif
