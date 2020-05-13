#include "gjk_functions.h"

gjk_functions::gjk_functions(primitives_drawer* drawer)
	: drawer{ drawer }
{
}

void gjk_functions::clipping(
	std::vector<vector2>& a_vectors,
	std::vector<vector2>& b_vectors,
	std::vector<edge>& a_edges,
	std::vector<edge>& b_edges,
	epa_result& epa_result) 
{
	vector2 normal = epa_result.flip
		? -epa_result.collision_normal
		: epa_result.collision_normal;

	auto& a_farthest = farthest_point(a_vectors, normal);
	auto& b_farthest = farthest_point(b_vectors, -normal);

	auto &a_best_edge = gjk_functions::find_best_edge(a_edges, a_farthest, normal);
	auto &b_best_edge = gjk_functions::find_best_edge(b_edges, b_farthest, normal);

	edge* reference_edge;
	edge* incident_edge;

	if (epa_result.flip) {
		reference_edge = &a_best_edge;
		incident_edge = &b_best_edge;
	}
	else {
		reference_edge = &b_best_edge;
		incident_edge = &a_best_edge;
	}

	auto incident_a_b = incident_edge->a - incident_edge->b;

	auto reference_edge_nolmalize = (reference_edge->a - reference_edge->b).normalize();

	vector2 incident_vertices[2] = { incident_edge->a, incident_edge->b };
	vector2 reference_vertices[2] = { reference_edge->a, reference_edge->b };

	//drawer->draw_line(reference_edge->a, reference_edge->b, sf::Color::Magenta);
	//drawer->draw_line(incident_edge->a, incident_edge->b, sf::Color::White);

	double incident_edge_a_dot = incident_edge->a.dot_product(reference_edge_nolmalize);
	double incident_edge_b_dot = incident_edge->b.dot_product(reference_edge_nolmalize);


	//return;
	// there was bugю копирую грань, а она хранит ссылки на точки 
	for (int i = 0; i < 2; i++) {
		auto& reference_vertex = reference_vertices[i];
		double reference_vertex_dot = reference_vertex.dot_product(reference_edge_nolmalize);
		
		for (int i2 = 0; i2 < 2; i2++) {
			auto& incident_vertex = incident_vertices[i2];
			double incident_vertex_dot = incident_vertex.dot_product(reference_edge_nolmalize);

			if (incident_vertex_dot > reference_vertex_dot != i) {
				double d = (reference_vertex_dot - incident_edge_b_dot) / 
					(incident_edge_a_dot - incident_edge_b_dot);
				incident_vertex = incident_a_b * d + incident_edge->b;
			}
		}
	}


	std::vector<vector2> contact_points;

	for (int i = 0; i < 2; i++) {
		auto& incident_vertex = incident_vertices[i];
		
		if (reference_edge->a.is_clockwise(incident_vertex, reference_edge->b) != 
				reference_edge_nolmalize.cross_product(epa_result.collision_normal) > 0) {
			//drawer->draw_cross(incident_vertex, sf::Color::Red);
			contact_points.push_back(incident_vertex);
		}
		else {
			//drawer->draw_cross(incident_vertex, sf::Color::Blue); 
		}
	}

	if (contact_points.empty()) {
		return;
	}

	vector2& summ = contact_points[0];
	for (int i = 1; i < contact_points.size(); i++)
	{
		summ += contact_points[i];
	}

	auto collision_point = summ / contact_points.size();
	auto proj_point = projection_point(reference_edge->a, reference_edge->b, collision_point);
	auto drawing_vector_point = proj_point + epa_result.collision_normal * 100;

	epa_result.collision_point = summ / contact_points.size();
	epa_result.collision_penetration_line = collision_point - proj_point;
	epa_result.collision_penetration = epa_result.collision_penetration_line.length();
	epa_result.collision_normal = epa_result.collision_penetration_line.normalize();

	//drawer->draw_line(proj_point, drawing_vector_point, sf::Color::Blue);
	//drawer->draw_line(epa_result.collision_point, proj_point, sf::Color::Red);
	drawer->draw_cross(epa_result.collision_point, sf::Color::White);
}

edge& gjk_functions::find_best_edge(
	std::vector<edge>& edges,
	vector2& farthest_point,
	vector2& normal)
{
	double min_dot = std::numeric_limits<double>::max();
	edge* best_edge = nullptr;

	for (auto& edge : edges) {
		if (edge.a == farthest_point || edge.b == farthest_point) {
			double dot = abs((edge.a - edge.b).dot_product(normal));

			if (dot < min_dot) {
				min_dot = dot;
				best_edge = &edge;
			}
		}
	}

	return *best_edge;
}

