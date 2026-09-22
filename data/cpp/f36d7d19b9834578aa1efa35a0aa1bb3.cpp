// Write a C++ function named `safeDivide` that takes two integer parameters, `dividend` and `divisor`, and returns their integer quotient. The function must handle the case where the divisor is zero by returning a special sentinel value of `0` (since the mathematical result of division by zero is undefined and the original snippet uses integer division). Assume both inputs are integers within the range of a standard 32-bit `int`, and the division should be performed using C++ integer division (truncation toward zero). The function must be `const`-correct and use `const` reference parameters or values with `const` qualifiers in its signature. Do not include `main` or any input/output logic; only the function.

// The algorithm is straightforward: check if the divisor equals zero. If it does, return `0` to indicate an error/undefined result; otherwise, return the result of integer division `dividend / divisor`. Integer division in C++ truncates toward zero for positive and negative operands, so no special handling is needed for sign combinations. Edge cases include: divisor zero (must return 0), negative dividend/divisor (normal integer division applies), and extremes like `INT_MIN / -1` which could overflow in some implementations (since the result would be 2147483648, which is out of `int` range for a 32-bit system). To be safe, we can document that behavior is undefined for that case, or add a guard: if `dividend == INT_MIN && divisor == -1`, return `INT_MAX` as a reasonable clamp (though this is a design choice; the simplest robust approach is to let the default behavior occur, but for a teaching task we can handle it). For complexity: The function uses only constant-time arithmetic and a single comparison, so time complexity is \(O(1)\) and space complexity is \(O(1)\).

#include <climits> // for INT_MIN, INT_MAX

// Safely compute integer division, returning 0 on division by zero.
// Handles the overflow case INT_MIN / -1 by returning INT_MAX.
int safeDivide(const int dividend, const int divisor) {
    // Division by zero: return sentinel 0.
    if (divisor == 0) {
        return 0;
    }
    // Handle potential overflow when dividing the most negative int by -1.
    if (dividend == INT_MIN && divisor == -1) {
        return INT_MAX;
    }
    // Normal integer division (truncates toward zero).
    return dividend / divisor;
}

#include <cassert>
#include <climits>

// Function declaration (or include the solution header)
int safeDivide(const int dividend, const int divisor);

int main() {
    // Basic positive division
    assert(safeDivide(10, 3) == 3);
    // Basic negative division
    assert(safeDivide(-10, 3) == -3);
    assert(safeDivide(10, -3) == -3);
    assert(safeDivide(-10, -3) == 3);
    // Zero dividend
    assert(safeDivide(0, 5) == 0);
    // Division by zero returns sentinel 0
    assert(safeDivide(7, 0) == 0);
    assert(safeDivide(-7, 0) == 0);
    // Edge case: INT_MIN / -1 overflow
    assert(safeDivide(INT_MIN, -1) == INT_MAX);
    // Exact division
    assert(safeDivide(100, 4) == 25);
    // Large divisor
    assert(safeDivide(1, 1000) == 0);
    return 0;
}
