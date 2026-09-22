// Write a C++ function `countDistinctSubsequences` that takes two strings, `source` and `target`, and returns the number of distinct ways the `target` string can appear as a subsequence of `source` (i.e., the number of ways to delete some characters from `source` so that the remaining characters equal `target`). The function must handle empty `target` (returning 1, since an empty subsequence can be chosen in exactly one way), empty `source` with non-empty `target` (returning 0), and arbitrary lowercase/uppercase letters. The result may exceed 32-bit range, so return a `long long`. For example, for `source = "rabbbit"` and `target = "rabbit"`, the return is 3, and for `source = "babgbag"` and `target = "bag"`, the return is 5. The function should be `const`-correct and use dynamic programming with a 2D table of size `(target.size()+1) x (source.size()+1)`.
#include <cassert>
#include <string>

// Free function declaration (assumed from solution)
long long countDistinctSubsequences(const std::string& source, const std::string& target);

int main() {
    // Basic examples
    assert(countDistinctSubsequences("rabbbit", "rabbit") == 3);
    assert(countDistinctSubsequences("babgbag", "bag") == 5);
    
    // Empty target - always 1
    assert(countDistinctSubsequences("abc", "") == 1);
    assert(countDistinctSubsequences("", "") == 1);
    
    // Empty source with non-empty target - always 0
    assert(countDistinctSubsequences("", "a") == 0);
    
    // Exact match - only one subsequence
    assert(countDistinctSubsequences("abc", "abc") == 1);
    
    // All same characters - large number of combinations
    assert(countDistinctSubsequences("aaaa", "aa") == 6);  // C(4,2) = 6
    assert(countDistinctSubsequences("aaaaa", "aaa") == 10); // C(5,3) = 10
    
    // No subsequence possible
    assert(countDistinctSubsequences("abc", "def") == 0);
    
    // Longer target than source
    assert(countDistinctSubsequences("abc", "abcd") == 0);
    
    // Case sensitivity
    assert(countDistinctSubsequences("aA", "a") == 1);  // Only lowercase 'a'
    assert(countDistinctSubsequences("Aa", "a") == 1);  // Only lowercase 'a'
    
    // Large count test (could overflow int)
    assert(countDistinctSubsequences("aaaaaaaaaaaaaaaa", "aaaaaaaa") == 12870); // C(16,8) = 12870
    
    return 0;
}
#include <string>
#include <vector>

// Count the number of distinct subsequences of source that equal target.
long long countDistinctSubsequences(const std::string& source, const std::string& target) {
    int n = target.size();
    int m = source.size();
    
    // dp[i][j] = number of ways to form target[0..i-1] from source[0..j-1]
    std::vector<std::vector<long long>> dp(n + 1, std::vector<long long>(m + 1, 0));
    
    // Empty target can be formed in exactly one way (choose no characters)
    for (int j = 0; j <= m; ++j) {
        dp[0][j] = 1;
    }
    // For non-empty target, empty source gives 0 (already initialized)
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (target[i - 1] == source[j - 1]) {
                // Use current source character or skip it
                dp[i][j] = dp[i][j - 1] + dp[i - 1][j - 1];
            } else {
                // Current source character cannot be used
                dp[i][j] = dp[i][j - 1];
            }
        }
    }
    
    return dp[n][m];
}
// The problem is a classic dynamic programming counting problem. Define `dp[i][j]` as the number of distinct subsequences of the first `j` characters of `source` that equal the first `i` characters of `target`. Initialize the first row (`i=0`) to 1 because an empty target can be formed in exactly one way (by choosing no characters). All other entries start at 0. Then iterate through `i` from 1 to `target.size()` and `j` from 1 to `source.size()`. If `target[i-1] == source[j-1]`, then `dp[i][j] = dp[i][j-1] + dp[i-1][j-1]` — the first term counts subsequences that do not use the current `source` character, and the second term counts those that do use it (and match the previous target character). If the characters differ, then `dp[i][j] = dp[i][j-1]` (the current `source` character cannot be used). The final answer is `dp[target.size()][source.size()]`. Edge cases: empty target always returns 1; empty source with non-empty target returns 0; very large counts (e.g., all same characters) may exceed `int`, so use `long long`. Time complexity is `O(n*m)` where `n = target.size()` and `m = source.size()`, and space complexity is `O(n*m)` for the DP table.
