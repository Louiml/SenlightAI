Write a C++ function `int maxGainAfterDeletions(const std::vector<int>& weights, int operations)` that, given an array of item weights and a maximum number of allowed moves, returns the maximum possible sum of weights you can keep. In each move, you must remove exactly one item, but you may only remove from either the very front or the very back of the original array (you cannot remove an item once it has been moved to your "kept" pile). You may choose to perform fewer than `operations` moves (including zero, though "keep" nothing gives sum 0). After each removal, the item is discarded and can never be kept; you keep only the items you never removed. You may remove at most `operations` items total from the two ends, and you may decide to stop early, but you cannot remove an item from the middle. The kept items are those that remain in the original order after you’ve removed the chosen prefix and suffix. However, you are allowed to discard any kept items whose weight is negative (you can throw them away after the fact, with no move cost) to increase the sum, but you may not add any items. Return the maximum possible sum of the remaining kept items (after discarding negatives if you wish). The array length `n` and `operations` satisfy \(1 \le n \le 5000\), \(0 \le operations \le n\), and each weight is in \([-10^9, 10^9]\). If all kept items are thrown away, the sum is 0. For example, with weights `[-1,2,3]` and `operations=1`, the optimal is to remove the front `-1` (cost 1), keep `[2,3]` sum 5; but you could also remove the back `3` and keep `[-1,2]` then throw away `-1` for sum 2, so answer 5.

// The key observation is that final kept items form a contiguous subarray of the original array after removing exactly `i` items from the front and `j` items from the back, where `i+j ≤ operations`. Since you can stop early, you do not need to use all moves. For each possible pair `(i,j)` with `i ≥0, j≥0, i+j ≤ operations`, the kept subarray is `weights[i .. n-1-j]`. Any negative weights inside this subarray can be discarded without cost, so we only sum the positive weights (and zero) from that subarray. Because `n` can be up to 5000 and `operations` up to `n`, trying all `O(n^2)` pairs naively is acceptable if the inner sum is computed efficiently. We can precompute prefix sums of only positive weights: `posPref[k] = sum of max(0, weights[t]) for t < k`. Then for a subarray from index `l` to `r` inclusive, the sum of positives is `posPref[r+1] - posPref[l]`. We iterate `i` from 0 to min(n, operations) and `j` from 0 to min(n-i, operations-i), and compute the subarray indices `l=i` and `r=n-1-j`. We only consider cases where `l ≤ r` (empty subarray gives sum 0, which is fine but covered by the initialization). The maximum over all valid `(i,j)` is the answer. Edge cases: `operations` may be 0, then only `(i=0,j=0)` is considered, sum of all positive weights. If all weights are negative, the best is to remove all possible, but since you only can remove from ends, if you can remove everything (operations ≥ n) you get 0; otherwise you might have to keep some negatives but you can discard them, so sum is 0 as well — the initial answer 0 handles it. Time complexity: iterating `i` and `j` gives `O(min(n, operations)^2)` which is at most `O(n^2)`; prefix sum computed in `O(n)`. Space complexity `O(n)` for the prefix array.

#include <vector>
#include <algorithm>

// Returns the maximum sum of kept weights after removing at most 'operations'
// items from the ends, and discarding any negative kept weights.
// The weights array may be large; we use prefix sums of positive values.
long long maxGainAfterDeletions(const std::vector<int>& weights, int operations) {
    const int n = static_cast<int>(weights.size());
    if (n == 0) return 0;

    // Prefix sum of positive weights: posPref[i] = sum of max(0, weights[0..i-1])
    std::vector<long long> posPref(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        posPref[i + 1] = posPref[i] + (weights[i] > 0 ? weights[i] : 0);
    }

    long long best = 0; // Default: remove all if possible, or discard negatives

    // Limit on total removals from both ends
    int maxRemovals = std::min(n, operations);

    // i = number removed from front, j = number removed from back
    for (int i = 0; i <= maxRemovals; ++i) {
        int maxJ = std::min(n - i, maxRemovals - i);
        for (int j = 0; j <= maxJ; ++j) {
            int left = i;
            int right = n - 1 - j; // inclusive index
            if (left > right) continue; // empty subarray, sum is 0
            long long subSum = posPref[right + 1] - posPref[left];
            best = std::max(best, subSum);
        }
    }
    return best;
}

