Write a C++ function that takes two strings as input and returns the minimum number of operations required to convert the first string into the second string using only three allowed operations: insertion (add a character), deletion (remove a character), and substitution (replace one character with another). The strings may be empty, contain lowercase English letters, and may have different lengths. The function should compute the classic edit distance (Levenshtein distance) between the two strings and return the integer result. For example, converting `"kitten"` to `"sitting"` requires 3 operations (substitute 'k'→'s', substitute 'e'→'i', insert 'g').
The solution uses dynamic programming with a 2D table `dp[i][j]` representing the minimum edit distance between the first `i` characters of string `s1` and the first `j` characters of string `s2`. Initialize the first row and column by setting `dp[0][j] = j` (inserting `j` characters) and `dp[i][0] = i` (deleting `i` characters). For each cell `(i, j)` with `i ≥ 1` and `j ≥ 1`, if the current characters match (`s1[i-1] == s2[j-1]`), then `dp[i][j] = dp[i-1][j-1]` (no operation needed). Otherwise, take the minimum of three possibilities: a substitution from `dp[i-1][j-1]` (replace one character), an insertion from `dp[i][j-1]`, or a deletion from `dp[i-1][j]`, and add 1. The answer is `dp[n1][n2]`. Edge cases include empty strings (the answer is simply the length of the other string) and identical strings (answer is 0). Time complexity is `O(n1 * n2)` and space complexity is also `O(n1 * n2)` due to the full table.
#include <string>
#include <vector>
#include <algorithm>

// Compute the minimum edit distance (Levenshtein distance) between two strings.
int editDistance(const std::string& s1, const std::string& s2) {
    int n1 = static_cast<int>(s1.length());
    int n2 = static_cast<int>(s2.length());

    // dp[i][j] = edit distance between s1[0..i-1] and s2[0..j-1]
    std::vector<std::vector<int>> dp(n1 + 1, std::vector<int>(n2 + 1, 0));

    // Base cases: converting to/from empty strings
    for (int j = 0; j <= n2; ++j) {
        dp[0][j] = j;  // insert j characters into empty s1
    }
    for (int i = 0; i <= n1; ++i) {
        dp[i][0] = i;  // delete i characters from s1
    }

    // Fill the DP table
    for (int i = 1; i <= n1; ++i) {
        for (int j = 1; j <= n2; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];  // characters match, no cost
            } else {
                dp[i][j] = 1 + std::min({
                    dp[i - 1][j - 1],  // substitution
                    dp[i][j - 1],      // insertion
                    dp[i - 1][j]       // deletion
                });
            }
        }
    }

    return dp[n1][n2];
}
#include <cassert>
#include <string>

// Declaration from the solution function (assumed to be included separately)
int editDistance(const std::string& s1, const std::string& s2);

int main() {
    // Basic cases
    assert(editDistance("", "") == 0);
    assert(editDistance("abc", "abc") == 0);
    assert(editDistance("", "abc") == 3);
    assert(editDistance("abc", "") == 3);

    // Classic examples
    assert(editDistance("kitten", "sitting") == 3);
    assert(editDistance("horse", "ros") == 3);
    assert(editDistance("intention", "execution") == 5);

    // Insertion and deletion only
    assert(editDistance("a", "ab") == 1);
    assert(editDistance("ab", "a") == 1);
    assert(editDistance("abcd", "abcde") == 1);

    // Substitution only
    assert(editDistance("cat", "cut") == 1);
    assert(editDistance("a", "b") == 1);

    // Longer strings with mixed operations
    assert(editDistance("algorithm", "altruistic") == 6);
    assert(editDistance("book", "back") == 2);

    // Edge case: single character differences
    assert(editDistance("x", "x") == 0);
    assert(editDistance("x", "y") == 1);
}
