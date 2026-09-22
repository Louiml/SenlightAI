/*
Write a C++ function `maxReward` that takes two vectors of non-negative integers, `reward1` and `reward2`, of equal length `n`, and an integer `k` (where `0 ≤ k ≤ n`), and returns the maximum possible total reward by choosing exactly `k` indices to take the reward from `reward1` (first vector) and the remaining `n - k` indices to take the reward from `reward2` (second vector). The goal is to maximize the sum of the selected rewards.
*/

#include <vector>
#include <algorithm>
#include <tuple>

// Given equal-length vectors reward1 and reward2, choose exactly k indices
// to take from reward1 (and the rest from reward2) to maximize the total reward.
// Returns the maximum possible total reward.
int maxReward(const std::vector<int>& reward1, const std::vector<int>& reward2, int k) {
    int n = static_cast<int>(reward1.size());
    // Store (difference, reward1_value, reward2_value) for each index.
    std::vector<std::tuple<int, int, int>> items;
    items.reserve(n);
    for (int i = 0; i < n; ++i) {
        items.emplace_back(reward1[i] - reward2[i], reward1[i], reward2[i]);
    }
    // Sort descending by difference (first tuple element).
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        return std::get<0>(a) > std::get<0>(b);
    });
    int total = 0;
    for (int i = 0; i < n; ++i) {
        if (i < k) {
            total += std::get<1>(items[i]); // reward1 for selected best gains
        } else {
            total += std::get<2>(items[i]); // reward2 for the rest
        }
    }
    return total;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case: pick index 0 for reward1 (gain 4) and the rest for reward2.
    std::vector<int> r1 = {5, 2, 3};
    std::vector<int> r2 = {1, 4, 1};
    assert(maxReward(r1, r2, 1) == 10); // 5 + 4 + 1 = 10

    // k = 0: all from reward2
    assert(maxReward(r1, r2, 0) == 6); // 1+4+1 = 6

    // k = n: all from reward1
    assert(maxReward(r1, r2, 3) == 10); // 5+2+3 = 10

    // Negative gains: pick smallest negative if forced
    std::vector<int> r1b = {3, 4};
    std::vector<int> r2b = {10, 1};
    // gain: -7, 3 -> pick second index for reward1 => 1 + 4 = 5 better than 3+1=4
    assert(maxReward(r1b, r2b, 1) == 5);

    // Ties in difference
    std::vector<int> r1c = {7, 7, 1};
    std::vector<int> r2c = {2, 2, 9};
    // gains: 5,5,-8 -> pick any two of first two, total = 7+7+9=23
    assert(maxReward(r1c, r2c, 2) == 23);

    // All equal differences
    std::vector<int> r1d = {4, 4};
    std::vector<int> r2d = {2, 2};
    assert(maxReward(r1d, r2d, 1) == 6); // either way 4+2=6

    // Single element
    std::vector<int> r1e = {9};
    std::vector<int> r2e = {1};
    assert(maxReward(r1e, r2e, 1) == 9);
    assert(maxReward(r1e, r2e, 0) == 1);

    return 0;
}

// The key insight is that we must decide which `k` indices are most advantageous to assign to `reward1` rather than `reward2`. For each index `i`, taking the reward from `reward1` instead of `reward2` gives a "gain" of `reward1[i] - reward2[i]`. To maximize the total, we should select the `k` indices with the largest gains (i.e., the largest differences `reward1[i] - reward2[i]`). Any index with a negative gain should still be considered if we are forced to pick `k` indices, but the greedy choice of the largest differences is optimal because the total reward can be expressed as `sum(reward2) + sum of selected gains`. Sorting all indices by their difference in descending order and taking the first `k` for `reward1`, then summing the corresponding `reward1` values for those, and `reward2` for the rest, yields the maximum. Edge cases: `k = 0` means all rewards come from `reward2`; `k = n` means all from `reward1`; equal differences require no special handling because any tie yields the same total. Time complexity: O(n log n) due to sorting, space complexity: O(n) for storing the triples.
