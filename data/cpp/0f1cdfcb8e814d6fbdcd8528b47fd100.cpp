// Write a C++ function named `longestCommonSubsequenceLength` that takes two vectors of integers (by const reference) and returns the length of the longest common subsequence (LCS) between them. A subsequence is a sequence that appears in the same relative order but not necessarily contiguous. The function must handle empty input vectors, vectors with all distinct elements, and vectors with repeated elements. The solution should use dynamic programming with a 2D table, but avoid manual memory allocation by using `std::vector<std::vector<int>>` for safety and clarity. The function must be `const`-correct and reside in a standalone file with only necessary headers and no `main` function.
// The problem is a classic dynamic programming task: compute the length of the longest common subsequence between two integer sequences. Define a 2D DP table `dp[i][j]` where `i` ranges from 0 to `a.size()` and `j` from 0 to `b.size()`. `dp[i][j]` represents the LCS length of the prefixes `a[0..i-1]` and `b[0..j-1]`. The base cases are `dp[i][0] = 0` and `dp[0][j] = 0` for all `i` and `j` since an empty sequence has no common subsequence with anything. For each `i` from 1 to `a.size()` and `j` from 1 to `b.size()`, if the last elements match (`a[i-1] == b[j-1]`), then `dp[i][j] = dp[i-1][j-1] + 1`. Otherwise, `dp[i][j] = max(dp[i-1][j], dp[i][j-1])`, taking the better of excluding the current element of `a` or the current element of `b`. The answer is `dp[a.size()][b.size()]`. Edge cases: if either vector is empty, the LCS length is 0 (covered by base cases). If all elements are distinct or repeated, the algorithm correctly handles them. Time complexity is O(n*m) where n and m are the sizes of the input vectors, and space complexity is also O(n*m) for the DP table. The implementation uses `std::vector<std::vector<int>>` to avoid manual memory management and ensure exception safety.
#include <vector>
#include <algorithm>

// Returns the length of the longest common subsequence between two integer vectors.
int longestCommonSubsequenceLength(const std::vector<int>& a, const std::vector<int>& b) {
    const std::size_t m = a.size();
    const std::size_t n = b.size();

    // DP table: dp[i][j] = LCS length of a[0..i-1] and b[0..j-1]
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    // Fill the table iteratively
    for (std::size_t i = 1; i <= m; ++i) {
        for (std::size_t j = 1; j <= n; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    return dp[m][n];
}
#include <cassert>
#include <vector>
#include "solution.h" // assuming the solution is in a header or included

int main() {
    // Test 1: Empty vectors
    std::vector<int> a1 = {};
    std::vector<int> b1 = {};
    assert(longestCommonSubsequenceLength(a1, b1) == 0);

    // Test 2: One empty vector
    std::vector<int> a2 = {1, 2, 3};
    std::vector<int> b2 = {};
    assert(longestCommonSubsequenceLength(a2, b2) == 0);
    assert(longestCommonSubsequenceLength(b2, a2) == 0);

    // Test 3: Identical sequences
    std::vector<int> a3 = {1, 2, 3, 4};
    std::vector<int> b3 = {1, 2, 3, 4};
    assert(longestCommonSubsequenceLength(a3, b3) == 4);

    // Test 4: No common elements
    std::vector<int> a4 = {1, 2, 3};
    std::vector<int> b4 = {4, 5, 6};
    assert(longestCommonSubsequenceLength(a4, b4) == 0);

    // Test 5: Common subsequence with gaps
    std::vector<int> a5 = {1, 2, 3, 4, 5};
    std::vector<int> b5 = {2, 4, 5};
    assert(longestCommonSubsequenceLength(a5, b5) == 3);

    // Test 6: Repeated elements
    std::vector<int> a6 = {1, 1, 2, 2};
    std::vector<int> b6 = {1, 2, 1, 2};
    assert(longestCommonSubsequenceLength(a6, b6) == 3); // e.g., {1,1,2} or {1,2,2}

    // Test 7: Reverse order
    std::vector<int> a7 = {1, 2, 3};
    std::vector<int> b7 = {3, 2, 1};
    assert(longestCommonSubsequenceLength(a7, b7) == 1); // any single element

    // Test 8: Single element common
    std::vector<int> a8 = {10};
    std::vector<int> b8 = {10, 20};
    assert(longestCommonSubsequenceLength(a8, b8) == 1);

    // Test 9: Longer sequences with mixed matches
    std::vector<int> a9 = {5, 3, 8, 9, 1, 2};
    std::vector<int> b9 = {3, 8, 2, 1, 7};
    assert(longestCommonSubsequenceLength(a9, b9) == 3); // e.g., {3,8,2} or {3,8,1}

    return 0;
}
