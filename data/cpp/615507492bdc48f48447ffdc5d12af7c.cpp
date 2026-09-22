Write a C++ function named `longestCommonSubsequence` that takes two non-empty strings `a` and `b` as parameters, along with their lengths `m` and `n` (where `m = a.length()`, `n = b.length()`). The function must compute the length of the longest common subsequence (LCS) between the two strings and return a string containing the actual LCS. In case there are multiple possible LCSs (e.g., due to repeated characters), return any valid one. The function should be `const`-correct and operate efficiently using dynamic programming. The input strings contain only lowercase English letters. The function must be callable from an external test harness and must not print anything.
The classic solution uses dynamic programming with a 2D table `T` of size `(m+1) × (n+1)`. `T[i][j]` stores the length of the LCS of the first `i` characters of `a` and the first `j` characters of `b`. The recurrence is: if `i == 0` or `j == 0`, then `T[i][j] = 0`; else if `a[i-1] == b[j-1]`, then `T[i][j] = T[i-1][j-1] + 1`; else `T[i][j] = max(T[i-1][j], T[i][j-1])`. After filling the table, we reconstruct one LCS by backtracking from `T[m][n]`: when characters match, we take that character and move diagonally; otherwise, we move in the direction of the larger neighbor (prefer up if equal). Edge cases: empty strings would produce `""`, but the task guarantees non-empty; strings with no common characters (e.g., "abc" and "def") produce the empty string. Time complexity is `O(m*n)` for building the table and `O(m+n)` for backtracking, space complexity is `O(m*n)` for the table.
#include <string>
#include <vector>
#include <algorithm>

// Compute the longest common subsequence (LCS) of two strings a and b.
// m = length of a, n = length of b. Returns one valid LCS as a string.
// If no common characters exist, returns an empty string.
std::string longestCommonSubsequence(const std::string& a, const std::string& b, int m, int n) {
    // Build DP table
    std::vector<std::vector<int>> T(m + 1, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1]) {
                T[i][j] = T[i - 1][j - 1] + 1;
            } else {
                T[i][j] = std::max(T[i - 1][j], T[i][j - 1]);
            }
        }
    }
    // Backtrack to reconstruct one LCS
    int index = T[m][n];
    std::string result(index, '\0');
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            result[index - 1] = a[i - 1];
            --index;
            --i;
            --j;
        } else if (T[i - 1][j] > T[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    return result;
}
#include <cassert>
#include <string>

std::string longestCommonSubsequence(const std::string& a, const std::string& b, int m, int n);

int main() {
    // Basic tests
    assert(longestCommonSubsequence("abcde", "ace", 5, 3) == "ace");
    assert(longestCommonSubsequence("abc", "abc", 3, 3) == "abc");
    assert(longestCommonSubsequence("abc", "def", 3, 3) == "");

    // Repeated characters, check length and that it is a subsequence
    std::string lcs1 = longestCommonSubsequence("aab", "aba", 3, 3);
    assert(lcs1.length() == 2);
    assert(lcs1 == "aa" || lcs1 == "ab" || lcs1 == "ba");

    std::string lcs2 = longestCommonSubsequence("AGGTAB", "GXTXAYB", 6, 7);
    assert(lcs2.length() == 4);
    assert(lcs2 == "GTAB");

    // Longer strings
    std::string a = "ABCDGH", b = "AEDFHR";
    std::string lcs3 = longestCommonSubsequence(a, b, 6, 6);
    assert(lcs3.length() == 3);
    assert(lcs3 == "ADH");

    // Non-empty but no common characters
    assert(longestCommonSubsequence("xyz", "abc", 3, 3) == "");

    // Single character matches
    assert(longestCommonSubsequence("x", "x", 1, 1) == "x");
    assert(longestCommonSubsequence("x", "y", 1, 1) == "");

    // All same character
    assert(longestCommonSubsequence("aaa", "aaa", 3, 3) == "aaa");

    // Mixed with trailing and leading similarities
    assert(longestCommonSubsequence("abc", "cba", 3, 3) == "a" || longestCommonSubsequence("abc", "cba", 3, 3) == "b" || longestCommonSubsequence("abc", "cba", 3, 3) == "c");

    // Larger test with random-like input
    assert(longestCommonSubsequence("abcdefghij", "acegik", 10, 5) == "acegi");
}
