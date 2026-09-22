// Write a C++ function that computes the 2D convex hull of a set of points using the Graham scan algorithm. The function should accept a vector of 2D points (each point represented by x and y coordinates as `double`) and return a vector of points that form the convex hull in counter-clockwise order, starting from the lowest x-coordinate (and lowest y-coordinate if ties exist). The input may contain duplicate points and degenerate cases such as all points being collinear (in which case the hull should contain only the two extreme endpoints) or a single unique point (in which case the hull contains just that point). The function must be robust to any input size, including empty input (return an empty hull) and input with exactly one point. Implement the standard Graham scan approach: sort points lexicographically, build the lower hull, then build the upper hull, and concatenate them excluding duplicate endpoints.

// The solution uses the classic Andrew's monotone chain variant of the Graham scan, which is simpler and avoids trigonometric angle computations. First, sort all points lexicographically by x, then by y. If the smallest and largest points are identical (meaning all points are the same), return just that point. Otherwise, build the lower hull by iterating through sorted points and maintaining a stack: while the last three points (including the new candidate) do not make a counter-clockwise turn (cross product ≤ 0), pop the middle point. After processing all points, build the upper hull by iterating in reverse sorted order using the same logic. Finally, combine the lower and upper hulls, removing the last point of each since they duplicate the first point of the other hull. The cross product determines orientation: for points A, B, C, the sign of (B.x - A.x)*(C.y - A.y) - (B.y - A.y)*(C.x - A.x) tells if the turn is left (positive) or right (non-positive). Edge cases: empty input returns empty; one unique point returns that point; collinear points produce only the two extremes because the cross product condition pops intermediate points. Time complexity is O(n log n) due to sorting and O(n) for hull construction, giving O(n log n) overall. Space complexity is O(n) for storing sorted points and the hull.

#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

struct Point2D {
    double x, y;

    bool operator<(const Point2D& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator==(const Point2D& other) const {
        return std::abs(x - other.x) < 1e-9 && std::abs(y - other.y) < 1e-9;
    }
};

// Helper to compute cross product of vectors AB and AC.
static double cross(const Point2D& A, const Point2D& B, const Point2D& C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

// Compute the convex hull of a set of points using Graham scan (Andrew's monotone chain).
std::vector<Point2D> convexHull2D(const std::vector<Point2D>& points) {
    if (points.empty()) return {};

    std::vector<Point2D> sorted = points;
    std::sort(sorted.begin(), sorted.end());

    // If all points are the same, return that single point.
    if (sorted.front() == sorted.back()) {
        return {sorted.front()};
    }

    std::vector<Point2D> hull;

    // Build lower hull.
    for (const auto& p : sorted) {
        while (hull.size() >= 2 && cross(hull[hull.size()-2], hull.back(), p) <= 0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }

    // Build upper hull.
    size_t lower_size = hull.size();
    for (auto it = sorted.rbegin() + 1; it != sorted.rend(); ++it) {
        while (hull.size() > lower_size && cross(hull[hull.size()-2], hull.back(), *it) <= 0) {
            hull.pop_back();
        }
        hull.push_back(*it);
    }

    // Remove the last point (duplicate of the first).
    hull.pop_back();
    return hull;
}

#include <cassert>
#include <vector>
#include <cmath>

// (Point2D and convexHull2D definitions go here)

int main() {
    // Empty input.
    std::vector<Point2D> empty;
    assert(convexHull2D(empty).empty());

    // Single point.
    std::vector<Point2D> single = {{1.0, 2.0}};
    auto hull1 = convexHull2D(single);
    assert(hull1.size() == 1 && hull1[0] == Point2D{1.0, 2.0});

    // All identical points.
    std::vector<Point2D> identical = {{0,0}, {0,0}, {0,0}};
    auto hull2 = convexHull2D(identical);
    assert(hull2.size() == 1 && hull2[0] == Point2D{0,0});

    // Collinear points.
    std::vector<Point2D> collinear = {{0,0}, {1,1}, {2,2}, {3,3}};
    auto hull3 = convexHull2D(collinear);
    assert(hull3.size() == 2);
    assert(hull3[0] == Point2D{0,0});
    assert(hull3[1] == Point2D{3,3});

    // Square.
    std::vector<Point2D> square = {{0,0}, {1,0}, {1,1}, {0,1}};
    auto hull4 = convexHull2D(square);
    assert(hull4.size() == 4);
    assert(hull4[0] == Point2D{0,0});
    assert(hull4[1] == Point2D{1,0});
    assert(hull4[2] == Point2D{1,1});
    assert(hull4[3] == Point2D{0,1});

    // Triangle with interior point.
    std::vector<Point2D> tri = {{0,0}, {2,0}, {1,2}, {1,1}};
    auto hull5 = convexHull2D(tri);
    assert(hull5.size() == 3);
    assert(hull5[0] == Point2D{0,0});
    assert(hull5[1] == Point2D{2,0});
    assert(hull5[2] == Point2D{1,2});

    // Pentagon with duplicates.
    std::vector<Point2D> pent = {{0,0}, {1,0}, {1,1}, {0,1}, {0.5,0.5}, {0,0}, {1,0}};
    auto hull6 = convexHull2D(pent);
    assert(hull6.size() == 4);
    assert(hull6[0] == Point2D{0,0});
    assert(hull6[1] == Point2D{1,0});
    assert(hull6[2] == Point2D{1,1});
    assert(hull6[3] == Point2D{0,1});

    // Points with negative and decimal coordinates.
    std::vector<Point2D> mixed = {{-1.5, -2.0}, {3.0, 0.5}, {0.0, 0.0}, {2.0, -1.0}};
    auto hull7 = convexHull2D(mixed);
    assert(hull7.size() == 4);
    assert(hull7[0] == Point2D{-1.5, -2.0});
    assert(hull7[1] == Point2D{0.0, 0.0});
    assert(hull7[2] == Point2D{3.0, 0.5});
    assert(hull7[3] == Point2D{2.0, -1.0});

    return 0;
}
