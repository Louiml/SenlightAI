Write a C++ function `reverseDigits(int x)` that returns the integer formed by reversing the decimal digits of `x`, but returns `0` if the reversed value would overflow a signed 32-bit integer (`INT_MAX` = 2147483647, `INT_MIN` = -2147483648). The function must correctly handle positive numbers, negative numbers, and numbers ending in zero (e.g., `120` reverses to `21`). For example, `reverseDigits(123)` returns `321`, `reverseDigits(-456)` returns `-654`, `reverseDigits(2147483647)` returns `0` because reversing gives `7463847412` > `INT_MAX`, and `reverseDigits(-2147483648)` returns `0` because `-8463847412` < `INT_MIN`. Do not use any external libraries beyond standard headers.
// The solution processes the absolute value of the input digit by digit. For each digit, we build the result by multiplying the current result by 10 and adding the new digit. The key challenge is overflow detection: we must check before performing the multiplication and addition whether the new result would exceed the 32-bit signed range. Since we work with `long long` internally, we can safely compute the reversed value using 64-bit arithmetic, then check if the final result fits within `INT_MIN` to `INT_MAX`. If not, return `0`. For the sign, we preserve the original sign by checking if `x` is negative (i.e., `x < 0`). Edge cases: `x = 0` returns `0` (the loop never runs). Leading zeros in the reversed number disappear automatically because we build the result numerically. Time complexity is O(d) where d is the number of digits (at most 10 for 32-bit ints), so effectively O(1). Space complexity is O(1) since we only use a few scalar variables.
#include <climits>
#include <cstdlib> // for std::abs, but we use long long to handle INT_MIN safely

// Reverse the decimal digits of x, returning 0 if the result overflows a signed 32-bit int.
int reverseDigits(int x) {
    // Work with a long long to safely hold the absolute value of INT_MIN.
    long long value = x;
    if (x < 0) {
        value = -value; // value is now non-negative and fits in long long
    }

    long long reversed = 0;
    while (value != 0) {
        reversed = reversed * 10 + value % 10;
        value /= 10;
    }

    // Apply the sign and check for overflow.
    if (x < 0) {
        reversed = -reversed;
    }

    if (reversed > INT_MAX || reversed < INT_MIN) {
        return 0;
    }

    return static_cast<int>(reversed);
}
#include <cassert>
#include <climits>

int reverseDigits(int x); // declaration from the solution

int main() {
    assert(reverseDigits(123) == 321);
    assert(reverseDigits(-123) == -321);
    assert(reverseDigits(120) == 21);
    assert(reverseDigits(0) == 0);
    assert(reverseDigits(9) == 9);
    assert(reverseDigits(2147483647) == 0); // reverse would be 7463847412, overflow
    assert(reverseDigits(-2147483648) == 0); // reverse would be -8463847412, underflow
    assert(reverseDigits(1534236469) == 0); // reverse is 9646324351, overflow
    assert(reverseDigits(1000000003) == 0); // reverse is 3000000001, overflow
    assert(reverseDigits(123456789) == 987654321); // fits exactly in INT_MAX
    return 0;
}
