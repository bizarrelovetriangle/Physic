#include "clipping_result.h"

clipping_result clipping_result::operator-() const
{
	clipping_result result;
	result.collision_point = collision_point;
	result.collision_normal = -collision_normal;
	result.collision_penetration_line = -collision_penetration_line;
	return result;
}

clipping_result clipping_result::operator+(const clipping_result& clipping_res) const
{
	clipping_result a = is_object_a_normal ? *this : -*this;
	clipping_result b = clipping_res.is_object_a_normal ? clipping_res : -clipping_res;

	clipping_result result;
	result.collision_point = a.collision_point + b.collision_point;
	result.collision_normal = (a.collision_normal + b.collision_normal).normalize();
	result.collision_penetration_line = a.collision_penetration_line + b.collision_penetration_line;
	return result;
}

clipping_result clipping_result::operator/(const double& ratio) const
{
	clipping_result result;
	result.collision_point = collision_point / ratio;
	result.collision_normal = collision_normal;
	result.collision_penetration_line = collision_penetration_line / ratio;
	return result;
}
