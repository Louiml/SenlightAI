/*
Write a C++ function `longestCommonSubsequenceLength` that takes two null-terminated C-strings (`const char* X` and `const char* Y`) and returns the length of the longest common subsequence (LCS) as an `int`. The function must handle empty strings correctly, be case-sensitive, and must not modify the input strings. Use dynamic programming with a 2D table to compute the result. The function should be self-contained (no reliance on global variables) and include only necessary headers. Your solution will be tested with various pairs of strings, including those with repeated characters and differing lengths.
*/

#include <string>
#include <algorithm>

// Returns the length of the longest common subsequence between two C-strings.
int longestCommonSubsequenceLength(const char* X, const char* Y) {
    if (X == nullptr || Y == nullptr) return 0;

    int m = 0;
    while (X[m] != '\0') ++m;
    int n = 0;
    while (Y[n] != '\0') ++n;

    if (m == 0 || n == 0) return 0;

    // Use dynamic programming table
    int L[m + 1][n + 1];

    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            if (i == 0 || j == 0) {
                L[i][j] = 0;
            } else if (X[i - 1] == Y[j - 1]) {
                L[i][j] = L[i - 1][j - 1] + 1;
            } else {
                L[i][j] = std::max(L[i - 1][j], L[i][j - 1]);
            }
        }
    }

    return L[m][n];
}

#include <cassert>

int main() {
    // Basic cases
    assert(longestCommonSubsequenceLength("ABC", "ABC") == 3);
    assert(longestCommonSubsequenceLength("ABC", "DEF") == 0);
    assert(longestCommonSubsequenceLength("", "ABC") == 0);
    assert(longestCommonSubsequenceLength("ABC", "") == 0);
    assert(longestCommonSubsequenceLength("", "") == 0);

    // Classic examples
    assert(longestCommonSubsequenceLength("AGGTAB", "GXTXAYB") == 4); // "GTAB"
    assert(longestCommonSubsequenceLength("ABCDGH", "AEDFHR") == 3); // "ADH"
    assert(longestCommonSubsequenceLength("ABCDEFG", "BCDGK") == 4); // "BCDG"

    // Case sensitivity and repeated characters
    assert(longestCommonSubsequenceLength("abc", "ABC") == 0);
    assert(longestCommonSubsequenceLength("AAA", "AA") == 2);
    assert(longestCommonSubsequenceLength("ABCBDAB", "BDCABA") == 4); // "BCBA" or "BDAB"

    // Different lengths
    assert(longestCommonSubsequenceLength("abcde", "ace") == 3); // "ace"
    assert(longestCommonSubsequenceLength("abcdef", "xyz") == 0);

    // Single character matches
    assert(longestCommonSubsequenceLength("a", "a") == 1);
    assert(longestCommonSubsequenceLength("a", "b") == 0);

    return 0;
}

// The problem is solved using standard dynamic programming for LCS. Let `m = strlen(X)` and `n = strlen(Y)`. Create a 2D table `L` of size `(m+1) × (n+1)`, where `L[i][j]` stores the LCS length of the first `i` characters of `X` and the first `j` characters of `Y`. Initialize the first row and column to zero because an empty string has LCS length 0 with any string. Then, for each `i` from 1 to `m` and `j` from 1 to `n`, if `X[i-1] == Y[j-1]`, set `L[i][j] = L[i-1][j-1] + 1`; otherwise, set `L[i][j] = max(L[i-1][j], L[i][j-1])`. The answer is `L[m][n]`. Edge cases include both strings empty (returns 0), one empty (returns 0), and strings with no common characters (returns 0). The time complexity is `O(m*n)` and space complexity is `O(m*n)` due to the table. For very large inputs, one could optimize space to O(min(m,n)), but the task does not require that.
