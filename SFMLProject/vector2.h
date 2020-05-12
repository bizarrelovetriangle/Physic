#pragma once
#include "math.h"
#include <SFML/Graphics.hpp>

class vector2 : public sf::Vector2f{
public:
	double x = 0., y = 0.;

	static vector2 zero_vector;

	vector2();

	vector2(double& v);

	vector2(double v);

	vector2(double x, double y);

	vector2 rotate(double angle);

	void rotate_it(double angle);

	vector2 normalize();

	double length();

	double distance(vector2 o);
			
	auto negate();

	void negate_it();

	double dot_product(vector2 o);

	double cross_product(vector2 o);

	bool is_clockwise(vector2 o);

	bool is_clockwise(vector2 o, vector2 relative);

	vector2 operator*(double d);

	void operator*=(double d);

	vector2 operator/(double d);

	void operator/=(double d);

	vector2 operator+(vector2 v);

	void operator+=(vector2 v);

	vector2 operator-(vector2 v);

	void operator-=(vector2 v);

	void operator=(vector2 v);

	vector2 operator-();

	bool operator==(vector2 a);
};