epa_result gjk_functions::EPA(
	std::vector<vector2>& a_vectors,
	std::vector<vector2>& b_vectors,
	gjk_result gjk_result)
{
	if (!gjk_result.is_collide) {
		return epa_result();;
	}

	auto& mink_a = gjk_result.mink_a;
	auto& mink_b = gjk_result.mink_b;
	auto& mink_c = gjk_result.mink_c;

	double a_b_distance_o = line_point_distance_convex(mink_a.differens, mink_b.differens, vector2::zero_vector);
	double a_c_distance_o = line_point_distance_convex(mink_a.differens, mink_c.differens, vector2::zero_vector);
	double b_c_distance_o = line_point_distance_convex(mink_b.differens, mink_c.differens, vector2::zero_vector);

	std::list<minkowski_edge_distance> edges_sort_by_distance;

	inseart_into_sorted_list(edges_sort_by_distance, minkowski_edge_distance(a_b_distance_o, mink_a, mink_b));
	inseart_into_sorted_list(edges_sort_by_distance, minkowski_edge_distance(a_c_distance_o, mink_a, mink_c));
	inseart_into_sorted_list(edges_sort_by_distance, minkowski_edge_distance(b_c_distance_o, mink_b, mink_c));

	for (int i = 0; i < 10; i++) {
		auto nearest_edge = *edges_sort_by_distance.begin();
		edges_sort_by_distance.pop_front();

		auto& nearest_mink_a = nearest_edge.mink_a;
		auto& nearest_mink_b = nearest_edge.mink_b;

		auto perpendicular_from_zero = 
			perpendicular_from_point(nearest_mink_a.differens, nearest_mink_b.differens, vector2::zero_vector);

		auto new_mink_point = support_function(a_vectors, b_vectors, perpendicular_from_zero);

		if (nearest_mink_a == new_mink_point || nearest_mink_b == new_mink_point || i == 10)
		{
			return get_collider_result(nearest_mink_a, nearest_mink_b);
		}

		double new_mink_point_nearest_a_distance_o =
			line_point_distance_convex(new_mink_point.differens, nearest_mink_a.differens, vector2::zero_vector);
		double new_mink_point_nearest_b_distance_o =
			line_point_distance_convex(new_mink_point.differens, nearest_mink_b.differens, vector2::zero_vector);

		inseart_into_sorted_list(edges_sort_by_distance,
			minkowski_edge_distance(new_mink_point_nearest_a_distance_o, new_mink_point, nearest_mink_a));
		inseart_into_sorted_list(edges_sort_by_distance,
			minkowski_edge_distance(new_mink_point_nearest_b_distance_o, new_mink_point, nearest_mink_b));
	}

	return epa_result();
}

epa_result gjk_functions::get_collider_result(minkowski_differens& mink_a, minkowski_differens& mink_b)
{
	epa_result result;

	vector2* penetration_point;
	vector2* collision_edje_a;
	vector2* collision_edje_b;

	if (mink_a.point_a == mink_b.point_a) {
		penetration_point = mink_a.point_a;
		collision_edje_a =  mink_a.point_b;
		collision_edje_b =  mink_b.point_b; // or mink_a.point_b
		result.flip = false;
	}
	else {
		penetration_point = mink_a.point_b;
		collision_edje_a =  mink_a.point_a;
		collision_edje_b =  mink_b.point_a; // or mink_a.point_a
		result.flip = true;
	}

	auto proj_point = projection_point(*collision_edje_a, *collision_edje_b, *penetration_point);

	result.collision_point = *penetration_point;
	result.collision_penetration_line = *penetration_point - proj_point;
	result.collision_penetration = result.collision_penetration_line.length();
	result.collision_normal = result.collision_penetration_line.normalize();

	//auto drawing_vector_point = proj_point + result.collision_normal * 100;
	//drawer->draw_line(proj_point, drawing_vector_point, sf::Color::Blue);

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

	for (int i = 0; i < 10; i++) {
		direction = perpendicular_to_point(mink_a.differens, mink_b.differens, vector2::zero_vector);
		auto mink_c = support_function(a_vectors, b_vectors, direction);

		if (mink_a == mink_c || mink_b == mink_c) {
			return false;
		}

		if (triangle_contains(mink_a.differens, mink_b.differens, mink_c.differens, vector2::zero_vector)) {
			return gjk_result(true, mink_a, mink_b, mink_c);
		}

		if (line_point_distance(mink_a.differens, mink_c.differens, vector2::zero_vector) <
			line_point_distance(mink_b.differens, mink_c.differens, vector2::zero_vector)) {
			mink_b = mink_c;
		}
		else {
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
	auto& a = farthest_point(a_vectors, direction);
	auto& b = farthest_point(b_vectors, -direction);
	return minkowski_differens(&a, &b, a - b);
}

bool gjk_functions::contains_point(
	std::vector<vector2>& vectors,
	vector2 point)
{
	vector2 direction(1, 0);

	auto& a = farthest_point(vectors, direction);
	auto& b = farthest_point(vectors, -direction);

	for (int i = 0; i < 10; i++) {
		direction = perpendicular_to_point(a, b, point);
		auto& c = farthest_point(vectors, direction);

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

vector2& gjk_functions::farthest_point(
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

	return *farthest_point;
}

vector2& gjk_functions::perpendicular_to_point(
	vector2& a, vector2& b,
	vector2& o)
{
	auto b_a = b - a;
	auto o_a = o - a;

	// negative cross product means that point is clockwise relative b_a
	if (b_a.is_clockwise(o_a)) {
		b_a = -b_a;
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
	if (!b_a.is_clockwise(o_a)) {
		b_a = -b_a;
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

	double proj_length = b_a_normalize.dot_product(o_a);

	vector2 proj_point;

	if (proj_length < 0) {
		proj_point = a;
	}
	else if (proj_length > b_a.length()) {
		proj_point = b;
	}
	else {
		proj_point = (b_a_normalize * proj_length) + a;
	}

	double line_point_distance = proj_point.distance(o);

	return line_point_distance;
}


// for convex shape
double gjk_functions::line_point_distance_convex(
	vector2& a, vector2& b,
	vector2& o)
{
	vector2 proj_point = projection_point(a, b, o);

	double line_point_distance = proj_point.distance(o);

	return line_point_distance;
}

vector2 gjk_functions::projection_point(
	vector2& a, vector2& b,
	vector2& o) 
{
	auto b_a = b - a;
	auto b_a_normalize = b_a.normalize();
	auto o_a = o - a;

	double proj_length = b_a_normalize.dot_product(o_a);

	vector2 proj_point = (b_a_normalize * proj_length) + a;

	return proj_point;
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