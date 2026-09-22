Write a C++ function named `isPalindromeNumber` that accepts a positive integer `n` and returns a boolean value indicating whether `n` is a palindrome (i.e., a number that reads the same forward and backward, such as 121, 12321, or 5). The function must handle single-digit numbers correctly (they are always palindromes) and must not rely on string conversion—only arithmetic operations are allowed. Ensure proper handling for numbers up to a typical 32-bit signed integer range (up to 2147483647), and note that reversing a number may overflow if not handled carefully; use a `long long` for intermediate reversal to avoid overflow.
int main() {
    // Single-digit numbers are palindromes.
    assert(isPalindromeNumber(5) == true);
    assert(isPalindromeNumber(9) == true);
    
    // Two-digit palindromes.
    assert(isPalindromeNumber(11) == true);
    assert(isPalindromeNumber(22) == true);
    
    // Non-palindromes.
    assert(isPalindromeNumber(12) == false);
    assert(isPalindromeNumber(123) == false);
    
    // Multi-digit palindromes.
    assert(isPalindromeNumber(121) == true);
    assert(isPalindromeNumber(12321) == true);
    assert(isPalindromeNumber(1001) == true);
    
    // Edge case: large number that reverses to within long long range.
    assert(isPalindromeNumber(2147447412) == true); // Reverse is 2147447412, fits in long long.
    assert(isPalindromeNumber(2147483647) == false); // Not a palindrome.
    
    // Zero (if allowed) would be true, but positive only; test for consistency.
    assert(isPalindromeNumber(0) == true); // Handled by loop not running, reversed = 0.
}
#include <cstdint>

// Returns true if the positive integer n is a palindrome, false otherwise.
// Uses only arithmetic operations and avoids overflow by using long long for reversal.
bool isPalindromeNumber(int n) {
    // Handle negative numbers (not expected but safe) and zero trivially.
    if (n < 0) {
        return false;
    }
    
    int num = n;
    long long reversed = 0;
    
    // Reverse the digits of num.
    while (num > 0) {
        int digit = num % 10;
        reversed = reversed * 10 + digit;
        num /= 10;
    }
    
    // Compare the reversed value with the original (both as long long for consistency).
    return (reversed == static_cast<long long>(n));
}
// The solution reverses the digits of the input number using arithmetic. The main algorithm: first, copy the input into a mutable variable (e.g., `num`), initialize a `long long` accumulator `reversed` to 0. While `num` is greater than 0, extract the last digit with `num % 10`, append it to `reversed` by `reversed = reversed * 10 + digit`, then remove the last digit from `num` with `num /= 10`. After the loop, compare the original `n` (cast to `long long` for safety) with `reversed`. Edge cases: zero is not considered a positive integer but if it were, the loop would run once and reverse to 0, so it would return true; however, the task specifies positive integers. Single-digit numbers (e.g., 5) result in `reversed = 5`, so they are correctly identified as palindromes. Numbers that could overflow an `int` when reversed (e.g., 2147447412) are handled by using `long long` for `reversed`. Time complexity is O(d) where d is the number of digits (at most 10 for 32-bit int), and space complexity is O(1).
