// Write a C++ function named `collideCircleWithAxisAlignedSquare` that detects collision between a circle and a square whose edges are aligned with the global x and y axes. The function should accept the circle's center coordinates `(cx, cy)`, its radius `r`, and the square's axis-aligned bounding box defined by its minimum corner `(minX, minY)` and maximum corner `(maxX, maxY)`. It must return a `bool`: `true` if the circle and the square overlap or touch (including when the circle is entirely inside the square, the square is entirely inside the circle, or their boundaries touch), and `false` otherwise. Assume all inputs are valid with `r > 0`, `minX < maxX`, and `minY < maxY`. The function must be self-contained, use only standard C++ libraries, and not rely on any external geometric libraries. Handle edge cases such as the circle center being exactly on a corner or edge of the square, and ensure floating-point comparisons are robust.

The core idea is to find the closest point on the axis-aligned square to the circle's center. If the distance between that closest point and the circle's center is less than or equal to the circle's radius, then there is an overlap (or touching). The square is defined by intervals `[minX, maxX]` and `[minY, maxY]`. The closest point on the square to the circle center can be computed component-wise: for the x-coordinate, clamp the circle's x `cx` to the interval `[minX, maxX]`; similarly for the y-coordinate. This yields a point `(closestX, closestY)` that lies on the square's boundary or inside it. If the center is inside the square, the clamp gives the center itself, and the distance is zero, so the function returns `true`. If the center is outside, the clamp picks the nearest edge or corner. Then compute the squared Euclidean distance between the center and the closest point. Compare this squared distance to `r * r` to avoid a square root for efficiency and to reduce floating-point error. A distance exactly equal to the radius counts as a collision (touching). This algorithm runs in constant time `O(1)` with `O(1)` auxiliary space. Edge cases include: circle center outside but overlapping an edge (clamped to edge), outside but overlapping a corner (clamped to corner), inside the square (returns true regardless of radius), and the square fully inside the circle (closest point is a square corner or edge, and distance from center to that corner/edge is less than or equal to radius, so true). Floating-point comparisons use `<=` to include touching.

#include <algorithm>
#include <cmath>

// Checks if a circle with center (cx, cy) and radius r
// overlaps or touches an axis-aligned square defined by
// corners (minX, minY) and (maxX, maxY).
bool collideCircleWithAxisAlignedSquare(
    double cx, double cy, double r,
    double minX, double minY, double maxX, double maxY)
{
    // Find the closest point on the square to the circle's center
    // by clamping the center's coordinates to the square's intervals.
    double closestX = std::clamp(cx, minX, maxX);
    double closestY = std::clamp(cy, minY, maxY);

    // Compute the squared distance between the center and the closest point.
    double dx = cx - closestX;
    double dy = cy - closestY;
    double distSquared = dx * dx + dy * dy;

    // Overlap occurs if the squared distance is <= radius squared.
    // Using squared distance avoids a square root and is exact for comparisons.
    return distSquared <= r * r;
}

#include <cassert>
#include <cmath>

// The solution function declaration (assumed above, but included for completeness).
bool collideCircleWithAxisAlignedSquare(
    double cx, double cy, double r,
    double minX, double minY, double maxX, double maxY);

int main() {
    // Circle entirely inside square
    assert(collideCircleWithAxisAlignedSquare(0.0, 0.0, 0.5, -2.0, -2.0, 2.0, 2.0) == true);

    // Square entirely inside circle, circle centered at square center
    assert(collideCircleWithAxisAlignedSquare(0.0, 0.0, 5.0, -1.0, -1.0, 1.0, 1.0) == true);

    // Circle far away, no overlap
    assert(collideCircleWithAxisAlignedSquare(10.0, 10.0, 1.0, 0.0, 0.0, 2.0, 2.0) == false);

    // Circle touching the edge of the square exactly (distance == radius)
    double touchingX = 2.0 + 1.0; // center is exactly radius away from edge minX=2, but maxX=4? Let's use simple case
    assert(collideCircleWithAxisAlignedSquare(4.0, 1.0, 1.0, 2.0, 0.0, 4.0, 2.0) == true); // touching right edge

    // Circle overlapping a corner (distance to corner < radius)
    assert(collideCircleWithAxisAlignedSquare(3.0, 3.0, 2.0, 0.0, 0.0, 2.0, 2.0) == true); // center near corner (2,2), distance ~1.414 < 2

    // Circle center exactly on a corner of the square
    assert(collideCircleWithAxisAlignedSquare(2.0, 2.0, 0.5, 0.0, 0.0, 2.0, 2.0) == true);

    // Circle outside but touching a corner exactly (distance equals radius)
    // Corner at (2,2), circle center at (3,3), radius sqrt(2) gives distance sqrt(2)
    double r = std::sqrt(2.0);
    assert(collideCircleWithAxisAlignedSquare(3.0, 3.0, r, 0.0, 0.0, 2.0, 2.0) == true);

    // Circle outside but slightly too far from corner
    assert(collideCircleWithAxisAlignedSquare(3.0, 3.0, 1.0, 0.0, 0.0, 2.0, 2.0) == false); // distance sqrt(2) ≈ 1.414 > 1

    // Circle with very small radius just inside the square
    assert(collideCircleWithAxisAlignedSquare(-1.9, 1.9, 0.01, -2.0, -2.0, 2.0, 2.0) == true);

    return 0;
}
