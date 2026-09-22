Write a C++ function named `isIntegerPalindrome` that accepts a single integer `x` and returns a boolean indicating whether `x` is a palindrome (reads the same forward and backward). The function must correctly handle negative numbers (which are never palindromes), numbers ending in zero (e.g., 10, -10, 100 are not palindromes unless the number is 0 itself), and large positive integers up to the range of a 32-bit signed `int`. Do not convert the number to a string; solve it using arithmetic digit reversal. Your solution should use `const` correctness where appropriate and must not modify the input parameter.
// The core idea is to reverse the digits of the number and compare the reversed value with the original. The naive reverse-all approach works but risks integer overflow for large inputs when using a 32-bit `int` accumulator; therefore, the reference solution uses the half-reversal technique to avoid overflow.  
// - If `x` is negative, return `false` immediately.  
// - If `x` is non-zero and ends with a zero (i.e., `x % 10 == 0`), it cannot be a palindrome (e.g., 10, 20, 100) except for 0 itself.  
// - For the remaining non-negative numbers, repeatedly take the last digit of `x` and append it to `reversed`, while removing that last digit from `x`. Continue only while `x > reversed` (i.e., we have processed at most half the digits).  
// - After the loop, if the original number has an even number of digits, `x` and `reversed` must be equal; if odd, we need to drop the middle digit from `reversed` (divide by 10) and then compare.  
// This approach ensures `reversed` never exceeds the original number’s magnitude, so no overflow occurs.  
// Time complexity is O(d) where d is the number of digits (worst case O(log₁₀ x)). Space complexity is O(1) auxiliary, as only a few scalar variables are used.
#include <cstdbool>

// Returns true if the given integer is a palindrome, false otherwise.
// Uses arithmetic half-reversal to avoid overflow and handles edge cases.
bool isIntegerPalindrome(int x) {
    // Negative numbers cannot be palindromes.
    if (x < 0) {
        return false;
    }
    // Any non-zero number ending in zero cannot be a palindrome.
    if (x != 0 && x % 10 == 0) {
        return false;
    }

    int reversed = 0;
    // Reverse only half the digits to avoid overflow.
    while (x > reversed) {
        reversed = reversed * 10 + (x % 10);
        x /= 10;
    }

    // For even digit lengths: x == reversed; for odd: x == reversed / 10.
    return (x == reversed) || (x == reversed / 10);
}
int main() {
    // Basic palindromes
    assert(isIntegerPalindrome(121) == true);
    assert(isIntegerPalindrome(1221) == true);
    // Single digit (including zero)
    assert(isIntegerPalindrome(7) == true);
    assert(isIntegerPalindrome(0) == true);
    // Negative numbers
    assert(isIntegerPalindrome(-121) == false);
    // Non-palindromes
    assert(isIntegerPalindrome(123) == false);
    assert(isIntegerPalindrome(10) == false);
    // Numbers ending in zero (other than 0)
    assert(isIntegerPalindrome(100) == false);
    assert(isIntegerPalindrome(1010) == false);
    // Large input that would overflow naive reversal
    assert(isIntegerPalindrome(2147447412) == true);  // 2,147,447,412 is a palindrome
    assert(isIntegerPalindrome(2147483647) == false); // max int, not palindrome
    return 0;
}
