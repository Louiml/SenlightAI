Write a C++ function `computeCircleTangent` that takes two circles, represented by their centers (as `std::pair<double,double>`) and radii (as `double`), and returns a `std::pair<double,double>` representing the point where the two circles are tangent. The function must handle the following cases: (1) if the circles already intersect at exactly one point, return that single intersection point; (2) if the circles intersect at two points, return the point that lies on the line connecting the two centers and is closer to the center of the first circle; (3) if the circles are separate (no intersection), adjust both circles by increasing the radius of the smaller circle and decreasing the radius of the larger circle by the same amount so that they become tangent, ensuring the adjustment is minimal (i.e., the smallest possible adjustment), then return the tangent point. Assume the circles are not identical and have positive radii. Use double precision and avoid floating-point comparisons with `==`; instead, use a tolerance of `1e-9`.

#include <cmath>
#include <cassert>
#include <utility>

int main() {
    const double tol = 1e-7;

    // Case 1: Externally tangent circles.
    {
        Circle c1 = {{0.0, 0.0}, 1.0};
        Circle c2 = {{3.0, 0.0}, 2.0};
        Point p = computeCircleTangent(c1, c2);
        assert(std::abs(p.first - 1.0) < tol && std::abs(p.second - 0.0) < tol);
    }

    // Case 2: Internally tangent (small inside big).
    {
        Circle c1 = {{0.0, 0.0}, 1.0};
        Circle c2 = {{5.0, 0.0}, 4.0};
        Point p = computeCircleTangent(c1, c2);
        assert(std::abs(p.first - 1.0) < tol && std::abs(p.second - 0.0) < tol);
    }

    // Case 3: Two intersections, pick the point closer to c1.
    {
        Circle c1 = {{0.0, 0.0}, 2.0};
        Circle c2 = {{2.0, 0.0}, 2.0};
        Point p = computeCircleTangent(c1, c2);
        assert(std::abs(p.first - 2.0) < tol && std::abs(p.second - 0.0) < tol);
    }

    // Case 4: Separate circles, adjust both radii.
    {
        Circle c1 = {{0.0, 0.0}, 1.0};
        Circle c2 = {{10.0, 0.0}, 3.0};
        // d = 10, r1+r2 = 4, delta = 3
        // After adjustment: r1' = 4, r2' = 0 (but radius cannot be zero; still mathematically tangent).
        // Tangency point at distance 4 from c1 along x-axis.
        Point p = computeCircleTangent(c1, c2);
        assert(std::abs(p.first - 4.0) < tol && std::abs(p.second - 0.0) < tol);
    }

    // Case 5: One circle inside the other without touching.
    {
        Circle c1 = {{0.0, 0.0}, 5.0};
        Circle c2 = {{3.0, 0.0}, 1.0};
        // d=3, outer=5, inner=1, delta = (5 - 3 - 1)/2 = 0.5
        // Adjusted: outer radius=4.5, inner radius=1.5, they touch at x=4.5 from outer center.
        Point p = computeCircleTangent(c1, c2);
        assert(std::abs(p.first - 4.5) < tol && std::abs(p.second - 0.0) < tol);
    }

    // Case 6: Identical-sized circles at distance 2r.
    {
        Circle c1 = {{0.0, 0.0}, 2.0};
        Circle c2 = {{4.0, 0.0}, 2.0};
        Point p = computeCircleTangent(c1, c2);
        assert(std::abs(p.first - 2.0) < tol && std::abs(p.second - 0.0) < tol);
    }

    // Case 7: One circle small, the other huge, barely separate.
    {
        Circle c1 = {{0.0, 0.0}, 0.1};
        Circle c2 = {{2.0, 0.0}, 1.8};
        // d=2, r1+r2=1.9, delta=0.05 -> adjusted r1=0.15, r2=1.75 tangent at x=0.15.
        Point p = computeCircleTangent(c1, c2);
        assert(std::abs(p.first - 0.15) < tol && std::abs(p.second - 0.0) < tol);
    }

    // Case 8: Arbitrary orientation, externally tangent.
    {
        Circle c1 = {{0.0, 0.0}, 1.0};
        Circle c2 = {{3.0, 4.0}, 4.0};
        double d = 5.0;
        // Tangent point is 1 unit from c1 toward c2.
        Point p = computeCircleTangent(c1, c2);
        double expected_x = 0.0 + 1.0 * (3.0/5.0);
        double expected_y = 0.0 + 1.0 * (4.0/5.0);
        assert(std::abs(p.first - expected_x) < tol && std::abs(p.second - expected_y) < tol);
    }

    // Case 9: Circles with one inside the other after adjustment.
    {
        Circle c1 = {{2.0, 3.0}, 6.0};
        Circle c2 = {{6.0, 8.0}, 2.0};
        double d = dist(c1.center, c2.center);
        // d = sqrt(16+25)=sqrt(41) ≈ 6.403, outer=6, inner=2, d+inner > outer? 6.403+2 > 6 => intersects? No, d+2>6, d<6+2? 6.403<8 => intersects at two points. So case 3.
        Point p = computeCircleTangent(c1, c2);
        // Point at distance 6 from c1 toward c2.
        double dx = (c2.center.first - c1.center.first)/d;
        double dy = (c2.center.second - c1.center.second)/d;
        double ex = c1.center.first + dx * 6.0;
        double ey = c1.center.second + dy * 6.0;
        assert(std::abs(p.first - ex) < 1e-6 && std::abs(p.second - ey) < 1e-6);
    }

    // Case 10: Very far apart circles.
    {
        Circle c1 = {{0.0, 0.0}, 1.0};
        Circle c2 = {{100.0, 0.0}, 2.0};
        Point p = computeCircleTangent(c1, c2);
        double delta = (100.0 - 3.0)/2.0;
        double adj1 = 1.0 + delta;
        assert(std::abs(p.first - adj1) < 1e-6 && std::abs(p.second - 0.0) < 1e-6);
    }

    return 0;
}

