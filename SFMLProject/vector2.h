#include "math.h"
#include <SFML/Graphics.hpp>

namespace primitives {

#ifndef VECTOR2
#define VECTOR2

	class vector2 {
	public:
		double x = 0., y = 0.;

		double& width = x;
		double& height = y;

		static vector2 zero_vector;

		vector2() {

		}

		vector2(double& v) {
			x = v;
			y = v;
		}

		vector2(double v) {
			x = v;
			y = v;
		}

		vector2(double x, double y) {
			this->x = x;
			this->y = y;
		}

		//vector2 rotate(double angle) {
		//	double radians = angle / 180 * 3.1415;
		//	double _x = x * cos(radians) + y * sin(radians);
		//	double _y = x * sin(radians) - y * cos(radians);
		//	return vector2(_x, _y);
		//}

		void rotate(double angle) {
			double radians = angle / 180 * 3.1416;
			double _x = x * cos(radians) + y * sin(radians);
			double _y = x * sin(radians) - y * cos(radians);
			
			x = _x;
			y = _y;
		}

		auto normalize() {
			double length = this->length();
			return vector2(x / length, y / length);
		}

		double length() {
			return sqrt(x * x + y * y);
		}

		double distanse(vector2 o) {
			return (*this - o).length();
		}
			
		void negate() {
			x = -x;
			y = -y;
		}

		//auto negate() {
		//	return vector2(-x, -y);
		//}

		double dot_product(primitives::vector2 o) {
			return x * o.x + y * o.y;
		}

		double cross_product(primitives::vector2 o) {
			return x * o.y - y * o.x;
		}

		bool is_clockwise(primitives::vector2 o) {
			return cross_product(o) < 0;
		}

		bool is_clockwise(primitives::vector2 o, primitives::vector2 relative) {
			return (*this - relative).cross_product(o - relative) < 0;
		}

		auto operator*(double d) {
			return vector2(this->x * d, this->y * d);
		}

		auto operator/(double d) {
			return vector2(this->x / d, this->y / d);
		}

		auto operator+(vector2 v) {
			return vector2(this->x + v.x, this->y + v.y);
		}

		vector2 operator-(vector2 v) {
			return vector2(this->x - v.x, this->y - v.y);
		}

		auto operator=(vector2 v) {
			this->x = v.x;
			this->y = v.y;
		}

		auto operator-() {
			return vector2(-this->x, -this->y);
		}

		auto operator==(vector2 a) {
			return x == a.x && y == a.y;
		}

		sf::Vector2f sf_vector() {
			return sf::Vector2f(x, y);
		}
	};
#endif
}