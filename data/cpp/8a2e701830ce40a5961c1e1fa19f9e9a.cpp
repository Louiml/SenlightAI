Write a C++ function that takes a non-empty string `s` consisting only of lowercase English letters and returns the longest palindromic substring of `s`. If there are multiple substrings of the same maximum length, return the one that appears first (i.e., the one with the smallest starting index; if ties persist, the smaller ending index). The function must handle strings of length 1 up to length 1000. You must implement the dynamic programming approach and not use built-in palindrome checks or expand-around-center optimization. The function should be named `longestPalindrome` and should accept the string by `const` reference, returning a `std::string` by value.
// The solution uses a 2D dynamic programming table `dp[i][j]` where `dp[i][j]` is `true` if the substring `s[i..j]` (inclusive) is a palindrome. Initialize all `dp[i][i] = true` because every single character is a palindrome. Also initialize the current longest palindrome as `s.substr(0,1)` to handle the single-character case. Then iterate over all possible substring lengths `len` from 2 to `s.size()`. For each starting index `i` where `j = i + len - 1` is within bounds, check if `s[i] == s[j]` and either `len == 2` (so the inner substring is empty) or `dp[i+1][j-1]` is `true`. If both conditions hold, set `dp[i][j] = true` and update the longest substring if `len` is greater than the current maximum length. Because we iterate lengths in increasing order and from left to right for each length, the first time we find a palindrome of a given maximum length will be the one with the smallest start index (since we process `i` from 0 upward). Time complexity: O(n²) where n is length of string, due to filling the 2D DP table. Space complexity: O(n²) for the table. Edge cases: empty string not allowed per spec; length 1 handled by initial longest; if no palindrome longer than 1 exists, the function returns the first character. The function uses `const` correctness by accepting `const std::string&` and not modifying input.
#include <string>
#include <vector>

// Returns the longest palindromic substring of s.
// If multiple longest palindromes exist, returns the one with smallest start index.
std::string longestPalindrome(const std::string& s) {
    const int n = static_cast<int>(s.size());
    if (n == 0) return "";

    // dp[i][j] is true if substring s[i..j] is a palindrome.
    std::vector<std::vector<bool>> dp(n, std::vector<bool>(n, false));
    for (int i = 0; i < n; ++i) {
        dp[i][i] = true;
    }

    // Keep track of the longest palindrome found so far.
    int start = 0;
    int max_len = 1;

    // Iterate over all substring lengths from 2 to n.
    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            const int j = i + len - 1;
            // A substring of length 2 is a palindrome if characters match.
            // For longer substrings, check inner substring.
            if (s[i] == s[j] && (len == 2 || dp[i + 1][j - 1])) {
                dp[i][j] = true;
                // Since we process lengths in increasing order and i increasing,
                // the first time we see a palindrome of a given length is the
                // one with the smallest start index.
                if (len > max_len) {
                    max_len = len;
                    start = i;
                }
            }
        }
    }

    return s.substr(start, max_len);
}
#include <cassert>
#include <string>

// Declaration of the function to test.
std::string longestPalindrome(const std::string& s);

int main() {
    // Single character.
    assert(longestPalindrome("a") == "a");

    // Two identical characters.
    assert(longestPalindrome("bb") == "bb");

    // Two different characters - longest is first char.
    assert(longestPalindrome("ab") == "a");

    // Classic example with even length palindrome.
    assert(longestPalindrome("cbbd") == "bb");

    // Classic example with odd length palindrome.
    assert(longestPalindrome("babad") == "bab"); // first occurrence of length 3.

    // Longer string with multiple palindromes: "forgeeksskeegfor" -> "geeksskeeg"
    assert(longestPalindrome("forgeeksskeegfor") == "geeksskeeg");

    // All characters same: entire string is a palindrome.
    assert(longestPalindrome("aaaa") == "aaaa");

    // Mixed with unique characters: longest palindrome is a single character.
    assert(longestPalindrome("abcdef") == "a");

    // Palindrome at the end, not at start: "abacdfgdcaba" -> "aba" (first 3-length)
    assert(longestPalindrome("abacdfgdcaba") == "aba");

    // Longest palindrome in middle: "racecarXYZ" -> "racecar"
    assert(longestPalindrome("racecarXYZ") == "racecar");

    return 0;
}
