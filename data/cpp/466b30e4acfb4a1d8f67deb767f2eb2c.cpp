/*
Write a C++ function `minimumEditDistance(const std::string& source, const std::string& target)` that returns the minimum number of single-character insertions, deletions, or substitutions required to transform `source` into `target`. The function must compute the classic Levenshtein distance using dynamic programming. The first input string is the source (what we start with), the second is the target (what we want to reach). The strings may be empty, may contain any printable ASCII characters, and may have different lengths. The function should be efficient for strings up to 1000 characters each. Return the integer distance.
*/

#include <string>
#include <vector>
#include <algorithm>

// Compute the minimum edit distance (Levenshtein) between two strings.
// source: the initial string, target: the desired string.
// Returns the minimum number of insertions, deletions, or substitutions.
int minimumEditDistance(const std::string& source, const std::string& target) {
    const int n = static_cast<int>(source.size());
    const int m = static_cast<int>(target.size());

    // dp[i][j] = edit distance between source[i:] and target[j:]
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    // Base cases: one string empty
    for (int j = m - 1; j >= 0; --j) {
        dp[n][j] = dp[n][j + 1] + 1; // delete all target chars (or insert into source)
    }
    for (int i = n - 1; i >= 0; --i) {
        dp[i][m] = dp[i + 1][m] + 1; // delete all source chars (or insert into target)
    }

    // Fill table bottom-up
    for (int i = n - 1; i >= 0; --i) {
        for (int j = m - 1; j >= 0; --j) {
            if (source[i] == target[j]) {
                dp[i][j] = dp[i + 1][j + 1]; // no operation
            } else {
                dp[i][j] = 1 + std::min({
                    dp[i + 1][j],     // delete source[i]
                    dp[i][j + 1],     // insert target[j]
                    dp[i + 1][j + 1]  // substitute
                });
            }
        }
    }

    return dp[0][0];
}

#include <cassert>
#include <string>

// Declare the function from the solution (or include the solution header)
int minimumEditDistance(const std::string& source, const std::string& target);

int main() {
    // Base cases
    assert(minimumEditDistance("", "") == 0);
    assert(minimumEditDistance("abc", "") == 3);
    assert(minimumEditDistance("", "xyz") == 3);

    // Simple cases
    assert(minimumEditDistance("kitten", "sitting") == 3); // classic example
    assert(minimumEditDistance("flaw", "lawn") == 2);
    assert(minimumEditDistance("intention", "execution") == 5);

    // Identical strings
    assert(minimumEditDistance("same", "same") == 0);

    // Single character differences
    assert(minimumEditDistance("a", "b") == 1);
    assert(minimumEditDistance("ab", "ac") == 1);

    // Reverse strings – full replacement needed
    assert(minimumEditDistance("abc", "cba") == 2); // e.g., substitute a->c and c->a

    // Longer strings with mixed operations
    assert(minimumEditDistance("horse", "ros") == 3);

    // Unicode/ASCII with spaces and punctuation
    assert(minimumEditDistance("hello world", "hello there") == 5); // "world" vs "there" differ by 5

    return 0;
}

// The problem is the standard edit distance (Levenshtein distance). We define `dp[i][j]` as the minimum edit distance between the suffix `source[i:]` and `target[j:]` (or equivalently, the prefix version). Using a bottom-up DP table of size `(n+1) × (m+1)` where `n = source.length()`, `m = target.length()`. Base cases: `dp[n][m] = 0` (both empty); `dp[n][j] = m - j` (deleting all remaining target characters); `dp[i][m] = n - i` (inserting all remaining source characters). The recurrence: if `source[i] == target[j]`, then `dp[i][j] = dp[i+1][j+1]` (no operation needed); otherwise, `dp[i][j] = 1 + min(dp[i+1][j]` (delete from source), `dp[i][j+1]` (insert into source), `dp[i+1][j+1]` (substitute)). The answer is `dp[0][0]`. Edge cases: both empty returns 0; one empty returns the length of the other. Time complexity is `O(n*m)` and space is `O(n*m)`. The code avoids unnecessary includes and uses `const` references.
