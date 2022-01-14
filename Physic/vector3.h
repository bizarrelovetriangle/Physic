#pragma once
#include "vector2.h"

class vector3
{
public:
	double x, y, z;
	vector3();
	double dot_product(const vector2& v, const double& z) const;
};

