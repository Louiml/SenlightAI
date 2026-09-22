// Given an array `a` of length `n` (1-indexed), define for each index `i`:
// - `l1[i]` = the smallest index `L` such that `a[i]` is the unique minimum in the subarray `a[L..i]` (i.e., `a[i] < a[k]` for all `k` in `[L, i-1]`).
// - `r1[i]` = the largest index `R` such that `a[i]` is the unique minimum in `a[i..R]` (i.e., `a[i] < a[k]` for all `k` in `[i+1, R]`).
// - `l2[i]`, `r2[i]` are defined similarly but for **maximum** (strictly greater than all others in the subarray).
//
// Write a function `findLongestSubarray` that takes the vector `a` (using 1-indexed access, so `a[0]` is unused) and returns a pair `{length, start}` where `length` is the length of the longest contiguous subarray such that there exists at least one index `minPos` where `a[minPos]` is the unique minimum **and** at least one index `maxPos` where `a[maxPos]` is the unique maximum within that subarray. If multiple subarrays have the same maximum length, return the one with the smallest starting index. The subarray must have length at least 1. The input array may contain duplicate values, so uniqueness means strict inequality.
// The core idea is based on the provided snippet: for each position `i` as the unique minimum, we can compute the range `[l1[i], r1[i]]` where `a[i]` is the unique minimum. For any subarray starting at some `L` and ending at `R`, `a[i]` being unique minimum implies `L ≤ i ≤ R` and `L ≥ l1[i]`, `R ≤ r1[i]`. Similarly, for each position `j` as unique maximum, the valid range is `[l2[j], r2[j]]`. For a fixed `i` (minimum position), the maximum position `j` must satisfy: `l1[i] ≤ j ≤ r1[i]` and also `l2[j] ≤ l1[i]` (so that the maximum can extend to at least the subarray's start) and `r2[j] ≥ i` (so that the maximum can reach at least the subarray's end). Then the possible subarrays that have `i` as unique minimum and `j` as unique maximum can have start `L = l1[i]` (to maximize length with this combination) and end `R = min(r1[i], r2[j])`. The length is `R - L + 1`. To handle all `j` efficiently, we sort events: we process by increasing `l2[j]`, insert `j` with value `r2[j]` into a segment tree indexed by `j` (position), and when we process a minimum at `i`, we need the maximum `r2[j]` among all `j` such that `l2[j] ≤ l1[i]` and `j` in `[l1[i], r1[i]]` (because the condition `r2[j] ≥ i` is automatically satisfied if we also check that the queried maximum `j` is at least `l1[i]`? Actually, we need `r2[j] ≥ i`, so we must filter by that. But the segment tree stores `r2[j]`, and when we query a range `[l1[i], r1[i]]`, we get the maximum `r2[j]` in that range among inserted `j`s. However, we also need `l2[j] ≤ l1[i]`. So we process positions in increasing `l1[i]` and insert all `j` with `l2[j] ≤ current l1[i]`. Then query the segment tree for range `[l1[i], r1[i]]` to get max `r2[j]`. If that max `r2[j] ≥ i`, then we can form subarrays with start `L = l1[i]` and end `R = min(r1[i], r2[j])`, length = `R - L + 1`. We also need to consider that the minimum might be on the right side of the maximum, but the symmetry is handled by swapping the roles of min and max and running the same algorithm again, as in the snippet. That is, after computing with `l1/r1` as minima and `l2/r2` as maxima, we swap the arrays and run again, which covers the case where the maximum is to the left of the minimum. Edge cases: duplicates require strict uniqueness, so monotonic queues must use strict comparisons (e.g., pop while top > current for minima, not >=). Also, the segment tree must support point updates (insert a value at a position) and range maximum queries. The time complexity: computing `l1,r1,l2,r2` via monotonic stacks/queues takes O(n). The main processing involves sorting events or using buckets of `l1` and `l2` values; using an array of vectors for events by `l2` value, and processing `i` from 1 to n in increasing `l1[i]` would require sorting, but since `l1` values are between 1..n, we can bucket by `l1[i]`. The segment tree operations are O(log n) per insertion and query, leading to O(n log n) overall. Space is O(n). The function returns the best `{length, start}`.
#include <vector>
#include <algorithm>
#include <utility>

