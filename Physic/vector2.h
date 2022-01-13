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
	vector2 rotate(double radians) const;
	vector2 normalize() const;
	vector2 clockwise_perpendicular() const;
	bool is_zero() const;
	bool is_nan() const;
	double length() const;
	double distance(vector2 o) const;
	auto negate() const;
	double dot_product(vector2 o) const;
	double cross_product(vector2 o) const;
	bool is_clockwise(vector2 o) const;
	bool is_clockwise(vector2 o, vector2 relative) const;
	vector2 operator*(double d) const;
	void operator*=(double d);
	vector2 operator/(double d);
	void operator/=(double d);
	vector2 operator+(vector2 v);
	void operator+=(vector2 v);
	vector2 operator-(vector2 v) const;
	void operator-=(vector2 v);
	void operator=(vector2 v);
	vector2 operator-() const;
	bool operator==(vector2 a);
};