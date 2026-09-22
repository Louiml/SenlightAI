// Write a C++ function that takes a single positive real number representing the length of a skid mark (in consistent units, e.g., meters) and returns the approximate speed of the vehicle (in the corresponding velocity units, e.g., m/s) using the empirical formula `v = 14.6 * sqrt(c)`, where `c` is the skid mark length. The function must correctly handle inputs including zero (where speed should be zero) and very small or very large positive values, and it must not modify its input. The returned value should be the raw computed speed (a real number) with no rounding or formatting applied.

// The solution directly applies the given formula using the C++ standard library's `sqrt` function. The main algorithm is trivial: read the input length `c`, compute `14.6 * sqrt(c)`, and return the result. Edge cases include `c = 0`, which yields `0` (a valid physical result of zero speed), and very small positive values like `1e-6`, which produce a positive but tiny speed. Since `sqrt` is defined for all non-negative real numbers, no special handling is needed beyond ensuring the input is non-negative (which the task assumes). Time complexity is O(1) because only a single square root and multiplication are performed, and space complexity is O(1) as only a few local variables are used. The function is `const`-correct by taking the input by value (a simple `double`), which avoids modifying any external state and is efficient for a scalar.

#include <cmath>

// Computes the approximate speed (v) of a vehicle based on the skid mark length (c).
// Formula: v = 14.6 * sqrt(c), where c >= 0.
double computeSpeedFromSkidMark(const double skidMarkLength) {
    return 14.6 * std::sqrt(skidMarkLength);
}

#include <cassert>
#include <cmath>

// The solution function is declared here (or included from the header).
double computeSpeedFromSkidMark(const double skidMarkLength);

int main() {
    // Zero length yields zero speed.
    assert(computeSpeedFromSkidMark(0.0) == 0.0);

    // A length of 1 gives exactly 14.6.
    assert(std::abs(computeSpeedFromSkidMark(1.0) - 14.6) < 1e-9);

    // A length of 4 gives 14.6 * 2 = 29.2.
    assert(std::abs(computeSpeedFromSkidMark(4.0) - 29.2) < 1e-9);

    // A small positive length gives a small positive speed.
    assert(computeSpeedFromSkidMark(0.25) == 14.6 * 0.5);

    // Test a length of 100 (sqrt = 10, speed = 146.0).
    assert(std::abs(computeSpeedFromSkidMark(100.0) - 146.0) < 1e-9);

    // Very large length: 1e6 (sqrt = 1000, speed = 14600.0).
    assert(std::abs(computeSpeedFromSkidMark(1e6) - 14600.0) < 1e-6);

    // Very small length: 1e-4 (sqrt = 0.01, speed = 0.146).
    assert(std::abs(computeSpeedFromSkidMark(1e-4) - 0.146) < 1e-12);

    // Fraction non-perfect square: length 2.0.
    double expected = 14.6 * std::sqrt(2.0);
    assert(std::abs(computeSpeedFromSkidMark(2.0) - expected) < 1e-9);

    // Length 0.01 (sqrt = 0.1, speed = 1.46).
    assert(std::abs(computeSpeedFromSkidMark(0.01) - 1.46) < 1e-12);

    // Test that repeated calls are consistent (pure function).
    assert(computeSpeedFromSkidMark(9.0) == computeSpeedFromSkidMark(9.0));

    return 0;
}
