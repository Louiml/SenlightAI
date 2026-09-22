// Write a standalone C++ function named `lineRectangleIntersection` that determines whether a line segment from point `a` to point `b` intersects an axis-aligned rectangle defined by its center position `center` and half-extents `halfSize` (so the rectangle spans `center.x - halfSize.x` to `center.x + halfSize.x` in x and similarly for y). The function must return `true` if the segment touches or crosses the rectangle boundary or lies entirely inside the rectangle, and `false` otherwise. The segment endpoints may be anywhere, including outside or inside the rectangle. Handle degenerate cases such as zero-length segments and perfectly aligned (vertical/horizontal) segments correctly. You may assume all inputs are finite floating-point numbers.
#include <cassert>

int main() {
    // Rectangle: center (0,0), halfSize (2,1) => spans x[-2,2], y[-1,1]
    Vec2 center{0, 0};
    Vec2 half{2, 1};

    // Segment fully outside (to the right)
    assert(!lineRectangleIntersection({3, 0}, {5, 0}, center, half));

    // Segment crossing through the middle
    assert(lineRectangleIntersection({-3, 0}, {3, 0}, center, half));

    // Segment entirely inside
    assert(lineRectangleIntersection({0, 0}, {1, 0}, center, half));

    // Segment touching a corner (point (2,1) is a corner)
    assert(lineRectangleIntersection({2, 1}, {3, 2}, center, half));

    // Segment touching an edge tangentially
    assert(lineRectangleIntersection({0, 1}, {1, 1}, center, half));

    // Vertical segment parallel to left boundary but outside
    assert(!lineRectangleIntersection({-3, -2}, {-3, 2}, center, half));

    // Vertical segment parallel to left boundary and inside
    assert(lineRectangleIntersection({-2, -1}, {-2, 1}, center, half));

    // Segment with endpoint exactly on boundary
    assert(lineRectangleIntersection({-2, 0}, {-5, 0}, center, half));

    // Segment that misses rectangle but crosses its extended lines
    assert(!lineRectangleIntersection({-3, -3}, {3, -3}, center, half));

    // Zero-length segment inside
    assert(lineRectangleIntersection({0, 0}, {0, 0}, center, half));

    // Zero-length segment outside
    assert(!lineRectangleIntersection({5, 5}, {5, 5}, center, half));

    return 0;
}
The above test code is runnable and exercises the solution across common scenarios: outside, crossing, inside, touching, parallel, zero-length, and corner/edge cases.
#include <cmath>
#include <algorithm>

// Represents a 2D point/vector.
struct Vec2 {
    double x, y;
};

// Checks if a line segment from a to b intersects an axis-aligned rectangle.
// Rectangle defined by its center and half extents.
bool lineRectangleIntersection(const Vec2& a, const Vec2& b,
                               const Vec2& center, const Vec2& halfSize) {
    // Rectangle bounds
    double minX = center.x - halfSize.x;
    double maxX = center.x + halfSize.x;
    double minY = center.y - halfSize.y;
    double maxY = center.y + halfSize.y;

    // Direction vector of segment
    double dx = b.x - a.x;
    double dy = b.y - a.y;

    // Parametric t interval for intersection
    double tEnter = 0.0;
    double tLeave = 1.0;

    // Clip against each boundary using Liang-Barsky
    // Left boundary (x = minX)
    if (dx == 0.0) {
        // Segment parallel to left boundary; must be to the right of it
        if (a.x < minX) return false;
    } else {
        double t = (minX - a.x) / dx;
        if (dx > 0) {
            tEnter = std::max(tEnter, t);
        } else {
            tLeave = std::min(tLeave, t);
        }
    }

    // Right boundary (x = maxX)
    if (dx == 0.0) {
        if (a.x > maxX) return false;
    } else {
        double t = (maxX - a.x) / dx;
        if (dx > 0) {
            tLeave = std::min(tLeave, t);
        } else {
            tEnter = std::max(tEnter, t);
        }
    }

    // Bottom boundary (y = minY)
    if (dy == 0.0) {
        if (a.y < minY) return false;
    } else {
        double t = (minY - a.y) / dy;
        if (dy > 0) {
            tEnter = std::max(tEnter, t);
        } else {
            tLeave = std::min(tLeave, t);
        }
    }

    // Top boundary (y = maxY)
    if (dy == 0.0) {
        if (a.y > maxY) return false;
    } else {
        double t = (maxY - a.y) / dy;
        if (dy > 0) {
            tLeave = std::min(tLeave, t);
        } else {
            tEnter = std::max(tEnter, t);
        }
    }

    // Check if valid interval exists
    return tEnter <= tLeave;
}
// The solution uses a simplified form of the Liang-Barsky line clipping algorithm adapted for axis-aligned rectangles. The algorithm treats the line segment parametrically: `P(t) = a + t*(b - a)` for `t` in `[0,1]`. For each of the four rectangle boundaries (left, right, bottom, top), we compute the `t` values where the segment crosses that boundary. For a boundary, if the segment direction along that axis is zero (i.e., parallel to the boundary), then the segment only intersects if the point lies within the rectangle’s range on that axis; otherwise there is no intersection for that boundary. If the direction is non-zero, we compute `t` for entering and leaving the rectangle’s range and track the maximum entering `t` and minimum leaving `t`. If this interval `[tEnter, tLeave]` overlaps with `[0,1]`, the segment intersects (including touching at a single point). The algorithm handles all cases: segment entirely inside (the interval is `[0,1]`), crossing, touching a corner or edge, and no intersection. Time complexity is O(1) (only a few arithmetic operations), and space complexity is O(1). Edge cases include zero-length segments (where `tEnter` and `tLeave` may be degenerate but the point is checked against the rectangle directly) and segments lying exactly on a boundary line (handled by the parallel case check).
