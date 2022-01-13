#include "gjk_functions.h"

gjk_functions::gjk_functions(primitives_drawer& drawer)
	: drawer(drawer)
{
}

clipping_result gjk_functions::clipping(
	const std::vector<vector2>& a_vertices,
	const std::vector<vector2>& b_vertices,
	const std::vector<edge>& a_edges,
	const std::vector<edge>& b_edges,
	const epa_result& epa_result) 
{
	vector2 normal = epa_result.is_object_1_normal
		? epa_result.collision_normal
		: -epa_result.collision_normal;

	auto& a_farthest = farthest_point(a_vertices, normal);
	auto& b_farthest = farthest_point(b_vertices, -normal);

	auto& a_best_edge = gjk_functions::find_best_edge(a_edges, a_farthest, normal);
	auto& b_best_edge = gjk_functions::find_best_edge(b_edges, b_farthest, normal);

	const edge* reference_edge;
	const edge* incident_edge;

	if (epa_result.is_object_1_normal) {
		reference_edge = &b_best_edge;
		incident_edge = &a_best_edge;
	}
	else {
		reference_edge = &a_best_edge;
		incident_edge = &b_best_edge;
	}

	auto incident_a_b = incident_edge->a - incident_edge->b;

	auto reference_edge_nolmalize = (reference_edge->a - reference_edge->b).normalize();

	vector2 incident_vertices[2] = { incident_edge->a, incident_edge->b };
	vector2 reference_vertices[2] = { reference_edge->a, reference_edge->b };

	double incident_edge_a_dot = incident_edge->a.dot_product(reference_edge_nolmalize);
	double incident_edge_b_dot = incident_edge->b.dot_product(reference_edge_nolmalize);

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
			//drawer.draw_cross(incident_vertex, sf::Color::Red);
			contact_points.push_back(incident_vertex);
		}
		else {
			//drawer.draw_cross(incident_vertex, sf::Color::Blue); 
		}
	}

	if (contact_points.empty()) {
		return clipping_result();
	}

	vector2& summ = contact_points[0];
	for (int i = 1; i < contact_points.size(); i++)
	{
		summ += contact_points[i];
	}

	auto collision_point = summ / contact_points.size();
	auto proj_point = projection_point(reference_edge->a, reference_edge->b, collision_point);
	auto drawing_vector_point = proj_point + epa_result.collision_normal * 100;

	clipping_result clipping_res;
	clipping_res.collision_point = summ / contact_points.size();
	clipping_res.collision_penetration_line = collision_point - proj_point;
	clipping_res.collision_penetration = clipping_res.collision_penetration_line.length();
	clipping_res.collision_normal = clipping_res.collision_penetration_line.normalize();
	clipping_res.is_object_1_normal = epa_result.is_object_1_normal;

	//drawer.draw_line(proj_point, drawing_vector_point, sf::Color::Blue);
	//drawer.draw_line(clipping_res.collision_point, proj_point, sf::Color::Red);
	drawer.draw_cross(clipping_res.collision_point, sf::Color::White);

	return clipping_res;
}

epa_result gjk_functions::EPA(
	const std::vector<vector2>& a_vertices,
	const std::vector<vector2>& b_vertices,
	const gjk_result& gjk_result)
{
	double a_b_distance_o = line_point_distance_convex(
		gjk_result.mink_a.distance, gjk_result.mink_b.distance, vector2::zero_vector);
	double a_c_distance_o = line_point_distance_convex(
		gjk_result.mink_a.distance, gjk_result.mink_c.distance, vector2::zero_vector);
	double b_c_distance_o = line_point_distance_convex(
		gjk_result.mink_b.distance, gjk_result.mink_c.distance, vector2::zero_vector);

	// create triangular simplex
	std::map<double, std::tuple<minkowski_difference, minkowski_difference>> edge_map {
		{a_b_distance_o, std::make_tuple(gjk_result.mink_a, gjk_result.mink_b)},
		{a_c_distance_o, std::make_tuple(gjk_result.mink_a, gjk_result.mink_c)},
		{b_c_distance_o, std::make_tuple(gjk_result.mink_b, gjk_result.mink_c)}
	};

	for (int i = 0; i < 10; i++) {
		auto nearest_edge = edge_map.begin()->second;
		edge_map.erase(edge_map.begin());
		auto& mink_a = std::get<0>(nearest_edge);
		auto& mink_b = std::get<1>(nearest_edge);

		auto perpendicular_from_zero = -perpendicular_to_point(
			mink_a.distance, mink_b.distance, vector2::zero_vector);
		auto mink_c = support_function(a_vertices, b_vertices, perpendicular_from_zero);

		if (mink_a == mink_c || mink_b == mink_c || i == 10)
		{
			return get_collider_result(mink_a, mink_b);
		}

		// add two new edges to the simplex
		edge_map.insert({
			line_point_distance_convex(mink_c.distance, mink_a.distance, vector2::zero_vector),
			std::make_tuple(mink_c, mink_a) });
		edge_map.insert({
			line_point_distance_convex(mink_c.distance, mink_b.distance, vector2::zero_vector),
			std::make_tuple(mink_c, mink_b) });
	}

	return epa_result();
}