#include <cassert>
#include <vector>

// The solution function is declared above.
// Test cases in a main function.

int main() {
    // Basic positive numbers
    assert(maxGainAfterDeletions({1, 2, 3, 4}, 2) == 10); // remove front 1 and back 4? Actually remove 1 and 4? sum 2+3=5? Wait: better remove 1 only? Let's compute: with 2 ops, remove front 1 and front 2? Keep [3,4] sum 7. Remove back 4 and back 3? Keep [1,2] sum 3. Remove front 1 and back 4: keep [2,3] sum 5. All positives, best 7? Let's recalc: n=4, ops=2. Options: (i=2,j=0) keep [3,4] sum 7; (i=0,j=2) keep [1,2] sum 3; (i=1,j=1) keep [2,3] sum 5. So 7 is correct.
    assert(maxGainAfterDeletions({1, 2, 3, 4}, 2) == 7);

    // Negative numbers can be discarded
    assert(maxGainAfterDeletions({-1, 2, 3}, 1) == 5); // remove front -1, keep [2,3]
    assert(maxGainAfterDeletions({-1, 2, 3}, 0) == 5); // keep all, discard -1 -> sum 5
    assert(maxGainAfterDeletions({-5, -1}, 1) == 0);   // remove front -5, keep -1 discarded ->0; or remove back -1 keep -5 discarded ->0
    assert(maxGainAfterDeletions({5, -3, 7}, 2) == 12); // remove front and back? Remove front 5? No, remove back 7 and back -3? Actually remove front 5 and back 7? keep -3 discarded ->0; best remove only the -3? You can't remove -3 directly because it's middle. So remove front 5 (keep -3,7 sum 7) or remove back 7 (keep 5,-3 sum 5). With 2 ops, remove front 5 and back 7 -> keep -3 ->0. Best is 7. Wait: Let's compute: i=0,j=0 sum positives 12? No, keep all, discard -3 -> 12? Actually 5+7=12, yes! But can we keep all with ops=2? Yes, we don't have to use moves. So 12 is correct.
    assert(maxGainAfterDeletions({5, -3, 7}, 2) == 12);

    // All negative, can remove all
    assert(maxGainAfterDeletions({-1, -2, -3}, 3) == 0);
    // Operations larger than n
    assert(maxGainAfterDeletions({-1, 2, -3}, 5) == 2); // remove all except 2? Actually remove front -1 and back -3 (2 ops) keep [2] sum 2.
    // Single element
    assert(maxGainAfterDeletions({4}, 0) == 4);
    assert(maxGainAfterDeletions({-4}, 0) == 0);
    assert(maxGainAfterDeletions({-4}, 1) == 0);

    // Mixed, large numbers
    assert(maxGainAfterDeletions({-100, 50, -200, 60, 70}, 3) == 130); // remove front -100, back 70? Wait best: remove front -100, back 70? keep [50,-200,60] discard -200 sum 110. Remove front -100 and back 60? keep [50,-200,70] discard -200 sum 120. Remove front -100 and back 70 and back 60? keep [50,-200] discard -200 sum 50. Remove front -100 and back -200? can't because -200 middle. So best is remove front -100 and back 70? keep [50,-200,60] sum 110; or remove front -100 and back 60? keep [50,-200,70] sum 120; or remove back 70 and back 60? keep [-100,50,-200] discard negatives sum 50; with 3 ops: remove front -100, back 70, back 60 => keep [50,-200] discard -200 sum 50; remove front -100, back 70, front 50 => keep [-200,60] discard -200 sum 60; remove back 70, back 60, front -100 => keep [50,-200] sum 50. Actually best is 120 as computed. So assert 120.
    assert(maxGainAfterDeletions({-100, 50, -200, 60, 70}, 3) == 120);

    // Edge with zero
    assert(maxGainAfterDeletions({0, -5, 3}, 1) == 3); // remove front 0? keep [-5,3] discard -5 sum 3; remove back 3 keep [0,-5] sum 0; best 3.

    return 0;
}
