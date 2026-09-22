/*
Write a C++ function `bool raySegmentIntersection(b2Vec2 p1, b2Vec2 p2, b2Vec2 v1, b2Vec2 v2, b2Vec2& intersection)` that determines whether a ray from point `p1` through point `p2` intersects a line segment from `v1` to `v2`. If an intersection exists, store the intersection point in `intersection` and return `true`; if no intersection exists, return `false`. The function should handle the case where the ray originates exactly on the segment (include that point as a valid intersection), and should return `false` if the ray is parallel to the segment, the segment has zero length, or the intersection point lies outside the segment’s parameter range `[0,1]`. Assume all coordinates are finite.
*/

#include <cmath>

struct b2Vec2 {
    float x, y;
    b2Vec2() : x(0), y(0) {}
    b2Vec2(float x_, float y_) : x(x_), y(y_) {}
};

// Return true if the ray from p1 through p2 intersects the segment v1-v2.
// Compute the intersection point and store it in 'intersection'.
bool raySegmentIntersection(const b2Vec2& p1, const b2Vec2& p2,
                            const b2Vec2& v1, const b2Vec2& v2,
                            b2Vec2& intersection) {
    b2Vec2 d(p2.x - p1.x, p2.y - p1.y);
    b2Vec2 e(v2.x - v1.x, v2.y - v1.y);

    float det = d.x * e.y - d.y * e.x;

    // Parallel or degenerate segment/ray.
    if (std::fabs(det) < 1e-12f) {
        return false;
    }

    b2Vec2 w(v1.x - p1.x, v1.y - p1.y);

    float cross_w_e = w.x * e.y - w.y * e.x;
    float cross_w_d = w.x * d.y - w.y * d.x;

    float t = cross_w_e / det;
    float s = cross_w_d / det;

    // t < 0 means intersection is behind the ray origin.
    // s outside [0,1] means intersection is outside the segment.
    if (t < 0.0f || s < 0.0f || s > 1.0f) {
        return false;
    }

    intersection.x = p1.x + t * d.x;
    intersection.y = p1.y + t * d.y;
    return true;
}

#include <cassert>
#include <cmath>

// b2Vec2 definition and function are assumed to be included above.

int main() {
    // Basic intersection.
    b2Vec2 q;
    assert(raySegmentIntersection(b2Vec2(0,0), b2Vec2(1,1), b2Vec2(0,1), b2Vec2(1,0), q));
    assert(std::fabs(q.x - 0.5f) < 1e-5f && std::fabs(q.y - 0.5f) < 1e-5f);

    // Ray starts exactly on segment.
    assert(raySegmentIntersection(b2Vec2(0.2f,0.2f), b2Vec2(1,1), b2Vec2(0,0), b2Vec2(1,1), q));
    assert(std::fabs(q.x - 0.2f) < 1e-5f && std::fabs(q.y - 0.2f) < 1e-5f);

    // No intersection (segment behind ray).
    assert(!raySegmentIntersection(b2Vec2(0,0), b2Vec2(-1,0), b2Vec2(1,0), b2Vec2(2,0), q));

    // No intersection (segment to the side).
    assert(!raySegmentIntersection(b2Vec2(0,0), b2Vec2(1,0), b2Vec2(1,2), b2Vec2(2,2), q));

    // Parallel but not collinear.
    assert(!raySegmentIntersection(b2Vec2(0,0), b2Vec2(1,0), b2Vec2(0,1), b2Vec2(1,1), q));

    // Zero-length segment.
    assert(!raySegmentIntersection(b2Vec2(0,0), b2Vec2(1,0), b2Vec2(2,2), b2Vec2(2,2), q));

    // Intersection at segment endpoint.
    assert(raySegmentIntersection(b2Vec2(0,0), b2Vec2(1,1), b2Vec2(2,2), b2Vec2(3,3), q));
    assert(std::fabs(q.x - 2.0f) < 1e-5f && std::fabs(q.y - 2.0f) < 1e-5f);

    // Ray direction exactly opposite to segment, but still intersects.
    assert(raySegmentIntersection(b2Vec2(2,2), b2Vec2(1,1), b2Vec2(0,0), b2Vec2(3,3), q));
    assert(std::fabs(q.x - 2.0f) < 1e-5f && std::fabs(q.y - 2.0f) < 1e-5f);

    // No intersection when ray points away from segment.
    assert(!raySegmentIntersection(b2Vec2(2,2), b2Vec2(3,3), b2Vec2(0,0), b2Vec2(1,1), q));
}

// The core of the problem is a classic ray‑segment intersection test. Represent the ray as `p(t) = p1 + t * d` with `d = p2 - p1` and `t >= 0`. Represent the segment as `v(s) = v1 + s * e` with `e = v2 - v1` and `s in [0,1]`. Solve the linear system: `p1 + t d = v1 + s e`. Rewrite as `t d - s e = v1 - p1`. This is a 2x2 linear system in unknowns `t` and `s`. Compute the determinant `det = d.x * e.y - d.y * e.x`. If `det == 0`, the ray and segment are parallel; return `false`. Otherwise, `t = cross(v1 - p1, e) / det` and `s = cross(v1 - p1, d) / det` where `cross(a,b) = a.x*b.y - a.y*b.x`. If `t < 0` or `s < 0` or `s > 1`, return `false`. Otherwise compute the intersection point `q = p1 + t * d` and set `intersection = q`. Edge cases: a zero‑length segment (`v1 == v2`) yields `e == 0`, so determinant is `0`; that returns `false`. If the ray starts exactly on the segment, we get `t=0` and `s` within `[0,1]`, which is allowed. Numerical robustness: use `float` arithmetic as given, but avoid comparing determinant to exactly `0`; instead check `fabs(det) < 1e-12f` to treat near‑parallel cases as no intersection. Time complexity is O(1), space complexity O(1).
