/*
Write a standalone C++ function named `countPointsWithinDistance` that takes a vector of pairs of doubles (representing 2D points) and a non-negative double `d` (the maximum allowed distance from the origin), and returns an integer count of how many points lie at a Euclidean distance less than or equal to `d` from the origin `(0,0)`. The input vector may be empty (return 0), may contain points with negative coordinates, and the distance threshold `d` can be zero (only the origin point exactly at `(0,0)` would count). The function must be self-contained, use `const` correctly, and not rely on global variables or external input.
*/

#include <vector>
#include <cmath>
#include <utility>

// Count points whose Euclidean distance from the origin is <= d.
int countPointsWithinDistance(const std::vector<std::pair<double, double>>& points, double d) {
    // Compare squared distances to avoid sqrt and floating-point rounding.
    const double d_squared = d * d;
    int count = 0;
    for (const auto& p : points) {
        const double dist_squared = p.first * p.first + p.second * p.second;
        if (dist_squared <= d_squared) {
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or link it).

int main() {
    using Point = std::pair<double, double>;
    // Basic test with 3 points, 2 within distance 5.
    std::vector<Point> pts1 = {{3.0, 4.0}, {5.0, 0.0}, {0.0, 6.0}};
    assert(countPointsWithinDistance(pts1, 5.0) == 2);

    // Empty list returns 0.
    std::vector<Point> pts2 = {};
    assert(countPointsWithinDistance(pts2, 10.0) == 0);

    // Negative coordinates handled correctly.
    std::vector<Point> pts3 = {{-3.0, -4.0}, {-1.0, 0.0}};
    assert(countPointsWithinDistance(pts3, 5.0) == 2);

    // Zero distance: only the origin counts.
    std::vector<Point> pts4 = {{0.0, 0.0}, {0.1, 0.0}, {-0.0, 0.0}};
    assert(countPointsWithinDistance(pts4, 0.0) == 2); // both 0.0 and -0.0 are reference-equal in double arithmetic

    // Exact boundary: (3,4) distance 5 counted when d=5.
    std::vector<Point> pts5 = {{3.0, 4.0}, {3.0, 4.0001}};
    assert(countPointsWithinDistance(pts5, 5.0) == 1);

    // Large coordinates (but within double range).
    std::vector<Point> pts6 = {{1000.0, 1000.0}, {1.0, 1.0}};
    assert(countPointsWithinDistance(pts6, 1500.0) == 2);

    // All points outside.
    std::vector<Point> pts7 = {{10.0, 10.0}, {0.0, 20.0}};
    assert(countPointsWithinDistance(pts7, 5.0) == 0);

    return 0;
}

// The core algorithm is to iterate through all points in the input vector, compute the Euclidean distance from the origin using the formula `sqrt(x*x + y*y)`, and increment a counter if this distance is less than or equal to `d`. Since we only need the distance for comparison, we can compare `x*x + y*y <= d*d` to avoid floating-point inaccuracies and the costly square root operation—this is both faster and more numerically stable. Edge cases: an empty vector correctly returns 0; a point exactly at `(0,0)` will have `x*x + y*y = 0`, so it counts when `d` is 0 or positive; negative coordinates are squared so they pose no issue; very large values (up to `double` limits) may overflow when squared, but that is an inherent limitation of the direct approach—we assume input magnitudes are within reasonable bounds. Time complexity is O(n) where n is the number of points, since we scan each point exactly once. Space complexity is O(1) extra space beyond the input vector, as we only use a few scalar variables.
