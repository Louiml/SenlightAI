Create a C++ standalone function that, given a polygon represented by a vector of `std::pair<double, double>` (x, y coordinates in counter-clockwise or clockwise order, without repeated first point at the end), computes the convex hull using the monotone chain algorithm and returns the perimeter length of that hull. If the polygon has fewer than 3 points, return 0.0. The function should be const-correct, use only standard library functions (no OpenCV), and must handle duplicate points, collinear points, and both clockwise and counter-clockwise input order. The returned perimeter should be the Euclidean length of the closed hull boundary (i.e., sum of distances between consecutive hull vertices, including the closing edge from last back to first).
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// The function convexHullPerimeter is assumed to be defined above.

int main() {
    // Square with side length 2: perimeter = 8
    std::vector<std::pair<double,double>> square = {{0,0},{2,0},{2,2},{0,2}};
    assert(std::fabs(convexHullPerimeter(square) - 8.0) < 1e-9);

    // Duplicate points and collinear points: same square plus extra points
    std::vector<std::pair<double,double>> square_extra = {{0,0},{1,0},{2,0},{2,2},{0,2},{1,0},{0,0}};
    assert(std::fabs(convexHullPerimeter(square_extra) - 8.0) < 1e-9);

    // Reverse order (clockwise) still works
    std::vector<std::pair<double,double>> square_rev = {{0,2},{2,2},{2,0},{0,0}};
    assert(std::fabs(convexHullPerimeter(square_rev) - 8.0) < 1e-9);

    // Triangle with vertices (0,0), (3,0), (0,4): perimeter = 3+4+5 = 12
    std::vector<std::pair<double,double>> triangle = {{0,0},{3,0},{0,4}};
    assert(std::fabs(convexHullPerimeter(triangle) - 12.0) < 1e-9);

    // All collinear points: return 0
    std::vector<std::pair<double,double>> collinear = {{0,0},{1,1},{2,2},{3,3}};
    assert(convexHullPerimeter(collinear) == 0.0);

    // Fewer than 3 distinct points: return 0
    std::vector<std::pair<double,double>> tiny = {{1,2}};
    assert(convexHullPerimeter(tiny) == 0.0);
    std::vector<std::pair<double,double>> two = {{0,0},{1,1}};
    assert(convexHullPerimeter(two) == 0.0);

    // Single duplicate point repeated many times: return 0
    std::vector<std::pair<double,double>> duplicates = {{5,5},{5,5},{5,5}};
    assert(convexHullPerimeter(duplicates) == 0.0);

    // Pentagon approximated by a regular shape: use a simple polygon (0,0),(2,0),(3,1),(1,3),(-1,1)
    std::vector<std::pair<double,double>> pentagon = {{0,0},{2,0},{3,1},{1,3},{-1,1}};
    // Expected perimeter manually: distances: 2, sqrt(2), sqrt(8), sqrt(8), sqrt(2)
    double expected = 2.0 + std::sqrt(2.0) + std::sqrt(8.0) + std::sqrt(8.0) + std::sqrt(2.0);
    assert(std::fabs(convexHullPerimeter(pentagon) - expected) < 1e-9);

    // Empty vector
    std::vector<std::pair<double,double>> empty;
    assert(convexHullPerimeter(empty) == 0.0);

    return 0;
}
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

// Compute the convex hull perimeter of a set of 2D points.
// Returns 0.0 if the hull has fewer than 3 distinct points.
double convexHullPerimeter(const std::vector<std::pair<double, double>>& points) {
    using Point = std::pair<double, double>;
    
    // Cross product of vectors OA and OB: positive if counter-clockwise.
    auto cross = [](const Point& O, const Point& A, const Point& B) -> double {
        return (A.first - O.first) * (B.second - O.second) - 
               (A.second - O.second) * (B.first - O.first);
    };
    
    // Euclidean distance between two points.
    auto dist = [](const Point& a, const Point& b) -> double {
        double dx = a.first - b.first;
        double dy = a.second - b.second;
        return std::sqrt(dx * dx + dy * dy);
    };
    
    // Sort and remove duplicates.
    std::vector<Point> pts = points;
    if (pts.size() < 3) return 0.0;
    std::sort(pts.begin(), pts.end());
    pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
    if (pts.size() < 3) return 0.0;
    
    // Build convex hull using monotone chain.
    std::vector<Point> hull;
    for (const auto& p : pts) {
        while (hull.size() >= 2 && cross(hull[hull.size()-2], hull.back(), p) <= 0.0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }
    size_t lower_size = hull.size() + 1;
    for (int i = static_cast<int>(pts.size()) - 2; i >= 0; --i) {
        const auto& p = pts[i];
        while (hull.size() >= lower_size && cross(hull[hull.size()-2], hull.back(), p) <= 0.0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }
    // Remove the last point (duplicate of first).
    hull.pop_back();
    
    if (hull.size() < 3) return 0.0;
    
    // Compute perimeter.
    double perimeter = 0.0;
    for (size_t i = 0; i < hull.size(); ++i) {
        size_t j = (i + 1) % hull.size();
        perimeter += dist(hull[i], hull[j]);
    }
    return perimeter;
}
// The solution uses the Andrew's monotone chain algorithm for convex hull construction. First, sort the points lexicographically by x then y, and remove consecutive duplicates. Then build the lower hull by iterating through sorted points, maintaining a stack where the last three points must form a counter-clockwise turn (using cross product > 0 for strict convexity; if cross product ≤ 0, pop the middle point). Then build the upper hull similarly by iterating from right to left. The concatenation (excluding the last point of each half to avoid duplicates) forms the convex hull in counter-clockwise order. If the resulting hull has fewer than 3 points (i.e., all points collinear or less than 3 unique points), return 0.0. Otherwise, compute the perimeter by summing Euclidean distances between consecutive hull points, including the closing edge from the last hull point back to the first. Time complexity is O(n log n) due to sorting, and O(n) auxiliary space for the hull vector. Edge cases include points all collinear (hull has 2 points, return 0), duplicate coordinates, and input of size 0, 1, or 2 (return 0). Using `double` for coordinates and arithmetic, and `std::sqrt` for distances. The function is `const` and takes a `const std::vector<std::pair<double,double>>&`.
