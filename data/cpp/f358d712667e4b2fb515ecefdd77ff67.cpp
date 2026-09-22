/*
Write a C++ function `editDistance` that takes two strings `s1` and `s2` and returns the minimum number of operations required to convert `s1` into `s2`, where allowed operations are insertion, deletion, or substitution of a single character. The strings may be empty, may contain any printable ASCII characters, and can differ arbitrarily in length (including one being much longer than the other). The function must be efficient for strings up to 1000 characters each. Return the integer edit distance.
*/

#include <string>
#include <vector>
#include <algorithm>

// Compute the Levenshtein edit distance between two strings.
int editDistance(const std::string& s1, const std::string& s2) {
    const int n = static_cast<int>(s1.size());
    const int m = static_cast<int>(s2.size());
    
    // dp[i][j] = min edit distance between s1[0..i-1] and s2[0..j-1]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    
    // Base cases: converting to/from empty string
    for (int i = 0; i <= n; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= m; ++j) {
        dp[0][j] = j;
    }
    
    // Fill the table
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::min({dp[i - 1][j],      // delete
                                     dp[i][j - 1],      // insert
                                     dp[i - 1][j - 1]}) // substitute
                           + 1;
            }
        }
    }
    
    return dp[n][m];
}

#include <cassert>
#include <string>

// (The solution function is assumed to be declared above.)

int main() {
    assert(editDistance("", "") == 0);
    assert(editDistance("abc", "abc") == 0);
    assert(editDistance("abc", "") == 3);
    assert(editDistance("", "xyz") == 3);
    assert(editDistance("kitten", "sitting") == 3);
    assert(editDistance("flaw", "lawn") == 2);
    assert(editDistance("intention", "execution") == 5);
    assert(editDistance("a", "b") == 1);
    assert(editDistance("ab", "ba") == 2);
    assert(editDistance("short", "longer") == 5);
    return 0;
}

// The solution uses dynamic programming with a 2D table `dp` of size `(n+1) × (m+1)`, where `n = s1.size()` and `m = s2.size()`. `dp[i][j]` represents the minimum edit distance between the first `i` characters of `s1` and the first `j` characters of `s2`. The base cases are: `dp[i][0] = i` (delete all `i` characters from `s1`) and `dp[0][j] = j` (insert all `j` characters into `s1`). For each `i` from 1 to `n` and `j` from 1 to `m`, if `s1[i-1] == s2[j-1]`, then `dp[i][j] = dp[i-1][j-1]` (no operation needed). Otherwise, take the minimum of three options: delete `s1[i-1]` (dp[i-1][j]), insert `s2[j-1]` (dp[i][j-1]), or substitute (dp[i-1][j-1]), and add 1 for the operation. The final answer is `dp[n][m]`. Edge cases: both strings empty returns 0; one string empty returns the length of the other; identical strings return 0. The algorithm runs in O(n·m) time and O(n·m) space. It is optimal for this problem because we test all possible prefixes and consider all three edit operations systematically.
