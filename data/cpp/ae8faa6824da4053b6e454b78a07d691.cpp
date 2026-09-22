Create a C++ function that determines whether a given point lies inside a simple polygon (a polygon whose edges do not self-intersect and whose first and last vertices are identical). The polygon is provided as a vector of `std::pair<double, double>` coordinates, where consecutive pairs define edges, and the first and last entries are equal. Your function should return `true` if the point is strictly inside the polygon, `false` if it is outside or on the boundary. The point may be inside, outside, or exactly on an edge or vertex. Handle polygons with zero area (e.g., a degenerate line) correctly, and use floating-point arithmetic with appropriate tolerance to avoid precision issues.

The standard ray-casting algorithm is used: cast a horizontal ray from the point in the positive x-direction and count how many polygon edges it crosses. If the count is odd, the point is inside; if even, it is outside. To avoid issues with vertices lying exactly on the ray, apply the half-open rule: an edge is considered to cross the ray if one endpoint is strictly above the ray (y > point.y) and the other endpoint is at or below (y <= point.y), or vice versa. This ensures each vertex is counted exactly once when it is shared between two edges. For each candidate edge, compute the x-coordinate of intersection with the ray using linear interpolation: `x_intersect = x1 + (point.y - y1) * (x2 - x1) / (y2 - y1)`. Count the edge if `x_intersect > point.x`. To handle boundary cases (point exactly on an edge or vertex), check if the point lies on any edge using a cross-product test with a small epsilon (e.g., 1e-9). If the point is on any edge, return `false` immediately. For degenerate polygons with zero area (e.g., all collinear), the algorithm naturally returns `false` because no legitimate edges cross the ray. Time complexity is O(n) where n is the number of polygon vertices (or edges), because we scan each edge once. Space complexity is O(1) auxiliary.

#include <vector>
#include <cmath>
#include <utility>

// Check if a point is strictly inside a simple polygon (first and last vertices identical).
// Returns true if inside, false if outside or on boundary.
bool isPointInsidePolygon(const std::vector<std::pair<double, double>>& polygon,
                          const std::pair<double, double>& point) {
    const double eps = 1e-9;
    const double px = point.first;
    const double py = point.second;
    const size_t n = polygon.size();
    if (n < 4) return false; // Need at least a triangle (4 points with closing duplicate)

    bool inside = false;

    // First, check if point lies on any edge (boundary case)
    for (size_t i = 0; i + 1 < n; ++i) {
        double x1 = polygon[i].first;
        double y1 = polygon[i].second;
        double x2 = polygon[i+1].first;
        double y2 = polygon[i+1].second;

        // Check if point is on the segment using cross product and bounding box
        double cross = (px - x1) * (y2 - y1) - (py - y1) * (x2 - x1);
        if (std::fabs(cross) < eps) {
            // Check if point is within the bounding rectangle of segment
            if (px >= std::min(x1, x2) - eps && px <= std::max(x1, x2) + eps &&
                py >= std::min(y1, y2) - eps && py <= std::max(y1, y2) + eps) {
                return false; // On boundary
            }
        }
    }

    // Ray casting: count edges crossing horizontal ray to the right
    for (size_t i = 0; i + 1 < n; ++i) {
        double x1 = polygon[i].first;
        double y1 = polygon[i].second;
        double x2 = polygon[i+1].first;
        double y2 = polygon[i+1].second;

        // Half-open rule: edge crosses ray if one endpoint strictly above, other <= below
        bool crosses = ((y1 > py) != (y2 > py));
        if (!crosses) continue;

        // Compute x-intersection of the edge with the horizontal line y = py
        double x_intersect = x1 + (py - y1) * (x2 - x1) / (y2 - y1);
        if (x_intersect > px) {
            inside = !inside;
        }
    }

    return inside;
}

#include <cassert>
#include <vector>
#include <utility>

// Test cases
int main() {
    // Define a unit square: (0,0) -> (1,0) -> (1,1) -> (0,1) -> (0,0)
    std::vector<std::pair<double, double>> square = {
        {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0}, {0.0, 0.0}
    };

    // Inside points
    assert(isPointInsidePolygon(square, {0.5, 0.5}) == true);
    assert(isPointInsidePolygon(square, {0.1, 0.9}) == true);

    // Outside points
    assert(isPointInsidePolygon(square, {-0.1, 0.5}) == false);
    assert(isPointInsidePolygon(square, {1.1, 0.5}) == false);
    assert(isPointInsidePolygon(square, {0.5, 1.1}) == false);

    // Boundary points
    assert(isPointInsidePolygon(square, {0.0, 0.5}) == false);
    assert(isPointInsidePolygon(square, {0.5, 1.0}) == false);
    assert(isPointInsidePolygon(square, {0.0, 0.0}) == false);

    // Triangle: (0,0), (2,0), (1,2), (0,0)
    std::vector<std::pair<double, double>> triangle = {
        {0.0, 0.0}, {2.0, 0.0}, {1.0, 2.0}, {0.0, 0.0}
    };
    assert(isPointInsidePolygon(triangle, {1.0, 0.5}) == true);
    assert(isPointInsidePolygon(triangle, {0.5, 0.5}) == true);
    assert(isPointInsidePolygon(triangle, {0.2, 1.0}) == false);
    assert(isPointInsidePolygon(triangle, {1.0, 2.0}) == false); // vertex

    // Degenerate polygon (collinear points) - not a valid simple polygon, but test robustness
    std::vector<std::pair<double, double>> degenerate = {
        {0.0, 0.0}, {1.0, 0.0}, {2.0, 0.0}, {0.0, 0.0}
    };
    assert(isPointInsidePolygon(degenerate, {0.5, 0.0}) == false);
    assert(isPointInsidePolygon(degenerate, {0.5, 0.5}) == false);

    // Concave polygon: arrow shape
    std::vector<std::pair<double, double>> concave = {
        {0.0, 0.0}, {4.0, 0.0}, {4.0, 4.0}, {2.0, 2.0}, {0.0, 4.0}, {0.0, 0.0}
    };
    assert(isPointInsidePolygon(concave, {1.0, 1.0}) == true);
    assert(isPointInsidePolygon(concave, {3.0, 1.0}) == true);
    assert(isPointInsidePolygon(concave, {2.0, 3.0}) == false); // outside due to concavity

    // Test floating-point precision: point very close to edge (should be outside)
    assert(isPointInsidePolygon(square, {0.5, 1e-10}) == false); // just above bottom edge
    assert(isPointInsidePolygon(square, {0.5, 1e-8}) == true);   // slightly inside (if above eps? Here it is inside)

    return 0;
}
