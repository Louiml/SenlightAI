Write a C++ function `double computeFencedAreaPerimeter(const std::vector<std::pair<double,double>>& vertices, double ropeRadius)`. The function receives a polygon defined by its vertices in order (either clockwise or counterclockwise, no self-intersections), and a positive radius `ropeRadius` representing the radius of circular posts placed at each vertex. The perimeter of the fenced area is the sum of the straight-line distances along the polygon edges plus the circumference of a circle of radius `ropeRadius` (as if a rope is stretched around the outside of all posts, with arcs around each post). The polygon may be degenerate (e.g., a line segment or a single point) but vertices are guaranteed to be distinct. The function must return the perimeter as a `double`. You may assume `ropeRadius >= 0`. The input vector can be empty (then return `0.0`). For non-empty `n` vertices, the polygon has `n` edges, each connecting the i-th vertex to the (i+1)-th (mod n). Use `3.141592653589793` for π (or any high-precision constant). Compute the sum of Euclidean distances between consecutive vertices and add `2 * pi * ropeRadius`.

The main algorithm is straightforward: iterate over each vertex (if `n >= 1`) and compute the distance to the next vertex using the standard Euclidean formula. Sum these distances. Then add the circumference `2 * π * r`. Edge cases: empty vector returns `0.0` (no polygon, no posts). If `n == 1`, the only edge is from the vertex to itself (distance 0), so perimeter = `2 * π * r`. If `n == 2`, there are two edges: (v0→v1) and (v1→v0), so the sum is twice the distance, plus the circle. This matches the concept of two posts with a rope around them – the rope would go from post0 to post1 and back, but in reality two posts share one segment; however, the snippet provided adds the circle once regardless of n, which we follow. Complexity: O(n) time and O(1) auxiliary space (excluding the input vector). Precision: use `double` and `std::hypot` or manual sqrt. No special handling needed for negative coordinates. Ensure the function is `const`-correct (pass vector by const reference).

#include <vector>
#include <cmath>

// Compute the perimeter of a polygon with circular posts of given radius.
// The perimeter is sum of edge lengths plus the circumference of one circle.
// If vertices is empty, returns 0.0.
double computeFencedAreaPerimeter(const std::vector<std::pair<double, double>>& vertices, double ropeRadius) {
    const size_t n = vertices.size();
    if (n == 0) {
        return 0.0;
    }

    double edgeSum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const auto& a = vertices[i];
        const auto& b = vertices[(i + 1) % n];
        double dx = a.first - b.first;
        double dy = a.second - b.second;
        edgeSum += std::sqrt(dx * dx + dy * dy);
    }

    const double pi = 3.141592653589793;
    return edgeSum + 2.0 * pi * ropeRadius;
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Empty vector
    assert(std::fabs(computeFencedAreaPerimeter({}, 2.0) - 0.0) < 1e-9);

    // Single point at origin, radius 1 -> circumference = 2*pi
    assert(std::fabs(computeFencedAreaPerimeter({{0.0, 0.0}}, 1.0) - (2.0 * 3.141592653589793)) < 1e-9);

    // Two points distance 3 apart, radius 0 -> perimeter = 2 * 3 = 6
    assert(std::fabs(computeFencedAreaPerimeter({{0.0, 0.0}, {3.0, 0.0}}, 0.0) - 6.0) < 1e-9);

    // Square of side 1, radius 0 -> perimeter = 4
    assert(std::fabs(computeFencedAreaPerimeter({{0.0,0.0},{1.0,0.0},{1.0,1.0},{0.0,1.0}}, 0.0) - 4.0) < 1e-9);

    // Triangle with sides 3,4,5, radius 2 -> perimeter = 12 + 4*pi
    double expected = 12.0 + 4.0 * 3.141592653589793;
    assert(std::fabs(computeFencedAreaPerimeter({{0.0,0.0},{3.0,0.0},{0.0,4.0}}, 2.0) - expected) < 1e-9);

    // Negative coordinates, parallelogram
    assert(std::fabs(computeFencedAreaPerimeter({{-1.0,-1.0},{2.0,-1.0},{1.0,1.0},{-2.0,1.0}}, 0.5) - (2.0*3.0 + 2.0*std::sqrt(5.0) + 2.0*3.141592653589793*0.5)) < 1e-9);
}
