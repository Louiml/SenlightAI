Write a C++ function named `nearestPointOnSegment` that takes a 2D point `p` and a line segment defined by endpoints `a` and `b`, and returns the point on the segment that is closest to `p`. The function must also return the squared distance from `p` to that closest point, using an output parameter. Use a simple 2D vector structure with `float` components and basic dot product operations. The function should correctly handle degenerate segments (where `a` equals `b`), and the returned point must lie exactly on the segment (including endpoints). The function should be `const`-correct and self-contained with no external dependencies.
The solution uses vector projection to find the closest point. Compute the segment direction vector `d = b - a` and its squared length. If the squared length is zero (degenerate segment), the closest point is simply `a`. Otherwise, project vector `p - a` onto `d` using the dot product, producing parameter `t = dot(p - a, d) / dot(d, d)`. Clamp `t` to the range `[0, 1]` so the point lies between the endpoints. The closest point is then `a + t * d`. The squared distance is `dot(p - closestPoint, p - closestPoint)`. This is a classic computational geometry technique with time complexity O(1) and space complexity O(1). Important edge cases include: degenerate segment, point exactly on the segment, point beyond either endpoint (clamped to the nearest endpoint), and perpendicular projection falling exactly at an endpoint. The method handles all cases uniformly via clamping.
#include <cmath>

// Simple 2D vector type.
struct Vec2 {
    float x, y;
    
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x_, float y_) : x(x_), y(y_) {}
    
    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }
    
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }
    
    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }
    
    float dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }
};

// Returns the point on segment [a, b] closest to p, and stores the squared distance.
Vec2 nearestPointOnSegment(const Vec2& p, const Vec2& a, const Vec2& b, float& squaredDistance) {
    Vec2 d = b - a;
    float lenSq = d.dot(d);
    
    if (lenSq < 1e-12f) {
        // Degenerate segment (a == b).
        Vec2 diff = p - a;
        squaredDistance = diff.dot(diff);
        return a;
    }
    
    float t = (p - a).dot(d) / lenSq;
    t = std::max(0.0f, std::min(1.0f, t));
    
    Vec2 closest = a + d * t;
    Vec2 diff = p - closest;
    squaredDistance = diff.dot(diff);
    return closest;
}
#include <cassert>
#include <cmath>

int main() {
    float dist = 0.0f;
    Vec2 result;
    
    // Point directly above the middle of a horizontal segment.
    result = nearestPointOnSegment(Vec2(5.0f, 3.0f), Vec2(0.0f, 0.0f), Vec2(10.0f, 0.0f), dist);
    assert(result.x == 5.0f && result.y == 0.0f);
    assert(std::fabs(dist - 9.0f) < 1e-6f);
    
    // Point beyond the right endpoint.
    result = nearestPointOnSegment(Vec2(12.0f, -2.0f), Vec2(0.0f, 0.0f), Vec2(10.0f, 0.0f), dist);
    assert(result.x == 10.0f && result.y == 0.0f);
    assert(std::fabs(dist - 8.0f) < 1e-6f);
    
    // Point beyond the left endpoint.
    result = nearestPointOnSegment(Vec2(-3.0f, 4.0f), Vec2(0.0f, 0.0f), Vec2(10.0f, 0.0f), dist);
    assert(result.x == 0.0f && result.y == 0.0f);
    assert(std::fabs(dist - 25.0f) < 1e-6f);
    
    // Degenerate segment (a == b).
    result = nearestPointOnSegment(Vec2(7.0f, 8.0f), Vec2(1.0f, 2.0f), Vec2(1.0f, 2.0f), dist);
    assert(result.x == 1.0f && result.y == 2.0f);
    assert(std::fabs(dist - (36.0f + 36.0f)) < 1e-6f);
    
    // Point exactly on the segment (should return the point itself, distance 0).
    result = nearestPointOnSegment(Vec2(4.0f, 2.0f), Vec2(0.0f, 0.0f), Vec2(8.0f, 4.0f), dist);
    assert(std::fabs(result.x - 4.0f) < 1e-6f && std::fabs(result.y - 2.0f) < 1e-6f);
    assert(dist < 1e-6f);
    
    // Diagonal segment with point perpendicular to the middle.
    result = nearestPointOnSegment(Vec2(1.0f, 1.0f), Vec2(0.0f, 0.0f), Vec2(2.0f, 0.0f), dist);
    assert(std::fabs(result.x - 1.0f) < 1e-6f && std::fabs(result.y - 0.0f) < 1e-6f);
    assert(std::fabs(dist - 1.0f) < 1e-6f);
}
