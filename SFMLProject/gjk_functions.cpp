#include "gjk_functions.h"

gjk_functions::gjk_functions(primitives_drawer* drawer)
	: drawer{ drawer }
{

}

collision_result gjk_functions::EPA(
	std::vector<vector2>& a_vectors,
	std::vector<vector2>& b_vectors,
	gjk_result gjk_result)
{
	if (!gjk_result.is_collide) {
		return collision_result();;
	}

	auto& mink_a = gjk_result.mink_a;
	auto& mink_b = gjk_result.mink_b;
	auto& mink_c = gjk_result.mink_c;

	drawer->draw_line(mink_a.differens, mink_b.differens, sf::Color::Cyan);
	drawer->draw_line(mink_b.differens, mink_c.differens, sf::Color::Cyan);
	drawer->draw_line(mink_c.differens, mink_a.differens, sf::Color::Cyan);

	double a_b_distance_o = line_point_distance(mink_a.differens, mink_b.differens, vector2::zero_vector);
	double a_c_distance_o = line_point_distance(mink_a.differens, mink_c.differens, vector2::zero_vector);
	double b_c_distance_o = line_point_distance(mink_b.differens, mink_c.differens, vector2::zero_vector);

	std::list<minkowski_edge_distance> edges_sort_by_distance;

	inseart_into_sorted_list(edges_sort_by_distance, minkowski_edge_distance(a_b_distance_o, mink_a, mink_b));
	inseart_into_sorted_list(edges_sort_by_distance, minkowski_edge_distance(a_c_distance_o, mink_a, mink_c));
	inseart_into_sorted_list(edges_sort_by_distance, minkowski_edge_distance(b_c_distance_o, mink_b, mink_c));

	for (int i = 0; i < 10; i++) {
		auto nearest_edge = *edges_sort_by_distance.begin();
		edges_sort_by_distance.pop_front();

		auto& nearest_mink_a = nearest_edge.mink_a;
		auto& nearest_mink_b = nearest_edge.mink_b;

		drawer->draw_line(nearest_mink_a.differens, nearest_mink_b.differens, sf::Color::Magenta);

		auto perpendicular_from_zero = 
			perpendicular_from_point(nearest_mink_a.differens, nearest_mink_b.differens, vector2::zero_vector);

		auto new_mink_point = support_function(a_vectors, b_vectors, perpendicular_from_zero);

		if (nearest_mink_a == new_mink_point || nearest_mink_b == new_mink_point || i == 10)
		{
			drawer->draw_line(nearest_mink_a.differens, nearest_mink_b.differens, sf::Color::Blue);

			return get_collider_result(nearest_mink_a, nearest_mink_b);
		}

		double new_mink_point_nearest_a_distance_o =
			line_point_distance(new_mink_point.differens, nearest_mink_a.differens, vector2::zero_vector);
		double new_mink_point_nearest_b_distance_o =
			line_point_distance(new_mink_point.differens, nearest_mink_b.differens, vector2::zero_vector);

		inseart_into_sorted_list(edges_sort_by_distance,
			minkowski_edge_distance(new_mink_point_nearest_a_distance_o, new_mink_point, nearest_mink_a));
		inseart_into_sorted_list(edges_sort_by_distance,
			minkowski_edge_distance(new_mink_point_nearest_b_distance_o, new_mink_point, nearest_mink_b));

		drawer->draw_line(new_mink_point.differens, nearest_mink_a.differens, sf::Color::Yellow);
		drawer->draw_line(new_mink_point.differens, nearest_mink_b.differens, sf::Color::Yellow);
	}

	return collision_result();
}

collision_result gjk_functions::get_collider_result(minkowski_differens& mink_a, minkowski_differens& mink_b)
{
	collision_result result;

	if (mink_a.point_a == mink_b.point_a) {
		drawer->draw_line(*mink_a.point_b, *mink_b.point_b, sf::Color::Blue);
		result.collision_point = *mink_a.point_a;
		result.collision_normal = perpendicular_from_point(*mink_a.point_b, *mink_b.point_b, *mink_a.point_a);
	}
	else {
		drawer->draw_line(*mink_a.point_a, *mink_b.point_a, sf::Color::Blue);
		result.collision_point = *mink_a.point_b;
		result.collision_normal = perpendicular_from_point(*mink_a.point_a, *mink_b.point_a, *mink_a.point_b);
	}

	auto norm = result.collision_normal.normalize() * 100;

	auto ttyyt = result.collision_point + norm;

	drawer->draw_line(result.collision_point, ttyyt, sf::Color::Red);

	return result;
}

void gjk_functions::inseart_into_sorted_list(
	std::list<minkowski_edge_distance>& edges_sort_by_distance,
	minkowski_edge_distance&& new_edge_distance)
{
	if (edges_sort_by_distance.empty() ||
		new_edge_distance.distance > edges_sort_by_distance.back().distance)
	{
		edges_sort_by_distance.push_back(new_edge_distance);
		return;
	}

	auto position = std::find_if(edges_sort_by_distance.begin(), edges_sort_by_distance.end(),
		[&new_edge_distance](auto edge_distance)
		{
			return new_edge_distance.distance < edge_distance.distance;
		});

	edges_sort_by_distance.emplace(position, new_edge_distance);
}

