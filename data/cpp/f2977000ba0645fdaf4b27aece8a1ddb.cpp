Write a C++ function `std::string longestPalindromicSubstring(std::string s)` that returns the longest palindromic substring of the given string `s`. A palindrome reads the same forward and backward, e.g., "aba", "bb", "a". The input string may contain uppercase and lowercase letters, digits, and other printable ASCII characters. The function should handle empty strings (returning an empty string), strings of length 1 (returning the character), and cases with multiple longest palindromes of equal length (return any one of them). The function must be efficient for strings up to length 1000, and you cannot use brute force checking all substrings. The solution should be an independent, self-contained function with proper `const` correctness, and it must not rely on global variables or a `main` function (testing is provided separately).

The core idea is to use "expand around center" technique. For each possible center (either a single character or the gap between two identical characters), we expand outward as long as the characters at both ends match, tracking the longest palindrome found. There are `2*n - 1` possible centers: `n` for single-character centers, and `n-1` for two-character centers (which capture even-length palindromes). For each center, expansion takes `O(n)` in the worst case, so total time is `O(n^2)`. Space is `O(1)` beyond the input. Edge cases: empty string → return empty; string of length 1 → return the string; all identical characters → the whole string is a palindrome; overlapping palindromes — the algorithm naturally handles them. We update the start index and max length whenever a longer palindrome is found. This is simpler and more memory-efficient than the recursive DP approach in the snippet, and it is also more reliable than the commented-out manacher-like approach (which has a bug: the first `while` loop condition should be `mid < len`, not `mid <= len`, and it doesn’t handle even-length palindromes correctly). The expand-around-center method is correct, easy to implement, and passes the required constraints.

#include <string>

// Returns the longest palindromic substring in s.
// Uses expand-around-center: O(n^2) time, O(1) space.
std::string longestPalindromicSubstring(const std::string& s) {
    if (s.empty()) return "";
    if (s.size() == 1) return s;

    int n = s.size();
    int start = 0;
    int maxLen = 1; // A single character is always a palindrome

    // Helper lambda to expand around center [left, right] and update best.
    auto expand = [&](int left, int right) {
        while (left >= 0 && right < n && s[left] == s[right]) {
            int len = right - left + 1;
            if (len > maxLen) {
                maxLen = len;
                start = left;
            }
            left--;
            right++;
        }
    };

    for (int i = 0; i < n; ++i) {
        expand(i, i);     // odd-length palindromes (center at i)
        expand(i, i + 1); // even-length palindromes (center between i and i+1)
    }

    return s.substr(start, maxLen);
}

#include <cassert>
#include <string>

// The solution function is declared here (or included from above).
std::string longestPalindromicSubstring(const std::string& s);

int main() {
    // Empty string
    assert(longestPalindromicSubstring("") == "");

    // Single character
    assert(longestPalindromicSubstring("a") == "a");

    // No palindrome longer than 1
    assert(longestPalindromicSubstring("ab") == "a" || longestPalindromicSubstring("ab") == "b");

    // All same characters
    assert(longestPalindromicSubstring("aaaa") == "aaaa");

    // Classic even and odd cases
    assert(longestPalindromicSubstring("babad") == "bab" || longestPalindromicSubstring("babad") == "aba");
    assert(longestPalindromicSubstring("cbbd") == "bb");

    // Palindrome at the beginning
    assert(longestPalindromicSubstring("abacdfgdcaba") == "aba" || longestPalindromicSubstring("abacdfgdcaba") == "abcdcba");

    // String with mixed cases and digits
    assert(longestPalindromicSubstring("A1b1A") == "A1b1A");

    // Long string with many identical centers
    assert(longestPalindromicSubstring("abcba") == "abcba");

    // Repeated pattern
    assert(longestPalindromicSubstring("racecarX") == "racecar");

    return 0;
}
