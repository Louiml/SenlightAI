Write a C++ function that takes four coordinates `(x1, y1)` and `(x2, y2)` as `double` values representing two points in a 2D plane, and returns the Euclidean distance between them as a `double`, rounded to six decimal places. The function must compute the distance using the standard formula `sqrt((x1 - x2)^2 + (y1 - y2)^2)`. You must not use any pre-existing `sqrt` function; instead, implement your own square root calculation (e.g., using Newton-Raphson method) with a precision of at least `1e-8` absolute error for the square root. The coordinates may be negative, zero, or positive, and the distance can be zero when points coincide.
#include <cassert>
#include <cmath>

// The solution function is declared here for completeness; in a real test it would be included from the solution file.
double euclideanDistanceRounded(double x1, double y1, double x2, double y2);

int main() {
    // Zero distance for identical points
    assert(euclideanDistanceRounded(0.0, 0.0, 0.0, 0.0) == 0.0);
    assert(euclideanDistanceRounded(5.0, -3.0, 5.0, -3.0) == 0.0);

    // Simple axis-aligned distances
    assert(euclideanDistanceRounded(0.0, 0.0, 3.0, 0.0) == 3.0);
    assert(euclideanDistanceRounded(0.0, 0.0, -4.0, 0.0) == 4.0);

    // Diagonal with exact known value: 3-4-5 triangle
    assert(euclideanDistanceRounded(0.0, 0.0, 3.0, 4.0) == 5.0);

    // Negative coordinates
    assert(euclideanDistanceRounded(-1.0, -1.0, -4.0, -5.0) == 5.0); // dx=3, dy=4

    // Non-integer result, check with tolerance using fabs difference < 1e-9
    double result = euclideanDistanceRounded(1.0, 2.0, 4.0, 6.0); // sqrt(25) = 5
    assert(std::fabs(result - 5.0) < 1e-6);

    // Large coordinates to test stability
    double large = euclideanDistanceRounded(1e7, 1e7, 0.0, 0.0);
    double expected = std::sqrt(2.0) * 1e7;
    assert(std::fabs(large - std::round(expected * 1e6) / 1e6) < 1e-3);

    return 0;
}
#include <cmath>
#include <limits>

// Compute Euclidean distance between two points (x1, y1) and (x2, y2),
// using a custom Newton-Raphson square root, then round to 6 decimal places.
double euclideanDistanceRounded(double x1, double y1, double x2, double y2) {
    const double dx = x1 - x2;
    const double dy = y1 - y2;
    const double squared = dx * dx + dy * dy;

    // Custom square root via Newton-Raphson
    // Handle zero specially to avoid division by zero
    if (squared == 0.0) {
        return 0.0;
    }

    long double x = squared;          // initial guess
    long double value = static_cast<long double>(squared);
    const long double epsilon = 1e-12L;  // tighter than required 1e-8

    // Iterate until convergence or max 100 iterations
    for (int i = 0; i < 100; ++i) {
        long double next = 0.5L * (x + value / x);
        if (std::fabsl(next - x) < epsilon) {
            x = next;
            break;
        }
        x = next;
    }

    // Round to 6 decimal places
    double distance = static_cast<double>(x);
    return std::round(distance * 1e6) / 1e6;
}
// The core of the problem is computing the Euclidean distance without relying on the standard library `sqrt`. The distance is defined as `sqrt(dx*dx + dy*dy)`, where `dx = x1 - x2` and `dy = y1 - y2`. Since the squared sum is always non-negative, we can feed it into a custom square root function. A robust approach is Newton-Raphson iteration: start with an initial guess (e.g., `x = value`), then repeatedly update `x = 0.5 * (x + value / x)` until the change between iterations is less than `1e-8` or a maximum iteration count is reached. Edge cases include `value == 0`, where the root is trivially 0, and very large or very small values where floating-point stability matters; using `long double` internally for the calculation improves precision, but the result should be cast back to `double`. Time complexity is `O(log(1/epsilon))` iterations (usually converging in a few dozen iterations), and space complexity is `O(1)`. After computing the square root, we format the result to six decimal places by rounding. Since the function returns a `double`, we can perform rounding by adding `0.5e-6` before truncation if needed, but it's simpler to just return the raw value and let the caller format it; for the task, the function should return the distance as a `double` already rounded to six decimal places, e.g., `std::round(value * 1e6) / 1e6`.
