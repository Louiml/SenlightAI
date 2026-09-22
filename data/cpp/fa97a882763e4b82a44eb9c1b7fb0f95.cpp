/*
Implement a C++ function `rangeMinimumQuery` that, given a vector of 32-bit signed integers `arr` and a list of queries `queries` (each query is a pair of inclusive 0-based indices `[l, r]`), returns a vector of integers containing the minimum value in each queried subarray. The function must handle empty queries gracefully (return an empty vector) and must assume that all query indices are valid (i.e., `0 <= l <= r < arr.size()`). The solution should be built using a sparse table data structure for efficient static range minimum queries, with preprocessing in `O(n log n)` time and each query answered in `O(1)` time, using `O(n log n)` space. The implementation must use the classic sparse table method with powers of two and logarithms.
*/
#include <vector>
#include <cmath>
#include <algorithm>

// Given a static array and a list of inclusive range queries [l, r],
// return the minimum value in each range.
std::vector<int> rangeMinimumQuery(const std::vector<int>& arr, const std::vector<std::pair<int, int>>& queries) {
    int n = (int)arr.size();
    std::vector<int> result;
    if (n == 0 || queries.empty()) {
        return result;
    }

    // Precompute logarithms for all lengths up to n
    std::vector<int> log2(n + 1, 0);
    for (int i = 2; i <= n; ++i) {
        log2[i] = log2[i / 2] + 1;
    }

    // Build sparse table: st[k][i] = min of arr[i ... i + 2^k - 1]
    int maxK = log2[n];
    std::vector<std::vector<int>> st(maxK + 1, std::vector<int>(n));
    for (int i = 0; i < n; ++i) {
        st[0][i] = arr[i];
    }
    for (int k = 1; k <= maxK; ++k) {
        int len = 1 << k;
        for (int i = 0; i + len - 1 < n; ++i) {
            st[k][i] = std::min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
        }
    }

    // Answer each query in O(1)
    result.reserve(queries.size());
    for (const auto& q : queries) {
        int l = q.first;
        int r = q.second;
        int k = log2[r - l + 1];
        int minVal = std::min(st[k][l], st[k][r - (1 << k) + 1]);
        result.push_back(minVal);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

// Function declaration (assume the solution is included above)
std::vector<int> rangeMinimumQuery(const std::vector<int>& arr, const std::vector<std::pair<int, int>>& queries);

int main() {
    // Simple case
    std::vector<int> arr1 = {1, 3, 2, 7, 9, 11};
    std::vector<std::pair<int, int>> q1 = {{0, 1}, {1, 3}, {2, 5}};
    std::vector<int> r1 = rangeMinimumQuery(arr1, q1);
    assert((r1 == std::vector<int>{1, 2, 2}));

    // Single element
    std::vector<int> arr2 = {42};
    std::vector<std::pair<int, int>> q2 = {{0, 0}};
    assert((rangeMinimumQuery(arr2, q2) == std::vector<int>{42}));

    // All same
    std::vector<int> arr3 = {5, 5, 5, 5};
    std::vector<std::pair<int, int>> q3 = {{0, 3}, {1, 2}, {2, 3}};
    assert((rangeMinimumQuery(arr3, q3) == std::vector<int>{5, 5, 5}));

    // Full range and reversed order
    std::vector<int> arr4 = {10, 20, 30, 40};
    std::vector<std::pair<int, int>> q4 = {{0, 3}, {3, 3}, {0, 0}};
    assert((rangeMinimumQuery(arr4, q4) == std::vector<int>{10, 40, 10}));

    // Larger with typical values
    std::vector<int> arr5 = {7, 2, 9, 1, 5, 6, 3, 8};
    std::vector<std::pair<int, int>> q5 = {{0, 7}, {1, 4}, {4, 7}, {2, 5}, {0, 0}};
    std::vector<int> r5 = rangeMinimumQuery(arr5, q5);
    assert((r5 == std::vector<int>{1, 1, 3, 1, 7}));

    // Edge cases: empty queries
    std::vector<int> arr6 = {1, 2, 3};
    std::vector<std::pair<int, int>> q6 = {};
    assert(rangeMinimumQuery(arr6, q6).empty());

    // Edge cases: empty array
    std::vector<int> arr7 = {};
    std::vector<std::pair<int, int>> q7 = {{0, 0}}; // invalid but function should handle gracefully
    assert(rangeMinimumQuery(arr7, q7).empty());

    // Query length exactly power of two
    std::vector<int> arr8 = {4, 1, 3, 8, 2, 7, 5, 6};
    std::vector<std::pair<int, int>> q8 = {{1, 4}, {0, 3}, {4, 7}};
    assert((rangeMinimumQuery(arr8, q8) == std::vector<int>{1, 1, 2}));

    return 0;
}
// The core idea is to precompute, for every starting index `i` and every power of two `k`, the minimum of the subarray of length `2^k` starting at `i`. We store these in a 2D table `st[i][k]`. For `k = 0`, `st[i][0] = arr[i]`. For `k > 0`, we combine two overlapping intervals of length `2^(k-1)`: `st[i][k] = min(st[i][k-1], st[i + 2^(k-1)][k-1])`. This works because the two intervals cover the full range of length `2^k` without gaps. To answer a query `[l, r]`, let `len = r - l + 1` and `k = floor(log2(len))`. The minimum is `min(st[l][k], st[r - 2^k + 1][k])`, because these two intervals of length `2^k` together cover `[l, r]` completely (they may overlap, which is fine for min queries). Edge cases: if `arr` is empty, return empty. If `queries` is empty, return empty. We precompute a logarithm array `log2` for all possible lengths up to `n` to avoid calling `std::log2` repeatedly for floating-point issues. Time complexity: preprocessing is `O(n log n)`, each query is `O(1)`, so total is `O(n log n + q)`. Space: `O(n log n)` for the table plus `O(n)` for the log array. Must ensure no signed overflow when computing `2^k`; since `n` is at most 10^5 typical, safe.
