// Write a C++ function that takes a string `s` and returns the minimum number of characters that must be inserted into `s` to make it a palindrome. For example, for the string "ab", the minimum insertions is 1 (producing "aba" or "bab"), and for "abcba" it is 0 because it is already a palindrome. The function must handle empty strings (return 0), single-character strings (return 0), and strings containing only lowercase English letters. The solution should be efficient for strings up to length 1000.

#include <cassert>

int main() {
    assert(minimumInsertionsForPalindrome("") == 0);
    assert(minimumInsertionsForPalindrome("a") == 0);
    assert(minimumInsertionsForPalindrome("ab") == 1);
    assert(minimumInsertionsForPalindrome("abc") == 2);
    assert(minimumInsertionsForPalindrome("abcba") == 0);
    assert(minimumInsertionsForPalindrome("abca") == 1);
    assert(minimumInsertionsForPalindrome("abcd") == 3);
    assert(minimumInsertionsForPalindrome("racecar") == 0);
    assert(minimumInsertionsForPalindrome("geeks") == 3); // longest pal subsequence "ee", n=5 -> 3
    assert(minimumInsertionsForPalindrome("banana") == 1); // longest pal subsequence "anana", n=6 -> 1
    return 0;
}

#include <string>
#include <vector>
#include <algorithm>

// Returns the minimum number of character insertions needed to make s a palindrome.
// This equals s.length() - length_of_longest_palindromic_subsequence(s).
int minimumInsertionsForPalindrome(const std::string& s) {
    const int n = static_cast<int>(s.size());
    if (n <= 1) return 0;
    
    std::string r = s;
    std::reverse(r.begin(), r.end());
    
    // dp[i][j] = LCS length of s[i..] and r[j..]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));
    
    for (int i = n - 1; i >= 0; --i) {
        for (int j = n - 1; j >= 0; --j) {
            if (s[i] == r[j]) {
                dp[i][j] = 1 + dp[i + 1][j + 1];
            } else {
                dp[i][j] = std::max(dp[i + 1][j], dp[i][j + 1]);
            }
        }
    }
    
    int lpsLength = dp[0][0];
    return n - lpsLength;
}

// The minimum number of insertions to make a string a palindrome equals the length of the string minus the length of its longest palindromic subsequence (LPS). A longest palindromic subsequence is found by comparing the original string with its reverse using dynamic programming on the longest common subsequence (LCS) — because any subsequence that is common to the string and its reverse is a palindromic subsequence, and the longest such is the LPS. We compute LCS using a 2D DP table where `dp[i][j]` represents the length of the LCS of the suffix starting at index `i` of `s` and the suffix starting at index `j` of `r` (the reverse). The base case is when either index reaches the end, returning 0. If characters match, we add 1 and move both pointers; otherwise we take the maximum of moving only one pointer. Since the problem only requires the length of the LPS, we can use either recursion with memoization (as in the snippet) or an iterative approach. The time complexity is `O(n^2)` and space complexity is `O(n^2)` for the DP table, where `n` is the length of the string. Edge cases include empty string, single character, and already-palindromic strings, all of which are handled by the DP naturally.
