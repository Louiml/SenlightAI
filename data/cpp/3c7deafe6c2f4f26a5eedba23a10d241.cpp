/*
Write a C++ function named `computeOffsetPolygonCounts` that takes a symmetric, star-shaped polygon defined by the 8 points `(-1,-1), (0,-12), (1,-1), (12,0), (1,1), (0,12), (-1,1), (-12,0)` (in that order) and a positive offset distance `d`, and returns a `std::pair<size_t, size_t>` containing the number of polygons produced by computing the interior and exterior straight-skeleton offsets (using CGAL). The function must include only the CGAL kernel and polygon/offset headers. The input polygon is guaranteed to be counterclockwise-oriented. For `d = 1`, the expected counts are 1 interior offset polygon and 1 exterior offset polygon. For very large `d` (e.g., 100), the interior offset must yield 0 polygons (since the offset collapses), while the exterior offset still yields 1. Handle the case where the interior offset may degenerate or produce no polygons.
*/

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_offset_polygons_2.h>

#include <boost/shared_ptr.hpp>
#include <vector>
#include <utility>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::FT FT;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef boost::shared_ptr<Polygon_2> PolygonPtr;
typedef std::vector<PolygonPtr> PolygonPtrVector;

// Returns the number of interior and exterior offset polygons for the given polygon and offset distance.
std::pair<size_t, size_t> computeOffsetPolygonCounts(const Polygon_2& poly, FT offset) {
    // The input is expected to be counterclockwise oriented.
    PolygonPtrVector inner = CGAL::create_interior_skeleton_and_offset_polygons_2(offset, poly);
    PolygonPtrVector outer = CGAL::create_exterior_skeleton_and_offset_polygons_2(offset, poly);

    return {inner.size(), outer.size()};
}

#include <cassert>
#include <utility>
#include <vector>

// The same headers as in the solution are needed here; for brevity we assume the solution is included.
// This test file must be linked with CGAL and Boost.

int main() {
    Polygon_2 poly;
    poly.push_back(Point(-1,-1));
    poly.push_back(Point(0,-12));
    poly.push_back(Point(1,-1));
    poly.push_back(Point(12,0));
    poly.push_back(Point(1,1));
    poly.push_back(Point(0,12));
    poly.push_back(Point(-1,1));
    poly.push_back(Point(-12,0));
    
    assert(poly.is_counterclockwise_oriented());

    auto counts1 = computeOffsetPolygonCounts(poly, FT(1));
    assert(counts1.first == 1);   // interior offset produces exactly one polygon
    assert(counts1.second == 1);  // exterior offset produces exactly one polygon

    auto counts100 = computeOffsetPolygonCounts(poly, FT(100));
    assert(counts100.first == 0); // interior offset collapses to nothing
    assert(counts100.second == 1); // exterior still exists

    auto counts05 = computeOffsetPolygonCounts(poly, FT(0.5));
    assert(counts05.first == 1);
    assert(counts05.second == 1);

    auto counts2 = computeOffsetPolygonCounts(poly, FT(2));
    assert(counts2.first == 1);
    assert(counts2.second == 1);

    // Test with a tiny offset
    auto countsSmall = computeOffsetPolygonCounts(poly, FT(0.1));
    assert(countsSmall.first == 1);
    assert(countsSmall.second == 1);

    return 0;
}

// The solution leverages CGAL’s `create_interior_skeleton_and_offset_polygons_2` and `create_exterior_skeleton_and_offset_polygons_2` functions, which internally build a straight skeleton and offset the polygon inward or outward by the given distance. The polygon is defined exactly as in the snippet, with a star-like shape. The function returns the counts of the resulting polygon pointer vectors. For the interior offset, if the offset distance is large enough to collapse the polygon completely, the vector may be empty (size 0) — this is the expected behavior for `d=100`. For the exterior offset, the result is always at least one polygon for any positive `d`, because the exterior grows without bound. The main complexity is in the CGAL library calls themselves, but from the perspective of the wrapper function, the time complexity is linear in the output polygon count, and the space complexity is proportional to the number of output polygons and their vertices. Edge cases: the polygon must be strictly simple and counterclockwise; using exact predicates with inexact constructions kernel avoids robustness issues for the given input. For `d` values that are extremely large, the interior offset may fail or return an empty vector, which we handle by checking the `.size()`.
