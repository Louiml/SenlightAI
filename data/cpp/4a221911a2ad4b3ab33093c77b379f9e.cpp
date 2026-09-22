/*
Write a C++ function that takes a Well-Known Text (WKT) polygon string and a point represented by x and y coordinates (as doubles), parses the polygon and point using the Boost.Geometry library (with `boost::geometry::model::d2::point_xy<double>` and `boost::geometry::model::polygon<point_type>`), and returns a `std::string` containing the 9-intersection model (DE-9IM) relation matrix between the point and the polygon, as produced by `boost::geometry::relation`. The function must handle polygons with holes (multiple rings in the WKT) correctly, and the input polygon is guaranteed to be valid and non-empty. The returned string must be exactly 9 characters long (e.g., `"0FFFFF212"`). The function should be const-correct and not modify the inputs.
*/

#include <string>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

// Computes the DE-9IM relation matrix string between a point and a polygon.
// Parameters:
//   wkt_polygon - a valid WKT polygon string (may contain holes).
//   x, y        - coordinates of the point.
// Returns:
//   A 9-character string representing the DE-9IM matrix (e.g., "0FFFFF212").
std::string pointPolygonRelation(const std::string& wkt_polygon, double x, double y) {
    using point_type = boost::geometry::model::d2::point_xy<double>;
    using polygon_type = boost::geometry::model::polygon<point_type>;

    polygon_type poly;
    boost::geometry::read_wkt(wkt_polygon, poly);

    point_type p(x, y);

    boost::geometry::de9im::matrix matrix = boost::geometry::relation(p, poly);
    return matrix.str();
}

#include <cassert>
#include <string>

// The solution function is already declared above (include it here in a real project).
// For the test, we replicate the declaration to avoid link issues.
std::string pointPolygonRelation(const std::string& wkt_polygon, double x, double y);

int main() {
    // Polygon with a hole (from the example)
    std::string poly_with_hole =
        "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,"
        "5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
        "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))";

    // Point inside the polygon (but not in the hole) -> should be "0FFFFF212"
    assert(pointPolygonRelation(poly_with_hole, 3.0, 1.5) == "0FFFFF212");

    // Point inside the hole (outside the polygon) -> interior is disjoint, so "FF2FFF212" for point inside a hole? Actually, point is not inside polygon -> relation "FF2FFF212" typically
    assert(pointPolygonRelation(poly_with_hole, 4.2, 1.8) == "FF2FFF212");

    // Point on the polygon boundary -> relation should have 'B' in appropriate positions, e.g., "F0FFFF212" for a boundary point
    assert(pointPolygonRelation(poly_with_hole, 2.0, 1.3) == "F0FFFF212");

    // Point far outside the polygon -> relation "FF2FFF212" (point is exterior to everything)
    assert(pointPolygonRelation(poly_with_hole, 10.0, 10.0) == "FF2FFF212");

    // Simple polygon without holes, point inside
    std::string simple_poly = "POLYGON((0 0,0 10,10 10,10 0,0 0))";
    assert(pointPolygonRelation(simple_poly, 5.0, 5.0) == "0FFFFF212");

    // Simple polygon, point outside
    assert(pointPolygonRelation(simple_poly, 20.0, 5.0) == "FF2FFF212");

    // Simple polygon, point on boundary
    assert(pointPolygonRelation(simple_poly, 0.0, 5.0) == "F0FFFF212");

    // Point exactly at a polygon vertex
    assert(pointPolygonRelation(simple_poly, 0.0, 0.0) == "F0FFFF212");

    // Empty polygon? Not valid but test with a degenerate? Use a valid one with zero area? Better skip.
    // Additional check: point inside the polygon but exactly on the hole boundary
    // hole boundary point (4.0,2.0) is on the hole ring, which is boundary of the polygon
    assert(pointPolygonRelation(poly_with_hole, 4.0, 2.0) == "F0FFFF212");

    return 0;
}

// The solution uses Boost.Geometry's `read_wkt` to parse the polygon from the input string into a `polygon_type` object. The point is constructed directly from the provided x and y coordinates using the `point_type` constructor. Then, `boost::geometry::relation` is called with the point and polygon, returning a `boost::geometry::de9im::matrix` object; its `str()` method produces the 9-character DE-9IM string. Edge cases: the polygon may have holes; Boost.Geometry handles these automatically. If the point lies exactly on the boundary, the matrix will reflect that (e.g., `'F'` in the interior-interior position if not inside, `'B'` on boundary, etc.). The input is guaranteed valid, so no special error handling for malformed WKT is needed. Time complexity is O(n) where n is the number of polygon vertices (for parsing and relation computation), and space complexity is O(n) due to polygon storage; the relation matrix is constant size.
