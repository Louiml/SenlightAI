// Write a C++ function named `smoothstepClamp` that takes a single `float` argument and returns a `float`. The function must implement a clamped smoothstep mapping: take the absolute value of the input, then apply the standard 3rd-order smoothstep formula `y = (2x³ - 3x² + 1)` for inputs whose absolute value is less than 1, and return `0.0f` for inputs whose absolute value is greater than or equal to 1. This is a common easing function used in graphics and animation to smoothly fade a value to zero at the boundaries. The function must be `const`-correct, include only necessary headers, and be defined as a free function.
// The main algorithm is straightforward: first compute the absolute value of the input using `std::abs`, which works for integral types but here is used with `float` and requires `<cmath>`. Then compare the absolute value against `1.0f`. For the case where the absolute value is less than 1, evaluate the polynomial `(2*t - 3)*t*t + 1.0f`, which is the expanded form of `2t³ - 3t² + 1`. For the case where the absolute value is greater than or equal to 1, return `0.0f` exactly. Edge cases:  
// - Input `0.0f` → absolute value 0 → smoothstep(0)=1.0f.  
// - Input `±1.0f` → absolute value 1 → returns 0.0f (the condition is `<` not `<=`).  
// - Input `±0.5f` → absolute value 0.5 → smoothstep(0.5)=0.5.  
// - Negative inputs are handled by taking absolute value first, making the function even.  
// - Very large inputs (e.g., `1e30f`) simply return 0.0f.  
// - Special values like `NaN`: `std::abs(NaN)` yields NaN, and the comparison `NaN < 1.0f` is false, so it returns 0.0f.  
//
// Time complexity is O(1) and space complexity is O(1) — no loops, no dynamic memory.
#include <cmath>

// Return a clamped smoothstep of the absolute value of the input.
// For |input| < 1: returns 1 - 3*t^2 + 2*t^3, where t = |input|.
// For |input| >= 1: returns 0.0f.
float smoothstepClamp(float value) {
    const float t = std::abs(value);
    if (t < 1.0f) {
        return (2.0f * t - 3.0f) * t * t + 1.0f;
    }
    return 0.0f;
}
#include <cassert>
#include <cmath>

int main() {
    // When |x| >= 1, result is exactly 0.0f
    assert(smoothstepClamp(1.0f) == 0.0f);
    assert(smoothstepClamp(-1.0f) == 0.0f);
    assert(smoothstepClamp(5.0f) == 0.0f);
    assert(smoothstepClamp(-100.0f) == 0.0f);

    // At x = 0, result is 1.0f
    assert(smoothstepClamp(0.0f) == 1.0f);

    // Symmetry: positive and negative identical
    assert(smoothstepClamp(0.25f) == smoothstepClamp(-0.25f));

    // Exact midpoint: smoothstep(0.5) = 0.5
    assert(smoothstepClamp(0.5f) == 0.5f);
}
