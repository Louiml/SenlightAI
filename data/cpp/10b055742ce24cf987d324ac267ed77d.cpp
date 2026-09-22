// Write a C++ function named `isPalindromeNumber` that accepts an integer `x` and returns a boolean value indicating whether `x` is a palindrome. A palindrome reads the same forward and backward (e.g., 121, -121 is not, 10 is not). The function must handle negative numbers correctly (they are never palindromes), avoid integer overflow during reversal, and consider only the numeric value (ignoring leading zeros, which are irrelevant in integer representation). Provide a robust solution that works for all 32-bit signed integers.

// The simplest correct approach is to reverse the integer and compare it with the original. However, reversing a large integer like 2147483647 could overflow a 32-bit `int`, so we use a `long long` (64-bit) for the reversed value and a copy of the original. The algorithm is: if `x` is negative, return `false` immediately. Otherwise, initialize `reversed = 0` and `original = x`. While `x > 0`, extract the last digit via `x % 10`, append it to `reversed` by `reversed = reversed * 10 + digit`, then remove the last digit from `x` via `x /= 10`. After the loop, compare `original` with `reversed`. This handles all positive numbers including 0 (which is trivially a palindrome). Edge cases: negative numbers return false; single-digit numbers are palindromes; numbers ending in zero (like 10) are not palindromes because the reversed would have leading zero, but since we use integer arithmetic, 10 reversed becomes 1, which does not equal 10. Complexity: O(d) time where d is the number of digits (at most 10 for 32-bit ints), and O(1) auxiliary space (excluding the temporary copy).

#include <cstdint>

// Returns true if the given integer is a palindrome, false otherwise.
// Handles negative numbers and avoids overflow by using 64-bit arithmetic.
bool isPalindromeNumber(int x) {
    if (x < 0) {
        return false;
    }

    long long original = static_cast<long long>(x);
    long long reversed = 0;
    int temp = x;

    while (temp > 0) {
        int digit = temp % 10;
        reversed = reversed * 10 + digit;
        temp /= 10;
    }

    return original == reversed;
}

#include <cassert>

int main() {
    // Positive palindromes
    assert(isPalindromeNumber(121) == true);
    assert(isPalindromeNumber(1221) == true);
    assert(isPalindromeNumber(0) == true);
    assert(isPalindromeNumber(7) == true);

    // Negative numbers are never palindromes
    assert(isPalindromeNumber(-121) == false);
    assert(isPalindromeNumber(-1) == false);

    // Non-palindromes
    assert(isPalindromeNumber(10) == false);
    assert(isPalindromeNumber(12321) == false);
    assert(isPalindromeNumber(1001) == true);

    // Large numbers that might overflow if using int
    assert(isPalindromeNumber(2147447412) == true);
    assert(isPalindromeNumber(2147483647) == false);

    // Edge case: maximum 32-bit int, not a palindrome
    assert(isPalindromeNumber(2147483647) == false);
}
