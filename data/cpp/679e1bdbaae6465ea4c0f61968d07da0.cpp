// Write a C++ function that, given two positive integers `A` and `B` (with `1 ≤ A ≤ B ≤ 1000000`), returns the count of *palindromic* integers in the inclusive range `[A, B]`. A palindromic integer is one whose decimal representation reads the same forwards and backwards (e.g., 121, 5, 99, 1001). The function must handle ranges where `A == B`, and must correctly treat single-digit numbers as palindromes. You are not allowed to use any string-reversal or digit-extraction shortcuts that involve precomputing all palindromes; instead, implement the palindrome check explicitly. The function should be named `countPalindromesInRange` and take two integer arguments, returning an `int`.

The simplest and most robust approach is to iterate over every integer from `A` to `B` inclusive, convert each to a string using `std::to_string`, and check if that string is a palindrome. A string is a palindrome if it equals its reverse; you can use `std::equal` comparing the first half of the string with the reverse of the second half, or simply copy the string, reverse it, and compare. Edge cases: when `A == B`, the function should return either 0 or 1 depending on whether that single number is palindromic; single-digit numbers (0-9) are always palindromes because their string representation has one character. Also, numbers like 10, 100 are not palindromes because their string representations "10" and "100" are not symmetric. Time complexity is \(O((B - A + 1) \cdot d)\), where \(d\) is the number of digits (at most 7 for 1,000,000), so it is effectively linear in the range size with a small constant. Space complexity is \(O(d)\) for the temporary string, which is negligible. An alternative more efficient approach would generate palindromes, but for the given bounds (up to 1,000,000) the brute-force iteration is perfectly acceptable and simpler.

#include <string>
#include <algorithm>

// Count how many integers in the inclusive range [A, B] are palindromic.
int countPalindromesInRange(int A, int B) {
    int count = 0;
    for (int number = A; number <= B; ++number) {
        const std::string str = std::to_string(number);
        std::string reversed = str;
        std::reverse(reversed.begin(), reversed.end());
        if (str == reversed) {
            ++count;
        }
    }
    return count;
}

#include <cassert>

// Free function prototype (replace with actual include or definition in same file).
int countPalindromesInRange(int A, int B);

int main() {
    // Single-digit ranges: all numbers are palindromes.
    assert(countPalindromesInRange(1, 9) == 9);
    assert(countPalindromesInRange(5, 5) == 1);
    assert(countPalindromesInRange(10, 10) == 0);

    // Classic palindromes in small ranges.
    assert(countPalindromesInRange(1, 10) == 9); // 1-9 are palindromes, 10 is not.
    assert(countPalindromesInRange(10, 20) == 1); // only 11.
    assert(countPalindromesInRange(1, 100) == 18); // 1-9 (9) + 11,22,...,99 (9)

    // Larger range with known palindromes: 101, 111, 121, 131, 141, 151, 161, 171, 181, 191.
    assert(countPalindromesInRange(100, 200) == 10);

    // Full range from 1 to 1000.
    assert(countPalindromesInRange(1, 1000) == 108); // 9 one-digit + 9 two-digit (11..99) + 90 three-digit (101..999)

    // Edge case: A > B is not allowed, but we assume caller ensures A ≤ B.
    // Negative numbers are not in input domain.
    return 0;
}
