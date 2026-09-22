// Write a C++ function that takes a non-empty string `s` consisting only of lowercase English letters and returns the longest palindromic substring of `s`. If there are multiple substrings with the same maximum length, return the one that appears first (i.e., with the smallest starting index). The function must handle strings of length 1 up to length 1000 efficiently. For example, for input `"babad"`, the function should return `"bab"` (not `"aba"` because `"bab"` appears first), and for `"cbbd"`, it should return `"bb"`. The solution must use dynamic programming to avoid exponential time.
The problem is the classic "Longest Palindromic Substring" problem. The dynamic programming approach defines a 2D boolean table `dp[i][j]` that is `true` if the substring `s[i..j]` (inclusive) is a palindrome. The base cases are: every single character (`dp[i][i]`) is a palindrome, and every two-character substring is a palindrome if `s[i] == s[i+1]`. For longer substrings of length `L` (from 3 to `n`), a substring `s[i..j]` is a palindrome if `s[i] == s[j]` and the inner substring `s[i+1..j-1]` is already a palindrome (i.e., `dp[i+1][j-1]` is true). We track the starting index and length of the longest palindrome found, and if a tie occurs, we keep the earliest because we iterate increasing lengths and increasing start indices. The edge case is when `n == 1`, where the entire string is the answer. Another edge: the input is non-empty, so no need to handle empty string. Time complexity is O(n²) because we fill an n×n table, and space complexity is O(n²) for the table. This is optimal for the constraints (n ≤ 1000).
#include <string>
#include <vector>

// Returns the longest palindromic substring of s.
// If there are multiple longest palindromes, returns the one with the smallest starting index.
std::string longestPalindrome(const std::string& s) {
    int n = static_cast<int>(s.size());
    if (n < 2) {
        return s;
    }

    // dp[i][j] is true if s[i..j] is a palindrome.
    std::vector<std::vector<bool>> dp(n, std::vector<bool>(n, false));
    int bestStart = 0;
    int bestLength = 1;

    // All single characters are palindromes.
    for (int i = 0; i < n; ++i) {
        dp[i][i] = true;
    }

    // Check two-character substrings.
    for (int i = 0; i < n - 1; ++i) {
        if (s[i] == s[i + 1]) {
            dp[i][i + 1] = true;
            bestStart = i;
            bestLength = 2;
        }
    }

    // For lengths 3 to n.
    for (int length = 3; length <= n; ++length) {
        for (int start = 0; start <= n - length; ++start) {
            int end = start + length - 1;
            if (s[start] == s[end] && dp[start + 1][end - 1]) {
                dp[start][end] = true;
                if (length > bestLength) {
                    bestLength = length;
                    bestStart = start;
                }
            }
        }
    }

    return s.substr(bestStart, bestLength);
}
#include <cassert>

int main() {
    // Basic cases.
    assert(longestPalindrome("babad") == "bab");
    assert(longestPalindrome("cbbd") == "bb");
    assert(longestPalindrome("a") == "a");
    assert(longestPalindrome("ac") == "a");  // either 'a' or 'c', but 'a' appears first.
    assert(longestPalindrome("racecar") == "racecar");
    assert(longestPalindrome("abcde") == "a");
    assert(longestPalindrome("aaaa") == "aaaa");
    assert(longestPalindrome("abacdfgdcaba") == "aba");  // the longest palindrome is "aba" (start 0), not the full string.
    assert(longestPalindrome("aaabaaaa") == "aaabaaa");  // length 7, first occurrence.
    assert(longestPalindrome("forgeeksskeegfor") == "geeksskeeg");
    return 0;
}
