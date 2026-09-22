Write a standalone C++ function `interpolate_position_linear` that performs linear interpolation on a set of pre-defined keyframe positions. The function should accept a floating-point `time` value in the range `[0.0, 1.0]` and return the interpolated position as a 3D vector (using a simple struct with `x`, `y`, `z` fields). The keyframes are fixed: keyframe 0 is at position `(0.0, 0.0, 0.0)` and keyframe 1 is at position `(10.0, 20.0, 30.0)`. If the input `time` is outside the range `[0.0, 1.0]`, the function should return `std::nullopt` (or a sentinel value) to indicate failure. The interpolation should be performed component-wise using the formula `result = p0 + (p1 - p0) * time`. The function must be `const`-correct, use proper error handling, and be self-contained with no external dependencies beyond standard headers.

#include <cassert>
#include <cmath>

int main() {
    // Valid time = 0.0 gives start keyframe
    auto p0 = interpolate_position_linear(0.0f);
    assert(p0.has_value());
    assert(std::fabs(p0->x - 0.0f) < 1e-6);
    assert(std::fabs(p0->y - 0.0f) < 1e-6);
    assert(std::fabs(p0->z - 0.0f) < 1e-6);

    // Valid time = 1.0 gives end keyframe
    auto p1 = interpolate_position_linear(1.0f);
    assert(p1.has_value());
    assert(std::fabs(p1->x - 10.0f) < 1e-6);
    assert(std::fabs(p1->y - 20.0f) < 1e-6);
    assert(std::fabs(p1->z - 30.0f) < 1e-6);

    // Valid time = 0.5 gives midpoint
    auto p_half = interpolate_position_linear(0.5f);
    assert(p_half.has_value());
    assert(std::fabs(p_half->x - 5.0f) < 1e-6);
    assert(std::fabs(p_half->y - 10.0f) < 1e-6);
    assert(std::fabs(p_half->z - 15.0f) < 1e-6);

    // Invalid time < 0
    auto p_neg = interpolate_position_linear(-0.1f);
    assert(!p_neg.has_value());

    // Invalid time > 1
    auto p_over = interpolate_position_linear(1.1f);
    assert(!p_over.has_value());

    // Edge: exactly at boundaries still valid
    auto p_edge0 = interpolate_position_linear(0.0f);
    auto p_edge1 = interpolate_position_linear(1.0f);
    assert(p_edge0.has_value() && p_edge1.has_value());

    return 0;
}

#include <optional>

struct Vector3 {
    float x, y, z;
};

// Linearly interpolates between fixed keyframe positions (0,0,0) and (10,20,30).
// time must be in [0.0, 1.0]; returns std::nullopt if out of range.
std::optional<Vector3> interpolate_position_linear(float time) {
    if (time < 0.0f || time > 1.0f) {
        return std::nullopt;
    }

    // Fixed keyframes: keyframe 0 at (0,0,0), keyframe 1 at (10,20,30)
    const Vector3 start = {0.0f, 0.0f, 0.0f};
    const Vector3 end   = {10.0f, 20.0f, 30.0f};

    Vector3 result;
    result.x = start.x + (end.x - start.x) * time;
    result.y = start.y + (end.y - start.y) * time;
    result.z = start.z + (end.z - start.z) * time;

    return result;
}

// The solution is straightforward: define a `Vector3` struct with `x`, `y`, `z` members, and a function that takes a `float` time parameter. First, validate that `time` is within `[0.0, 1.0]`; if not, return an empty optional (or a sentinel). Then compute each coordinate as `start + (end - start) * time`, where `start` is the fixed keyframe 0 `(0,0,0)` and `end` is keyframe 1 `(10,20,30)`. Since keyframe 0 is at the origin, the result simplifies to `(10*time, 20*time, 30*time)`, but the general formula is implemented for clarity. The main edge case is invalid time input, which is handled by validating the range before any arithmetic. Time complexity is O(1) with O(1) auxiliary space, as only a few scalar operations and a struct return are involved. Using `std::optional` provides clear failure signaling without exception overhead.
