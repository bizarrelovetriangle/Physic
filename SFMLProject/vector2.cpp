#include "vector2.h"

vector2 vector2::zero_vector = {0};

vector2::vector2() 
	: x(0), y(0)
{
}

vector2::vector2(double& v) : sf::Vector2f((float) v, (float) v) {
	x = v;
	y = v;
}

vector2::vector2(double v) : sf::Vector2f((float) v, (float) v) {
	x = v;
	y = v;
}

vector2::vector2(double x, double y) : sf::Vector2f((float) x, (float) y) {
	this->x = x;
	this->y = y;
}

vector2 vector2::rotate(double angle) {
	double radians = angle / 180 * 3.14159265359;
	double _x = x * cos(radians) - y * sin(radians);
	double _y = x * sin(radians) + y * cos(radians);
	return vector2(_x, _y);
}

void vector2::rotate_it(double angle) {
	double radians = angle / 180 * 3.14159265359;
	double _x = x * cos(radians) - y * sin(radians);
	double _y = x * sin(radians) + y * cos(radians);
	x = _x;
	y = _y;
	sf::Vector2f::x = (float) _x;
	sf::Vector2f::y = (float) _y;
}

vector2 vector2::normalize() {
	double length = this->length();

	if (length == 0) {
		return vector2(0, -1);
	}

	return vector2(x / length, y / length);
}

double vector2::length() {
	double _sqrt = sqrt(x * x + y * y);
	return std::isnan(_sqrt) ? 0 : _sqrt;
}

double vector2::distance(vector2 o) {
	return (*this - o).length();
}
			
auto vector2::negate() {
	return vector2(-x, -y);
}

void vector2::negate_it() {
	sf::Vector2f::x = (float) -x;
	sf::Vector2f::y = (float) -y;
	x = -x;
	y = -y;
}

double vector2::dot_product(vector2 o) {
	return x * o.x + y * o.y;
}

double vector2::cross_product(vector2 o) {
	return x * o.y - y * o.x;
}

bool vector2::is_clockwise(vector2 o) {
	return cross_product(o) < 0;
}

bool vector2::is_clockwise(vector2 o, vector2 relative) {
	return (*this - relative).cross_product(o - relative) < 0;
}

vector2 vector2::operator*(double d) {
	return vector2(x * d, y * d);
}

void vector2::operator*=(double d)
{
	x = x * d;
	y = y * d;
	sf::Vector2f::x *= (float) d;
	sf::Vector2f::y *= (float) d;
}

vector2 vector2::operator/(double d) {
	return vector2(x / d, y / d);
}

void vector2::operator/=(double d)
{
	x = x / d;
	y = y / d;
	sf::Vector2f::x /= (float) d;
	sf::Vector2f::y /= (float) d;
}

vector2 vector2::operator+(vector2 v) {
	return vector2(x + v.x, y + v.y);
}

void vector2::operator+=(vector2 v)
{
	x = x + v.x;
	y = y + v.y;
	sf::Vector2f::x += (float) v.x;
	sf::Vector2f::y += (float) v.y;
}

vector2 vector2::operator-(vector2 v) {
	return vector2(x - v.x, y - v.y);
}

void vector2::operator-=(vector2 v)
{
	x = x - v.x;
	y = y - v.y;
	sf::Vector2f::x -= (float) v.x;
	sf::Vector2f::y -= (float) v.y;
}

void vector2::operator=(vector2 v) {
	x = v.x;
	y = v.y;
	sf::Vector2f::x = (float) v.x;
	sf::Vector2f::y = (float) v.y;
}

vector2 vector2::operator-() {
	return vector2(-x, -y);
}

bool vector2::operator==(vector2 a) {
	return x == a.x && y == a.y;
}