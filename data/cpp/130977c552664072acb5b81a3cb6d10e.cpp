/*
Write a standalone C++ function named `simplifyPolygon` that takes a `std::vector<std::pair<double,double>>` representing a closed 2D polygon (ordered vertices, no self-intersections, at least 3 vertices) and an `epsilon` value (a positive double). The function must apply the Ramer–Douglas–Peucker (RDP) algorithm to reduce the number of vertices while approximately preserving the shape, and then close the simplified polygon by ensuring the first and last points are the same. The output should be a new `std::vector<std::pair<double,double>>` containing the simplified closed polygon (i.e., the last vertex equals the first). If the input has fewer than 3 vertices, return an empty vector. The function must be `const`-correct, use only standard library headers, and must not modify the input. The algorithm must keep the original first and last points (which are the same for closure) and recursively process interior points based on perpendicular distance to the line segment between endpoints.
*/

#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

using Point = std::pair<double, double>;

// Compute perpendicular distance from point p to line segment (a, b)
double perpendicularDistance(const Point& p, const Point& a, const Point& b) {
    double dx = b.first - a.first;
    double dy = b.second - a.second;
    if (dx == 0.0 && dy == 0.0) {
        // Degenerate segment, treat as distance to point a
        return std::hypot(p.first - a.first, p.second - a.second);
    }
    // Line equation: ax + by + c = 0, where a=dy, b=-dx, c=dx*a.y - dy*a.x
    double num = std::abs(dy * (p.first - a.first) - dx * (p.second - a.second));
    double den = std::hypot(dx, dy);
    return num / den;
}

// Recursive RDP helper for a list of points (open polyline, no duplicate closure)
std::vector<Point> rdpRecursive(const std::vector<Point>& points, double epsilon) {
    if (points.size() < 3) {
        return points;
    }

    int index = -1;
    double maxDist = 0.0;
    for (size_t i = 1; i < points.size() - 1; ++i) {
        double d = perpendicularDistance(points[i], points.front(), points.back());
        if (d > maxDist) {
            maxDist = d;
            index = static_cast<int>(i);
        }
    }

    if (maxDist > epsilon) {
        std::vector<Point> left(points.begin(), points.begin() + index + 1);
        std::vector<Point> right(points.begin() + index, points.end());
        auto leftResult = rdpRecursive(left, epsilon);
        auto rightResult = rdpRecursive(right, epsilon);
        leftResult.insert(leftResult.end(), rightResult.begin() + 1, rightResult.end());
        return leftResult;
    } else {
        return {points.front(), points.back()};
    }
}

// Simplify a closed polygon (first and last points equal) using RDP
std::vector<Point> simplifyPolygon(const std::vector<Point>& polygon, double epsilon) {
    if (polygon.size() < 3) {
        return {};
    }

    // Ensure input is closed: if not, add closing point
    std::vector<Point> closed = polygon;
    if (closed.front() != closed.back()) {
        closed.push_back(closed.front());
    }

    // Remove the duplicate closing point for processing
    std::vector<Point> unique(closed.begin(), closed.end() - 1);
    auto simplified = rdpRecursive(unique, epsilon);

    // If simplification removed too much (fewer than 2 points), return empty
    if (simplified.size() < 2) {
        return {};
    }

    // Close the polygon again
    simplified.push_back(simplified.front());
    return simplified;
}

#include <cassert>
#include <vector>
#include <utility>

// Declare the function (should match the solution)
std::vector<Point> simplifyPolygon(const std::vector<Point>& polygon, double epsilon);

int main() {
    // Simple square
    std::vector<Point> square = {{0,0}, {10,0}, {10,10}, {0,10}, {0,0}};
    auto result = simplifyPolygon(square, 0.1);
    assert(result.size() == 5);
    assert(result.front() == result.back());
    assert(result[0] == Point(0,0));
    assert(result[1] == Point(10,0));
    assert(result[2] == Point(10,10));
    assert(result[3] == Point(0,10));

    // Polygon with collinear points should be reduced
    std::vector<Point> withCollinear = {{0,0}, {5,0}, {10,0}, {10,5}, {10,10}, {5,10}, {0,10}, {0,5}, {0,0}};
    result = simplifyPolygon(withCollinear, 1.0);
    assert(result.size() == 5); // corners only + closing
    assert(result[0] == Point(0,0));
    assert(result[2] == Point(10,10));

    // Polygon with a slight bump that should be removed with large epsilon
    std::vector<Point> bumpy = {{0,0}, {5,0.1}, {10,0}, {10,10}, {0,10}, {0,0}};
    result = simplifyPolygon(bumpy, 1.0);
    assert(result.size() == 5); // bump removed
    assert(result[1] == Point(10,0));

    // Small epsilon should keep bump
    result = simplifyPolygon(bumpy, 0.01);
    assert(result.size() == 6); // keeps the bump

    // Triangle
    std::vector<Point> tri = {{0,0}, {5,8}, {10,0}, {0,0}};
    result = simplifyPolygon(tri, 0.5);
    assert(result.size() == 4);
    assert(result.front() == result.back());

    // Invalid input
    std::vector<Point> invalid = {{1,1}, {2,2}};
    result = simplifyPolygon(invalid, 0.5);
    assert(result.empty());

    // Empty input
    result = simplifyPolygon({}, 0.5);
    assert(result.empty());

    return 0;
}

// The core idea is to implement the classic recursive RDP simplification. Given a closed polygon, the first vertex is also the last (duplicate), so we ignore the duplicate during recursion. The algorithm starts with the full closed loop; since the first and last are identical, we only process the unique points from index 0 to N-1 (the duplicate last is handled at the end). In each recursive call, we take the first and last points of the current segment and find the point in between with the maximum perpendicular distance to the line connecting them. If that maximum distance exceeds `epsilon`, we keep that point and recursively simplify the two subsegments; otherwise, we discard all intermediate points and return just the endpoints. After recursion, we append the duplicate closing point (equal to the first) to the result. Edge cases: if the polygon has fewer than 3 unique vertices (e.g., input length < 3 or after simplification results in a degenerate shape), we might return an empty vector or handle gracefully. We must also compute perpendicular distance correctly for vertical and horizontal lines to avoid division by zero. Time complexity is O(n log n) on average for typical inputs, but worst-case can be O(n^2) if the polygon is shaped adversarially (e.g., all points are far from the line). Space complexity is O(n) due to recursion stack and result vectors.
