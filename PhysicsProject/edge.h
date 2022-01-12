#pragma once
#include "vector2.h"

class edge {
public:
	vector2 &a, &b;
	edge(vector2& a, vector2& b);
};