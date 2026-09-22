/*
Write a C++ function named `isPalindrome` that accepts a non-negative 64-bit integer (`long long`) and returns `true` if the decimal representation of the number reads the same forward and backward (i.e., it is a palindrome), and `false` otherwise. The function must handle edge cases such as `0` (which is a palindrome), numbers that are large and near the maximum representable `long long` value (e.g., `9223372036854775807`), and numbers with trailing zeros (e.g., `12321` is a palindrome, but `120` is not). Do not convert the number to a string; use only integer arithmetic to reverse the digits. The function must be self-contained and not rely on any global state.
*/

#include <cstdint>

// Determine if a non-negative 64-bit integer is a decimal palindrome.
bool isPalindrome(const long long n) {
    if (n < 0) return false;  // Problem specifies non-negative, but guard anyway.
    unsigned long long original = static_cast<unsigned long long>(n);
    unsigned long long reversed = 0;
    unsigned long long temp = original;

    while (temp != 0) {
        reversed = reversed * 10ULL + (temp % 10ULL);
        temp /= 10ULL;
    }

    return reversed == original;
}

#include <cassert>

// Global main function to test the solution function.
int main() {
    assert(isPalindrome(0) == true);
    assert(isPalindrome(1) == true);
    assert(isPalindrome(7) == true);
    assert(isPalindrome(121) == true);
    assert(isPalindrome(12321) == true);
    assert(isPalindrome(12345) == false);
    assert(isPalindrome(10) == false);
    assert(isPalindrome(120) == false);
    assert(isPalindrome(9223372036854775807LL) == true);  // Largest 64-bit palindrome
    assert(isPalindrome(1234567890123456789LL) == false);
    return 0;
}

// The solution reverses the digits of the input number by repeatedly extracting the last digit using modulo 10 and building a reversed number by multiplying the current reversed value by 10 and adding the digit. After processing all digits, the reversed number is compared with the original. Important edge cases: `n = 0` has a single digit and is a palindrome; the loop naturally terminates because `tmp` becomes 0, so no special-case is needed. For very large numbers, the reversed value may overflow a `long long`; however, in practice, the largest palindrome that fits in a 64-bit signed integer is `9223372036854775807` (which is itself a palindrome), and its reverse is the same value, so no overflow occurs for inputs that are palindromes. For non-palindromes, overflow is not a correctness issue because we only need equality; if overflow occurs, the reversed value will differ from the original, and the function will correctly return `false`. However, to be safe and avoid undefined behavior from signed integer overflow, we can use `unsigned long long` internally for the reversed accumulation. Time complexity is \(O(d)\) where \(d\) is the number of decimal digits (at most 19 for 64-bit), and space complexity is \(O(1)\).