epa_result gjk_functions::get_collider_result(
	const minkowski_difference& mink_a, const minkowski_difference& mink_b)
{
	epa_result epa_res;
	const vector2* penetration_point;
	const vector2* collision_edge_a;
	const vector2* collision_edge_b;

	if (mink_a.point_a == mink_b.point_a) {
		penetration_point = mink_a.point_a; // or mink_b.point_a
		collision_edge_a = mink_a.point_b;
		collision_edge_b = mink_b.point_b;
	}
	else {
		penetration_point = mink_a.point_b; // or mink_b.point_b
		collision_edge_a = mink_a.point_a;
		collision_edge_b = mink_b.point_a;
		epa_res.is_object_1_normal = false;
	}

	auto proj_point = projection_point(*collision_edge_a, *collision_edge_b, *penetration_point);
	epa_res.collision_normal = (*penetration_point - proj_point).normalize();
	return epa_res;
}

gjk_result gjk_functions::GJK(
	const std::vector<vector2>& a_vertices,
	const std::vector<vector2>& b_vertices)
{
	vector2 direction(1, 0);
	auto mink_a = support_function(a_vertices, b_vertices, direction);
	auto mink_b = support_function(a_vertices, b_vertices, -direction);

	for (int i = 0; i < 10; i++) {
		direction = perpendicular_to_point(mink_a.distance, mink_b.distance, vector2::zero_vector);
		auto mink_c = support_function(a_vertices, b_vertices, direction);

		if (mink_a == mink_c || mink_b == mink_c) {
			return gjk_result(false);
		}

		if (triangle_contains(mink_a.distance, mink_b.distance, mink_c.distance, vector2::zero_vector)) {
			return gjk_result(true, mink_a, mink_b, mink_c);
		}

		auto& replaceable_mink =
			line_point_distance(mink_a.distance, mink_c.distance, vector2::zero_vector) >
			line_point_distance(mink_b.distance, mink_c.distance, vector2::zero_vector)
			? mink_a : mink_b;
		replaceable_mink = mink_c;
	}

	return gjk_result(false);
}

minkowski_difference gjk_functions::support_function(
	const std::vector<vector2>& a_vertices,
	const std::vector<vector2>& b_vertices,
	const vector2 direction)
{
	auto& a = farthest_point(a_vertices, direction);
	auto& b = farthest_point(b_vertices, -direction);
	return minkowski_difference(&a, &b, a - b);
}

bool gjk_functions::contains_point(
	const std::vector<vector2>& vectors,
	const vector2& point)
{
	vector2 direction(1, 0);

	auto* a = &farthest_point(vectors, direction);
	auto* b = &farthest_point(vectors, -direction);

	for (int i = 0; i < 10; i++) {
		direction = perpendicular_to_point(*a, *b, point);
		auto* c = &farthest_point(vectors, direction);

		if (a == c || b == c) {
			return false;
		}

		if (triangle_contains(*a, *b, *c, point)) {
			return true;
		}

		if (line_point_distance(*a, *c, point) < line_point_distance(*b, *c, point)) {
			b = c;
		}
		else {
			a = c;
		}
	}

	return false;
}

const edge& gjk_functions::find_best_edge(
	const std::vector<edge>& edges,
	const vector2& farthest_point,
	const vector2& normal)
{
	const edge* best_edge = nullptr;
	double min_dot_product = std::numeric_limits<double>::max();

	for (int i = 0; i < edges.size(); i++) {
		auto& edge = edges[i];

		if (edge.a == farthest_point || edge.b == farthest_point) {
			double dot_product = abs((edge.a - edge.b).dot_product(normal));

			if (dot_product < min_dot_product) {
				min_dot_product = dot_product;
				best_edge = &edge;
			}
		}
	}

	return *best_edge;
}

const vector2& gjk_functions::farthest_point(
	const std::vector<vector2>& vectors,
	const vector2& direction)
{
	const vector2* farthest_point = &vectors[0];
	double max_dot_product = vectors[0].dot_product(direction);

	for (int i = 1; i < vectors.size(); i++) {
		auto dot_product = vectors[i].dot_product(direction);

		if (dot_product > max_dot_product) {
			max_dot_product = dot_product;
			farthest_point = &vectors[i];
		}
	}

	return *farthest_point;
}

vector2 gjk_functions::perpendicular_to_point(
	const vector2& a, const vector2& b,
	const vector2& o)
{
	return b.is_clockwise(o, a)
		? - (b - a).clockwise_perpendicular()
		: (b - a).clockwise_perpendicular();
}

// for convex and non convex shape
double gjk_functions::line_point_distance(
	const vector2& a, const vector2& b,
	const vector2& o)
{
	vector2 b_a = b - a;
	vector2 b_a_normalize = b_a.normalize();
	vector2 o_a = o - a;
	double proj_length = b_a_normalize.dot_product(o_a);

	if (proj_length < 0) {
		return a.distance(o);
	}
	else if (proj_length > b_a.length()) {
		return b.distance(o);
	}
	else {
		return (b_a_normalize * proj_length + a).distance(o);
	}
}

// for convex shape
double gjk_functions::line_point_distance_convex(
	const vector2& a, const vector2& b,
	const vector2& o)
{
	vector2 proj_point = projection_point(a, b, o);
	return proj_point.distance(o);
}

vector2 gjk_functions::projection_point(
	const vector2& a, const vector2& b,
	const vector2& o)
{
	auto b_a_normalize = (b - a).normalize();
	return b_a_normalize * b_a_normalize.dot_product(o - a) + a;
}

bool gjk_functions::triangle_contains(
	const vector2& a, const vector2& b, const vector2& c,
	const vector2& o)
{
	bool b_a_cw = b.is_clockwise(o, a);
	bool c_b_cw = c.is_clockwise(o, b);
	bool a_c_cw = a.is_clockwise(o, c);

	return (b_a_cw && c_b_cw && a_c_cw) ||
		!(b_a_cw || c_b_cw || a_c_cw);
}