Write a standalone C++ function `bool circleRectCollision(float cx, float cy, float radius, float rx, float ry, float rw, float rh)` that determines whether a circle (centered at `(cx, cy)` with given `radius`) overlaps an axis-aligned rectangle (whose top-left corner is `(rx, ry)`, with width `rw` and height `rh`). The rectangle's coordinates are in standard Cartesian space (y increases upward? No—use screen-space convention: y increases downward, as in the original sprite code). The function must return `true` if they intersect or touch (including edges/vertices) and `false` otherwise. Handle edge cases where the circle is entirely inside the rectangle, the rectangle is entirely inside the circle, or they are tangent. Use a `const`-correct free function with no side effects. The input values are all positive or zero floats (non-negative coordinates and dimensions), but the circle may be positioned anywhere relative to the rectangle. The implementation must not rely on any external libraries beyond standard headers.
// The classic approach is to find the closest point on the rectangle to the circle's center. Since the rectangle is axis-aligned, the closest point’s x-coordinate is the clamp of `cx` to the interval `[rx, rx+rw]`, and similarly for y with `[ry, ry+rh]`. Then compute the squared distance between the circle center and that closest point. If that squared distance is less than or equal to `radius * radius`, overlap occurs. This covers all cases: if the circle center is inside the rectangle, the closest point is the center itself, distance is zero, so overlap is true. If the circle is outside but near an edge, the closest point is on the boundary. For corners, the closest point is the rectangle’s vertex. Tangent cases (distance exactly equal to radius) are included via `<=`. Edge cases: zero-radius circle (treat as a point; if the point is inside or on the rectangle boundary, return true), zero-width or zero-height rectangle (degenerate line segment; the clamp still works). The algorithm is O(1) time and O(1) space. Use `std::clamp` from `<algorithm>` or implement a manual clamp to avoid potential compatibility issues. Since the task asks for a standalone function, include necessary headers (`<algorithm>`, `<cmath>` or just `<algorithm>` for clamp and no math needed if squared distance avoids sqrt). This avoids floating-point precision issues from sqrt and is efficient.
#include <algorithm>

// Determine if a circle overlaps an axis-aligned rectangle.
// Circle: center (cx, cy), radius (radius).
// Rectangle: top-left (rx, ry), width (rw), height (rh), using y-down coordinates.
bool circleRectCollision(float cx, float cy, float radius,
                         float rx, float ry, float rw, float rh) {
    // Clamp circle center to the rectangle's bounds to get the closest point.
    float closestX = std::clamp(cx, rx, rx + rw);
    float closestY = std::clamp(cy, ry, ry + rh);

    // Compute squared distance from circle center to closest point.
    float dx = cx - closestX;
    float dy = cy - closestY;
    float distSquared = dx * dx + dy * dy;

    // Overlap if squared distance <= radius squared.
    return distSquared <= radius * radius;
}
#include <cassert>

int main() {
    // Circle completely inside rectangle
    assert(circleRectCollision(5, 5, 1, 0, 0, 10, 10) == true);
    // Rectangle completely inside circle
    assert(circleRectCollision(0, 0, 100, 10, 10, 5, 5) == true);
    // Circle far away
    assert(circleRectCollision(50, 50, 1, 0, 0, 10, 10) == false);
    // Circle touching rectangle edge
    assert(circleRectCollision(10, 5, 1, 0, 0, 10, 10) == true); // touches right edge
    // Circle tangent to corner
    assert(circleRectCollision(11, 11, 1, 0, 0, 10, 10) == true); // just touches corner
    // Circle slightly beyond corner (no overlap)
    assert(circleRectCollision(11.1f, 11.1f, 1, 0, 0, 10, 10) == false);
    // Zero-radius circle inside rectangle
    assert(circleRectCollision(5, 5, 0, 0, 0, 10, 10) == true);
    // Zero-radius circle on rectangle border
    assert(circleRectCollision(0, 0, 0, 0, 0, 10, 10) == true);
    // Zero-width rectangle (vertical line) with circle centered on it
    assert(circleRectCollision(0, 5, 1, 0, 0, 0, 10) == true);
    // Zero-height rectangle (horizontal line) with circle near it
    assert(circleRectCollision(3, 1, 0.5f, 0, 0, 10, 0) == true);
    assert(circleRectCollision(3, 1.1f, 0.5f, 0, 0, 10, 0) == false);
}
