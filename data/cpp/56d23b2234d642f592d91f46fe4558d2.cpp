Write a C++ function named `isPalindromeWord` that takes a single `std::string` parameter (which may contain lowercase letters only, no spaces, and is guaranteed non-empty) and returns a `bool` indicating whether the string reads the same forward and backward (i.e., is a palindrome). The function must be case-sensitive and must not modify the input string. The task focuses on implementing the palindrome check without using any library functions like `std::reverse`; instead, you must compare characters from both ends moving inward. The function should return `true` for palindromes like `"racecar"` and `"a"`, and `false` for non-palindromes like `"hello"` or `"abca"`.
// The solution approach is to use a two‑pointer technique. Initialize a left index at the start of the string (position 0) and a right index at the end (position `length - 1`). While `left < right`, compare the characters at these positions. If they differ, return `false` immediately. If they match, increment `left` and decrement `right` and continue. If the loop completes without finding a mismatch, the string is a palindrome, so return `true`. This works for all non‑empty lowercase strings. Edge cases include a single‑character string (which is trivially a palindrome) and even‑length strings where the pointer loop terminates cleanly after all pairs are compared. The algorithm runs in \(O(n)\) time because each character is examined at most once, and uses \(O(1)\) auxiliary space since no extra data structures are needed. The function must be `const`‑correct by taking the parameter as `const std::string&` to avoid copying and to promise not to alter the input.
#include <string>

// Check if a given non-empty lowercase string is a palindrome.
// Compares characters from both ends toward the center.
bool isPalindromeWord(const std::string& word) {
    int left = 0;
    int right = static_cast<int>(word.size()) - 1;

    while (left < right) {
        if (word[left] != word[right]) {
            return false; // Mismatch found
        }
        ++left;
        --right;
    }
    return true; // All pairs matched
}
#include <cassert>
#include <string>
#include "solution.h" // assumes the solution code is in this header or included above

int main() {
    // Basic palindromes
    assert(isPalindromeWord("racecar") == true);
    assert(isPalindromeWord("madam") == true);
    assert(isPalindromeWord("a") == true);

    // Non-palindromes
    assert(isPalindromeWord("hello") == false);
    assert(isPalindromeWord("abca") == false);

    // Even-length palindromes
    assert(isPalindromeWord("abba") == true);
    assert(isPalindromeWord("aa") == true);

    // Edge case: all same characters
    assert(isPalindromeWord("zzz") == true);

    // Case sensitivity (lowercase only input)
    assert(isPalindromeWord("abA") == false); // 'b' vs 'A' mismatch

    // Longer string
    assert(isPalindromeWord("abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba") == true);

    return 0;
}
