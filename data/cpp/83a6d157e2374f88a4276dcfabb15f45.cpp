/*
Write a C++ function that takes a non-empty string `x` representing a non-negative integer (which may be very large, up to thousands of digits) and returns its remainder when divided by 11, as an integer in the range `[0, 10]`. The function must not convert the entire string to a numeric type (e.g., `int` or `long long`), because the number may exceed the limits of standard integer types. Instead, compute the remainder using an efficient digit-wise method. The input string contains only decimal digits (`0`-`9`) and will not have leading zeros unless the number is zero itself. The function should be `const`-correct and work for any valid input length.
*/
#include <string>

// Compute the remainder when the large non-negative integer represented by
// the string x (decimal digits only) is divided by 11.
// Returns an integer in the range [0, 10].
int remainderMod11(const std::string& x) {
    long long alternatingSum = 0;  // Use long long to avoid overflow for very long inputs

    // Add digits at even indices, subtract digits at odd indices.
    for (std::size_t i = 0; i < x.size(); ++i) {
        int digit = x[i] - '0';
        if (i % 2 == 0) {
            alternatingSum += digit;
        } else {
            alternatingSum -= digit;
        }
    }

    // Reduce to non-negative remainder modulo 11.
    return static_cast<int>((alternatingSum % 11 + 11) % 11);
}
#include <cassert>
#include <string>
#include "solution.h"  // Assume the solution is in a header file

int main() {
    // Single-digit cases
    assert(remainderMod11("0") == 0);
    assert(remainderMod11("5") == 5);
    assert(remainderMod11("9") == 9);

    // Two-digit cases
    assert(remainderMod11("10") == 10);
    assert(remainderMod11("11") == 0);
    assert(remainderMod11("99") == 0);

    // Three-digit cases
    assert(remainderMod11("121") == 0);
    assert(remainderMod11("123") == 2);
    assert(remainderMod11("999") == 9);  // 999 / 11 = 90 remainder 9

    // Large numbers
    assert(remainderMod11("123456789012345678901234567890") == 4);
    assert(remainderMod11("10000000000000000000000000000000000000000000000000") == 1); // 10^50, remainder 1
    assert(remainderMod11("99999999999999999999999999999999999999999999999999") == 0); // 50 nines, divisible by 11

    // Alternating sum zero
    assert(remainderMod11("121212121212121212121212121212") == 0);
    assert(remainderMod11("1234567890") == 5); // Known: 1234567890 % 11 = 5

    return 0;
}
// The standard divisibility rule for 11 states that a number is divisible by 11 if the alternating sum of its digits (starting from the leftmost digit with a positive sign, or equivalently from the rightmost with a positive sign) is divisible by 11. The remainder modulo 11 can be computed by taking the alternating sum of digits and then reducing that sum modulo 11 to a non-negative remainder. Specifically, iterate over the string from left to right, adding the digit value to a running sum if its index is even, and subtracting if its index is odd. After processing all digits, compute `((sum % 11) + 11) % 11` to get the result in `[0, 10]`. Edge cases include: a single-digit number (e.g., "0" returns 0, "9" returns 9), very long strings (the sum fits in `int` because each digit is at most 9 and the maximum length is bounded by typical input constraints; even for thousands of digits, the sum is at most `9 * length` which fits in a 32-bit `int` for lengths up to several hundred million, but we use `long long` for safety). Time complexity is O(n) where n is the number of digits, and space complexity is O(1) auxiliary (excluding input storage). No special handling is needed for leading zeros beyond the rule, as the alternating sum directly gives the correct remainder.
