/*
Write a C++ function `minimumRodCutCost` that takes an integer `n` and a vector of `n` positive integer prices `prices` (1-indexed conceptually, where `prices[i]` is the price for a rod of length `i`), and returns the minimum total cost to obtain a rod of length exactly `n` by cutting it into pieces and paying the sum of the prices of the resulting pieces. You may cut the rod into any number of pieces (including zero cuts, i.e., using the whole rod), but every piece must have an integer length between 1 and `n`. The function should handle `n = 0` by returning `0` (since an empty rod costs nothing), and for `n >= 1`, the vector must have at least `n` elements (with indices 0..n-1, where element at index 0 corresponds to length 1, etc.). If prices are such that no combination yields the required length (which cannot happen because using a single piece of length n always works when prices[n-1] exists), the function should still return the best achievable sum. The task is to compute the optimal (minimum) sum using a dynamic programming approach similar to the classic rod-cutting problem.
*/
#include <vector>
#include <algorithm>
#include <cstddef>

// Compute the minimum total cost to obtain a rod of exact length n.
// prices[i] is the price for a piece of length i+1 (0-indexed).
// Returns 0 when n == 0.
int minimumRodCutCost(int n, const std::vector<int>& prices) {
    if (n == 0) {
        return 0;
    }

    // dp[len] = minimum cost to obtain a rod of length 'len'
    std::vector<int> dp(n + 1, 0);

    for (int len = 1; len <= n; ++len) {
        // Option 1: use the whole piece of this length
        dp[len] = prices[len - 1];

        // Option 2: split into two parts j and len-j (j from 1 to len/2)
        for (int j = 1; j <= len / 2; ++j) {
            dp[len] = std::min(dp[len], dp[j] + dp[len - j]);
        }
    }

    return dp[n];
}
#include <cassert>
#include <vector>

int main() {
    // Whole rod is cheapest
    assert(minimumRodCutCost(4, std::vector<int>{1, 5, 8, 9}) == 4);
    // Multiple cuts better: lengths 2+2 cost 5+5=10 vs whole 8? Actually whole 8 is better, but test 3: prices {1,5,8} => whole index2=8, split 1+2=1+5=6, so 6
    assert(minimumRodCutCost(3, std::vector<int>{1, 5, 8}) == 6);
    // With expensive large piece, cutting into ones may be cheaper
    assert(minimumRodCutCost(5, std::vector<int>{2, 2, 100, 100, 100}) == 4);
    // n=1 returns price[0]
    assert(minimumRodCutCost(1, std::vector<int>{7}) == 7);
    // n=0 returns 0
    assert(minimumRodCutCost(0, std::vector<int>{}) == 0);
    // Symmetric test: 4 with prices {10,1,1,1} => whole length4 cost1, best is 1
    assert(minimumRodCutCost(4, std::vector<int>{10, 1, 1, 1}) == 1);
    // All prices equal: any cut same as whole, but whole is fine
    assert(minimumRodCutCost(3, std::vector<int>{4, 4, 4}) == 4);
    // Larger n with random-like values
    assert(minimumRodCutCost(6, std::vector<int>{1, 2, 3, 4, 5, 6}) == 6); // whole is best? Actually 1+1+... costs 6, whole costs 6, so 6
    // Mixed where splitting yields lower cost than whole
    assert(minimumRodCutCost(4, std::vector<int>{5, 1, 1, 5}) == 2); // 2+2 costs 1+1=2
}
// The problem is the classic "minimum-cost rod cutting" (a minimization variant). Let `dp[i]` represent the minimum cost to obtain a rod of length `i`. Base case: `dp[0] = 0` (empty rod). For each `i` from 1 to `n`, we have the option to take the whole piece of length `i` priced at `prices[i-1]`, or we can split the rod into two parts of lengths `j` and `i-j` for any `j` from 1 to `i-1`, paying `dp[j] + dp[i-j]`. However, to avoid redundant checks, we can instead iterate `j` from 1 to `i/2` (since symmetric) and compare with the whole-piece price. Initializing `dp[i] = prices[i-1]` (the whole piece), then for each `j` from 1 to `i/2`, we update `dp[i] = min(dp[i], dp[j] + dp[i-j])`. This is correct because any optimal cut can be expressed as a sum of two subproblems, and the recursion bottoms out at whole pieces. Edge cases: `n = 0` returns 0; `n = 1` returns `prices[0]` (no cuts possible). The algorithm runs in `O(n^2)` time due to the nested loops, and uses `O(n)` space for the `dp` array. It handles any positive integer prices; duplicate or large values are fine. The vector may be longer than necessary, but we only use the first `n` elements.
