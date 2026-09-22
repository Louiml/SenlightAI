/*
Write a C++ function named `splitGreedyCoins` that accepts a non-empty `std::vector<int>` representing coin values (all integers, may be negative or zero) and splits it into two non-empty contiguous subarrays — a "left" part and a "right" part — such that the absolute difference between the sum of the left part and the sum of the right part is minimized. The function should return a `std::pair<int, int>` containing (leftSum, rightSum) for that optimal split. The split must place at least one coin in each part. If multiple splits give the same minimal difference, return the one where the left part has the smallest number of coins (i.e., the split earliest from the left). For example, given `{7, 3}`, the only split is left `{7}` (sum 7) and right `{3}` (sum 3), difference 4, so return `(7,3)`. For `{1,2,3}`, splits: left `{1}` sum 1 vs `{2,3}` sum 5 diff 4; left `{1,2}` sum 3 vs `{3}` sum 3 diff 0 — optimal is `(3,3)`. For negative values, e.g., `{-1,2,-3}`, all splits must be considered. The function should be `const`-correct, use only standard headers, and have `O(n)` time complexity (where `n` is the vector size) and `O(1)` auxiliary space.
*/
#include <vector>
#include <utility>
#include <cstdlib> // for std::abs

// Returns the (leftSum, rightSum) for the contiguous split that minimizes
// |leftSum - rightSum|. On ties, chooses the split with fewer left elements.
std::pair<int, int> splitGreedyCoins(const std::vector<int>& coins) {
    // Total sum of all coins
    int total = 0;
    for (int value : coins) {
        total += value;
    }

    int bestDiff = -1; // will be set to a non-negative value
    int bestLeft = 0;
    int bestRight = 0;
    int leftSum = 0;

    // Iterate split points: left gets coins[0..i], right gets coins[i+1..end]
    // At least one coin on each side, so i goes from 0 to size-2
    for (size_t i = 0; i + 1 < coins.size(); ++i) {
        leftSum += coins[i];
        int rightSum = total - leftSum;
        int diff = std::abs(leftSum - rightSum);

        // Update if first candidate or strictly better difference
        // This keeps the earliest split index on ties (smallest left count)
        if (bestDiff == -1 || diff < bestDiff) {
            bestDiff = diff;
            bestLeft = leftSum;
            bestRight = rightSum;
        }
    }

    return {bestLeft, bestRight};
}
#include <cassert>
#include <vector>
#include <utility>

// Solution function declaration (assume it's included from above)
std::pair<int, int> splitGreedyCoins(const std::vector<int>&);

int main() {
    // Basic example from snippet
    assert(splitGreedyCoins({7, 3}) == std::make_pair(7, 3));

    // Even split
    assert(splitGreedyCoins({1, 2, 3}) == std::make_pair(3, 3));

    // Tie-breaking: earliest split with same diff
    // {5,1,4} splits: left {5} sum 5 vs right {1,4} sum 5 diff 0; left {5,1} sum 6 vs {4} diff 2
    assert(splitGreedyCoins({5, 1, 4}) == std::make_pair(5, 5));

    // All equal values
    assert(splitGreedyCoins({4,4,4,4}) == std::make_pair(8, 8)); // split at i=1 gives 4 vs 12 diff 8; i=2 gives 8 vs 8 diff 0

    // Negative numbers
    assert(splitGreedyCoins({-1, 2, -3}) == std::make_pair(-1, -1)); // split at i=0: left -1, right -1 diff 0; split at i=1: left 1, right -3 diff 4

    // Larger vector
    assert(splitGreedyCoins({1, 2, 3, 4, 5}) == std::make_pair(6, 9)); // split at i=2: left 6, right 9 diff 3; i=3: left 10, right 5 diff 5

    // Single split with negative sum
    assert(splitGreedyCoins({-5, 10}) == std::make_pair(-5, 10));

    // Zero values
    assert(splitGreedyCoins({0, 0, 0}) == std::make_pair(0, 0)); // any split gives diff 0, earliest i=0 left sum 0

    return 0;
}
// The problem is essentially a "minimum absolute difference split" problem for a 1D array. The naive approach would try every split index `i` from 1 to `n-1`, compute left sum of first `i` elements and right sum of the rest, and track the best. That would take `O(n^2)` if recomputing sums each time, but we can do it in `O(n)` using prefix sums or incremental updates. Since `n` can be large, we need linear time. Approach: First compute the total sum of the entire vector in one pass. Then iterate with a running `leftSum` that starts at `0` and accumulates element by element. At each step `i` from 0 to `n-2` (ensuring at least one element left for right), we add `vec[i]` to `leftSum`, compute `rightSum = total - leftSum`, and compute `abs(leftSum - rightSum)`. Track the minimal difference and the corresponding pair. Since we want the left part with the smallest number of coins on tie, we iterate from left to right and update only when we find a strictly smaller difference (or update on tie? We need smallest number of coins, meaning the earliest split index, so we should update only when `currentDiff < bestDiff`, and keep the first found with that minimal diff). Edge cases: vector size at least 2 (guaranteed non‑empty but must have at least two elements for a valid split; if size is 1, the problem is invalid, but we can handle gracefully — but the task says non‑empty and each part must be non‑empty, so assume size ≥ 2). Negative numbers work fine with absolute difference. Time complexity: one pass for total, one pass for iteration → `O(n)`. Space: `O(1)` auxiliary.