// Returns {length, start} of the longest subarray where there exists a unique minimum and a unique maximum.
std::pair<int, int> findLongestSubarray(const std::vector<int>& a) {
    int n = (int)a.size() - 1; // a[0] unused, 1-indexed
    if (n == 0) return {0, 0};

    // Compute l1/r1 (unique minima) and l2/r2 (unique maxima) using monotonic stacks.
    std::vector<int> l1(n+1), r1(n+1), l2(n+1), r2(n+1);
    std::vector<int> st;
    st.reserve(n);

    // For minima: strictly greater than current -> pop
    st.clear();
    st.push_back(0);
    for (int i = 1; i <= n; ++i) {
        while (st.back() != 0 && a[st.back()] > a[i]) st.pop_back();
        l1[i] = st.back() + 1;
        st.push_back(i);
    }
    st.clear();
    st.push_back(n+1);
    for (int i = n; i >= 1; --i) {
        while (st.back() != n+1 && a[st.back()] > a[i]) st.pop_back();
        r1[i] = st.back() - 1;
        st.push_back(i);
    }

    // For maxima: strictly less than current -> pop
    st.clear();
    st.push_back(0);
    for (int i = 1; i <= n; ++i) {
        while (st.back() != 0 && a[st.back()] < a[i]) st.pop_back();
        l2[i] = st.back() + 1;
        st.push_back(i);
    }
    st.clear();
    st.push_back(n+1);
    for (int i = n; i >= 1; --i) {
        while (st.back() != n+1 && a[st.back()] < a[i]) st.pop_back();
        r2[i] = st.back() - 1;
        st.push_back(i);
    }

    int best_len = 1, best_start = 1;
    auto process = [&]() {
        // For each minimum position i, we want maximum r2[j] with l2[j] <= l1[i], j in [l1[i], r1[i]], and r2[j] >= i.
        // Build buckets: for each l2 value, store list of (j, r2[j])
        std::vector<std::vector<std::pair<int,int>>> by_l2(n+1);
        for (int j = 1; j <= n; ++j) {
            by_l2[l2[j]].push_back({j, r2[j]});
        }

        // Segment tree for max value over positions 1..n
        int size = 1;
        while (size < n) size <<= 1;
        std::vector<int> seg(2*size, -1);

        auto update = [&](int pos, int val) {
            int idx = pos + size - 1;
            seg[idx] = std::max(seg[idx], val);
            idx >>= 1;
            while (idx) {
                seg[idx] = std::max(seg[idx<<1], seg[idx<<1|1]);
                idx >>= 1;
            }
        };
        auto query = [&](int l, int r) {
            if (l > r) return -1;
            l += size - 1;
            r += size - 1;
            int res = -1;
            while (l <= r) {
                if (l & 1) res = std::max(res, seg[l++]);
                if (!(r & 1)) res = std::max(res, seg[r--]);
                l >>= 1; r >>= 1;
            }
            return res;
        };

        int inserted_upto = 0;
        for (int i = 1; i <= n; ++i) {
            int L = l1[i];
            // Insert all j with l2[j] <= L
            while (inserted_upto < L) {
                ++inserted_upto;
                for (const auto& p : by_l2[inserted_upto]) {
                    update(p.first, p.second);
                }
            }
            // Query range [L, r1[i]]
            int max_r2 = query(L, r1[i]);
            if (max_r2 >= i) {
                int R = std::min(r1[i], max_r2);
                int len = R - L + 1;
                if (len > best_len || (len == best_len && L < best_start)) {
                    best_len = len;
                    best_start = L;
                }
            }
        }
    };

    process();

    // Swap roles: now l1/r1 are maxima, l2/r2 are minima
    for (int i = 1; i <= n; ++i) {
        std::swap(l1[i], l2[i]);
        std::swap(r1[i], r2[i]);
    }
    process();

    return {best_len, best_start};
}
#include <cassert>
#include <vector>
#include <utility>

// Function declaration
std::pair<int, int> findLongestSubarray(const std::vector<int>& a);

int main() {
    // Test 1: simple case with unique min and max
    std::vector<int> a1 = {0, 1, 3, 2, 4};
    auto res1 = findLongestSubarray(a1);
    assert(res1.first == 5 && res1.second == 1);

    // Test 2: duplicates, need strict uniqueness
    std::vector<int> a2 = {0, 1, 1, 1, 2};
    auto res2 = findLongestSubarray(a2);
    // Longest: [1,2] length 2 (min=1 at index 1, max=2 at index 4) or [1,3]? Actually [1,2] length 2, start=1
    assert(res2.first == 2 && res2.second == 1);

    // Test 3: all same values -> any subarray length 1, smallest start
    std::vector<int> a3 = {0, 5, 5, 5};
    auto res3 = findLongestSubarray(a3);
    assert(res3.first == 1 && res3.second == 1);

    // Test 4: decreasing array, min at end, max at start
    std::vector<int> a4 = {0, 5, 4, 3, 2, 1};
    auto res4 = findLongestSubarray(a4);
    // Whole array works: min=1 @6, max=5 @1, length 6 start 1
    assert(res4.first == 6 && res4.second == 1);

    // Test 5: increasing array
    std::vector<int> a5 = {0, 1, 2, 3, 4};
    auto res5 = findLongestSubarray(a5);
    assert(res5.first == 5 && res5.second == 1);

    // Test 6: pattern where min and max swapped sides
    std::vector<int> a6 = {0, 2, 1, 4, 3};
    auto res6 = findLongestSubarray(a6);
    // Whole array works? min=1 @3, max=4 @4, yes
    assert(res6.first == 5 && res6.second == 1);

    // Test 7: exact from snippet style
    std::vector<int> a7 = {0, 3, 1, 2};
    auto res7 = findLongestSubarray(a7);
    // [1,3] length 3: min=1@3, max=3@1; also [2,3] length 2. So length 3 start 1
    assert(res7.first == 3 && res7.second == 1);

    // Test 8: tie-breaking for equal length
    std::vector<int> a8 = {0, 2, 1, 2, 1, 2};
    auto res8 = findLongestSubarray(a8);
    // Possible longest length 2, starts at 1,2,3,4,5? Start 1 is smallest
    assert(res8.first == 2 && res8.second == 1);

    // Test 9: n=1
    std::vector<int> a9 = {0, 42};
    auto res9 = findLongestSubarray(a9);
    assert(res9.first == 1 && res9.second == 1);

    // Test 10: random larger test
    std::vector<int> a10 = {0, 7, 2, 9, 1, 5, 8, 3, 6, 4};
    auto res10 = findLongestSubarray(a10);
    // Whole array: min=1@5, max=9@4, works => length 10 start 1
    assert(res10.first == 10 && res10.second == 1);

    return 0;
}
