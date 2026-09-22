// Write a C++ function named `isPalindromeNumber` that takes an integer `x` and returns `true` if `x` is a palindrome (reads the same forward and backward), and `false` otherwise. The function must handle negative numbers correctly (they are never palindromes), avoid integer overflow during reversal by using a wider type, and work for any 32-bit integer input. Do not use string conversion or any standard library reverse algorithms; implement the reversal arithmetically.

The main idea is to reverse the digits of the number using arithmetic operations and compare the reversed result with the original value. First, immediately return `false` for any negative input because the minus sign cannot be mirrored in a numeric palindrome. For non‑negative numbers, repeatedly extract the last digit using `%10`, append it to a running reversed value by multiplying the current reversed by 10 and adding the digit, and then discard that last digit via integer division by 10. Continue until the original number is reduced to zero. To prevent overflow when reversing a number like `2147483647` (which would become `7463847412` exceeding 32‑bit range), store the reversed value in a `long long` (or `long int` on platforms where it is 64‑bit) rather than in `int`. Finally, compare the reversed value with the original `x`; if they are equal, `x` is a palindrome. Edge cases include `0` (which is a palindrome, and the loop correctly does nothing, then compares `0 == 0`), single‑digit positive numbers (always palindromes), and numbers ending with zero (e.g., `10` reverses to `1`, which does not equal `10`). The time complexity is \(O(d)\) where \(d\) is the number of digits (at most 10 for 32‑bit integers), and space complexity is \(O(1)\) additional memory.

#include <cstdint>

// Returns true if the given integer is a palindrome, false otherwise.
// Negative numbers are never palindromes.
bool isPalindromeNumber(int x) {
    if (x < 0) {
        return false;
    }

    // Use a larger type to avoid overflow during reversal.
    long long reversed = 0LL;
    long long original = static_cast<long long>(x);

    while (original > 0) {
        reversed = reversed * 10LL + (original % 10LL);
        original /= 10LL;
    }

    return reversed == static_cast<long long>(x);
}

#include <cassert>

int main() {
    // Positive palindromes
    assert(isPalindromeNumber(121) == true);
    assert(isPalindromeNumber(12321) == true);
    // Single digit is palindrome
    assert(isPalindromeNumber(7) == true);
    // Zero is palindrome
    assert(isPalindromeNumber(0) == true);
    // Negative numbers are never palindromes
    assert(isPalindromeNumber(-121) == false);
    assert(isPalindromeNumber(-1) == false);
    // Non-palindromes
    assert(isPalindromeNumber(123) == false);
    assert(isPalindromeNumber(10) == false);
    // Edge cases with large values
    assert(isPalindromeNumber(2147447412) == true);   // within 32-bit and palindrome
    assert(isPalindromeNumber(2147483647) == false);  // max int, not palindrome
    return 0;
}
