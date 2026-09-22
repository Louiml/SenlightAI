Given an integer `n` (1 ≤ n ≤ 10^5), write a C++ function that returns the number of ways to tile a 2 x n board using 2 x 1 dominoes and 2 x 2 squares (which can be placed as a single 2x2 block, covering all four cells). The function should return the answer modulo 1e9+7. This is a classic DP problem where the recurrence is `dp[i] = dp[i-1] + 2 * dp[i-2]` because the last column can be either a vertical domino (1 way) or a 2x2 square plus a horizontal domino pair (2 ways). The base cases are `dp[0] = 1` and `dp[1] = 1`. Edge cases: if `n=0` return 1, if `n=1` return 1. For `n=2`, the answer should be 3 (two vertical dominoes, two horizontal dominoes stacked, or one 2x2 square). Your function must be standalone, use `long long` for intermediate multiplication to avoid overflow, and apply modulo at each step.

// The problem is a classic dynamic programming counting problem. We define `dp[i]` as the number of distinct tilings for a 2 x i board. Consider the leftmost uncovered column: either we place a vertical 2x1 domino covering the first column (leaving a 2 x (i-1) board), or we place a 2x2 square covering the first two columns (leaving a 2 x (i-2) board), or we place two horizontal dominoes stacked covering the first two columns — each of the latter two options is distinct and both leave a 2 x (i-2) board. Therefore the recurrence is `dp[i] = dp[i-1] + 2 * dp[i-2]`. Base cases: `dp[0] = 1` (empty board) and `dp[1] = 1` (only one vertical domino). For `n=2`, we get `dp[2] = dp[1] + 2*dp[0] = 1+2=3`, which matches the reasoning. We must take modulo 1e9+7 at each step to keep numbers small. For multiplication `2 * dp[i-2]`, use `long long` and then modulo. The algorithm runs in O(n) time and O(n) space (or we can optimize to O(1) space since only last two values are needed). Edge cases: n=0 (should return 1), n=1 (1), and large n (ensure `i-2` never negative by looping from i=2). The time complexity is O(n), space O(1) if optimized.

#include <vector>

// Count ways to tile a 2 x n board with 2x1 dominoes and 2x2 squares.
// Returns the answer modulo 1e9+7.
long long countTilings2xn(int n) {
    const long long MOD = 1000000007LL;
    if (n == 0) return 1;
    if (n == 1) return 1;
    
    long long prev2 = 1; // dp[0]
    long long prev1 = 1; // dp[1]
    
    for (int i = 2; i <= n; ++i) {
        long long current = (prev1 + 2 * prev2) % MOD;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}

#include <cassert>

int main() {
    // Base cases
    assert(countTilings2xn(0) == 1);
    assert(countTilings2xn(1) == 1);
    // n=2: vertical, two horizontal, one square => 3
    assert(countTilings2xn(2) == 3);
    // n=3: dp[3] = dp[2] + 2*dp[1] = 3 + 2 = 5
    assert(countTilings2xn(3) == 5);
    // n=4: dp[4] = dp[3] + 2*dp[2] = 5 + 6 = 11
    assert(countTilings2xn(4) == 11);
    // n=5: dp[5] = dp[4] + 2*dp[3] = 11 + 10 = 21
    assert(countTilings2xn(5) == 21);
    // Check recurrence for a moderate value
    assert(countTilings2xn(10) == 683);
    // Large n modulo check: compute known value? Just ensure it doesn't crash and is non-negative.
    long long big = countTilings2xn(100000);
    assert(big >= 0 && big < 1000000007LL);
    return 0;
}
