// Write a C++ function named `reverseInteger` that takes a 32-bit signed integer `x` and returns the integer formed by reversing its decimal digits. If reversing the digits causes the result to overflow the 32-bit signed integer range [−2^31, 2^31 − 1], the function must return 0. The function must not use any 64-bit integer types or any floating-point types, and must handle negative numbers correctly (e.g., -123 becomes -321). Leading zeros in the reversed result are dropped naturally by integer arithmetic (e.g., 120 becomes 21). The function should be robust for all possible inputs in the range of `int`.

The standard approach is to repeatedly extract the last digit of `x` using `x % 10` and append it to a result variable `res` by multiplying the current `res` by 10 and adding the digit, then divide `x` by 10 (using integer division, which truncates toward zero, correctly handling negative numbers). The key challenge is overflow detection. Since we cannot use 64-bit types, we must check before performing `res * 10 + digit` whether the operation would exceed `INT_MAX` or fall below `INT_MIN`. The condition `res > INT_MAX / 10` or `res < INT_MIN / 10` catches cases where multiplying by 10 would overflow. Additionally, we must consider the case where `res == INT_MAX / 10` (or `INT_MIN / 10`) and adding the digit still overflows; however, for `INT_MAX / 10 = 214748364` and `INT_MIN / 10 = -214748364`, the only problematic digit would be 8 or 9 for positive, but the digit is always `x % 10` (0-9) and we can also check `res == INT_MAX / 10 && digit > 7` and `res == INT_MIN / 10 && digit < -8` (since `INT_MIN = -2147483648`). A simpler and still correct approach used in many implementations is to only check the division bound and then perform the operation, but that misses the edge case where `res` equals the bound and the digit pushes it over. For correctness, we'll include both checks. The algorithm runs in O(d) time where d is the number of decimal digits (at most 10 for 32-bit integers), and uses O(1) auxiliary space. Edge cases include `x = 0` (returns 0), `x = INT_MIN` (reversing gives 0 because 8463847412 overflows), and `x` such as 1000000009 (reverse would exceed INT_MAX).

#include <climits>

// Reverse the decimal digits of a 32-bit signed integer.
// Returns 0 if the reversed value overflows the 32-bit signed range.
int reverseInteger(int x) {
    int res = 0;
    while (x != 0) {
        int digit = x % 10;
        // Check for overflow before performing res * 10 + digit.
        if (res > INT_MAX / 10 || (res == INT_MAX / 10 && digit > 7)) {
            return 0;
        }
        if (res < INT_MIN / 10 || (res == INT_MIN / 10 && digit < -8)) {
            return 0;
        }
        res = res * 10 + digit;
        x /= 10;
    }
    return res;
}

#include <cassert>
#include <climits>

// Declaration of the function to test (already defined above).
int reverseInteger(int x);

int main() {
    // Basic positive and negative cases.
    assert(reverseInteger(123) == 321);
    assert(reverseInteger(-123) == -321);
    assert(reverseInteger(120) == 21);
    assert(reverseInteger(0) == 0);
    assert(reverseInteger(1) == 1);
    assert(reverseInteger(-1) == -1);

    // Edge cases involving overflow.
    assert(reverseInteger(INT_MAX) == 0);           // 2147483647 reversed would overflow.
    assert(reverseInteger(INT_MIN) == 0);           // -2147483648 reversed would overflow.
    assert(reverseInteger(1463847412) == 2147483641); // Safe, just below INT_MAX.
    assert(reverseInteger(1563847412) == 0);        // Would exceed INT_MAX.
    assert(reverseInteger(-1463847412) == -2147483641); // Safe, just above INT_MIN.
    assert(reverseInteger(-1563847412) == 0);       // Would go below INT_MIN.

    // Cases with trailing zeros.
    assert(reverseInteger(100) == 1);
    assert(reverseInteger(-1000) == -1);

    // Large but valid reversal staying in range.
    assert(reverseInteger(2147483641) == 1463847412);
    assert(reverseInteger(-2147483641) == -1463847412);

    return 0;
}
