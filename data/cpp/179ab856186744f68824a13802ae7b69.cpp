// Given an array of `n` positive integers and an integer `k`, write a C++ function `int minDeletionsToMakeSpacious(const std::vector<int>& arr, int k)` that returns the minimum number of elements that must be removed so that in the remaining array, after sorting it in non-decreasing order, every adjacent pair has a difference of at most `k` (i.e., for all valid indices `i`, `remaining[i+1] - remaining[i] <= k`). The original array may contain duplicates and can be unsorted. The function must work for `n` up to 10^6 and `k` up to 10^9. The output is the count of deletions, not a list of which elements to remove. For example, if `arr = {1, 10, 11, 20, 21}` and `k = 5`, one optimal remaining subsequence (after sorting) could be `{10, 11}` or `{20, 21}`, each of length 2, so deletions needed = 5 - 2 = 3.
#include <cassert>
#include <vector>

// The function declaration is assumed to be included above.
int minDeletionsToMakeSpacious(const std::vector<int>& arr, int k);

int main() {
    // Basic example from the prompt.
    assert(minDeletionsToMakeSpacious({1, 10, 11, 20, 21}, 5) == 3); // keep {10,11} or {20,21}
    // All elements already satisfy condition.
    assert(minDeletionsToMakeSpacious({1, 2, 3, 4}, 1) == 0);
    // k = 0 requires all equal values.
    assert(minDeletionsToMakeSpacious({2, 2, 2, 2}, 0) == 0);
    assert(minDeletionsToMakeSpacious({1, 2, 2, 3}, 0) == 2); // keep either {2,2}
    // Single element.
    assert(minDeletionsToMakeSpacious({42}, 10) == 0);
    // Large gap requires keeping only one element.
    assert(minDeletionsToMakeSpacious({1, 100, 200}, 10) == 2);
    // Unsorted with duplicates.
    std::vector<int> v = {5, 1, 5, 2, 5};
    assert(minDeletionsToMakeSpacious(v, 0) == 2); // keep three 5's
    // Negative values? The original prompt says positive, but function should handle general int.
    assert(minDeletionsToMakeSpacious({-5, -1, -3}, 2) == 1); // keep {-3,-1} or {-5,-3}
    // Edge case: n=0 (empty vector) returns 0.
    assert(minDeletionsToMakeSpacious({}, 10) == 0);
    return 0;
}
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the minimum number of deletions so that the remaining array,
// after sorting, has every adjacent pair differing by at most k.
int minDeletionsToMakeSpacious(const std::vector<int>& arr, int k) {
    const int n = static_cast<int>(arr.size());
    if (n <= 1) {
        return 0; // Already satisfies the condition vacuously.
    }

    // Work on a sorted copy to allow two-pointer sliding window.
    std::vector<int> sorted = arr;
    std::sort(sorted.begin(), sorted.end());

    int max_len = 1; // At least one element always remains.
    int left = 0;
    for (int right = 0; right < n; ++right) {
        // Shrink window until the difference between the ends is <= k.
        while (sorted[right] - sorted[left] > k) {
            ++left;
        }
        max_len = std::max(max_len, right - left + 1);
    }

    return n - max_len;
}
// The key insight is that the property depends only on the sorted order of the elements. Sorting the entire array in non-decreasing order does not change the set of elements or the answer, because any valid remaining subsequence can be sorted to satisfy the adjacent-difference condition, and any subset that works when sorted also works in its original order after sorting. After sorting, we want to find the longest contiguous segment (in the sorted array) where every adjacent pair has difference ≤ k. The reason the segment must be contiguous is that if we pick two elements that are not adjacent in the sorted order and keep them, all elements between them are also ≤ the right one and ≥ the left one; to ensure the condition holds for adjacent kept elements, we can simply keep the whole contiguous block, since the differences between consecutive kept elements are at most the difference between any two in the block? Actually, more precisely: if we have a sorted array and we pick a subset, the adjacent kept elements in the sorted order must have differences ≤ k. If we skip an element, the next kept element could be further away, potentially violating the condition. Thus, the optimal subset must be a contiguous subarray of the sorted array. This is because if you have two kept elements at positions `i < j` and skip some element between them, then the difference between the kept elements at `i` and `j` is at least as large as some adjacent difference in that range; if the contiguous block has all adjacent differences ≤ k, then any two elements in it also have difference ≤ k (by triangle inequality), but the reverse is not true. However, the problem requires that adjacent kept elements (after sorting the remaining) differ by ≤ k, not every pair. So if we keep a non-contiguous set, the gap between consecutive kept elements might exceed k. Therefore, any valid subset must form a contiguous segment in the sorted array. Proof: Suppose sorted array `a[1..n]` and we keep indices `p1 < p2 < ... < pm`. The condition is `a[p_{i+1}] - a[p_i] <= k` for all i. If `p_{i+1} > p_i + 1`, then there exists an index `t` with `p_i < t < p_{i+1}`. But we could add `t` to the kept set without violating the condition (since `a[t] - a[p_i] <= a[p_{i+1}] - a[p_i] <= k` and `a[p_{i+1}] - a[t] <= a[p_{i+1}] - a[p_i] <= k`), so we can always expand a non-contiguous set to a contiguous one without reducing the size. Thus the maximum size of a valid remaining set is the maximum length of a contiguous subarray where each adjacent difference is ≤ k. To find that, sort the array and use a sliding window (two pointers) to find the longest segment with `arr[j] - arr[i] <= k` for all adjacent pairs. Since the array is sorted, the condition for all adjacent pairs in a window is equivalent to `arr[j] - arr[i] <= k` (the maximum difference in the window). Because if the maximum difference between the first and last elements is ≤ k, then every adjacent difference is also ≤ k. So we can simply find the longest subarray where `arr[j] - arr[i] <= k`. The answer is `n - maxLength`. Edge cases: `n = 0`? Problem likely has `n >= 1`; if n=0, answer 0. Duplicates: difference 0 ≤ k so they are always kept together. `k` can be 0, requiring all equal values. Complexity: sorting O(n log n), sliding window O(n), total O(n log n) time and O(1) auxiliary space (excluding the input vector itself). The original snippet sorts an array and does a similar sweep, but it has a bug: it starts with `maxx = 1` and uses a while loop that might miss the first element's count if n=1 or if the first segment is the longest, but the logic is essentially the same. Our solution corrects this by using a standard two-pointer approach.
