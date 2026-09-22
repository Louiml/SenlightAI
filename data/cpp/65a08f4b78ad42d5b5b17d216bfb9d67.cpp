// Write a standalone C++ function `bool circleIntersectsSegment(b2Vec2 circleCenter, float32 radius, b2Vec2 segA, b2Vec2 segB, b2Vec2& closestPoint)` that determines whether a circle (given by center and radius) intersects a line segment (given by endpoints `segA` and `segB`). The function must return `true` if any point on the segment lies at distance ≤ `radius` from the circle center, and `false` otherwise. If intersection occurs, store the point on the segment that is closest to the circle center in `closestPoint`. Handle all edge cases: the segment may have zero length (endpoints equal), the circle may fully contain the segment, the closest point may be an interior point of the segment, or one of the endpoints. Implement this without using any external math library beyond basic arithmetic; you may define your own simple 2D vector type and dot/product functions. The function should be `const`-correct and use references for output parameters. This task abstracts the geometric core of the first function in the snippet (`b2CollideEdgeAndCircle`) but focuses purely on the distance-to-segment computation, ignoring contact manifolds, feature IDs, and edge adjacency — those are intentionally omitted to make the task standalone and focused.

// The solution approach is to use the standard point-to-segment distance algorithm. Given the circle center `C`, and segment endpoints `A` and `B`, compute the vector `AB = B - A` and the parameter `t` that projects `C` onto the infinite line through `A` and `B`: `t = dot(C - A, AB) / dot(AB, AB)`. If `dot(AB, AB)` is zero (segment length zero), then the segment is a single point; the closest point is `A` (or `B`, same), and we check distance directly. Otherwise, clamp `t` to the range [0, 1] to find the closest point on the finite segment: `closest = A + t * AB` (with clamped `t`). Then compute the squared distance between `closest` and `C`; if it is ≤ `radius * radius`, return `true` and set `closestPoint` to `closest`; else return `false`. Edge cases: (1) zero-length segment — handle before division; (2) `t` exactly at 0 or 1 — works naturally after clamping; (3) floating-point precision — use squared distance to avoid sqrt, and use `<=` for inclusive boundary; (4) if intersection occurs, the closest point is unique for a non-degenerate segment; for zero-length, it's the single point. Time complexity is O(1), space O(1). This is robust and simple.

#include <cmath>
#include <cfloat>

// Minimal 2D vector type for this task.
struct b2Vec2 {
    float32 x, y;
    b2Vec2(float32 x_ = 0.0f, float32 y_ = 0.0f) : x(x_), y(y_) {}

    b2Vec2 operator+(const b2Vec2& other) const { return b2Vec2(x + other.x, y + other.y); }
    b2Vec2 operator-(const b2Vec2& other) const { return b2Vec2(x - other.x, y - other.y); }
    b2Vec2 operator*(float32 scalar) const { return b2Vec2(x * scalar, y * scalar); }

    float32 dot(const b2Vec2& other) const { return x * other.x + y * other.y; }
    float32 lengthSquared() const { return x * x + y * y; }
};

// Determines if a circle intersects a segment. If yes, outputs the closest point on segment.
bool circleIntersectsSegment(b2Vec2 circleCenter, float32 radius, b2Vec2 segA, b2Vec2 segB, b2Vec2& closestPoint) {
    b2Vec2 ab = segB - segA;
    float32 abLenSq = ab.lengthSquared();

    // Handle degenerate segment (zero length).
    if (abLenSq <= FLT_EPSILON) {
        closestPoint = segA;
        float32 distSq = (circleCenter - segA).lengthSquared();
        return distSq <= radius * radius;
    }

    // Project circle center onto the infinite line through segment.
    float32 t = (circleCenter - segA).dot(ab) / abLenSq;

    // Clamp to segment range [0, 1].
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    closestPoint = segA + ab * t;
    float32 distSq = (circleCenter - closestPoint).lengthSquared();
    return distSq <= radius * radius;
}

#include <cassert>

int main() {
    // Helper to compare with tolerance
    auto close = [](b2Vec2 a, b2Vec2 b) {
        const float32 eps = 1e-4f;
        return std::fabs(a.x - b.x) < eps && std::fabs(a.y - b.y) < eps;
    };

    b2Vec2 closest;

    // Segment away from circle -> no intersection
    assert(!circleIntersectsSegment(b2Vec2(0, 0), 1.0f, b2Vec2(5, 0), b2Vec2(10, 0), closest));

    // Circle touches segment endpoint exactly
    assert(circleIntersectsSegment(b2Vec2(0, 0), 1.0f, b2Vec2(1, 0), b2Vec2(3, 0), closest));
    assert(close(closest, b2Vec2(1, 0)));

    // Circle intersects interior of segment
    assert(circleIntersectsSegment(b2Vec2(0, 0), 1.0f, b2Vec2(-2, 0), b2Vec2(2, 0), closest));
    assert(close(closest, b2Vec2(0, 0)));

    // Segment completely inside circle
    assert(circleIntersectsSegment(b2Vec2(0, 0), 5.0f, b2Vec2(-1, 0), b2Vec2(1, 0), closest));
    assert(close(closest, b2Vec2(-1, 0)));  // closest to center is left endpoint

    // Zero-length segment (single point) exactly on circle boundary
    assert(circleIntersectsSegment(b2Vec2(0, 0), 2.0f, b2Vec2(2, 0), b2Vec2(2, 0), closest));
    assert(close(closest, b2Vec2(2, 0)));

    // Zero-length segment outside circle
    assert(!circleIntersectsSegment(b2Vec2(0, 0), 1.0f, b2Vec2(3, 3), b2Vec2(3, 3), closest));

    // Closest point is at a segment endpoint because projection is outside range
    assert(circleIntersectsSegment(b2Vec2(0, 0), 2.0f, b2Vec2(3, 1), b2Vec2(5, 4), closest));
    // The closest point on that segment to the origin is (3,1) — distance sqrt(10) ≈ 3.16 > 2? Check: expect false
    assert(!circleIntersectsSegment(b2Vec2(0, 0), 2.0f, b2Vec2(3, 1), b2Vec2(5, 4), closest));

    // Now with a larger radius to actually intersect
    assert(circleIntersectsSegment(b2Vec2(0, 0), 4.0f, b2Vec2(3, 1), b2Vec2(5, 4), closest));
    assert(close(closest, b2Vec2(3, 1)));

    // Diagonal segment crossing near origin
    assert(circleIntersectsSegment(b2Vec2(0, 0), 1.0f, b2Vec2(-1, -1), b2Vec2(1, 1), closest));
    assert(close(closest, b2Vec2(0, 0)));

    return 0;
}
