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
	gjk_functions(primitives_drawer& drawer);

	primitives_drawer drawer;

	clipping_result clipping(
		std::vector<vector2>& a_vertices,
		std::vector<vector2>& b_vertices,
		std::vector<edge>& a_edges,
		std::vector<edge>& b_edges,
		epa_result& epa_result);

	epa_result EPA(
		std::vector<vector2>& a_vertices,
		std::vector<vector2>& b_vertices,
		gjk_result& gjk_result);

	gjk_result GJK(
		std::vector<vector2>& a_vertices,
		std::vector<vector2>& b_vertices);

	bool contains_point(
		std::vector<vector2>& vectors,
		vector2 point);

private:
	static epa_result get_collider_result(
		minkowski_difference& a, minkowski_difference& b);

	static minkowski_difference support_function(
		std::vector<vector2>& a_vertices,
		std::vector<vector2>& b_vertices,
		vector2 direction);

	static vector2& farthest_point(
		std::vector<vector2>& vectors,
		vector2 direction = vector2());

	static edge& find_best_edge(
		std::vector<edge>& edges,
		vector2& farthest_point,
		vector2& normal);

	static vector2 perpendicular_to_point(
		vector2& a, vector2& b,
		vector2& o);

	static double line_point_distance(
		vector2& a, vector2& b,
		vector2& o);

	static double line_point_distance_convex(
		vector2& a, vector2& b,
		vector2& o);

	static vector2 projection_point(
		vector2& a, vector2& b,
		vector2& o);

	static bool triangle_contains(
		vector2& a, vector2& b, vector2& c,
		vector2& o);
};