// Write a C++ function named `isPalindromeInteger` that takes a single integer parameter (which may be negative, zero, or positive) and returns a boolean indicating whether the integer reads the same forward and backward (i.e., is a numeric palindrome). The function should handle all 32-bit signed integer values correctly, including edge cases like negative numbers (which are never palindromes) and the largest/smallest representable values. You may not convert the number to a string or use any standard library containers; you must solve it using arithmetic operations only.
#include <cassert>

int main() {
    // Basic positive cases
    assert(isPalindromeInteger(0) == true);
    assert(isPalindromeInteger(1) == true);
    assert(isPalindromeInteger(121) == true);
    assert(isPalindromeInteger(12321) == true);
    assert(isPalindromeInteger(1234321) == true);

    // Negative numbers are never palindromes
    assert(isPalindromeInteger(-121) == false);
    assert(isPalindromeInteger(-1) == false);
    assert(isPalindromeInteger(-2147483648) == false);

    // Non-palindromic positives
    assert(isPalindromeInteger(10) == false);
    assert(isPalindromeInteger(123) == false);
    assert(isPalindromeInteger(100) == false);
    assert(isPalindromeInteger(2147483647) == false);

    // Largest/smallest possible values
    assert(isPalindromeInteger(2147447412) == true); // reverse of 121474421? Actually check a real palindrome within int range
    assert(isPalindromeInteger(1234567899) == false);
    assert(isPalindromeInteger(1000000001) == true);

    return 0;
}
#include <cstddef>

// Returns true if the given integer reads the same backward as forward.
// Negative numbers are never palindromes. Uses arithmetic reversal with overflow protection.
bool isPalindromeInteger(int x) {
    if (x < 0) {
        return false;
    }

    int original = x;
    long long reversed = 0;

    while (x != 0) {
        reversed = reversed * 10 + x % 10;
        x /= 10;
    }

    return original == static_cast<int>(reversed);
}
// The solution uses a classic reverse-and-compare arithmetic approach. Since negative numbers can never be palindromes (the minus sign would not appear on the right side), we immediately return `false` for any negative input. For non-negative numbers, we reverse the digits by repeatedly extracting the last digit using the modulo operator and building the reversed number with multiplication and addition. To avoid integer overflow when reversing the largest possible 32-bit integer (e.g., 2147483647), we use a `long long` accumulator to safely store the intermediate reversed value; after the loop, we compare the original `x` with the reversed value (cast back to `int`). Edge cases include zero (which is trivially a palindrome) and numbers ending in zero (like 10, which are not palindromes because the reversed value would have a leading zero, but the arithmetic reversal naturally handles this correctly). The time complexity is O(d), where d is the number of digits in the number (at most 10 for 32-bit integers), and the auxiliary space complexity is O(1).
