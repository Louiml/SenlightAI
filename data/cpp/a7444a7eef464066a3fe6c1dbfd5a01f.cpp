/*
Write a C++ function `int leastSquaresSum(int n)` that, given a non-negative integer `n`, returns the minimum number of perfect squares (1, 4, 9, 16, ...) whose sum equals `n`. For example, `12 = 4 + 4 + 4` (3 squares) or `12 = 9 + 1 + 1 + 1` (4 squares), so the answer is 3; `13 = 9 + 4` (2 squares). The function must handle `n = 0` by returning 0, and must work for all `n` up to at least 50,000. Assume the input is always non-negative. Do not use any external libraries beyond the C++ standard library (e.g., `<vector>`, `<algorithm>`, `<climits>`). Your implementation must be efficient enough to handle the maximum input size in reasonable time.
*/

#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum number of perfect squares that sum to n.
// n must be non-negative.
int leastSquaresSum(int n) {
    if (n < 0) {
        return 0; // Not expected; handle gracefully.
    }
    if (n == 0) {
        return 0;
    }
    
    std::vector<int> dp(n + 1, INT_MAX);
    dp[0] = 0;
    dp[1] = 1;
    
    for (int i = 2; i <= n; ++i) {
        for (int j = 1; j * j <= i; ++j) {
            dp[i] = std::min(dp[i], dp[i - j * j] + 1);
        }
    }
    
    return dp[n];
}

#include <cassert>

int main() {
    assert(leastSquaresSum(0) == 0);
    assert(leastSquaresSum(1) == 1);
    assert(leastSquaresSum(2) == 2); // 1+1
    assert(leastSquaresSum(3) == 3); // 1+1+1
    assert(leastSquaresSum(4) == 1); // 4
    assert(leastSquaresSum(12) == 3); // 4+4+4
    assert(leastSquaresSum(13) == 2); // 9+4
    assert(leastSquaresSum(16) == 1); // 16
    assert(leastSquaresSum(18) == 2); // 9+9
    assert(leastSquaresSum(100) == 1); // 100
    return 0;
}

// This is a classic dynamic programming problem (also known as "Perfect Squares" or "Coin Change" with square denominations). The recurrence is based on the idea that for any number `i`, if we subtract a perfect square `j*j` from it, the remaining `i - j*j` must also be expressible as a sum of squares with minimal count. Thus, `dp[i] = min(dp[i - j*j] + 1)` for all `j` where `j*j <= i`. We initialize `dp[0] = 0` (0 squares to sum to 0) and `dp[1] = 1` (1 square = 1). For each `i` from 2 to `n`, we iterate over all possible squares up to `i` and take the minimum. Edge cases: `n = 0` returns 0; `n = 1` returns 1; numbers that are themselves perfect squares (e.g., 4, 9, 16) will return 1 because `j*j == i` yields `dp[0] + 1`. Time complexity is O(n * sqrt(n)), since for each `i` we loop over `j` up to sqrt(i). Space complexity is O(n) for the DP array. The algorithm is correct because it considers all possible last square used in the minimal decomposition.
