// Given two convex polygons specified as sequences of 2D points in counterclockwise order (with no self-intersections and no repeated closing vertex), write a C++ function that computes the area of their intersection. The function should take two `std::vector<std::pair<double,double>>` arguments representing the vertices of the two polygons and return the area as a double. You may assume that the input polygons are convex and that their vertices are given in counterclockwise order. The intersection may be empty, a single point, a line segment (area zero), or a non-degenerate polygon. Handle degeneracies gracefully and return an area of 0.0 for empty or degenerate intersections. You are not allowed to use any external geometry libraries; implement the computation from scratch using standard C++ algorithms and data structures.

The standard approach for intersecting two convex polygons is the Sutherland–Hodgman polygon clipping algorithm. It works by clipping one polygon (the subject) against each edge of the other polygon (the clip polygon). Since the clip polygon is convex, each clip edge defines a half-plane (the interior side). The algorithm iterates over the vertices of the subject polygon, and for each edge of the clip polygon, it keeps the vertices that lie inside the half-plane and inserts intersection points where the subject polygon edges cross the clip boundary. After processing all clip edges, the resulting polygon (if any) is the intersection.

Key steps:
- For each edge of the clip polygon (from vertex `i` to vertex `(i+1)` in counterclockwise order), compute the outward normal of that edge using a cross product. The interior half‑plane is defined by `cross(edge, point - vertex_i) >= 0` (assuming points are in the xy‑plane and counterclockwise order).
- For the subject polygon, iterate over all its edges. For each subject edge (from `current` to `next`), classify both endpoints: `inside_current` and `inside_next`. Add `current` if inside, and if the edge crosses the clip boundary (i.e., one endpoint inside, the other outside), compute the intersection point of the two line segments and add it.
- This process yields a new polygon that is the subject polygon clipped to the current half‑plane. Then feed this new polygon as the subject for the next clip edge.
- After processing all clip edges, the final polygon is the intersection. Compute its area using the shoelace formula. If the polygon has fewer than 3 vertices, area is zero.

Edge cases: If the subject polygon lies entirely outside the clip polygon, after some iteration the output may become empty or degenerate (fewer than 3 vertices). The algorithm handles this naturally by producing an empty vector. If the intersection is a single point or segment, the polygon will have fewer than 3 vertices and area zero.

Time complexity: O(n·m) where n is the number of vertices of the subject polygon and m is the number of edges of the clip polygon. Space complexity O(n+m) for storing intermediate polygons.

#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

using Point = std::pair<double, double>;

// Helper: cross product (a - b) x (c - b), with b as origin
double cross(const Point& a, const Point& b, const Point& c) {
    return (a.first - b.first) * (c.second - b.second)
         - (a.second - b.second) * (c.first - b.first);
}

// Helper: check if point p is inside half-plane defined by edge (a -> b)
// For counterclockwise polygon, interior is to the left: cross(a, p, b) >= 0
bool inside(const Point& a, const Point& p, const Point& b) {
    return cross(a, p, b) >= 0; // note: cross(a, p, b) = (a-p)x(b-p)
    // Wait: we want (b - a) x (p - a) >= 0. So correct test:
    // return ((b.first - a.first)*(p.second - a.second) - (b.second - a.second)*(p.first - a.first)) >= 0;
}

// Helper: compute intersection of segment p1-p2 with line through clip edge a-b
Point lineIntersect(const Point& a, const Point& b, const Point& p1, const Point& p2) {
    double x1 = p1.first, y1 = p1.second;
    double x2 = p2.first, y2 = p2.second;
    double x3 = a.first, y3 = a.second;
    double x4 = b.first, y4 = b.second;

    double denom = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
    if (std::fabs(denom) < 1e-12) return p1; // degenerate, return p1
    double t = ((x1 - x3)*(y3 - y4) - (y1 - y3)*(x3 - x4)) / denom;
    return { x1 + t*(x2 - x1), y1 + t*(y2 - y1) };
}

// Clip polygon `subject` against the half-plane defined by edge (a,b)
std::vector<Point> clip(const std::vector<Point>& subject, const Point& a, const Point& b) {
    std::vector<Point> output;
    if (subject.empty()) return output;

    for (size_t i = 0; i < subject.size(); ++i) {
        Point current = subject[i];
        Point next = subject[(i+1) % subject.size()];

        // Correct inside test: (b - a) x (p - a) >= 0
        auto isInside = [&](const Point& p) {
            double cross = (b.first - a.first)*(p.second - a.second)
                         - (b.second - a.second)*(p.first - a.first);
            return cross >= 0;
        };

        bool currentInside = isInside(current);
        bool nextInside = isInside(next);

        if (currentInside) output.push_back(current);
        if (currentInside != nextInside) {
            output.push_back(lineIntersect(a, b, current, next));
        }
    }
    return output;
}

// Compute area of intersection of two convex polygons
double polygonIntersectionArea(const std::vector<Point>& poly1, const std::vector<Point>& poly2) {
    if (poly1.size() < 3 || poly2.size() < 3) return 0.0;

    std::vector<Point> subject = poly1;
    // Clip `poly1` against each edge of `poly2`
    for (size_t i = 0; i < poly2.size(); ++i) {
        Point a = poly2[i];
        Point b = poly2[(i+1) % poly2.size()];
        subject = clip(subject, a, b);
        if (subject.size() < 3) return 0.0;
    }

    // Compute area using shoelace formula
    double area2 = 0.0;
    for (size_t i = 0; i < subject.size(); ++i) {
        size_t j = (i+1) % subject.size();
        area2 += subject[i].first * subject[j].second - subject[j].first * subject[i].second;
    }
    return std::fabs(area2) / 2.0;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

using Point = std::pair<double, double>;

// Declaration of the solution function
double polygonIntersectionArea(const std::vector<Point>& poly1, const std::vector<Point>& poly2);

int main() {
    // Two overlapping squares: one from (0,0) to (2,2), another from (1,1) to (3,3)
    std::vector<Point> square1 = {{0,0},{2,0},{2,2},{0,2}};
    std::vector<Point> square2 = {{1,1},{3,1},{3,3},{1,3}};
    double area = polygonIntersectionArea(square1, square2);
    assert(std::fabs(area - 1.0) < 1e-9); // overlap is a 1x1 square

    // Disjoint squares
    std::vector<Point> square3 = {{5,5},{6,5},{6,6},{5,6}};
    assert(std::fabs(polygonIntersectionArea(square1, square3)) < 1e-9); // empty intersection

    // One polygon completely inside the other
    std::vector<Point> smallSquare = {{0.5,0.5},{1.5,0.5},{1.5,1.5},{0.5,1.5}};
    double area2 = polygonIntersectionArea(square1, smallSquare);
    assert(std::fabs(area2 - 1.0) < 1e-9); // intersection equals the small square

    // Touching at a corner (area zero)
    std::vector<Point> touching = {{2,2},{3,2},{3,3},{2,3}};
    assert(std::fabs(polygonIntersectionArea(square1, touching)) < 1e-9);

    // Overlapping triangles
    std::vector<Point> tri1 = {{0,0},{2,0},{0,2}};
    std::vector<Point> tri2 = {{0,0},{3,0},{0,3}};
    double area3 = polygonIntersectionArea(tri1, tri2);
    assert(std::fabs(area3 - 2.0) < 1e-9); // tri1 is completely inside tri2

    return 0;
}
