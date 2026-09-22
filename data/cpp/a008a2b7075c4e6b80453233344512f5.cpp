/*
Write a C++ function that takes three integers `a`, `b`, and `c` as parameters and returns a string describing the result of the expression `a + b` compared to `c`. The function should return `"sum equals c"` if `a + b == c`, `"sum exceeds c"` if `a + b > c`, or `"sum is less than c"` if `a + b < c`. Handle all integer ranges including negative values, extremes like `INT_MAX` and `INT_MIN`, and ensure no overflow occurs when computing `a + b`. The function must be `const`-correct and use only standard library facilities.
*/
#include <string>

// Compares a + b to c without overflow using a wider type.
std::string compareSumToValue(int a, int b, int c) {
    long long sum = static_cast<long long>(a) + static_cast<long long>(b);
    long long target = static_cast<long long>(c);
    if (sum == target) {
        return "sum equals c";
    } else if (sum > target) {
        return "sum exceeds c";
    } else {
        return "sum is less than c";
    }
}
#include <cassert>
#include <climits>
#include <string>

// The solution function is declared above (or included here).

int main() {
    // Basic equality
    assert(compareSumToValue(2, 3, 5) == "sum equals c");
    // Sum greater
    assert(compareSumToValue(2, 3, 4) == "sum exceeds c");
    // Sum less
    assert(compareSumToValue(2, 3, 6) == "sum is less than c");
    // Negative values
    assert(compareSumToValue(-5, -3, -8) == "sum equals c");
    assert(compareSumToValue(-5, -3, -7) == "sum is less than c");
    assert(compareSumToValue(-5, -3, -9) == "sum exceeds c");
    // Extreme values (INT_MAX + 0, INT_MIN + 0, and their sums)
    assert(compareSumToValue(INT_MAX, 0, INT_MAX) == "sum equals c");
    assert(compareSumToValue(INT_MIN, 0, INT_MIN) == "sum equals c");
    assert(compareSumToValue(INT_MAX, 1, INT_MIN) == "sum exceeds c"); // INT_MAX+1 = 2147483648 > INT_MIN
    assert(compareSumToValue(INT_MIN, -1, INT_MAX) == "sum is less than c"); // INT_MIN-1 = -2147483649 < INT_MAX
    // No overflow occurs in computation.
    return 0;
}
// The core challenge is to compare `a + b` with `c` without risking integer overflow. A direct addition like `a + b` can overflow for large positive or negative values (e.g., `INT_MAX` + 1). To avoid overflow, we can use a safe comparison technique using the `__int128` type (available in GCC/Clang) or perform the addition with care using long long. Since the problem does not restrict the use of compiler extensions, using `long long` (which is at least 64 bits) is sufficient for all 32-bit `int` inputs—the sum of two `int` values fits in a 64-bit signed integer. We cast `a` and `b` to `long long`, sum them, then compare with `c` (also cast to `long long`). This yields a robust solution. Time complexity is O(1) and space is O(1). Edge cases include negative values, zero, and extreme limits—all handled by using a wider type.