#include <cmath>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <cassert>

// Represents a 2D point.
using Point = std::pair<double,double>;

// Represents a circle with center and radius.
struct Circle {
    Point center;
    double radius;
};

// Compute the squared distance between two points.
double distSq(const Point& a, const Point& b) {
    double dx = a.first - b.first;
    double dy = a.second - b.second;
    return dx * dx + dy * dy;
}

// Compute the distance between two points.
double dist(const Point& a, const Point& b) {
    return std::sqrt(distSq(a, b));
}

// Return a point at distance 'r' from 'center' along the direction to 'target'.
Point pointAtDistance(const Point& center, const Point& target, double r) {
    double d = dist(center, target);
    if (d < 1e-12) {
        // Degenerate: centers coincide. Use arbitrary direction.
        return {center.first + r, center.second};
    }
    double dx = (target.first - center.first) / d;
    double dy = (target.second - center.second) / d;
    return {center.first + dx * r, center.second + dy * r};
}

// Return the point where the two circles are tangent after minimal adjustment if needed.
Point computeCircleTangent(const Circle& c1, const Circle& c2) {
    const double tol = 1e-9;
    double d = dist(c1.center, c2.center);
    double r1 = c1.radius;
    double r2 = c2.radius;

    // Case 1: Externally tangent.
    if (std::abs(d - (r1 + r2)) < tol) {
        return pointAtDistance(c1.center, c2.center, r1);
    }

    // Case 2: Internally tangent (one inside the other, touching).
    if (std::abs(d + std::min(r1, r2) - std::max(r1, r2)) < tol) {
        if (r1 < r2) {
            // Circle 1 inside circle 2.
            return pointAtDistance(c1.center, c2.center, r1);
        } else {
            // Circle 2 inside circle 1.
            return pointAtDistance(c2.center, c1.center, r2);
        }
    }

    // Case 3: Two intersections. Pick the point on the line between centers closer to c1.
    if (std::abs(r1 - r2) < d && d < r1 + r2) {
        // Both points are symmetric; the one on the segment toward c2 at distance r1 from c1.
        return pointAtDistance(c1.center, c2.center, r1);
    }

    // Case 4: Separate circles (no intersection). Adjust radii minimally.
    if (d > r1 + r2) {
        double delta = (d - r1 - r2) / 2.0;
        // Increase smaller radius, decrease larger radius.
        if (r1 <= r2) {
            Circle adj1 = c1;
            Circle adj2 = c2;
            adj1.radius += delta;
            adj2.radius -= delta;
            // Now tangent externally.
            return pointAtDistance(adj1.center, adj2.center, adj1.radius);
        } else {
            Circle adj1 = c1;
            Circle adj2 = c2;
            adj1.radius -= delta;
            adj2.radius += delta;
            return pointAtDistance(adj2.center, adj1.center, adj2.radius);
        }
    }

    // Case 5: One circle inside the other without touching.
    double outer_radius = std::max(r1, r2);
    double inner_radius = std::min(r1, r2);
    if (d + inner_radius < outer_radius) {
        double delta = (outer_radius - d - inner_radius) / 2.0;
        if (r1 > r2) {
            // c1 is outer.
            Circle adj1 = c1;
            Circle adj2 = c2;
            adj1.radius -= delta;
            adj2.radius += delta;
            // c2 is inner, adjust outward toward c1.
            return pointAtDistance(adj1.center, adj2.center, adj1.radius);
        } else {
            // c2 is outer.
            Circle adj1 = c1;
            Circle adj2 = c2;
            adj1.radius += delta;
            adj2.radius -= delta;
            return pointAtDistance(adj2.center, adj1.center, adj2.radius);
        }
    }

    // Shouldn't reach here unless identical circles.
    throw std::invalid_argument("Circles are identical; no unique tangent point.");
}

// The core challenge is to handle three geometric cases robustly. First, compute the distance `d` between the centers. If `d` equals approximately `r1 + r2` (external tangency) or `abs(r1 - r2)` (internal tangency), the circles touch at one point. That point lies on the line connecting the centers, at a distance `r1` from the first center along the direction toward (or away from) the second center. If `abs(r1 - r2) < d < r1 + r2`, the circles intersect at two points; we must select the intersection point that lies on the line between the centers and is closer to the first center. This is the point on the segment between the two centers at distance `r1` from the first center. For the separate case where `d > r1 + r2`, we adjust both radii by adding or subtracting the same amount `delta = (d - r1 - r2)/2`. Specifically, if the circles are separate, we increase the radius of the smaller circle and decrease the radius of the larger circle by `delta`, so they become externally tangent. If one circle is entirely inside the other without touching (`d + min(r1,r2) < max(r1,r2)`), we adjust by increasing the inner circle's radius and decreasing the outer circle's radius by `(max(r1,r2) - d - min(r1,r2))/2` so they become internally tangent. After adjustment, the tangent point is the intersection of the line connecting centers with the newly adjusted circles at the appropriate side. Because the adjustment is linearly proportional, the tangent point's position on the line is a weighted average of the centers. The algorithm is O(1) time and O(1) space. Precision is handled using a relative/absolute tolerance.
