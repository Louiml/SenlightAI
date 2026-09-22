Write a C++ function named `circleStats` that takes a single positive `float` radius as input and returns a `CircleMetrics` struct (defined within the same header scope) containing the area and circumference of the circle, both computed with the approximation π = 3.14159. The function must validate that the radius is greater than zero; if not, it should return a `CircleMetrics` with both fields set to `-1.0f`. The task requires the struct to have two public float members: `area` and `circumference`. Do not include a `main` function; only provide the struct definition and the function implementation.
// The solution defines a simple struct `CircleMetrics` with two float members. The function `circleStats` begins by checking if the input radius is less than or equal to zero. If invalid, it immediately returns a default-constructed `CircleMetrics` where both members are set to `-1.0f` (achieved by initializing them in the struct's default constructor or explicitly assigning). For a valid radius, the area is computed as `3.14159 * radius * radius` and the circumference as `2 * 3.14159 * radius`. The function returns the populated struct by value. Edge cases: radius exactly zero or negative returns the error indicator; very large or very small radii may cause floating-point overflow or underflow, but this is acceptable for typical use. Time complexity is O(1), space complexity is O(1) as only two floats are used.
// circle_stats.h (or inline in the solution)
#include <cfloat> // not strictly needed, but for completeness

struct CircleMetrics {
    float area;
    float circumference;

    // Default constructor initializes to error sentinel
    CircleMetrics() : area(-1.0f), circumference(-1.0f) {}
};

// Compute area and circumference of a circle given its radius.
// Returns {-1,-1} if radius is non-positive.
CircleMetrics circleStats(float radius) {
    if (radius <= 0.0f) {
        return CircleMetrics{};
    }

    const float pi = 3.14159f;
    CircleMetrics result;
    result.area = pi * radius * radius;
    result.circumference = 2.0f * pi * radius;
    return result;
}
#include <cassert>
#include <cmath>

int main() {
    // Test positive radius
    CircleMetrics m1 = circleStats(1.0f);
    assert(std::fabs(m1.area - 3.14159f) < 1e-5f);
    assert(std::fabs(m1.circumference - 6.28318f) < 1e-5f);

    // Test radius 0 (invalid)
    CircleMetrics m2 = circleStats(0.0f);
    assert(m2.area == -1.0f);
    assert(m2.circumference == -1.0f);

    // Test negative radius (invalid)
    CircleMetrics m3 = circleStats(-5.0f);
    assert(m3.area == -1.0f);
    assert(m3.circumference == -1.0f);

    // Test a larger radius
    CircleMetrics m4 = circleStats(2.0f);
    assert(std::fabs(m4.area - (3.14159f * 4.0f)) < 1e-5f);
    assert(std::fabs(m4.circumference - (2.0f * 3.14159f * 2.0f)) < 1e-5f);

    // Test a small fractional radius
    CircleMetrics m5 = circleStats(0.5f);
    assert(std::fabs(m5.area - (3.14159f * 0.25f)) < 1e-5f);
    assert(std::fabs(m5.circumference - (2.0f * 3.14159f * 0.5f)) < 1e-5f);

    return 0;
}
