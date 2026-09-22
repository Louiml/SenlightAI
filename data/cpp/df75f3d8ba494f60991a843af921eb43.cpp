/*
Write a C++ function `countDistinctSubsequences` that takes two strings `A` and `B` as input and returns the number of distinct ways to form the sequence `B` as a subsequence of `A`. The function should compute the result modulo \(10^9+7\). Since the lengths of both strings can be up to 700, the solution must be efficient. The function should handle cases where `B` is longer than `A` (return 0), empty strings (if B is empty, return 1; if A is empty and B not, return 0), and duplicate characters in either string. The result must be returned as an integer.
*/

#include <string>
#include <vector>

// Count the number of distinct ways to form B as a subsequence of A, modulo 1e9+7.
int countDistinctSubsequences(const std::string& A, const std::string& B) {
    const int MOD = 1000000007;
    int lenA = A.size();
    int lenB = B.size();
    
    if (lenB > lenA) return 0;
    if (lenB == 0) return 1; // empty B can be formed in exactly one way
    
    // dp[i][j] = ways to form first i chars of B from first j chars of A
    std::vector<std::vector<long long>> dp(lenB + 1, std::vector<long long>(lenA + 1, 0));
    
    // Initialize: dp[0][j] = 1 for all j (empty B)
    for (int j = 0; j <= lenA; ++j) {
        dp[0][j] = 1;
    }
    
    // Fill the DP table
    for (int i = 1; i <= lenB; ++i) {
        for (int j = 1; j <= lenA; ++j) {
            if (B[i-1] != A[j-1]) {
                dp[i][j] = dp[i][j-1]; // skip A[j-1]
            } else {
                dp[i][j] = (dp[i][j-1] + dp[i-1][j-1]) % MOD;
            }
        }
    }
    
    return static_cast<int>(dp[lenB][lenA]);
}

#include <cassert>

int main() {
    // Example 1: identical strings
    assert(countDistinctSubsequences("abc", "abc") == 1);
    
    // Example 2: classic rabbbit/rabbit
    assert(countDistinctSubsequences("rabbbit", "rabbit") == 3);
    
    // B longer than A
    assert(countDistinctSubsequences("abc", "abcd") == 0);
    
    // Empty B (valid per constraints? The constraints say length >= 1, but we handle it robustly)
    assert(countDistinctSubsequences("anything", "") == 1);
    
    // Single character repeated
    assert(countDistinctSubsequences("aaa", "a") == 3);
    
    // Single character, two a's in B
    assert(countDistinctSubsequences("aaa", "aa") == 3);
    
    // No match at all
    assert(countDistinctSubsequences("abc", "xyz") == 0);
    
    // Large value: all a's (6C3 = 20)
    assert(countDistinctSubsequences("aaaaaa", "aaa") == 20);
    
    // Another test with distance
    assert(countDistinctSubsequences("abac", "ac") == 2); // positions 0-3 and 2-3
    
    // Ensure modulo works (not easy to test with small, but verify no crash)
    assert(countDistinctSubsequences("b" + std::string(699, 'a'), "b") == 1);
    
    return 0;
}

// The problem is a classic dynamic programming (DP) counting subsequence match. We define `dp[i][j]` as the number of ways to form the first `i` characters of `B` from the first `j` characters of `A`. The recurrence is:
// - Base cases: `dp[0][j] = 1` for all `j` (an empty B can always be formed by deleting all characters of A, exactly one way). `dp[i][0] = 0` for `i > 0` (a non-empty B cannot be formed from an empty A).
// - For `i > 0` and `j > 0`:
//   - If `B[i-1] != A[j-1]`, then we cannot use `A[j-1]` as the match for `B[i-1]`, so `dp[i][j] = dp[i][j-1]`.
//   - If `B[i-1] == A[j-1]`, then we have two options: either skip `A[j-1]` or use it to match `B[i-1]`. So `dp[i][j] = dp[i][j-1] + dp[i-1][j-1]`.
//
// The final answer is `dp[B.length()][A.length()]` modulo \(10^9+7\). Edge cases: if `B.length() > A.length()`, return 0 immediately. If `B` is empty, the number of ways is 1 (delete all characters from A). The algorithm uses a 2D DP table of size `(lenB+1) x (lenA+1)`, so time complexity is \(O(|A| \cdot |B|)\) and space complexity is \(O(|A| \cdot |B|)\). To reduce constant overhead, we could use 1D DP, but the 2D approach is straightforward and fits the constraints (700x700 = 490,000 cells, well within memory limits). To avoid overflow, each addition is taken modulo \(10^9+7\).
