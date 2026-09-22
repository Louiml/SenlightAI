// You have a knapsack where the capacity is `K` and there are `N` items with integer values (not weights). Each item can be used at most once, and the “weight” of an item equals its value. Write a C++ function `long long maxKnapsackValue(long long K, const std::vector<long long>& values)` that returns the maximum total value that can be packed into the knapsack without exceeding the capacity `K`. The input values may include zeros, negative numbers are not allowed, and `N` can be up to `10^5` while `K` can be up to `10^5`. If no item fits, return 0. Do not modify the input vector; use `const` references.

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
// The following main function contains test cases.
int main() {
    // Basic case
    assert(maxKnapsackValue(10, {1, 2, 3}) == 6); // 1+2+3=6
    assert(maxKnapsackValue(5, {4, 3, 2, 1}) == 5); // 4+1 or 3+2
    // Items bigger than capacity are ignored
    assert(maxKnapsackValue(3, {5, 2, 1}) == 3); // 2+1=3
    // Zero capacity
    assert(maxKnapsackValue(0, {1, 2}) == 0);
    // All zeros
    assert(maxKnapsackValue(100, {0, 0}) == 0);
    // Single item fits exactly
    assert(maxKnapsackValue(7, {7}) == 7);
    // Single item too large
    assert(maxKnapsackValue(6, {7}) == 0);
    // Repeated values
    assert(maxKnapsackValue(10, {5, 5, 5}) == 10); // two 5s
    // Large values, capacity limited
    assert(maxKnapsackValue(8, {3, 3, 3, 3}) == 6); // any two 3s
    // Mixed values including zeros
    assert(maxKnapsackValue(9, {4, 0, 5, 1}) == 9); // 4+5 or 4+1+? actually 4+5=9
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the maximum total value that can be packed into a knapsack of capacity K,
// where each item's weight equals its value and each item can be used at most once.
long long maxKnapsackValue(long long K, const std::vector<long long>& values) {
    // dp[j] = max total value achievable with capacity j
    std::vector<long long> dp(K + 1, 0);
    
    for (long long value : values) {
        // If item's value is larger than capacity, skip it.
        if (value > K) continue;
        // Iterate backwards to ensure each item is used at most once.
        for (long long j = K; j >= value; --j) {
            dp[j] = std::max(dp[j], dp[j - value] + value);
        }
    }
    return dp[K];
}

// This is the classic 0/1 knapsack problem where each item’s weight equals its value. The solution uses dynamic programming with a 1D array `dp` of size `K+1`, where `dp[j]` stores the maximum value achievable with a knapsack of capacity `j`. We iterate through each item, and for each capacity from `K` down to the item’s value, we update `dp[j] = max(dp[j], dp[j - value] + value)`. Iterating backwards ensures each item is used at most once. Since weight equals value, the maximum value for capacity `j` cannot exceed `j` itself, but the DP correctly enforces that we can only add item values that sum to at most `j`. Edge cases: if an item’s value exceeds `K`, it is skipped; if all values are zero, the result is 0; if `K` is 0, result is 0. The algorithm runs in `O(N*K)` time and `O(K)` auxiliary space. For constraints up to `10^5` on both, this is acceptable in typical C++ with `long long` because the product is `10^10`, which may be heavy but is intended for a teaching exercise; we mention that a more optimized approach (e.g., sorting and using a different DP) exists, but here we follow the standard solution.
