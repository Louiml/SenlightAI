// Write a C++ function `int cambioCerradura(const int v[], int n, int t)` that, given an array `v` of `n` positive integers (where `n ≥ 1`) and `t` equal to the sum of all elements, returns the position `p` (0-indexed) such that the absolute difference between the sum of elements `v[0]...v[p]` (the left group) and the sum of elements `v[p+1]...v[n-1]` (the right group) is minimized. If multiple positions achieve the same minimal absolute difference, return the smallest such position `p`. The function must treat `t` and `v` as read-only inputs. The position `p` is in the range `[0, n-2]` (since we must leave at least one element on the right), and for `n = 1` the function should return 0 (though the original snippet’s main loop only calls with `n > 0`; handle `n=1` gracefully by returning 0 since there is no valid split but the difference is `t` itself, and we define the default to be 0). Note: the original snippet has a subtle bug with `min` initialization and absolute value usage; your implementation should correctly find the minimum absolute difference. The function should not assume any global constants or I/O.
#include <cassert>
#include <cstdlib>

// Declare the solution function (assumed to be defined elsewhere or above).
int cambioCerradura(const int v[], int n, int t);

int main() {
    // Single element: no split, return 0.
    {
        int v[] = {5};
        assert(cambioCerradura(v, 1, 5) == 0);
    }

    // Two elements: only split at position 0.
    {
        int v[] = {1, 2};
        assert(cambioCerradura(v, 2, 3) == 0);
    }

    // Three elements: best split at position 1? (sums: 1 vs 5 -> diff 4; 3 vs 3 -> diff 0)
    {
        int v[] = {1, 2, 3};
        assert(cambioCerradura(v, 3, 6) == 1);
    }

    // All equal elements: any split gives same diff (0 or t). Original returns smallest.
    // Here, n=3: sums: 4 vs 0? Actually, example with positive: {2,2,2}, t=6.
    // Splits: k=0: left=2, right=4 -> diff 2; k=1: left=4, right=2 -> diff 2. Choose smallest (0).
    {
        int v[] = {2, 2, 2};
        assert(cambioCerradura(v, 3, 6) == 0);
    }

    // Larger array with clear minimum at a later position.
    // v = {1, 1, 1, 1, 4} t=8. Splits:
    // k=0: 1 vs 7 diff 6
    // k=1: 2 vs 6 diff 4
    // k=2: 3 vs 5 diff 2
    // k=3: 4 vs 4 diff 0 -> choose 3.
    {
        int v[] = {1, 1, 1, 1, 4};
        assert(cambioCerradura(v, 5, 8) == 3);
    }

    // Tie at multiple positions: choose smallest.
    // v = {3, 1, 2, 3} t=9.
    // k=0: 3 vs 6 diff 3
    // k=1: 4 vs 5 diff 1
    // k=2: 6 vs 3 diff 3
    // So best k=1.
    {
        int v[] = {3, 1, 2, 3};
        assert(cambioCerradura(v, 4, 9) == 1);
    }

    // Another tie: v = {10, 1, 1, 1, 1, 1, 1, 1, 1, 1} t=18.
    // Splits: k=0: 10 vs 8 diff 2; k=1: 11 vs 7 diff 4; ... best k=0.
    {
        int v[] = {10, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        assert(cambioCerradura(v, 10, 18) == 0);
    }

    // Large difference on one side: v = {100, 1, 1, 1} t=103.
    // k=0: 100 vs 3 diff 97; k=1: 101 vs 2 diff 99; k=2: 102 vs 1 diff 101 -> best 0.
    {
        int v[] = {100, 1, 1, 1};
        assert(cambioCerradura(v, 4, 103) == 0);
    }

    // Negative check: all positive, but include a zero? Actually spec says positive integers, but test with zero just in case.
    // v = {0, 5} t=5 => split 0: left 0, right 5 diff 5.
    {
        int v[] = {0, 5};
        assert(cambioCerradura(v, 2, 5) == 0);
    }

    return 0;
}
#include <cstdlib> // for std::abs

// Given an array v of n positive integers, and t = sum of all v[i],
// return the split index p (0 <= p < n-1) such that
// |sum(v[0..p]) - sum(v[p+1..n-1])| is minimized.
// If ties, return the smallest p.
// For n == 1, return 0 (no split possible).
int cambioCerradura(const int v[], int n, int t) {
    if (n <= 1) {
        return 0;
    }

    int left_sum = v[0];
    int right_sum = t - left_sum;
    int best_diff = std::abs(left_sum - right_sum);
    int best_pos = 0;

    // Iterate over all possible split positions; k is the last index of the left group.
    for (int k = 0; k < n - 1; ++k) {
        // For k=0, we already have left_sum=v[0] and right_sum=t-v[0].
        // For k>0, left_sum has been updated at the end of previous iteration.
        right_sum = t - left_sum;
        int current_diff = std::abs(left_sum - right_sum);
        if (current_diff < best_diff) {
            best_diff = current_diff;
            best_pos = k;
        }
        // Prepare left_sum for the next iteration (if any).
        if (k < n - 2) {
            left_sum += v[k + 1];
        }
    }

    return best_pos;
}
// The core idea is to iterate over all possible split positions from `left_sum = v[0]` (and `right_sum = t - left_sum`) up to `left_sum = sum of v[0]..v[n-2]` (and `right_sum = v[n-1]`). For each position `k` (where `k` is the current left group’s last index), compute `current_diff = abs(left_sum - right_sum)`. Track the minimum `current_diff` and the corresponding `k+1` (because the left group includes indices 0..k, so the split position is `k`). Initialize the best position to 0 and best difference to `abs(v[0] - (t - v[0]))` (or to a large value if n==1). Then for each `k` from 0 to `n-2`, update `left_sum` incrementally by adding `v[k]` after computing the diff for the previous split. This is done in a single pass, yielding `O(n)` time and `O(1)` auxiliary space. Edge cases: `n=1` → return 0 (no split, but define as default). When `n=2`, the only split is position 0, so return 0. When differences tie, choose the smaller position (which naturally happens if we only update when we find a strictly smaller difference). The original snippet used `abs(dif)` where `dif` was already a difference of absolute values; we simplify by directly comparing `abs(left_sum - right_sum)`.
