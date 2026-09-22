/*
Write a C++ function named `absoluteValueLong` that takes a single `long` integer argument and returns its absolute value as a `long`. The function must not use the standard library `std::abs`, `labs`, or any built-in absolute value functions—you must implement the logic manually using a conditional expression or an if-else statement. Consider edge cases such as zero (which returns 0) and negative values (including the most negative possible `long` value, for which the mathematical absolute value would overflow; in that case, you may return the value unchanged as is common in two's-complement systems, but ensure your implementation does not crash or invoke undefined behavior). The function should be marked `constexpr` if possible to allow compile-time evaluation.
*/

#include <climits>  // for LONG_MIN

// Returns the absolute value of a long integer without using standard library abs().
// For LONG_MIN (the most negative value), the true absolute value is unrepresentable;
// in that case the function returns the input unchanged (common two's-complement behavior).
constexpr long absoluteValueLong(long value) noexcept {
    // If value is negative, negate it; otherwise return value. 
    // Special-case LONG_MIN to avoid signed overflow (UB) during negation.
    return (value < 0) ? ((value == LONG_MIN) ? LONG_MIN : -value) : value;
}

int main() {
    // Basic positive, negative, and zero cases.
    assert(absoluteValueLong(0L) == 0L);
    assert(absoluteValueLong(42L) == 42L);
    assert(absoluteValueLong(-42L) == 42L);

    // Large positive and negative values.
    assert(absoluteValueLong(1000000000L) == 1000000000L);
    assert(absoluteValueLong(-1000000000L) == 1000000000L);

    // Boundary values: LONG_MAX and LONG_MIN.
    assert(absoluteValueLong(LONG_MAX) == LONG_MAX);
    // LONG_MIN's absolute value cannot be represented; function returns LONG_MIN unchanged.
    assert(absoluteValueLong(LONG_MIN) == LONG_MIN);

    // Edge: negative one.
    assert(absoluteValueLong(-1L) == 1L);
}

// The core algorithm is straightforward: check if the input value is non-negative; if so, return it as-is; if negative, return the negation (`-value`). The main edge case is the smallest representable `long` (e.g., `LONG_MIN` on a two's-complement system), where negating it would overflow the signed type, causing undefined behavior. A robust approach is to avoid negation when `value == LONG_MIN` and return `value` unchanged, since the mathematical absolute value cannot be represented anyway. Alternatively, one could use unsigned arithmetic for the negation (`-static_cast<unsigned long>(value)`), which wraps safely, but for simplicity and to match common library behavior (e.g., `labs(LONG_MIN)` returns `LONG_MIN` on many platforms), returning the original value is acceptable. The time complexity is O(1) and space complexity is O(1). Since the operation is simple and pure, marking it `constexpr` allows compile-time evaluation, improving flexibility.
