Write a C++ function `bool isPointInPolygon(const std::vector<std::pair<long long, long long>>& polygon, long long x, long long y)` that determines whether a given point lies inside a simple polygon (not necessarily convex), using the ray casting algorithm. The polygon is provided as a vector of vertices in either clockwise or counterclockwise order, and the point coordinates are 64-bit signed integers. The function should return `true` if the point is strictly inside the polygon (including on the boundary), and `false` otherwise. Handle edge cases such as polygons with fewer than 3 vertices (return `false`), horizontal edges, points exactly on edges, and points at vertices. Use integer arithmetic only to avoid floating-point precision issues, particularly for large coordinates.
The solution uses the classic ray-casting (even-odd) rule: cast a horizontal ray from the point to the right and count how many polygon edges it crosses. If the count is odd, the point is inside; if even, outside. To handle points on the boundary correctly, we first check if the point lies exactly on any polygon edge segment using a robust integer cross-product and bounding-box test. 

For each edge from vertex `i` to vertex `j = (i+1) % n`, we check if the horizontal ray at `y` intersects the edge. The edge crosses the ray if `(vertex[i].second > y) != (vertex[j].second > y)` (i.e., the edge straddles the ray vertically) and the x-coordinate of the intersection is strictly greater than `x`. We compute the intersection x-coordinate without floating point by using cross-multiplication: `x_intersect = vertex[i].first + (y - vertex[i].second) * (vertex[j].first - vertex[i].first) / (vertex[j].second - vertex[i].second)`. But this division could truncate, so we instead use the equivalent comparison: `(vertex[i].first - x) * (vertex[j].second - vertex[i].second) < (vertex[j].first - vertex[i].first) * (y - vertex[i].second)`, avoiding division and precision loss. We must be careful with signs and the denominator being negative—by swapping the order of the inequality appropriately. A cleaner method: for each edge, if the edge’s two y-values straddle `y`, and the intersection x is to the right of the test point, toggle the inside flag. For a horizontal edge (both y equal), it doesn’t straddle, so it is ignored. For the boundary check, we verify if the point lies on the segment: cross product of (edge vector) and (point - start) is zero, and the point is within the bounding box of the segment. This handles vertices and edges robustly.

Edge cases: degenerate polygons (fewer than 3 vertices) return `false`. Polygons with repeated consecutive vertices are fine. Large coordinates up to `long long` are safe with the cross-multiplication comparisons using `__int128` if needed to avoid overflow. The algorithm runs in O(n) time and O(1) extra space, where n is the number of vertices.
#include <vector>
#include <cstdint>

using Point = std::pair<long long, long long>;

// Helper to check if point p lies on segment ab (including endpoints)
bool isOnSegment(const Point& a, const Point& b, const Point& p) {
    // Cross product of (b-a) and (p-a) should be zero for collinear
    __int128 cross = (__int128)(b.first - a.first) * (p.second - a.second) -
                     (__int128)(b.second - a.second) * (p.first - a.first);
    if (cross != 0) return false;
    // Check bounding box
    if (p.first < std::min(a.first, b.first) || p.first > std::max(a.first, b.first) ||
        p.second < std::min(a.second, b.second) || p.second > std::max(a.second, b.second))
        return false;
    return true;
}

