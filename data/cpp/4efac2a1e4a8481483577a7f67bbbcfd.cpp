// Write a C++ function that accepts a floating-point number (as `double` or `float`) and returns the cube of that number (the number multiplied by itself three times). The function must be pure, meaning it cannot read from standard input or print to standard output; all computation must happen in the function. The function should be named `computeCube` and take a single floating-point argument by value. The task is to implement this function so that it handles positive, negative, and zero values correctly, and also handles very small and very large magnitudes without overflow or precision loss beyond what the floating-point type naturally allows. You are not allowed to use `pow` from `<cmath>`; you must compute the cube via multiplication.

The main algorithm is straightforward: return `value * value * value`. Because multiplication is associative, this expression computes the cube correctly for all representable floating-point numbers. Edge cases include:  
- Zero (0³ = 0).  
- Negative numbers (e.g., -2³ = -8), which the multiplication handles naturally because of sign rules.  
- Very large values: `value * value * value` may overflow to `inf` if the result exceeds `FLT_MAX` or `DBL_MAX`; that is acceptable because the function cannot prevent it without changing the type.  
- Very small values: underflow to zero or subnormal numbers is handled by the hardware.  
- NaN and infinities: multiplication propagates them as expected (e.g., `inf * inf * inf` is `inf`).  
No loops or extra data structures are needed. Time complexity is O(1) and space complexity is O(1) (only a floating-point register to hold the intermediate and final results). The function is `const`-correct by taking the argument by value, which is fine for a primitive type.

#include <cstdint> // not used, but included for completeness of standalone header usage

// Compute the cube of a floating-point number.
double computeCube(const double value) {
    return value * value * value;
}

#include <cassert>
#include <cmath>
#include <limits>

int main() {
    // Basic cases
    assert(computeCube(0.0) == 0.0);
    assert(computeCube(1.0) == 1.0);
    assert(computeCube(-1.0) == -1.0);
    assert(computeCube(2.0) == 8.0);
    assert(computeCube(-2.0) == -8.0);
    assert(computeCube(0.5) == 0.125);
    assert(computeCube(-0.5) == -0.125);

    // Extremely small fractional values
    assert(computeCube(1e-10) == 1e-30);
    assert(computeCube(-1e-10) == -1e-30);

    // Large values that do not overflow
    assert(computeCube(1e3) == 1e9);
    assert(computeCube(-1e3) == -1e9);

    // Very large values that overflow to infinity (if allowed)
    const double large = 1e200;
    assert(std::isinf(computeCube(large)));
    assert(std::isinf(computeCube(-large)));

    // Very small values that underflow to zero (if allowed)
    const double tiny = 1e-200;
    assert(computeCube(tiny) == 0.0);
    assert(computeCube(-tiny) == 0.0);

    // Non-finite input
    assert(std::isnan(computeCube(std::numeric_limits<double>::quiet_NaN())));
}
