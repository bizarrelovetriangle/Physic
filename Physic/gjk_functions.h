#pragma once
#include <fstream>
#include "gjk_result.h"
#include <list>
#include "minkowski_difference.h"
#include "primitives_drawer.h"
#include "epa_result.h"
#include "clipping_result.h"
#include "edge.h"
#include "line.h"
#include "numeric"

class gjk_functions {
public:
	gjk_functions(const primitives_drawer& drawer);

	const primitives_drawer& drawer;

	clipping_result clipping(
		const std::vector<vector2>& a_vertices,
		const std::vector<vector2>& b_vertices,
		const std::vector<edge>& a_edges,
		const std::vector<edge>& b_edges,
		const epa_result& epa_result) const;

	epa_result EPA(
		const std::vector<vector2>& a_vertices,
		const std::vector<vector2>& b_vertices,
		const gjk_result& gjk_result) const;

	gjk_result GJK(
		const std::vector<vector2>& a_vertices,
		const std::vector<vector2>& b_vertices) const;

	bool contains_point(
		const std::vector<vector2>& vertices,
		const vector2& point) const;

private:
	static epa_result get_collider_result(
		const minkowski_difference& a,
		const minkowski_difference& b);

	static minkowski_difference support_function(
		const std::vector<vector2>& a_vertices,
		const std::vector<vector2>& b_vertices,
		const vector2 direction);

	static const vector2& farthest_point(
		const std::vector<vector2>& vertices,
		const vector2& direction);

	static const edge& farthest_edge(
		const std::vector<vector2>& vertices,
		const std::vector<edge>& edges,
		const vector2& normal);

	static vector2 perpendicular_to_point(
		const vector2& a, const vector2& b,
		const vector2& o);

	static double line_point_distance(
		const vector2& a, const vector2& b,
		const vector2& o);

	static double line_point_distance_convex(
		const vector2& a, const vector2& b,
		const vector2& o);

	static bool triangle_contains(
		const vector2& a, const vector2& b, const vector2& c,
		const vector2& o);
};