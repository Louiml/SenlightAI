// Write a C++ function named `roundToInt` that takes a single `double` parameter and returns the integer obtained by truncating the fractional part (i.e., discarding everything after the decimal point without rounding). The function must handle positive, negative, and zero values correctly, and must use only the built-in cast to `int` without any external libraries or helper functions. The function should be declared with `const` correctness where applicable and must be placed in a namespace-free global scope for easy testing. Ensure the function works for values within the valid range of `int` (no need to handle overflow or NaN/Infinity).

#include <cassert>

int main() {
    // Positive values: truncate, not round
    assert(roundToInt(3.99) == 3);
    assert(roundToInt(3.01) == 3);
    assert(roundToInt(0.5) == 0);      // truncates down, not rounds up
    // Negative values: truncate toward zero
    assert(roundToInt(-3.99) == -3);
    assert(roundToInt(-3.01) == -3);
    assert(roundToInt(-0.5) == 0);     // truncates up (toward zero)
    // Zero and near-zero
    assert(roundToInt(0.0) == 0);
    assert(roundToInt(-0.0) == 0);     // negative zero becomes 0
    assert(roundToInt(0.0001) == 0);
    // Large values within int range
    assert(roundToInt(2147483647.0) == 2147483647);
    assert(roundToInt(-2147483648.0) == -2147483648);
    return 0;
}

// Convert a double to int by truncating the fractional part (toward zero).
int roundToInt(const double value) {
    return static_cast<int>(value);  // C++ static_cast truncates toward zero
}

// The core task is straightforward: truncation toward zero is exactly what a direct C-style cast from `double` to `int` performs. For positive numbers like `3.99`, the cast yields `3`; for negative numbers like `-3.99`, the cast yields `-3` (truncation toward zero, not floor). The only subtlety is that the function parameter should be passed by value (since it's a primitive), and the return type is `int`. Edge cases include `0.0` → `0`, very small values like `0.0001` → `0`, and negative zero `-0.0` → `0` (which the cast handles naturally). There is no need for rounding, floor, or ceil—just a direct cast. Time complexity is \(O(1)\) and space complexity is \(O(1)\), as the operation is a single arithmetic conversion with no loops or allocations.
