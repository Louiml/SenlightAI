// Write a C++ function named `isPalindromeNumber` that takes an integer `x` and returns a boolean indicating whether `x` is a palindrome. A palindrome integer reads the same forwards and backwards (e.g., 121, -121 is not a palindrome because reversed is -121, and 10 is not because reversed is 01 which equals 1). The function should handle negative numbers (always return `false`), zero (return `true`), and large positive integers without causing integer overflow. You may not convert the integer to a string; you must compute the reversed number using arithmetic operations. The function signature should be `bool isPalindromeNumber(int x)`.

// The core idea is to reverse the digits of the integer and then compare the reversed number to the original. Since negative numbers cannot be palindromes (the minus sign is not symmetric), we immediately return `false` for any negative input. For zero, the reverse is zero, so it is trivially a palindrome. For positive numbers, we repeatedly extract the last digit using modulo 10, build the reversed number by multiplying the current reversed value by 10 and adding the digit, and divide the original number by 10 to remove the last digit. A critical edge case is integer overflow: if the reversed number exceeds the range of `int`, the computation will overflow. To avoid this, we use a `long long` (or `long`) for the reversed accumulator. After the loop, we compare the reversed number (as a `long long`) to the original integer (cast to `long long`) and return `true` if they are equal. The time complexity is O(d) where d is the number of digits in the integer (at most 10 for a 32-bit int, so effectively constant). The space complexity is O(1) since we only use a few scalar variables.

#include <cstdint>

// Returns true if the integer x is a palindrome; negatives are always false.
bool isPalindromeNumber(int x) {
    if (x < 0) {
        return false;
    }
    // Use int64_t to avoid overflow during reversal.
    int64_t reversed = 0;
    int original = x;
    while (original != 0) {
        int digit = original % 10;
        reversed = reversed * 10 + digit;
        original /= 10;
    }
    return reversed == static_cast<int64_t>(x);
}

#include <cassert>

int main() {
    // Basic palindromes
    assert(isPalindromeNumber(121) == true);
    assert(isPalindromeNumber(0) == true);
    assert(isPalindromeNumber(1) == true);
    assert(isPalindromeNumber(12321) == true);
    
    // Non-palindromes
    assert(isPalindromeNumber(10) == false);
    assert(isPalindromeNumber(123) == false);
    assert(isPalindromeNumber(-121) == false);
    assert(isPalindromeNumber(100) == false);
    
    // Large values that could overflow if using int
    assert(isPalindromeNumber(2147447412) == true);  // 2,147,447,412 is a palindrome
    assert(isPalindromeNumber(2147483647) == false); // INT_MAX is not a palindrome
    
    return 0;
}
