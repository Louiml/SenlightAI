/*
You are given a rod of length `w` centimeters and a price list for pieces of length 1 through `w`. The price for a piece of length `i` is provided as `arr[i]`, where `arr[i] = -1` means that piece of that exact length cannot be sold individually (i.e., it is not available for direct purchase). You may cut the rod into any number of pieces (including keeping it whole), but you can only use pieces whose lengths have a non-negative price. Determine the maximum revenue obtainable by cutting the rod and selling the pieces, or return -1 if it is impossible to achieve exactly length `w` using any combination of available piece lengths. Write a C++ function `int maxRevenue(int w, const std::vector<int>& prices)` where `prices[i]` (for `i` from 1 to `w`) holds the price of a piece of length `i`, or `-1` if unavailable. The vector has size `w+1` with `prices[0]` unused (you may ignore index 0). If no valid combination sums to exactly `w`, return -1.
*/

#include <vector>
#include <algorithm>
#include <climits>

// Compute the maximum revenue for a rod of exact length w using available piece prices.
// prices[i] for i=1..w holds the price for a piece of length i, or -1 if unavailable.
// Returns -1 if no combination of available pieces sums to exactly w.
int maxRevenue(int w, const std::vector<int>& prices) {
    // dp[i] = max revenue for a rod of exact length i.
    // Use a large negative to represent impossible states.
    const int NEG_INF = INT_MIN / 2; // avoid overflow when adding
    std::vector<int> dp(w + 1, NEG_INF);
    dp[0] = 0; // empty rod yields zero revenue

    for (int i = 1; i <= w; ++i) {
        for (int j = 1; j <= i; ++j) {
            if (prices[j] == -1) continue; // piece length j not available
            if (dp[i - j] != NEG_INF) {
                dp[i] = std::max(dp[i], dp[i - j] + prices[j]);
            }
        }
    }

    return (dp[w] == NEG_INF) ? -1 : dp[w];
}

#include <cassert>
#include <vector>

int maxRevenue(int, const std::vector<int>&);

int main() {
    // prices[0] is unused; we provide index 1..w
    // Test 1: rod length 4, pieces: len1=2, len2=5, len3=9, len4=10
    // Best: 2+2 = 4 -> 2+2? Actually 2+2=4 gives 5+5=10, but 1+3=2+9=11, 4 gives 10 -> max 11
    std::vector<int> p1 = {0, 2, 5, 9, 10};
    assert(maxRevenue(4, p1) == 11); // 1+3 = 2+9 = 11

    // Test 2: rod length 5, only len3 available with price 7, len5 with price 12
    // Can't make 5 with 3s alone, but len5 available -> 12
    std::vector<int> p2 = {0, -1, -1, 7, -1, 12};
    assert(maxRevenue(5, p2) == 12);

    // Test 3: rod length 6, only len4 available with price 100 -> impossible (4+? can't sum to 6)
    std::vector<int> p3 = {0, -1, -1, -1, 100, -1, -1};
    assert(maxRevenue(6, p3) == -1);

    // Test 4: rod length 3, all available with prices 1 for len1, 10 for len2, 20 for len3
    // Best: len3 alone = 20, or 1+2 = 1+10=11, or 1+1+1=3 -> max 20
    std::vector<int> p4 = {0, 1, 10, 20};
    assert(maxRevenue(3, p4) == 20);

    // Test 5: rod length 0 (edge case) -> 0
    std::vector<int> p5 = {0};
    assert(maxRevenue(0, p5) == 0);

    // Test 6: rod length 2, len1 price 3, len2 price -1 (unavailable)
    // Use two length1 pieces: 3+3=6
    std::vector<int> p6 = {0, 3, -1};
    assert(maxRevenue(2, p6) == 6);

    // Test 7: rod length 5, only len2 price 4, len3 price 5
    // 2+3 = 9, or 2+2+? (can't use 2+2+1), so 9
    std::vector<int> p7 = {0, -1, 4, 5, -1, -1};
    assert(maxRevenue(5, p7) == 9);

    // Test 8: rod length 1 with price 0 (free piece) -> 0
    std::vector<int> p8 = {0, 0};
    assert(maxRevenue(1, p8) == 0);

    // Test 9: rod length 4, prices: len1=1, len2=3, len3=6, len4=9
    // Best: len4=9, or len2+len2=6, or len1+len3=7 -> 9
    std::vector<int> p9 = {0, 1, 3, 6, 9};
    assert(maxRevenue(4, p9) == 9);

    // Test 10: rod length 3, only len1 price 5 -> 5+5+5=15
    std::vector<int> p10 = {0, 5, -1, -1};
    assert(maxRevenue(3, p10) == 15);

    return 0;
}

// This is a classic unbounded knapsack / coin-change-like problem, but here we maximize revenue instead of minimizing count. The main algorithm uses dynamic programming: let `dp[i]` be the maximum revenue obtainable for a rod of length exactly `i` using any number of pieces, where each piece length is from 1 to `w` and has a non-negative price. Initialize `dp[0] = 0` (empty rod yields 0 revenue) and all other `dp[i] = -infinity` (or a very small number) to denote impossibility. For each target length `i` from 1 to `w`, iterate over all piece lengths `j` from 1 to `i`, and if `prices[j] != -1` and `dp[i-j]` is not impossible, update `dp[i] = max(dp[i], dp[i-j] + prices[j])`. This allows unlimited use of each piece because we consider all `j` for each `i` and reuse previously computed `dp[i-j]` which may themselves have used any pieces. Edge cases: if `w == 0`, the answer is 0 (no cut, no revenue). If for some length `i` no combination works, `dp[i]` remains impossible. The final answer is `dp[w]` if it is not impossible, else -1. Note that prices can be zero or positive; a zero price is still sellable and helpful for filling length. The time complexity is O(w^2) because for each of `w` target lengths we try up to `w` piece sizes, and space complexity is O(w) for the DP array. Potential pitfalls: handling `INT_MIN` to avoid overflow when adding, and ignoring index 0 since it is unused.
