Write a C++ function `int minPerfectSquares(int n)` that, given a positive integer `n`, returns the minimum number of perfect square numbers (e.g., 1, 4, 9, 16, ...) that sum to `n`. For example, `n = 12` can be expressed as `4 + 4 + 4` (3 squares) or `9 + 1 + 1 + 1` (4 squares), so the answer is 3. The function must handle all positive integers up to at least 10,000. You may assume `n >= 1`; do not worry about invalid inputs. The solution must use dynamic programming to avoid exponential time and be efficient for large `n`.
#include <cassert>

int minPerfectSquares(int n);  // Declaration from the solution.

int main() {
    assert(minPerfectSquares(1) == 1);
    assert(minPerfectSquares(2) == 2);      // 1+1
    assert(minPerfectSquares(3) == 3);      // 1+1+1
    assert(minPerfectSquares(4) == 1);      // 4
    assert(minPerfectSquares(12) == 3);     // 4+4+4
    assert(minPerfectSquares(13) == 2);     // 9+4
    assert(minPerfectSquares(16) == 1);     // 16
    assert(minPerfectSquares(17) == 2);     // 16+1
    assert(minPerfectSquares(18) == 2);     // 9+9
    assert(minPerfectSquares(7168) == 4);   // Known result from brute force.
    return 0;
}
#include <vector>
#include <algorithm>

// Returns the minimum number of perfect squares that sum to n.
int minPerfectSquares(int n) {
    // dp[i] stores the minimum squares needed for sum i.
    // Initialize with n+1 as an upper bound (worst case: n ones).
    std::vector<int> dp(n + 1, n + 1);
    dp[0] = 0;  // Zero sum requires zero squares.

    for (int i = 1; i <= n; ++i) {
        // Try every square less than or equal to i.
        for (int j = 1; j * j <= i; ++j) {
            dp[i] = std::min(dp[i], 1 + dp[i - j * j]);
        }
    }
    return dp[n];
}
// The problem is the classic "Perfect Squares" coin-change problem where the coin denominations are the perfect squares (1, 4, 9, ...). We use a bottom-up dynamic programming approach. Create a 1D array `dp` of size `n+1` where `dp[i]` stores the minimum number of squares needed to sum to `i`. Initialize all entries to a large sentinel value (e.g., `n+1` since the answer cannot exceed `n`; using all 1's), and set `dp[0] = 0` because zero requires zero squares. Then for each target value `i` from 1 to `n`, we consider every square `j*j` that is ≤ `i`. For each such square, we update `dp[i]` with `1 + dp[i - j*j]`, taking the minimum over all valid squares. The logic relies on the optimal substructure property: the optimal solution for `i` uses one square `j*j` plus the optimal solution for the remaining amount `i - j*j`. Edge cases: `n = 1` trivially returns 1; `n` being a perfect square returns 1 directly because we find it in the loop. Time complexity is `O(n * sqrt(n))` because for each `i` (up to `n`), we iterate over `j` from 1 to `sqrt(i)`; space complexity is `O(n)` for the `dp` array.
