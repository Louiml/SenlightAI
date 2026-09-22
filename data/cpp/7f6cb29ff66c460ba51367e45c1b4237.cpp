Write a C++ function named `safeFmod` that takes two floating-point parameters `x` and `y` and returns the floating-point remainder of the division `x / y`, exactly matching the behavior of the standard library `std::fmod` function (including handling of negative numbers and sign of the result). The function must handle all edge cases correctly: when `y` is zero, the result should be NaN (return `std::numeric_limits<float>::quiet_NaN()`); when `x` is infinite or `y` is infinite, the result should follow C++ `std::fmod` semantics (e.g., `fmod(inf, y)` returns NaN for finite `y`, `fmod(x, inf)` returns `x` for finite `x`). You may use standard library functions like `std::fmod`, `std::isfinite`, and `std::isnan` internally, but the task is to encapsulate this logic into a single reusable function that is safe against all inputs, including extreme values like `-0.0f`, `FLT_MAX`, and subnormal numbers. The function must be `const`-qualified where appropriate (though as a free function, this means it should not modify its parameters) and must be implemented with proper `float` precision.
// The solution revolves around directly delegating to the standard library's `fmod` function after explicitly checking for division by zero to ensure a NaN result (since `std::fmod` with zero denominator also returns NaN, but we want to guarantee it even if the implementation is non-compliant). The main algorithm: (1) If `y == 0.0f`, return `quiet_NaN`. (2) Otherwise, return `std::fmod(x, y)`. Edge cases: For `y` being `±0.0f`, the check catches both because `==` treats +0 and -0 as equal. For infinite `x` and finite `y`, `std::fmod` returns NaN, which we pass through. For finite `x` and infinite `y`, `std::fmod` returns `x` (preserving sign of zero if x is zero). Negative numbers work naturally because `fmod` preserves the sign of `x`. Subnormals and `FLT_MAX` are handled internally by `fmod`. Time complexity is O(1) because the function performs a constant number of operations. Space complexity is O(1) as no extra memory is allocated. The implementation is trivially correct because it wraps a tested standard library function while explicitly handling the zero–divisor case.
#include <cmath>
#include <limits>

// Compute the floating-point remainder of x/y, matching std::fmod behavior.
// Returns NaN when y is zero. Handles infinities, zeros, and subnormals.
float safeFmod(float x, float y) {
    // If y is zero (including -0.0f), the remainder is undefined -> NaN.
    if (y == 0.0f) {
        return std::numeric_limits<float>::quiet_NaN();
    }
    // Otherwise delegate to the standard library implementation.
    return std::fmod(x, y);
}
#include <cassert>
#include <cmath>
#include <limits>

// Ensure the previous function declaration is available (here repeated for standalone test).
float safeFmod(float x, float y) {
    if (y == 0.0f) {
        return std::numeric_limits<float>::quiet_NaN();
    }
    return std::fmod(x, y);
}

int main() {
    // Basic cases
    assert(safeFmod(5.0f, 2.0f) == 1.0f);
    assert(safeFmod(-5.0f, 2.0f) == -1.0f);
    assert(safeFmod(5.0f, -2.0f) == 1.0f);
    assert(safeFmod(-5.0f, -2.0f) == -1.0f);
    
    // Exact multiples
    assert(safeFmod(8.0f, 4.0f) == 0.0f);
    assert(safeFmod(9.0f, 3.0f) == 0.0f);
    
    // Zero divisor -> NaN
    assert(std::isnan(safeFmod(1.0f, 0.0f)));
    assert(std::isnan(safeFmod(-1.0f, -0.0f)));
    assert(std::isnan(safeFmod(0.0f, 0.0f)));
    
    // Infinite dividend
    assert(std::isnan(safeFmod(std::numeric_limits<float>::infinity(), 2.0f)));
    
    // Infinite divisor with finite dividend
    assert(safeFmod(3.5f, std::numeric_limits<float>::infinity()) == 3.5f);
    assert(safeFmod(-0.0f, std::numeric_limits<float>::infinity()) == -0.0f);
    
    // Zero dividend
    assert(safeFmod(0.0f, 5.0f) == 0.0f);
    assert(safeFmod(-0.0f, 5.0f) == -0.0f);
    
    // Large and subnormal values
    assert(safeFmod(std::numeric_limits<float>::max(), 1.0f) == 0.0f);
    assert(safeFmod(1.0f, std::numeric_limits<float>::denorm_min()) == 0.0f);
}
