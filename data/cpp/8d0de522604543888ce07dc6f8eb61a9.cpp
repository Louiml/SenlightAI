/*
Write a C++ function `int minimalBoats(int n, int k, const std::vector<int>& weights)` that, given `n` people with the integer weights in the vector `weights`, and a boat carrying capacity `k`, returns the minimum number of boats required to carry all people. Each boat can carry at most two people, and the total weight of the people in a boat must not exceed `k`. You may assume `n >= 1`, all weights are positive, and `k` is positive. The function must handle unsorted input and may reorder data internally.
*/
#include <vector>
#include <algorithm>

// Returns the minimum number of boats needed to carry all people.
// Each boat carries at most two people, and total weight in a boat must not exceed capacity k.
int minimalBoats(int n, int k, const std::vector<int>& weights) {
    // Work on a copy to avoid modifying the caller's vector.
    std::vector<int> sortedWeights = weights;
    std::sort(sortedWeights.begin(), sortedWeights.end());
    
    int left = 0;
    int right = n - 1;
    int boats = 0;
    
    while (left <= right) {
        ++boats;
        // Try to pair the lightest with the heaviest if they fit.
        if (left < right && sortedWeights[left] + sortedWeights[right] <= k) {
            ++left;
        }
        --right;
    }
    return boats;
}
#include <cassert>
#include <vector>

int minimalBoats(int n, int k, const std::vector<int>& weights);

int main() {
    // Basic case: pair two light people.
    assert(minimalBoats(4, 10, {1, 2, 3, 4}) == 2);
    // All pairs fit exactly.
    assert(minimalBoats(4, 5, {1, 4, 2, 3}) == 2);
    // Heavy people must go alone.
    assert(minimalBoats(3, 5, {5, 5, 5}) == 3);
    // One person only.
    assert(minimalBoats(1, 100, {50}) == 1);
    // Mixed: some pairs, some singles.
    assert(minimalBoats(5, 6, {1, 2, 3, 4, 5}) == 3);
    // Edge: all weights are 1 and capacity large, so all pairs.
    assert(minimalBoats(6, 10, {1, 1, 1, 1, 1, 1}) == 3);
    // Unsorted input, capacity exactly equals sum of two heaviest.
    assert(minimalBoats(2, 10, {6, 4}) == 1);
    // No pairing possible, all go alone.
    assert(minimalBoats(4, 3, {3, 3, 3, 3}) == 4);
    // Odd number of people.
    assert(minimalBoats(5, 7, {3, 2, 2, 1, 3}) == 3);
    return 0;
}
// The optimal strategy is to sort the weights in non-decreasing order, then use two pointers: one pointed at the lightest remaining person (`l`) and one at the heaviest remaining person (`r`). While `l <= r`, we allocate one boat. If the lightest and the heaviest remaining together fit in the capacity (`weights[l] + weights[r] <= k`), then we pair them; otherwise, the heaviest must go alone because even the lightest cannot fit with him. In either case, we move the right pointer left, and only move the left pointer right when a pairing is successful. This greedy is optimal because pairing the heaviest with the lightest possible partner (when possible) minimizes wasted capacity. Edge cases: if `n == 1`, the answer is `1`; if all weights exceed `k` (but are individually ≤ k by assumption, since people must be carryable), then they all go alone. The algorithm runs in `O(n log n)` time due to sorting, with `O(1)` auxiliary space (ignoring the input vector copy, if we sort in place or copy it). Sorting dominates.
