/*
Given two strings `s1` and `s2`, write a C++ function `editDistance` that returns the minimum number of single-character operations (insertion, deletion, or substitution) required to transform `s1` into `s2`. The strings may contain lowercase English letters only, and can be empty. The function must be efficient for strings up to 1000 characters in length, and must handle cases where one string is much longer than the other, identical strings, and completely different strings.
*/

#include <string>
#include <vector>
#include <algorithm>

// Compute the minimum edit distance (Levenshtein distance) between s1 and s2.
int editDistance(const std::string& s1, const std::string& s2) {
    int n = static_cast<int>(s1.size());
    int m = static_cast<int>(s2.size());
    
    // dp[i][j] = distance between first i chars of s2 and first j chars of s1.
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    
    // Base cases: empty string versus non-empty prefix.
    for (int i = 0; i <= m; ++i) dp[i][0] = i;  // inserting i chars into empty s1
    for (int j = 0; j <= n; ++j) dp[0][j] = j;  // deleting j chars from s1
    
    // Fill the table.
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s2[i - 1] == s1[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + std::min({dp[i - 1][j],      // delete from s1
                                         dp[i][j - 1],      // insert into s1
                                         dp[i - 1][j - 1]}); // substitute
            }
        }
    }
    
    return dp[m][n];
}

#include <cassert>

int main() {
    // Basic transformations
    assert(editDistance("", "") == 0);
    assert(editDistance("abc", "abc") == 0);
    assert(editDistance("abc", "") == 3);
    assert(editDistance("", "xyz") == 3);
    
    // Single character operations
    assert(editDistance("kitten", "sitting") == 3); // classic example
    assert(editDistance("flaw", "lawn") == 2);
    assert(editDistance("intention", "execution") == 5);
    
    // Reversal and different lengths
    assert(editDistance("abc", "cba") == 2);
    assert(editDistance("a", "ab") == 1);
    assert(editDistance("ab", "a") == 1);
    
    // More edge cases
    assert(editDistance("same", "same") == 0);
    assert(editDistance("longer", "short") == 5);
    assert(editDistance("abcde", "fghij") == 5); // all different, 5 substitutions
    
    // Up to 1000 length sanity check (should run quickly)
    std::string big1(1000, 'a');
    std::string big2(1000, 'a');
    assert(editDistance(big1, big2) == 0);
    std::string big3(1000, 'b');
    assert(editDistance(big1, big3) == 1000); // all substitutions
    
    return 0;
}

// The solution uses dynamic programming with a 2D table `dp` where `dp[i][j]` represents the minimum edit distance between the first `i` characters of `s2` and the first `j` characters of `s1` (note the orientation matches the original snippet, but it's symmetric). Initialize the first row and column with the cost of inserting/deleting all characters: `dp[0][j] = j` and `dp[i][0] = i`. For each pair `(i,j)`, if the current characters match (`s2[i-1] == s1[j-1]`), then the cost equals the diagonal `dp[i-1][j-1]` (no operation needed). If they differ, the cost is 1 plus the minimum of the three possible operations: deletion from `s1` (`dp[i-1][j]`), insertion into `s1` (`dp[i][j-1]`), or substitution (`dp[i-1][j-1]`). Edge cases include empty strings (distance equals the length of the other string) and identical strings (distance 0). Time complexity is O(m*n) where m and n are lengths of `s2` and `s1` respectively; space complexity is O(m*n) due to the full table, which is acceptable for the given constraints.
