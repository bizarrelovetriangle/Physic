#include <fstream>  
#include "gjk_result.h"
#include <list>
#include "minkowski_differens.h"
#include "minkowski_edge_distance.h"
#include "primitives_drawer.h"

#ifndef GJK_FUNCTIONS
#define GJK_FUNCTIONS
class gjk_functions {
public:
	gjk_functions(primitives_drawer* drawer);

	primitives_drawer* drawer;

	void EPA(
		std::vector<vector2>& a_vectors,
		std::vector<vector2>& b_vectors,
		gjk_result gjk_result);

	static void inseart_into_sorted_list(
		std::list<minkowski_edge_distance>& edges_sort_by_distance,
		minkowski_edge_distance&& new_edge_distance);

	gjk_result GJK(
		std::vector<vector2>& a_vectors,
		std::vector<vector2>& b_vectors);

	static minkowski_differens support_function(
		std::vector<vector2>& a_vectors,
		std::vector<vector2>& b_vectors,
		vector2 direction);
	
	bool contains_point(
		std::vector<vector2>& vectors,
		vector2 point);
	
	static vector2* farthest_point(
		std::vector<vector2>& vectors,
		vector2 direction = vector2());

	static vector2& perpendicular_to_point(
		vector2& a, vector2& b,
		vector2& o);

	static vector2& perpendicular_from_point(
		vector2& a, vector2& b,
		vector2& o);

	// for convex and non convex shape
	static double line_point_distance(
		vector2& a, vector2& b,
		vector2& o);

	static bool triangle_contains(
		vector2& a, vector2& b, vector2& c,
		vector2& o);
};
#endif