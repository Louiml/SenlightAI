Implement a C++ function `circleIntersectsSegment` that determines whether a circle, specified by a center point `(cx, cy)` and radius `r`, intersects a line segment from `(x1, y1)` to `(x2, y2)`. The function must return `true` if the segment touches or passes through the circle, including cases where the segment end lies exactly on the circle boundary, or one endpoint is inside the circle while the other is outside (i.e., partial overlap), and `false` if the segment is entirely outside the circle with no point at distance exactly `r` (or less) from the center. You must handle the degenerate case where the segment has zero length (i.e., both endpoints are identical) by checking the distance from that point to the center. Use floating-point arithmetic with a tolerance of `1e-9` to account for precision issues. Do not use any external libraries; implement distance, dot product, and quadratic formula logic manually. The function signature is: `bool circleIntersectsSegment(double cx, double cy, double r, double x1, double y1, double x2, double y2);`

The problem reduces to computing the minimum distance from the circle center to the segment and comparing it with the radius. The main algorithm uses the parametric representation of the segment: any point `P(t) = A + t*(B-A)` where `A = (x1,y1)`, `B = (x2,y2)`, and `t` is in `[0,1]`. The squared distance from center `C` to `P(t)` is a quadratic function `f(t) = |A-C|^2 + 2*t*((A-C)·(B-A)) + t^2*|B-A|^2`. To find the minimum distance (closest point on the segment to the center), we compute the projection parameter `t = -((A-C)·(B-A)) / |B-A|^2`. If this projected `t` lies within `[0,1]`, then the closest point is interior; otherwise the closest point is one of the endpoints. In the degenerate case where `|B-A|^2` is nearly zero, we treat the segment as a single point and directly compare its distance to the center against the radius. Edge cases include: (1) the segment is entirely inside the circle but does not cross the boundary? Actually a segment with both endpoints inside the circle may still intersect the circle if it crosses the boundary? No, if both endpoints are inside, the entire segment is inside (since the circle is convex), so it does not intersect the boundary, but the task says "touches or passes through the circle" meaning intersection with the disc (including interior). The function should return `true` if any point of the segment has distance <= r to center. That is the standard circle-segment intersection: it returns true if the minimum distance <= r. We use tolerance `1e-9` to compare distances to avoid floating errors. Time complexity is O(1), space O(1). We avoid square roots until the final comparison (instead compare squared distances for efficiency) but we need tolerance; use `distanceSquared <= r*r + tol` where tol is a small epsilon scaling. For robustness, compute `dist2` and compare to `r*r + 1e-9`. Also, when computing `t` for projection, handle division by zero. The algorithm: compute `dx = x2-x1`, `dy = y2-y1`; if both are 0 (length^2 < epsilon), then check distance from (x1,y1) to center <= r. Else compute `t = ((cx-x1)*dx + (cy-y1)*dy) / (dx*dx+dy*dy)`; clamp `t` to [0,1]; compute closest point `(x1+t*dx, y1+t*dy)`; compute squared distance from center to that point; return `dist2 <= r*r + tol`.

#include <cmath>

/**
 * Determines whether a circle and a line segment intersect (touch or overlap).
 * @param cx, cy Circle center coordinates.
 * @param r Circle radius (assumed non-negative).
 * @param x1, y1 First endpoint of the segment.
 * @param x2, y2 Second endpoint of the segment.
 * @return true if any point on the segment is within distance r of the center.
 */
bool circleIntersectsSegment(double cx, double cy, double r,
                             double x1, double y1, double x2, double y2) {
    // Use squared distances to avoid sqrt until needed.
    // Tolerance for floating-point comparison.
    const double tolerance = 1e-9;
    double r2 = r * r;

    // Vector along the segment.
    double dx = x2 - x1;
    double dy = y2 - y1;
    double length2 = dx * dx + dy * dy;

    // Degenerate segment: both endpoints identical.
    if (length2 < tolerance * tolerance) {
        double px = cx - x1;
        double py = cy - y1;
        double dist2 = px * px + py * py;
        return dist2 <= r2 + tolerance;
    }

    // Project center onto the infinite line, then clamp t to [0,1] for the segment.
    double t = ((cx - x1) * dx + (cy - y1) * dy) / length2;
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;

    // Compute closest point on segment.
    double closestX = x1 + t * dx;
    double closestY = y1 + t * dy;

    // Squared distance from center to closest point.
    double px = cx - closestX;
    double py = cy - closestY;
    double dist2 = px * px + py * py;

    return dist2 <= r2 + tolerance;
}

#include <cassert>
#include <cmath>

// Declare the solution function (must match the header in the solution).
bool circleIntersectsSegment(double cx, double cy, double r,
                             double x1, double y1, double x2, double y2);

int main() {
    // Circle at origin, radius 2.
    double cx = 0, cy = 0, r = 2.0;

    // Segment completely outside.
    assert(!circleIntersectsSegment(cx, cy, r, 3, 3, 5, 5));

    // Segment starts outside, ends inside -> intersects.
    assert(circleIntersectsSegment(cx, cy, r, 3, 0, 0, 0));

    // Segment endpoints both inside -> intersects (whole segment inside).
    assert(circleIntersectsSegment(cx, cy, r, 0, 0, 1, 1));

    // Segment exactly tangential: distance from center to line = r.
    // Line y = 2, from x=-1 to x=1.
    assert(circleIntersectsSegment(cx, cy, r, -1, 2, 1, 2));

    // Segment just misses: line y=2.0000001, from x=-1 to x=1.
    assert(!circleIntersectsSegment(cx, cy, r, -1, 2.0000001, 1, 2.0000001));

    // Degenerate segment: single point on the boundary.
    assert(circleIntersectsSegment(cx, cy, r, 2, 0, 2, 0));

    // Degenerate segment: single point inside.
    assert(circleIntersectsSegment(cx, cy, r, 0, 0, 0, 0));

    // Degenerate segment: single point outside.
    assert(!circleIntersectsSegment(cx, cy, r, 3, 0, 3, 0));

    // Segment that crosses through the circle (endpoints outside on opposite sides).
    assert(circleIntersectsSegment(cx, cy, r, -3, 0, 3, 0));

    // Segment with one endpoint exactly on boundary and other outside.
    assert(circleIntersectsSegment(cx, cy, r, 2, 0, 5, 5));

    return 0;
}
