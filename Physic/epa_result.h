#pragma once
#include "vector2.h"

class epa_result {
public:
	epa_result();
	epa_result(const vector2& collision_normal, bool is_object_1_normal);
	vector2 collision_normal;
	bool is_object_1_normal = true;
};