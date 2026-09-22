Write a C++ function `int maxPackableWeight(int W, const std::vector<int>& weights)` that solves the classic 0/1 knapsack feasibility problem: given a knapsack capacity `W` and a list of item weights (each item can be taken at most once), determine the maximum total weight that can be packed into the knapsack without exceeding `W`. The function should return that maximum achievable weight. The input `weights` may be unsorted, can contain duplicates, and may be empty. If no item fits, the answer is 0. For example, with `W = 10` and `weights = {2, 3, 5, 7}`, the optimal packed weight is 10 (2+3+5); with `W = 3` and `weights = {5, 6}`, the answer is 0. The function must be efficient for `W` up to 10,000 and up to 100 items.

// This is a subset-sum problem, a special case of 0/1 knapsack where each item's value equals its weight. We use dynamic programming with a 1D boolean array `possible` of size `W+1`, where `possible[w]` is `true` if some subset of the processed items sums exactly to `w`. Initialize `possible[0] = true` because an empty subset sums to 0. For each item weight `wi`, we iterate `weight` from `W` down to `wi` (descending order is crucial to avoid reusing the same item multiple times, preserving the "at most once" constraint). For each such `weight`, if `possible[weight - wi]` is `true`, then we can also achieve `weight`, so set `possible[weight] = true`. After processing all items, scan `possible` from `W` downward and return the first index that is `true`, which is the maximum achievable weight not exceeding `W`. Edge cases: empty item list → only `possible[0]` is true, answer is 0; all items heavier than `W` → no updates, answer 0; `W = 0` → possible[0] is true, answer 0. Time complexity is O(n * W) because we iterate over each item and each capacity. Space complexity is O(W) for the boolean array. This 1D optimization reduces memory from the 2D approach shown in the snippet, which uses O(n * W) space.

#include <vector>
#include <algorithm>

// Returns the maximum total weight <= W achievable by selecting a subset of 'weights' (each item at most once).
int maxPackableWeight(int W, const std::vector<int>& weights) {
    // possible[w] = true if some subset of processed items sums exactly to w.
    std::vector<bool> possible(W + 1, false);
    possible[0] = true;  // empty subset sums to 0

    for (int wi : weights) {
        // Iterate backward to ensure each item is used only once.
        for (int weight = W; weight >= wi; --weight) {
            if (possible[weight - wi]) {
                possible[weight] = true;
            }
        }
    }

    // Find the largest reachable sum.
    for (int weight = W; weight >= 0; --weight) {
        if (possible[weight]) {
            return weight;
        }
    }
    return 0;  // Should never reach here because possible[0] is true.
}

#include <cassert>
#include <vector>

// Declaration of the solution function (for testing).
int maxPackableWeight(int W, const std::vector<int>& weights);

int main() {
    // Empty list: only weight 0 is possible.
    assert(maxPackableWeight(10, {}) == 0);

    // All items heavier than capacity.
    assert(maxPackableWeight(3, {5, 6}) == 0);

    // Exact fit is possible.
    assert(maxPackableWeight(10, {2, 3, 5, 7}) == 10);

    // Cannot fill full capacity; best is 9.
    assert(maxPackableWeight(10, {1, 4, 4, 5}) == 9);

    // Duplicates allowed; use each at most once.
    assert(maxPackableWeight(6, {3, 3, 2}) == 6);

    // Capacity zero.
    assert(maxPackableWeight(0, {1, 2, 3}) == 0);

    // Single item that fits exactly.
    assert(maxPackableWeight(7, {7}) == 7);

    // Unsorted input, mixed weights.
    assert(maxPackableWeight(8, {4, 1, 3, 2, 5}) == 8);

    // Large capacity but small items; best sum is capacity itself.
    assert(maxPackableWeight(100, {1, 2, 3, 4, 5}) == 15);

    // Edge: capacity less than smallest item.
    assert(maxPackableWeight(2, {3, 4, 5}) == 0);

    return 0;
}
