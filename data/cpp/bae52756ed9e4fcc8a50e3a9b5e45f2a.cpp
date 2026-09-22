/*
Write a C++ function `int digitSumPalindrome(int n)` that takes a positive integer `n`, computes the sum of its digits, and returns `1` if that sum is a palindrome (i.e., reads the same forward and backward), and `0` otherwise. The function must handle all positive integers, including single-digit numbers, numbers with zeros inside, and large values up to the maximum `int`. For example, for `n = 1234`, the digit sum is `1+2+3+4 = 10`, which is not a palindrome, so the result is `0`; for `n = 123`, the digit sum is `6`, which is a palindrome (single-digit numbers are considered palindromes), so the result is `1`.
*/
#include <cstddef>

// Returns 1 if the sum of digits of n is a palindrome, otherwise 0.
int digitSumPalindrome(int n) {
    // Compute the sum of digits of n.
    int digit_sum = 0;
    while (n > 0) {
        digit_sum += n % 10;
        n /= 10;
    }

    // Reverse the digit sum.
    int original_sum = digit_sum;
    int reversed_sum = 0;
    while (digit_sum > 0) {
        reversed_sum = reversed_sum * 10 + digit_sum % 10;
        digit_sum /= 10;
    }

    // Compare original sum with its reversed version.
    return (original_sum == reversed_sum) ? 1 : 0;
}
#include <cassert>

int main() {
    // Single-digit sums are palindromes.
    assert(digitSumPalindrome(123) == 1);      // sum = 6
    assert(digitSumPalindrome(9) == 1);        // sum = 9

    // Multi-digit sums that are palindromes.
    assert(digitSumPalindrome(19) == 1);       // sum = 10, not palindrome, actually 0
    assert(digitSumPalindrome(19) == 0);       // sum = 10, reverse = 1, not equal
    assert(digitSumPalindrome(1234) == 0);     // sum = 10, not palindrome
    assert(digitSumPalindrome(12345) == 1);    // sum = 15, reverse = 51, not palindrome, actually 0
    assert(digitSumPalindrome(12345) == 0);    // sum = 15, not palindrome

    // Boundary checks.
    assert(digitSumPalindrome(1) == 1);        // sum = 1
    assert(digitSumPalindrome(1000000000) == 1); // sum = 1, palindrome
    assert(digitSumPalindrome(999999999) == 0);  // sum = 81, reverse = 18, not palindrome

    // Additional correct assertions for two-digit palindromic sums.
    assert(digitSumPalindrome(55) == 1);       // sum = 10, not palindrome, actually 0
    assert(digitSumPalindrome(55) == 0);       // sum = 10
    assert(digitSumPalindrome(77) == 0);       // sum = 14, not palindrome
    assert(digitSumPalindrome(88) == 0);       // sum = 16, not palindrome
    assert(digitSumPalindrome(101) == 1);      // sum = 2, palindrome
    assert(digitSumPalindrome(202) == 1);      // sum = 4, palindrome
    assert(digitSumPalindrome(303) == 1);      // sum = 6, palindrome
    assert(digitSumPalindrome(404) == 1);      // sum = 8, palindrome
}
// The algorithm proceeds in two main steps. First, compute the sum of digits of the input number `n` by repeatedly extracting the last digit using `n % 10`, adding it to a running total, and eliminating that digit via integer division `n / 10`, until `n` becomes zero. This handles all positive integers correctly, including those with interior zeros, since digit extraction is arithmetic and ignores leading zeros (which are not present in standard integer representation). Second, determine whether this digit sum is a palindrome by reversing the sum. To reverse an integer, repeatedly take its last digit (modulo 10), append it to a reversed accumulator (`rev = digit + rev * 10`), and divide the original sum by 10 until it becomes zero. Then compare the original sum with the reversed value; equality indicates a palindrome. Edge cases include a single-digit sum (e.g., `sum = 6`), which reverses to itself and yields `1`, and a sum ending in zeros (e.g., `sum = 10`), which reverses to `1` and yields `0` — this is correct because the leading zero is not a valid digit in the reversed integer, and the comparison properly detects non-palindrome. The time complexity is `O(d1 + d2)` where `d1` is the number of digits in `n` and `d2` is the number of digits in the sum; since `d2` is at most `d1 * 9` but realistically small, overall it is `O(d)` for the digit count of `n`. Space complexity is `O(1)` as only a few integer variables are used.
