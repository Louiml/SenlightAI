/*
Write a C++ function that, given an array of integers, its size, and two 0-based indices `l` and `r` (with `l <= r`), builds an implicit segment tree (stored in a vector of the appropriate size) and returns the minimum value among the elements in the subarray `arr[l...r]` (inclusive). The function must handle arbitrary valid indices, including the full range and single-element ranges, and must not modify the original array. The segment tree should be built internally each time the function is called (or you may design it as a helper that builds once and queries). The function signature must be `long long rangeMinimum(const std::vector<long long>& arr, int l, int r)`. Assume `arr` is non-empty and `0 <= l <= r < arr.size()`. You may implement helper functions for building and querying, but the public API must match exactly.
*/

#include <vector>
#include <algorithm>
#include <limits>

// Helper: build segment tree recursively.
void buildSegTree(const std::vector<long long>& arr, std::vector<long long>& seg, int s, int e, int idx) {
    if (s == e) {
        seg[idx] = arr[s];
        return;
    }
    int mid = s + (e - s) / 2;
    buildSegTree(arr, seg, s, mid, idx * 2);
    buildSegTree(arr, seg, mid + 1, e, idx * 2 + 1);
    seg[idx] = std::min(seg[idx * 2], seg[idx * 2 + 1]);
}

// Helper: query minimum in range [l, r] recursively.
long long querySegTree(const std::vector<long long>& seg, int s, int e, int l, int r, int idx) {
    // No overlap
    if (r < s || l > e) {
        return std::numeric_limits<long long>::max();
    }
    // Complete overlap
    if (l <= s && r >= e) {
        return seg[idx];
    }
    // Partial overlap
    int mid = s + (e - s) / 2;
    long long leftMin = querySegTree(seg, s, mid, l, r, idx * 2);
    long long rightMin = querySegTree(seg, mid + 1, e, l, r, idx * 2 + 1);
    return std::min(leftMin, rightMin);
}

// Public API: return minimum in arr[l...r] inclusive.
long long rangeMinimum(const std::vector<long long>& arr, int l, int r) {
    int n = static_cast<int>(arr.size());
    std::vector<long long> seg(4 * n + 1, 0);
    buildSegTree(arr, seg, 0, n - 1, 1);
    return querySegTree(seg, 0, n - 1, l, r, 1);
}

#include <cassert>
#include <vector>
#include <climits>

// Include the solution function here (or link to it).
// For testing, the function is assumed to be defined above.

int main() {
    // Basic test
    std::vector<long long> arr1 = {4, 1, 3, 7, 2};
    assert(rangeMinimum(arr1, 1, 3) == 1);
    assert(rangeMinimum(arr1, 0, 4) == 1);
    assert(rangeMinimum(arr1, 2, 2) == 3);
    assert(rangeMinimum(arr1, 0, 0) == 4);
    assert(rangeMinimum(arr1, 4, 4) == 2);

    // All same values
    std::vector<long long> arr2 = {5, 5, 5, 5};
    assert(rangeMinimum(arr2, 0, 3) == 5);
    assert(rangeMinimum(arr2, 1, 2) == 5);

    // Negative and large numbers
    std::vector<long long> arr3 = {-10, -5, -1, -100, 7};
    assert(rangeMinimum(arr3, 0, 4) == -100);
    assert(rangeMinimum(arr3, 0, 2) == -10);
    assert(rangeMinimum(arr3, 3, 4) == -100);

    // Single element
    std::vector<long long> arr4 = {42};
    assert(rangeMinimum(arr4, 0, 0) == 42);

    // Sorted ascending
    std::vector<long long> arr5 = {1, 2, 3, 4, 5};
    assert(rangeMinimum(arr5, 0, 4) == 1);
    assert(rangeMinimum(arr5, 2, 4) == 3);

    // Sorted descending
    std::vector<long long> arr6 = {9, 7, 5, 3, 1};
    assert(rangeMinimum(arr6, 0, 4) == 1);
    assert(rangeMinimum(arr6, 0, 2) == 5);
    assert(rangeMinimum(arr6, 3, 4) == 1);

    // Large range with many duplicates
    std::vector<long long> arr7(1000, 7);
    arr7[500] = -3;
    assert(rangeMinimum(arr7, 0, 999) == -3);
    assert(rangeMinimum(arr7, 0, 499) == 7);
    assert(rangeMinimum(arr7, 501, 999) == 7);

    return 0;
}

// The solution uses a classic recursive segment tree. The array is recursively divided into two halves until each leaf corresponds to a single element; each internal node stores the minimum of its two children. Building takes O(n) time and O(n) space (the segment tree vector is sized 4*n). Querying for range [l, r] uses three cases: if the current node’s segment is completely outside [l, r], return a very large sentinel (e.g., LLONG_MAX) so it never affects the min; if the segment is completely inside [l, r], return the stored minimum; otherwise (partial overlap), recursively query both children and take their minimum. The recursion depth is O(log n), so each query runs in O(log n) time. Edge cases include single-element queries (which hit the complete overlap case at a leaf) and full-range queries (which hit complete overlap at the root). The sentinel must be large enough (e.g., `std::numeric_limits<long long>::max()`) to avoid overflow when compared. The original array is passed by const reference to guarantee no mutation. Total complexity: build O(n) time and space, query O(log n) time, O(log n) recursion stack space.
