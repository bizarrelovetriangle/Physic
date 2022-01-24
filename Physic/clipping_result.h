#pragma once
#include "vector2.h"

class clipping_result {
public:
	vector2 collision_point;
	vector2 collision_normal;
	vector2 collision_penetration_line;
	bool is_object_a_normal = true;
	clipping_result operator-() const;
	clipping_result operator+(const clipping_result& clipping_res) const;
	clipping_result operator/(const double& ratio) const;
};