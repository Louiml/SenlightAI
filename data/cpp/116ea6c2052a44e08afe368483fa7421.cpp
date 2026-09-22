Given two non-negative real numbers `time` and `speed` (where `speed` is guaranteed to be greater than zero), write a C++ function `travelTime` that returns the time required to travel a distance of `time` units at a constant speed of `speed` units per unit time. The result must be computed as `time / speed` and returned as a `double` with the full precision of double-precision floating-point arithmetic. The function should handle all finite positive inputs correctly, including very large and very small values, without performing any unnecessary rounding or truncation beyond the inherent precision of the `double` type. The function must take two `double` parameters and return a `double`.
// The solution is straightforward: the travel time is simply the quotient of the distance and the speed. The main algorithm is a single floating-point division. Edge cases to consider include extremely small speeds (which produce very large times, possibly approaching infinity), and extremely large times or speeds that may cause overflow or underflow—but since the inputs are guaranteed to be finite and speed > 0, the division is well-defined. The result is a `double`, so we let the hardware perform the division with IEEE 754 double-precision semantics. No special handling is needed for negative values because the problem restricts inputs to non-negative. Time complexity is O(1) and space complexity is O(1). The function should be marked `const`-correct (the parameters are passed by value, so no mutation occurs). The only nuance is that the result should be returned directly without any formatting or rounding; any output formatting (such as fixed precision) is the caller's responsibility.
#include <cstddef>

// Compute the time to travel 'distance' units at 'speed' units per time unit.
// Precondition: speed > 0.0
double travelTime(double distance, double speed) {
    return distance / speed;
}
#include <cassert>
#include <cmath>

// Forward declaration of the function under test.
double travelTime(double distance, double speed);

int main() {
    // Basic case: 10 units at 2 units/time = 5 time units.
    assert(std::abs(travelTime(10.0, 2.0) - 5.0) < 1e-12);

    // Speed of 1: time equals distance.
    assert(travelTime(3.5, 1.0) == 3.5);

    // Distance zero: time is zero regardless of speed.
    assert(travelTime(0.0, 12345.678) == 0.0);

    // Large distance, small speed: result is large but finite.
    assert(std::abs(travelTime(1e308, 1e-308) - 1e616) < 1e600);

    // Small distance, large speed: result is tiny.
    assert(std::abs(travelTime(1e-308, 1e308) - 1e-616) < 1e-628);

    // Both large but ratio is moderate.
    assert(std::abs(travelTime(2.0e200, 4.0e100) - 5.0e99) < 1e85);

    // Both small but ratio is moderate.
    assert(std::abs(travelTime(3.0e-200, 6.0e-100) - 5.0e-101) < 1e-113);

    // Identical values: result must be exactly 1.0.
    assert(travelTime(42.0, 42.0) == 1.0);

    // Non-integer ratio produces typical double result.
    assert(std::abs(travelTime(7.0, 3.0) - 7.0/3.0) < 1e-15);

    // Test with a repeating decimal ratio.
    assert(std::abs(travelTime(1.0, 3.0) - 1.0/3.0) < 1e-15);

    // All tests passed.
    return 0;
}
