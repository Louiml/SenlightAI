Write a standalone C++ function named `triangleArea` that takes two `double` parameters representing the base and height of a triangle (both non-negative) and returns the area as a `double`. The area is computed as `0.5 * base * height`. The function must handle zero and positive values correctly, and for any negative inputs it should return `0.0` (treat them as invalid). Do not include a `main` function in your solution — only the function definition with proper `const` correctness.
#include <cassert>
#include <cmath>

int main() {
    // Basic positive values
    assert(std::fabs(triangleArea(10.0, 5.0) - 25.0) < 1e-9);
    // Zero base or height
    assert(triangleArea(0.0, 7.0) == 0.0);
    assert(triangleArea(3.0, 0.0) == 0.0);
    // Both zero
    assert(triangleArea(0.0, 0.0) == 0.0);
    // Fractional values
    assert(std::fabs(triangleArea(2.5, 4.0) - 5.0) < 1e-9);
    // Large values
    assert(std::fabs(triangleArea(1e6, 2e6) - 1e12) < 1e-3);
    // Negative inputs return 0.0
    assert(triangleArea(-1.0, 5.0) == 0.0);
    assert(triangleArea(3.0, -2.0) == 0.0);
    assert(triangleArea(-4.0, -4.0) == 0.0);
}
// Computes the area of a triangle given its base and height.
// Returns 0.0 if either input is negative (invalid).
double triangleArea(const double base, const double height) {
    if (base < 0.0 || height < 0.0) {
        return 0.0;
    }
    return 0.5 * base * height;
}
// The solution is straightforward: multiply the base by the height and then multiply by 0.5. Since the inputs are `double`, no integer overflow concerns arise, but floating-point precision is inherent. The main edge case is negative inputs — the problem statement expects non-negative values, so the function should defensively return `0.0` for any negative base or height to avoid nonsensical negative areas. The algorithm uses constant time \(O(1)\) and constant auxiliary space \(O(1)\). No additional data structures are needed. For very large or very small magnitudes, standard IEEE-754 double precision applies.
