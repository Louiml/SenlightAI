/*
Write a C++ function that, given a positive integer `n` (where `1 ≤ n ≤ 10^4`), returns the minimal number of perfect square numbers (such as 1, 4, 9, 16, ...) that sum exactly to `n`. For example, `n = 12` can be expressed as `4 + 4 + 4` (3 squares) or `9 + 1 + 1 + 1` (4 squares), so the answer is 3. The function must handle all values within the given range efficiently and return the result as an `int`.
*/
#include <vector>
#include <climits>
#include <algorithm>

// Returns the minimum number of perfect squares that sum to n.
int minimalPerfectSquares(int n) {
    // dp[k] = minimum number of perfect squares summing to k
    std::vector<int> dp(n + 1, INT_MAX / 2);
    dp[0] = 0;

    // Precompute all perfect squares <= n
    std::vector<int> squares;
    for (int j = 1; j * j <= n; ++j) {
        squares.push_back(j * j);
    }

    // Bottom-up DP: compute dp[1] through dp[n]
    for (int k = 1; k <= n; ++k) {
        for (int square : squares) {
            if (square > k) break; // larger squares cannot be used for this k
            dp[k] = std::min(dp[k], dp[k - square] + 1);
        }
    }

    return dp[n];
}
#include <cassert>

int main() {
    // Base cases
    assert(minimalPerfectSquares(1) == 1);   // 1
    assert(minimalPerfectSquares(2) == 2);   // 1 + 1
    assert(minimalPerfectSquares(4) == 1);   // 4
    // Common values
    assert(minimalPerfectSquares(12) == 3);  // 4 + 4 + 4
    assert(minimalPerfectSquares(13) == 2);  // 9 + 4
    assert(minimalPerfectSquares(16) == 1);  // 16
    assert(minimalPerfectSquares(17) == 2);  // 16 + 1
    assert(minimalPerfectSquares(18) == 2);  // 9 + 9
    assert(minimalPerfectSquares(19) == 3);  // 9 + 9 + 1 or 16 + 1 + 1 + 1
    // Larger boundary check
    assert(minimalPerfectSquares(10000) == 1); // 100^2
    // Legendre's three-square theorem example: 7 = 4 + 1 + 1 + 1? Actually 7 = 4+1+1+1 is 4 squares, but also 1+1+1+1+1+1+1 is 7. Minimum is 4.
    assert(minimalPerfectSquares(7) == 4);  // 4 + 1 + 1 + 1

    return 0;
}
// The solution uses bottom-up dynamic programming. Let `dp[k]` represent the minimum number of perfect squares needed to sum to exactly `k`. We initialize `dp[0] = 0` because zero requires zero squares, and set all other entries to a large sentinel like `INT_MAX / 2` to avoid overflow when adding 1. Precompute all perfect squares `s = j*j` where `s <= n`. Then iterate `k` from 1 to `n`; for each square `s`, if `s <= k`, we update `dp[k] = min(dp[k], dp[k - s] + 1)`. This transition assumes we use one square `s` and optimally solve the remainder `k - s`. The answer is `dp[n]`. Edge cases: `n = 1` returns 1 (since 1 is a perfect square); `n = 2` returns 2 (1+1); `n = 4` returns 1 (4 itself). The time complexity is `O(n * sqrt(n))` because for each `k` we consider at most `floor(sqrt(k))` squares, and space complexity is `O(n)` for the DP array plus `O(sqrt(n))` for the precomputed squares.
