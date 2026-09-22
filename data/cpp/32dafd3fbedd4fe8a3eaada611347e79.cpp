Write a C++ function named `isPalindromeNumber` that takes a single integer `x` as input and returns a `bool` indicating whether `x` is a palindrome when read in its decimal form without any leading zeros (except the number 0 itself). A palindrome reads the same forward and backward (e.g., 121, -121 is not a palindrome because the minus sign is not part of the integer representation, and numbers ending with zero like 10 are not palindromes because no palindrome integer can have a trailing zero unless it is exactly 0). The function must solve the problem without converting the integer to a string or using any auxiliary container; it should only use integer arithmetic. Handle negative numbers, zero, positive numbers, and numbers with multiple digits. The function should be efficient in both time and space.
#include <cassert>

int main() {
    // Positive palindromes
    assert(isPalindromeNumber(121) == true);
    assert(isPalindromeNumber(12321) == true);
    assert(isPalindromeNumber(1001) == true);
    assert(isPalindromeNumber(0) == true);

    // Non-palindromes
    assert(isPalindromeNumber(-121) == false);
    assert(isPalindromeNumber(10) == false);
    assert(isPalindromeNumber(123) == false);
    assert(isPalindromeNumber(100) == false);

    // Single-digit numbers (including negatives)
    assert(isPalindromeNumber(5) == true);
    assert(isPalindromeNumber(-5) == false);
    assert(isPalindromeNumber(7) == true);

    // Large numbers
    assert(isPalindromeNumber(2147447412) == true);  // 2^31-1 is not a palindrome, but this is
    assert(isPalindromeNumber(2147483647) == false); // INT_MAX is not a palindrome

    return 0;
}
#include <cstdint>

// Determines whether the given integer is a palindrome when read as a decimal number.
bool isPalindromeNumber(int x) {
    // Negative numbers cannot be palindromes (the minus sign disrupts symmetry).
    // Any non-zero number ending in 0 cannot be a palindrome because its reverse
    // would start with 0, which is not a valid decimal representation.
    if (x < 0 || (x != 0 && x % 10 == 0)) {
        return false;
    }

    int reverted = 0;
    // Build the reversed half of the number digit by digit.
    while (x > reverted) {
        reverted = reverted * 10 + (x % 10);
        x /= 10;
    }

    // For even digit counts, the halves are equal.
    // For odd digit counts, drop the middle digit from reverted before comparing.
    return reverted == x || reverted / 10 == x;
}
// The algorithm avoids string conversion by partially reversing the integer and comparing it with the remaining half. First, we immediately return `false` for any negative number (since the minus sign makes it impossible to be a palindrome) and for any number that is a multiple of 10 but not zero (e.g., 10, 100, 130) because the reversal would produce a leading zero, which cannot appear in a valid decimal representation. Then, we repeatedly extract the last digit of `x` using modulo 10 and append it to a `reverted` accumulator by multiplying the accumulator by 10 and adding the digit, while simultaneously removing that digit from `x` by integer division by 10. We continue this loop only while `x > reverted`. When the loop terminates, we have either processed all digits (if the length is even) or processed one extra digit from the middle (if the length is odd). For an even length, the original `x` (now reduced to its first half) must equal `reverted`; for an odd length, the middle digit is the last digit of `reverted`, so we can drop it by integer division by 10 and compare with `x`. This runs in O(log₁₀ n) time because each iteration removes one digit from `x`, and uses O(1) auxiliary space. Edge cases: `x = 0` is a palindrome (returns true); `x = -121` returns false; `x = 10` returns false; `x = 1001` is true; `x = 12321` is true.
