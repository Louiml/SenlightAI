// You are given a rod of length `n` (a positive integer) and two arrays: `lengths` and `prices`, each of size `m` (where `m > 0`), representing the available cut lengths and their corresponding prices. You may cut the rod into pieces whose lengths are taken from the `lengths` array, and you may reuse the same length as many times as you want. Write a C++ function `int maxRodValue(int n, const vector<int>& lengths, const vector<int>& prices)` that returns the maximum total price obtainable by cutting the rod of total length `n` into zero or more pieces, where no piece exceeds length `n`, and any un-used length is allowed (i.e., you don't need to use all parts of the rod?). Clarification: you must use the entire rod length exactly: the sum of the lengths of the chosen pieces must equal `n`. If it is impossible to achieve exactly `n` with the given lengths, return 0 (meaning no valid cutting yields the exact total, so the optimal value is 0). The `lengths` and `prices` arrays are parallel (lengths[i] corresponds to prices[i]), and lengths are positive integers. The input may contain duplicate lengths, but you should treat them as separate options (their prices may differ). The rod length `n` can be up to 1000, and the number of length-price pairs `m` up to 1000. You may assume all prices are non-negative integers.

#include <cassert>
#include <vector>

// Include the solution function (assume it is defined above or in a header).
// For completeness, we paste the function again here? No, in the test section we call it directly.
// The test code below assumes maxRodValue is already declared.

int main() {
    // Basic case: rod length 5, lengths {1,2,3}, prices {1,3,4}
    // Optimal: 2+3 = 5 → price 3+4=7, or 1+1+3=1+1+4=6, etc. Actually, 2+3=7 is best.
    {
        std::vector<int> lengths = {1,2,3};
        std::vector<int> prices = {1,3,4};
        assert(maxRodValue(5, lengths, prices) == 7);
    }

    // Exact length impossible: rod 4, lengths {3,5}, prices {10,20}
    // Cannot make 4, so return 0.
    {
        std::vector<int> lengths = {3,5};
        std::vector<int> prices = {10,20};
        assert(maxRodValue(4, lengths, prices) == 0);
    }

    // Single length equal to rod: rod 7, lengths {7}, prices {99}
    {
        std::vector<int> lengths = {7};
        std::vector<int> prices = {99};
        assert(maxRodValue(7, lengths, prices) == 99);
    }

    // Rod length 10 with unlimited length 1 priced 2: should give 20
    {
        std::vector<int> lengths = {1};
        std::vector<int> prices = {2};
        assert(maxRodValue(10, lengths, prices) == 20);
    }

    // Duplicate lengths with different prices: rod 4, lengths {2,2}, prices {5,3}
    // Use the better price (5) twice = 10
    {
        std::vector<int> lengths = {2,2};
        std::vector<int> prices = {5,3};
        assert(maxRodValue(4, lengths, prices) == 10);
    }

    // Zero rod length: returns 0
    {
        std::vector<int> lengths = {1,2};
        std::vector<int> prices = {10,20};
        assert(maxRodValue(0, lengths, prices) == 0);
    }

    // All prices zero: any achievable length returns 0
    {
        std::vector<int> lengths = {1,2,3};
        std::vector<int> prices = {0,0,0};
        assert(maxRodValue(3, lengths, prices) == 0);
    }

    // Larger case: rod 100, length 1 price 1, length 100 price 150
    // Best to use one 100-length piece (150) vs 100 ones (100)
    {
        std::vector<int> lengths = {1,100};
        std::vector<int> prices = {1,150};
        assert(maxRodValue(100, lengths, prices) == 150);
    }

    return 0;
}

#include <vector>
#include <algorithm>
#include <limits>

// Returns the maximum total price for a rod of length exactly n,
// using unlimited cuts of lengths from 'lengths' with parallel 'prices'.
// If exact length n is impossible, returns 0.
int maxRodValue(int n, const std::vector<int>& lengths, const std::vector<int>& prices) {
    if (n == 0) return 0;

    // Use -1 as sentinel for impossible states, since prices are non-negative.
    const int NEG = -1;
    std::vector<int> dp(n + 1, NEG);
    dp[0] = 0;  // empty rod has value 0

    for (size_t i = 0; i < lengths.size(); ++i) {
        int L = lengths[i];
        int P = prices[i];
        if (L <= 0 || L > n) continue;  // skip invalid lengths
        for (int x = L; x <= n; ++x) {
            if (dp[x - L] != NEG) {
                int candidate = dp[x - L] + P;
                if (candidate > dp[x]) dp[x] = candidate;
            }
        }
    }

    return (dp[n] == NEG) ? 0 : dp[n];
}

// This is an unbounded knapsack (or coin change with profit) problem where the capacity is the rod length `n`, each item has weight = `lengths[i]` and value = `prices[i]`, and you may use each item any number of times. The DP state is `dp[x]` = maximum total price for a rod of length exactly `x`. Initialize `dp[0] = 0` (empty cutting yields 0) and all other `dp[x] = -infinity` (or a very small sentinel) to indicate impossible states. For each length `L` and price `P`, for each capacity `x` from `L` to `n` (increasing order to allow reuse), update `dp[x] = max(dp[x], dp[x-L] + P)`. After processing all items, if `dp[n]` is still negative (or sentinel), it means no combination sums to exactly `n`, so return 0; otherwise, return `dp[n]`. Edge cases: (1) `n` is 0 → return 0. (2) If a length equals `n`, the price is considered as a single piece. (3) If `lengths[i] > n`, skip that item because it cannot be used. (4) Duplicate lengths with different prices are fine because we process each separately. Time complexity: O(m * n) for the DP. Space complexity: O(n) for the dp array.
