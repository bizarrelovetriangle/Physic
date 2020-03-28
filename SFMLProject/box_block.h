#include "vector2.h"
#include <vector>

#ifndef BOX_BLOCK
#define BOX_BLOCK
class box_block {
public:
	vector2 size;
	vector2 position;
	vector2 velocity;
	vector2 acceleration;

	double angle = 0;
	double angle_velocity = 0;

	std::vector<vector2> original_points { 0, 0, 0, 0 };
	vector2& a = original_points[0];
	vector2& b = original_points[1];
	vector2& c = original_points[2];
	vector2& d = original_points[3];

	std::vector<vector2> points { 0, 0, 0, 0 };

	sf::ConvexShape sfml_shape;

	box_block(vector2 pos = {}, vector2 size = {});

	void update_form();

	sf::ConvexShape create_sfml_shape();
};
#endif