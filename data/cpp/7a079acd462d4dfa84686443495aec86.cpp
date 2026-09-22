/*
Write a C++ function `int minEditDistance(const std::string& a, const std::string& b)` that computes the minimum number of single-character insertions, deletions, or substitutions required to transform string `a` into string `b`. The strings may contain lowercase letters, be empty, or of differing lengths. Use dynamic programming with a 2D table where `dp[i][j]` represents the minimum edit distance between the first `i` characters of `a` and the first `j` characters of `b`. Your solution must be self-contained, handle empty strings correctly, and not rely on recursion (to avoid exponential time). The function should return the distance as an integer, and must be declared `const`-correct (i.e., it does not modify its inputs). Provide a reference implementation and test it with typical, edge, and asymmetric cases.
*/

#include <string>
#include <vector>
#include <algorithm>

// Compute the minimum edit distance between two strings using dynamic programming.
int minEditDistance(const std::string& a, const std::string& b) {
    const size_t n = a.length();
    const size_t m = b.length();

    // dp[i][j] = edit distance between a[0..i-1] and b[0..j-1]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    // Base cases: transform empty string to prefix of b (insertions) and prefix of a to empty (deletions)
    for (size_t i = 0; i <= n; ++i) {
        dp[i][0] = static_cast<int>(i);
    }
    for (size_t j = 0; j <= m; ++j) {
        dp[0][j] = static_cast<int>(j);
    }

    // Fill the table
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + std::min({dp[i - 1][j],      // deletion
                                         dp[i][j - 1],      // insertion
                                         dp[i - 1][j - 1]}); // substitution
            }
        }
    }

    return dp[n][m];
}

#include <cassert>
#include <string>

// The solution function is declared above; here we only test it.

int main() {
    // Base cases
    assert(minEditDistance("", "") == 0);
    assert(minEditDistance("abc", "") == 3);
    assert(minEditDistance("", "xyz") == 3);

    // Identical strings
    assert(minEditDistance("same", "same") == 0);

    // Single substitution
    assert(minEditDistance("kitten", "sitten") == 1);

    // Classic example
    assert(minEditDistance("sitting", "kitten") == 3);

    // Insertion and deletion
    assert(minEditDistance("flaw", "lawn") == 2);  // delete f, insert n

    // Asymmetric lengths
    assert(minEditDistance("intention", "execution") == 5);

    // Simple case
    assert(minEditDistance("cat", "dog") == 3);

    // Character repeat and edge
    assert(minEditDistance("ab", "ba") == 2);

    // Longer strings
    assert(minEditDistance("algorithm", "altruistic") == 6);

    return 0;
}

// The classic Levenshtein edit distance is solved via bottom-up dynamic programming. Create a 2D vector `dp` of size `(a.length()+1) × (b.length()+1)`. Initialize `dp[i][0] = i` (deleting all characters from `a`) and `dp[0][j] = j` (inserting all characters into `b`). For each `i` from 1 to `a.length()` and `j` from 1 to `b.length()`, if the characters match, `dp[i][j] = dp[i-1][j-1]`; otherwise, `dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1])` representing deletion, insertion, and substitution respectively. This guarantees correctness because we build optimal solutions from smaller subproblems. Edge cases: both strings empty returns 0; one empty returns the length of the other; identical strings return 0; very different lengths are handled naturally by the table. Time complexity is O(n*m) and space complexity is O(n*m). For large strings, space could be optimized to O(min(n,m)) by keeping only two rows, but the task does not require that optimization.
