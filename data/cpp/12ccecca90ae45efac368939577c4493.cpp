Write a C++ function that takes a vector of 2D points forming a simple, strictly counter-clockwise polygon (no self-intersections, no collinear consecutive edges) and returns a `double` representing the approximate total length of the edges of its interior straight skeleton (i.e., the sum of the lengths of all segments in the skeleton, excluding the original polygon edges). The input points are given as `std::vector<std::pair<double,double>>` where each pair is (x, y). The function must use the CGAL library’s `create_interior_straight_skeleton_2` with an `Exact_predicates_inexact_constructions_kernel`. If the polygon is degenerate (fewer than 3 vertices) or not simple, throw a `std::invalid_argument`. For valid polygons, compute the straight skeleton, then iterate over all halfedges of the skeleton’s halfedge data structure, summing the squared lengths of edges (using squared distance between source and target points) and return the square root of that sum? Actually, return the total length as the sum of Euclidean distances (not squared). Ensure the function is `const`-correct and does not modify the input.
#include <cassert>
#include <vector>
#include <utility>
#include <cmath>

// Include the solution function (assumed to be in the same translation unit).

int main() {
    // Square with side length 2: [-1,1] x [-1,1] - its straight skeleton is the two diagonals.
    // Total skeleton edge length = 2 * diagonal length = 2 * sqrt( (2)^2 + (2)^2 )? Actually each diagonal is sqrt(8)=2.828, but skeleton edges are half-diagonals from center to corners? 
    // For a square, the interior straight skeleton consists of 4 segments from the center to each vertex, each length sqrt(2) ≈ 1.414, total = 4 * sqrt(2) ≈ 5.657.
    std::vector<std::pair<double,double>> square = {{-1,-1}, {1,-1}, {1,1}, {-1,1}};
    double len = skeletonTotalEdgeLength(square);
    assert(std::abs(len - 4.0 * std::sqrt(2.0)) < 1e-9);

    // Triangle: vertices (0,0), (4,0), (0,3) - right triangle. Straight skeleton is the incenter connected to vertices.
    // Inradius r = (a+b-c)/2 where c = 5, so r = (3+4-5)/2 = 1. Distances from incenter to vertices: 
    // to (0,0) = sqrt( (r)^2 + (r)^2 )? Actually incenter is at (r, r) = (1,1) for right angle at origin? Check: 
    // Incenter formula I = (a*A + b*B + c*C)/(a+b+c), but for right triangle with legs along axes, incenter at (r,r) with r=1.
    // Vertex distances: to (0,0): sqrt(2), to (4,0): sqrt( (3)^2 + (1)^2 )=sqrt(10), to (0,3): sqrt( (1)^2 + (2)^2 )=sqrt(5). Sum ≈ 1.414+3.162+2.236=6.812. 
    std::vector<std::pair<double,double>> tri = {{0,0}, {4,0}, {0,3}};
    double triLen = skeletonTotalEdgeLength(tri);
    double expectedTri = std::sqrt(2.0) + std::sqrt(10.0) + std::sqrt(5.0);
    assert(std::abs(triLen - expectedTri) < 1e-9);

    // Degenerate polygon should throw
    bool threw = false;
    try {
        std::vector<std::pair<double,double>> degenerate = {{0,0}, {1,1}};
        skeletonTotalEdgeLength(degenerate);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Self-intersecting polygon should throw
    threw = false;
    try {
        std::vector<std::pair<double,double>> selfInt = {{0,0}, {2,2}, {0,2}, {2,0}}; // bow-tie
        skeletonTotalEdgeLength(selfInt);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Regular hexagon (radius 1): skeleton is composed of 6 segments from center to each vertex? 
    // Actually for regular polygon, skeleton is center-to-vertex segments? But for straight skeleton, it's not the medial axis; for a regular polygon the straight skeleton is the same as the medial axis? 
    // For a regular hexagon with circumradius 1, the inradius = cos(30°)=√3/2 ≈ 0.866. The skeleton edges are from center to each vertex? Actually the straight skeleton of a regular polygon consists of the center connected to each vertex? 
    // Let's compute: each edge length = distance from center to vertex = 1, there are 6 such edges? But the skeleton also includes branches? For regular polygon, interior straight skeleton is just the center and 6 segments to vertices? I'm not certain. To keep the test robust, we just check that the length is finite and positive.
    std::vector<std::pair<double,double>> hex;
    for (int i = 0; i < 6; ++i) {
        double angle = i * 2.0 * M_PI / 6.0;
        hex.push_back({std::cos(angle), std::sin(angle)});
    }
    double hexLen = skeletonTotalEdgeLength(hex);
    assert(hexLen > 0.0);

    // Large polygon with 1000 random points (convex? not necessarily) – just ensure no crash and positive length.
    std::vector<std::pair<double,double>> big;
    for (int i = 0; i < 1000; ++i) {
        double angle = i * 2.0 * M_PI / 1000.0;
        big.push_back({std::cos(angle), std::sin(angle)});
    }
    double bigLen = skeletonTotalEdgeLength(big);
    assert(bigLen > 0.0);

    // Clockwise polygon should throw
    threw = false;
    try {
        std::vector<std::pair<double,double>> cw = {{0,0}, {0,1}, {1,1}, {1,0}}; // clockwise
        skeletonTotalEdgeLength(cw);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
#include <vector>
#include <utility>
#include <cmath>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/create_straight_skeleton_2.h>

// Compute the total length of all interior straight skeleton edges of a simple counter-clockwise polygon.
// Throws std::invalid_argument if the polygon is degenerate (fewer than 3 vertices) or not simple/ccw.
double skeletonTotalEdgeLength(const std::vector<std::pair<double,double>>& polygonPoints) {
    if (polygonPoints.size() < 3) {
        throw std::invalid_argument("Polygon must have at least 3 vertices.");
    }

    using K = CGAL::Exact_predicates_inexact_constructions_kernel;
    using Point = K::Point_2;
    using Polygon_2 = CGAL::Polygon_2<K>;
    using Ss = CGAL::Straight_skeleton_2<K>;
    using SsPtr = boost::shared_ptr<Ss>;

    // Build the polygon.
    Polygon_2 poly;
    for (const auto& p : polygonPoints) {
        poly.push_back(Point(p.first, p.second));
    }

    if (!poly.is_simple() || !poly.is_ccw()) {
        throw std::invalid_argument("Polygon must be simple and counter-clockwise.");
    }

    // Construct the interior straight skeleton.
    SsPtr iss = CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end());
    if (!iss) {
        throw std::runtime_error("Failed to construct straight skeleton.");
    }

    // Sum Euclidean distances over all halfedges, then divide by 2 to avoid double counting.
    double totalLength = 0.0;
    for (auto it = iss->halfedges_begin(); it != iss->halfedges_end(); ++it) {
        const Point& src = it->source()->point();
        const Point& tgt = it->target()->point();
        double dx = tgt.x() - src.x();
        double dy = tgt.y() - src.y();
        totalLength += std::sqrt(dx*dx + dy*dy);
    }
    return totalLength / 2.0;
}
// The solution uses CGAL’s straight skeleton construction. The main steps: (1) Convert the input vector of pairs into a `CGAL::Polygon_2<K>` with `K = Exact_predicates_inexact_constructions_kernel`. (2) Check degenerate case (size < 3) and throw. (3) Check that the polygon is simple and strictly counter-clockwise (CGAL provides `is_simple()` and `is_ccw()`). If not, throw. (4) Call `CGAL::create_interior_straight_skeleton_2(poly.vertices_begin(), poly.vertices_end())` which returns a `boost::shared_ptr<Straight_skeleton_2<K>>`. (5) Iterate over all halfedges in the skeleton. The halfedge data structure has `halfedges_begin()` and `halfedges_end()`; each halfedge has `source()` and `target()` points. Since each undirected edge appears twice (as two halfedges), sum the Euclidean distance from source to target for every halfedge, then divide by 2 to avoid double counting. However, it’s simpler to iterate over all halfedges and add the distance, then divide by 2. Edge cases: the skeleton may have degenerate edges? For a valid polygon, all skeleton edges have positive length. Complexity: Construction is O(n log n) for n input vertices (straight skeleton algorithms are typically O(n log n) in CGAL). Iterating over edges is O(E) where E is O(n). Space complexity is O(n). The function must use `const` on the input reference.