// Determine if point (x,y) is inside a simple polygon (including boundary)
bool isPointInPolygon(const std::vector<Point>& polygon, long long x, long long y) {
    size_t n = polygon.size();
    if (n < 3) return false;

    Point p = {x, y};

    // First check if point is on any edge
    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        if (isOnSegment(polygon[i], polygon[j], p))
            return true;
    }

    // Ray casting: count intersections with ray to the right (+x direction)
    bool inside = false;
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        long long xi = polygon[i].first, yi = polygon[i].second;
        long long xj = polygon[j].first, yj = polygon[j].second;

        // Edge must straddle the horizontal line at y (one endpoint above, one below)
        bool straddle = (yi > y) != (yj > y);
        if (!straddle) continue;

        // Compute intersection x using cross-multiplication to avoid division:
        // edge from (xi,yi) to (xj,yj), ray at height y
        // intersection occurs if (xi + (y-yi)/(yj-yi)*(xj-xi)) > x
        // Equivalent: (xi - x) * (yj - yi) < (xj - xi) * (y - yi) when (yj-yi) > 0
        // but (yj-yi) can be negative, so we must handle sign carefully.
        // Instead, we compare using the fraction sign:
        __int128 lhs = (__int128)(xi - x) * (yj - yi);
        __int128 rhs = (__int128)(xj - xi) * (y - yi);
        // Since straddle ensures (yj-yi) !=0, and we want intersection x > x.
        // The intersection x > x iff (xi - x) / (yj - yi) < (xj - xi) / (yj - yi)? 
        // Actually solve: xi + (y-yi)*(xj-xi)/(yj-yi) > x
        // => (xi - x)*(yj-yi) + (y-yi)*(xj-xi) > 0? Let's derive properly:
        // intersection x = xi + (y - yi) * (xj - xi) / (yj - yi)
        // We want: xi + (y-yi)*(xj-xi)/(yj-yi) > x
        // Multiply both sides by (yj-yi)^2 (positive): 
        // (xi - x)*(yj-yi)^2 + (y-yi)*(xj-xi)*(yj-yi) > 0
        // This is messy. Simpler: use the standard test:
        // The edge crosses the ray if (yi > y) != (yj > y) and
        // x < (xj - xi) * (y - yi) / (yj - yi) + xi
        // To avoid division, we do:
        // (x - xi) * (yj - yi) < (xj - xi) * (y - yi)  when (yj-yi) > 0
        // and the opposite when (yj-yi) < 0.
        // Let's implement that.
        if (yj > yi) {
            if ((__int128)(x - xi) * (yj - yi) < (__int128)(xj - xi) * (y - yi))
                inside = !inside;
        } else { // yj < yi
            if ((__int128)(x - xi) * (yj - yi) > (__int128)(xj - xi) * (y - yi))
                inside = !inside;
        }
    }
    return inside;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above.
// We'll just include it here for completeness in a real scenario.

int main() {
    using Point = std::pair<long long, long long>;
    
    // Square: (0,0)-(4,0)-(4,4)-(0,4)
    std::vector<Point> square = {{0,0},{4,0},{4,4},{0,4}};
    assert(isPointInPolygon(square, 2, 2) == true);       // inside
    assert(isPointInPolygon(square, 0, 0) == true);       // vertex
    assert(isPointInPolygon(square, 2, 0) == true);       // edge
    assert(isPointInPolygon(square, 5, 5) == false);      // outside
    assert(isPointInPolygon(square, -1, 2) == false);     // left

    // Triangle
    std::vector<Point> triangle = {{0,0},{5,0},{0,5}};
    assert(isPointInPolygon(triangle, 1, 1) == true);
    assert(isPointInPolygon(triangle, 3, 1) == false);    // just outside hypotenuse
    assert(isPointInPolygon(triangle, 0, 3) == true);     // on vertical edge

    // Concave pentagon (like a "U" shape)
    std::vector<Point> concave = {{0,0},{6,0},{6,6},{4,6},{4,2},{2,2},{2,6},{0,6}};
    assert(isPointInPolygon(concave, 3, 1) == true);      // in lower bar
    assert(isPointInPolygon(concave, 3, 4) == false);     // in the gap
    assert(isPointInPolygon(concave, 1, 4) == true);      // in left bar
    assert(isPointInPolygon(concave, 5, 4) == true);      // in right bar

    // Point on boundary of concave shape
    assert(isPointInPolygon(concave, 2, 2) == true);      // corner

    // Degenerate: fewer than 3 vertices
    std::vector<Point> line = {{0,0},{2,2}};
    assert(isPointInPolygon(line, 1, 1) == false);

    // Large coordinates
    std::vector<Point> bigSquare = {{-1000000000LL,-1000000000LL},{1000000000LL,-1000000000LL},{1000000000LL,1000000000LL},{-1000000000LL,1000000000LL}};
    assert(isPointInPolygon(bigSquare, 0, 0) == true);
    assert(isPointInPolygon(bigSquare, 1500000000LL, 0) == false);

    return 0;
}
