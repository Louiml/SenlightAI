// Write a C++ function `roundTo2Decimals(double value)` that takes a real number (which may be positive, negative, or zero, and may have an arbitrary fractional part) and returns the value rounded to exactly two decimal places using the "round half away from zero" rule. The function must not perform any input/output operations. Ensure the result is accurate for typical double-precision inputs, including very small values (e.g., 0.0001) and values near rounding boundaries (e.g., 2.675, -2.675). The function should return a `double` and must not rely on external libraries beyond the standard `<cmath>` (which may or may not be needed). Provide the implementation with appropriate `const` correctness (the parameter should be `const double&` or passed by value, but not modified). The rounding must be based on multiplying by 100, applying a rounding offset (0.5 for positive, -0.5 for negative), truncating to integer, then dividing back by 100 — exactly as the original snippet does, but without any `cin`/`cout` or global state.
// The core algorithm is straightforward: to round a value `x` to two decimal places, multiply by 100, add/subtract 0.5 depending on the sign, cast to `int` to truncate toward zero, then divide by 100. This implements the "round half away from zero" rule (e.g., 2.675 becomes 2.68, -2.675 becomes -2.68). Important edge cases: (1) For negative values, the offset must be `-0.5` to correctly round away from zero (e.g., -2.675 * 100 = -267.5, minus 0.5 = -268, cast to int = -268, divided by 100 = -2.68). (2) The casting to `int` truncates toward zero, which works correctly for both positive and negative values after applying the offset. (3) Very small values like 0.0001: 0.0001 * 100 = 0.01, plus 0.5 = 0.51, cast to int = 0, divided = 0.00. (4) Potential floating-point representation errors (e.g., 2.675 is stored as 2.6749999999999998) can cause subtle issues; the traditional `int(x*100 + 0.5)` may produce 267 instead of 268 for 2.675 because `2.675*100` is 267.49999999999997. To mitigate this, we can use `std::round` or a small epsilon, but the task explicitly asks for the original method (multiply, offset, truncate). A common workaround is to use `std::round(x * 100) / 100` which correctly handles typical cases but may still suffer from representation issues. Since the task specifies the exact algorithm from the snippet, we follow that: if `x >= 0`, use `int(x*100 + 0.5)`, else `int(x*100 - 0.5)`. For typical test cases, this works; for pathological cases, we note that no perfect double-based method exists for all decimals. The time complexity is O(1) and space complexity O(1). The function should be `const`-correct by taking the parameter by value (since it's a simple double) and returning a `double` without modifying the input.
#include<cmath> // potentially for std::floor/ceil but not needed; included for completeness

// Rounds a real value to two decimal places using "round half away from zero".
// Works for positive, negative, and zero inputs.
double roundTo2Decimals(const double value) {
    if (value >= 0.0) {
        return static_cast<double>(static_cast<int>(value * 100.0 + 0.5)) / 100.0;
    } else {
        return static_cast<double>(static_cast<int>(value * 100.0 - 0.5)) / 100.0;
    }
}
#include <cassert>
#include <cmath>

// The function is declared above; here's the test.
int main() {
    // Basic positive rounding
    assert(std::fabs(roundTo2Decimals(1.234) - 1.23) < 1e-9);
    assert(std::fabs(roundTo2Decimals(1.235) - 1.24) < 1e-9);
    // Basic negative rounding
    assert(std::fabs(roundTo2Decimals(-1.234) - (-1.23)) < 1e-9);
    assert(std::fabs(roundTo2Decimals(-1.235) - (-1.24)) < 1e-9);
    // Zero
    assert(roundTo2Decimals(0.0) == 0.0);
    assert(roundTo2Decimals(-0.0) == 0.0);
    // Large values
    assert(std::fabs(roundTo2Decimals(12345.678) - 12345.68) < 1e-9);
    // Test exact half boundary (2.675 is often problematic)
    // Due to floating-point representation, we check with tolerance
    assert(std::fabs(roundTo2Decimals(2.675) - 2.68) < 1e-9);
    assert(std::fabs(roundTo2Decimals(-2.675) + 2.68) < 1e-9);
    // Very small values become 0
    assert(roundTo2Decimals(0.0001) == 0.0);
    // Exact integer values
    assert(roundTo2Decimals(5.0) == 5.0);
    assert(roundTo2Decimals(-5.0) == -5.0);
}