gjk_result gjk_functions::GJK(
	std::vector<vector2>& a_vectors,
	std::vector<vector2>& b_vectors)
{
	int counter = 1;

	vector2 direction(1, 0);

	auto mink_a = support_function(a_vectors, b_vectors, direction);
	auto mink_b = support_function(a_vectors, b_vectors, -direction);
	//drawer->draw_line(mink_a.differens, mink_b.differens, window, sf::Color::Red);
	//drawer->draw_number(mink_a.differens, window, 1);
	//drawer->draw_number(mink_b.differens, window, 2);

	for (int i = 0; i < 10; i++) {
		direction = perpendicular_to_point(mink_a.differens, mink_b.differens, vector2::zero_vector);
		auto mink_c = support_function(a_vectors, b_vectors, direction);

		if (mink_a == mink_c || mink_b == mink_c) {
			return false;
		}

		//drawer->draw_line(b, c, window, sf::Color::Red);
		//drawer->draw_number(c, window, 3);

		if (triangle_contains(mink_a.differens, mink_b.differens, mink_c.differens, vector2::zero_vector)) {
			//drawer->draw_line(a, b, window, sf::Color::Cyan);
			//drawer->draw_line(b, c, window, sf::Color::Cyan);
			//drawer->draw_line(c, a, window, sf::Color::Cyan);
			return gjk_result(true, mink_a, mink_b, mink_c);
		}

		if (line_point_distance(mink_a.differens, mink_c.differens, vector2::zero_vector) <
			line_point_distance(mink_b.differens, mink_c.differens, vector2::zero_vector)) {
			//drawer->draw_line(mink_b.differens, mink_c.differens, sf::Color::Yellow);
			mink_b = mink_c;
		}
		else {
			//drawer->draw_line(mink_a.differens, mink_c.differens, sf::Color::Yellow);
			mink_a = mink_c;
		}
	}

	return gjk_result(false);
}

minkowski_differens gjk_functions::support_function(
	std::vector<vector2>& a_vectors,
	std::vector<vector2>& b_vectors,
	vector2 direction)
{
	auto a = farthest_point(a_vectors, direction);
	auto b = farthest_point(b_vectors, -direction);
	return minkowski_differens(a, b, *a - *b);
}

bool gjk_functions::contains_point(
	std::vector<vector2>& vectors,
	vector2 point)
{
	vector2 direction(1, 0);

	auto a = *farthest_point(vectors, direction);
	auto b = *farthest_point(vectors, -direction);

	for (int i = 0; i < 10; i++) {
		direction = perpendicular_to_point(a, b, point);
		auto c = *farthest_point(vectors, direction);

		if (a == c || b == c) {
			return false;
		}

		if (triangle_contains(a, b, c, point)) {
			return true;
		}

		if (line_point_distance(a, c, point) < line_point_distance(b, c, point)) {
			b = c;
		}
		else {
			a = c;
		}
	}

	return false;
}

vector2* gjk_functions::farthest_point(
	std::vector<vector2>& vectors,
	vector2 direction)
{
	auto* farthest_point = &vectors[0];
	auto farthest_dot_product = farthest_point->dot_product(direction);

	for (int i = 1; i < vectors.size(); i++) {
		auto dot_product = vectors[i].dot_product(direction);

		if (dot_product > farthest_dot_product) {
			farthest_dot_product = dot_product;
			farthest_point = &vectors[i];
		}
	}

	return farthest_point;
}

vector2& gjk_functions::perpendicular_to_point(
	vector2& a, vector2& b,
	vector2& o)
{
	auto b_a = b - a;
	auto o_a = o - a;

	// negative cross product means that point is clockwise relative b_a
	if (b_a.is_clockwise(o_a))
	{
		b_a.negate_it();
	}

	// this method always return non clockwise perpendicular
	vector2 perp_to_point(-b_a.y, b_a.x);
	return perp_to_point;
}

vector2& gjk_functions::perpendicular_from_point(
	vector2& a, vector2& b,
	vector2& o)
{
	auto b_a = b - a;
	auto o_a = o - a;

	// negative cross product means that point is clockwise relative b_a
	if (!b_a.is_clockwise(o_a))
	{
		b_a.negate_it();
	}

	// this method always return non clockwise perpendicular
	vector2 perp_to_point(-b_a.y, b_a.x);
	return perp_to_point;
}

// for convex and non convex shape
double gjk_functions::line_point_distance(
	vector2& a, vector2& b,
	vector2& o)
{
	auto b_a = b - a;
	auto b_a_normalize = b_a.normalize();
	auto o_a = o - a;

	double projection_length = b_a_normalize.dot_product(o_a);

	vector2 projection_point;

	if (projection_length < 0) {
		projection_point = a;
	}
	else if (projection_length > b_a.length()) {
		projection_point = b;
	}
	else {
		projection_point = (b_a_normalize * projection_length) + a;
	}

	double line_point_distanse = projection_point.distanse(o);

	return line_point_distanse;
}

bool gjk_functions::triangle_contains(
	vector2& a, vector2& b, vector2& c,
	vector2& o)
{
	bool b_a_cw = b.is_clockwise(o, a);
	bool c_b_cw = c.is_clockwise(o, b);
	bool a_c_cw = a.is_clockwise(o, c);

	if ((b_a_cw && c_b_cw && a_c_cw) ||
		!(b_a_cw || c_b_cw || a_c_cw)) {
		return true;
	}

	return false;
}