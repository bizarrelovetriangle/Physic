#include "vector3.h"

vector3::vector3()
	: x(0), y(0), z(0)
{

}

double vector3::dot_product(const vector2& v, const double& z)
{
	return this->x * v.x + this->y * v.y + this->z * z;
}
