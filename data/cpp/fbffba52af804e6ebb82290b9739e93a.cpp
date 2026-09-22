Write a C++ function named `lastDigitAbsoluteValue` that takes a single integer `n` (which may be negative, zero, or positive) and returns an `int` representing the absolute value (non-negative magnitude) of its last decimal digit. For example, for `n = 1234`, the function should return `4`; for `n = -987` it should return `7`; for `n = 0` it should return `0`; and for `n = -10` it should return `0`. The function must not use any standard library mathematical functions beyond `std::abs` (or manual handling) and must be correct for all valid `int` values within the C++ `int` range.
The core idea is to extract the last digit of an integer using the modulo operator (`%`). In C++, for negative numbers, the result of `n % 10` is negative (implementation-defined until C++11, but since C++11 it is guaranteed to have the sign of `n`). To obtain the absolute value of that last digit, we can use `std::abs(n % 10)` (from `<cstdlib>` or `<cmath>`). An alternative is to take the absolute value of `n` first and then modulo, but this could cause issues with `INT_MIN` (because `-INT_MIN` overflows), so it is safer to apply `std::abs` to the remainder rather than to `n`. Edge cases include zero (`0 % 10 == 0`), negative numbers (e.g., `-123 % 10 == -3`, absolute value 3), and numbers like `-10` (last digit is 0). The function uses `const` correctly because it does not modify the input. Time complexity is O(1) and space complexity is O(1).
#include <cstdlib>  // for std::abs

// Returns the absolute value of the last decimal digit of the integer n.
// Works for positive, negative, and zero inputs, including INT_MIN.
int lastDigitAbsoluteValue(const int n) {
    // Compute the last digit (possibly negative for negative n) and take its absolute value.
    // Using std::abs on the remainder avoids overflow that would occur with abs(n) for INT_MIN.
    return std::abs(n % 10);
}
#include <cassert>
#include <climits>  // for INT_MIN

// Function under test (declared here for the test, normally in a separate header/source)
int lastDigitAbsoluteValue(const int n) {
    return std::abs(n % 10);
}

int main() {
    // Basic positive numbers
    assert(lastDigitAbsoluteValue(0) == 0);
    assert(lastDigitAbsoluteValue(5) == 5);
    assert(lastDigitAbsoluteValue(1234) == 4);
    assert(lastDigitAbsoluteValue(100) == 0);

    // Negative numbers
    assert(lastDigitAbsoluteValue(-7) == 7);
    assert(lastDigitAbsoluteValue(-987) == 7);
    assert(lastDigitAbsoluteValue(-10) == 0);
    assert(lastDigitAbsoluteValue(-123456) == 6);

    // Edge cases: largest and smallest int values
    assert(lastDigitAbsoluteValue(INT_MAX) == 7);  // 2147483647 -> last digit 7
    assert(lastDigitAbsoluteValue(INT_MIN) == 8);  // -2147483648 -> last digit 8 (abs)

    // Additional random-like checks
    assert(lastDigitAbsoluteValue(42) == 2);
    assert(lastDigitAbsoluteValue(-42) == 2);
    assert(lastDigitAbsoluteValue(999) == 9);
    assert(lastDigitAbsoluteValue(-1) == 1);

    return 0;
}
