// Write a C++ function `int countOrderedWays(const std::vector<int>& coins, int total)` that returns the number of distinct ordered sequences of coins (positive integers) whose sum equals `total`. Each coin can be used unlimited times, and sequences that differ in order are considered distinct (e.g., `[1,2]` and `[2,1]` are different). The input vector `coins` may contain duplicates or unsorted values, and `total` is a non-negative integer. The result is guaranteed to fit within a 32-bit unsigned integer. Handle the case where `total == 0` by returning `1` (the empty sequence). If no sequence sums to `total`, return `0`.

// This problem is a classic "coin change" variation where permutations are counted, not combinations. The dynamic programming solution defines `dp[x]` as the number of ordered ways to sum to exactly `x`. Initialize `dp[0] = 1` because there is exactly one way to make sum `0` (using no coins). For each target value from `1` to `total`, iterate through all coin denominations. For each coin `c` that is less than or equal to the current target `x`, add `dp[x - c]` to `dp[x]` because any valid sequence for `x - c` can be followed by coin `c`. This correctly counts permutations because the outer loop runs over target values, and the inner loop over coins allows each coin to be the "last" coin in a sequence, naturally producing all orderings. Critical edge cases include: `total == 0` returns `1`; coins with values greater than `total` can be ignored because they can never be used; zero or negative coin values are not expected per the problem statement, but if they appear, they must be filtered out to avoid infinite loops. Sorting the coins and breaking early when `coin > target` slightly optimizes the inner loop but is not required for correctness. Time complexity is `O(total * n)` where `n` is the number of coins, and space complexity is `O(total)` for the DP array.

#include <vector>
#include <algorithm>

// Count the number of ordered sequences of coins summing to total.
// Each coin can be used any number of times; order matters.
int countOrderedWays(const std::vector<int>& coins, int total) {
    if (total == 0) return 1;

    // Use unsigned int to store counts; intermediate sums can be large, but final fits in 32-bit.
    std::vector<unsigned int> dp(total + 1, 0);
    dp[0] = 1;

    // Sort to allow early break when coin exceeds current target.
    std::vector<int> sortedCoins = coins;
    std::sort(sortedCoins.begin(), sortedCoins.end());

    for (int target = 1; target <= total; ++target) {
        for (int coin : sortedCoins) {
            if (coin > target) break;  // Since sorted, remaining coins are too large.
            dp[target] += dp[target - coin];
        }
    }

    // Return as int; the problem guarantees it fits in 32-bit signed int.
    return static_cast<int>(dp[total]);
}

#include <cassert>
#include <vector>

// The function is already defined above, but for completeness, include it here.
// (In an actual test, just include the solution header.)

int main() {
    // Basic cases
    assert(countOrderedWays({1, 2, 3}, 4) == 7);  // sequences: 1111,112,121,211,22,13,31
    assert(countOrderedWays({2, 3}, 7) == 3);     // 223,232,322
    assert(countOrderedWays({1}, 5) == 1);        // 11111
    assert(countOrderedWays({2}, 5) == 0);        // impossible

    // Edge cases
    assert(countOrderedWays({}, 0) == 1);         // empty total, no coins
    assert(countOrderedWays({5, 1}, 0) == 1);     // total zero always 1
    assert(countOrderedWays({1, 1}, 3) == 1? actually order same? Let's compute: only 111 but both coins identical, so 1 way
    assert(countOrderedWays({1, 1}, 3) == 1);

    // Larger values and duplicates
    assert(countOrderedWays({1, 2, 3}, 10) == 274);
    assert(countOrderedWays({1, 2, 5}, 10) == 121);

    // Unsorted input
    assert(countOrderedWays({3, 1, 2}, 4) == 7);  // same as sorted case

    return 0;
}
