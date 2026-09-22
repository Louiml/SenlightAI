// Write a C++ function named `applyInterest` that takes an integer `principal` and an optional `double` interest rate (with a default value of `1.04` representing a 4% multiplier) and returns the resulting amount as an integer by truncating any fractional part. The function must use a default argument for the rate, and the rate must be a multiplier (e.g., `1.10` means 10% growth). The function should handle negative principal values gracefully (the result should be truncated toward zero). Additionally, write a second function named `safeApplyInterest` that takes the same parameters but returns the result as a `double` without truncation, and it must also validate that the rate is positive; if the rate is non-positive, it should return the original principal unchanged (as a double). The task is to implement both functions and ensure they are `const`-correct and use appropriate parameter passing (by value for simple types).

// The core solution involves simple multiplicative logic: `result = principal * rate`. For `applyInterest`, the result must be truncated to an integer using C-style casting or `static_cast<int>` (which truncates toward zero for negative values, matching typical integer conversion). For `safeApplyInterest`, we return the double result directly, but we first check if `rate > 0`; if not, return `static_cast<double>(principal)`. The default argument `= 1.04` must be placed in the function declaration (or definition) only once; since we define the function without a separate declaration, we put it in the definition. No special data structures are needed. Complexity: O(1) time and O(1) space for both functions. Edge cases: zero principal returns zero; negative principal with positive rate yields negative truncated value; non-positive rate in safe version returns original principal (no multiplication).

#include <cmath>  // For potential fabs if needed, but not required here

// Truncates the product to an integer (toward zero) using default rate.
int applyInterest(int principal, double rate = 1.04) {
    return static_cast<int>(principal * rate);
}

// Returns the product as a double, but validates that rate is positive.
double safeApplyInterest(int principal, double rate = 1.04) {
    if (rate <= 0.0) {
        return static_cast<double>(principal);
    }
    return static_cast<double>(principal) * rate;
}

#include <cassert>

int main() {
    // Test default rate (1.04)
    assert(applyInterest(1000) == 1040);          // 1000 * 1.04 = 1040
    assert(safeApplyInterest(1000) == 1040.0);    // exactly 1040.0

    // Test custom rate
    assert(applyInterest(1000, 1.10) == 1100);    // truncation
    assert(safeApplyInterest(1000, 1.10) == 1100.0);

    // Test fractional results truncation
    assert(applyInterest(100, 1.045) == 104);     // 104.5 truncates to 104
    assert(safeApplyInterest(100, 1.045) == 104.5);

    // Test negative principal
    assert(applyInterest(-100, 1.04) == -104);    // -104.0 truncates to -104
    assert(safeApplyInterest(-100, 1.04) == -104.0);

    // Test zero principal
    assert(applyInterest(0, 1.5) == 0);
    assert(safeApplyInterest(0, 1.5) == 0.0);

    // Test non-positive rate in safe version
    assert(safeApplyInterest(500, 0.0) == 500.0);
    assert(safeApplyInterest(500, -1.5) == 500.0);

    // Test non-positive rate with applyInterest (no validation, multiplies)
    assert(applyInterest(500, 0.0) == 0);
}
