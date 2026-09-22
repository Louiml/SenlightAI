Given an array of 2D points represented by integer coordinates, write a C++ function that computes the vertices of the convex hull in counter-clockwise order using Andrew's monotone chain algorithm. The function should take a `std::vector<std::pair<int, int>>` and return a `std::vector<std::pair<int, int>>` containing the hull vertices in CCW order starting from the lexicographically smallest point. If there are fewer than 3 points or all points are collinear (i.e., the hull would have fewer than 3 vertices), return an empty vector. Points with duplicate coordinates should be treated as a single point. The output must be deterministic: for collinear points on the hull, include only the two extreme endpoints of any collinear run (no interior collinear points). For example, input `{(0,0), (1,1), (2,2)}` returns an empty vector because the hull is degenerate (all collinear). Input `{(0,0), (2,0), (1,1), (2,2), (0,2)}` returns `{(0,0), (2,0), (2,2), (0,2)}` in CCW order.
// The monotone chain algorithm sorts all unique points lexicographically (first by x, then by y). It then builds the lower hull by scanning left to right, maintaining a stack; a new point is added after popping any stack top that makes a non-left turn (i.e., cross product ≤ 0), which ensures we keep only convex vertices and remove collinear interior points. The upper hull is built by scanning right to left with the same turn condition. The two hulls (excluding duplicated endpoints) are concatenated to form the full hull in CCW order. Important edge cases: (1) duplicate points must be removed after sorting to avoid degenerate stack operations; (2) if the total number of unique points is less than 3, return empty; (3) if all points are collinear, both lower and upper hull construction will yield only 2 endpoints (or 1 if a single point), so we must verify the final hull has at least 3 vertices and return empty otherwise. The cross product `(b - a) × (c - a)` is computed as `(b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)`; using `≤ 0` (instead of `< 0`) pops collinear points, keeping only the extremes. Sorting takes \(O(n \log n)\), hull construction is \(O(n)\), so overall time is \(O(n \log n)\); auxiliary space is \(O(n)\) for the sorted vector and the hull stack. Output starts from the point with smallest x (and then y), which is the first element after sorting, and proceeds CCW around the hull.
#include <vector>
#include <algorithm>
#include <utility>

// Compute the convex hull of a set of points using Andrew's monotone chain.
// Returns hull vertices in counter-clockwise order, starting from the
// lexicographically smallest point. Returns empty vector if fewer than 3
// unique points or if all points are collinear.
std::vector<std::pair<int, int>> convexHull(std::vector<std::pair<int, int>> points) {
    // Remove duplicate points by sorting and using unique
    std::sort(points.begin(), points.end());
    points.erase(std::unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) {
        return {};
    }

    // Cross product of (a->b) and (a->c). Positive means counter-clockwise turn.
    auto cross = [](const std::pair<int, int>& a,
                    const std::pair<int, int>& b,
                    const std::pair<int, int>& c) -> long long {
        return static_cast<long long>(b.first - a.first) * (c.second - a.second)
             - static_cast<long long>(b.second - a.second) * (c.first - a.first);
    };

    std::vector<std::pair<int, int>> hull;

    // Build lower hull
    for (const auto& p : points) {
        // Pop while the last turn is not strictly counter-clockwise
        // (i.e., collinear or clockwise) to exclude interior collinear points
        while (hull.size() >= 2 &&
               cross(hull[hull.size() - 2], hull.back(), p) <= 0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }

    // Build upper hull
    int lower_size = hull.size();
    for (int i = static_cast<int>(points.size()) - 2; i >= 0; --i) {
        const auto& p = points[i];
        while (hull.size() > static_cast<size_t>(lower_size) &&
               cross(hull[hull.size() - 2], hull.back(), p) <= 0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }

    // Remove the duplicated last point (which equals the first)
    hull.pop_back();

    // If all points are collinear, hull will have only 2 vertices (the extreme endpoints)
    if (hull.size() < 3) {
        return {};
    }

    return hull;
}
#include <cassert>
#include <vector>
#include <utility>

// Declaration of the function under test
std::vector<std::pair<int, int>> convexHull(std::vector<std::pair<int, int>> points);

int main() {
    // Standard square
    std::vector<std::pair<int,int>> square = {{0,0}, {2,0}, {2,2}, {0,2}, {1,1}};
    std::vector<std::pair<int,int>> expected_square = {{0,0}, {2,0}, {2,2}, {0,2}};
    assert(convexHull(square) == expected_square);

    // All collinear -> degenerate hull, return empty
    std::vector<std::pair<int,int>> collinear = {{0,0}, {1,1}, {2,2}};
    assert(convexHull(collinear).empty());

    // Fewer than 3 unique points
    std::vector<std::pair<int,int>> two_points = {{1,1}, {2,2}};
    assert(convexHull(two_points).empty());

    // Duplicate points
    std::vector<std::pair<int,int>> duplicates = {{0,0}, {0,0}, {1,0}, {1,1}, {0,1}, {0,1}};
    std::vector<std::pair<int,int>> expected_dup = {{0,0}, {1,0}, {1,1}, {0,1}};
    assert(convexHull(duplicates) == expected_dup);

    // Single point
    std::vector<std::pair<int,int>> single = {{5,5}};
    assert(convexHull(single).empty());

    // Negative coordinates and asymmetric shape
    std::vector<std::pair<int,int>> neg_points = {{-2,-1}, {-1,2}, {1,1}, {3,-2}};
    std::vector<std::pair<int,int>> expected_neg = {{-2,-1}, {3,-2}, {1,1}, {-1,2}};
    assert(convexHull(neg_points) == expected_neg);

    // Triangle
    std::vector<std::pair<int,int>> triangle = {{0,0}, {3,0}, {1,2}};
    std::vector<std::pair<int,int>> expected_tri = {{0,0}, {3,0}, {1,2}};
    assert(convexHull(triangle) == expected_tri);

    // Rectangle with collinear points on edges (interior edge points excluded)
    std::vector<std::pair<int,int>> with_edge = {{0,0}, {1,0}, {2,0}, {2,1}, {2,2}, {1,2}, {0,2}, {0,1}};
    std::vector<std::pair<int,int>> expected_edge = {{0,0}, {2,0}, {2,2}, {0,2}};
    assert(convexHull(with_edge) == expected_edge);

    // All points identical
    std::vector<std::pair<int,int>> identical = {{4,4}, {4,4}, {4,4}};
    assert(convexHull(identical).empty());

    // Collinear with extreme points only (two points total after dedup)
    std::vector<std::pair<int,int>> collinear_two = {{0,0}, {0,5}};
    assert(convexHull(collinear_two).empty());

    return 0;
}
