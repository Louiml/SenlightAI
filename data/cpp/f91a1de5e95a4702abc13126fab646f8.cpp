// Write a C++ function `long long rangeMinimumSum(const std::vector<int>& arr, const std::vector<std::pair<int,int>>& queries)` that takes an array of integers and a list of query pairs `(L, R)` (with 0-based inclusive indices) and returns the sum of the minimum element in the subarray `arr[L..R]` over all queries. The array may have up to 10^6 elements, and there may be up to 10^6 queries. The function must handle overlapping, nested, and adjacent queries efficiently—a naive per-query scan is too slow. The result may exceed 32-bit integer range, so return a `long long`. The input array is not modified, and queries are guaranteed to have `0 <= L <= R < n`. The function should be optimized for large inputs using a sparse table for Range Minimum Queries (RMQ) with `O(n log n)` preprocessing and `O(1)` per query, and must work correctly when all elements are equal, when the array has a single element, and when queries have length 1.
The problem is a classic Range Minimum Query (RMQ) task. The naive approach would be to scan each subarray for each query, which is `O(n * m)` and would be too slow for limits up to 10^6. The optimal solution builds a sparse table: `st[i][j]` stores the minimum of the subarray of length `2^j` starting at index `i`. Preprocessing: first fill `st[i][0]` with the array values; then for each level `j` from 1 to `floor(log2(n))`, combine two intervals of length `2^(j-1)` to get the minimum of length `2^j`. We also precompute `logs[k]` for all `k` up to `n` using the recurrence `logs[1]=0`, `logs[i]=logs[i/2]+1`. To answer a query `(L, R)`, we compute `len = R-L+1`, `j = logs[len]`, and take the minimum of `st[L][j]` and `st[R - (1<<j) + 1][j]`. This works because the two intervals of length `2^j` cover the entire range (they overlap but the minimum is unaffected by overlap). Edge cases: when `len = 1`, `j=0` and both intervals point to the same element; when the array has one element, the sparse table has only one column. All elements equal: the function returns the same value for every query, and the sum is simply `value * m`. Time complexity: preprocessing `O(n log n)`, each query `O(1)`, total `O(n log n + m)`. Space: `O(n log n)` for the sparse table, `O(n)` for logs.
#include <vector>
#include <algorithm>
#include <cmath>

// Precomputed logs and sparse table are built internally per call to avoid global state.
// Returns the sum of minimums over all queries in 0-based inclusive range [L, R].
long long rangeMinimumSum(const std::vector<int>& arr, const std::vector<std::pair<int,int>>& queries) {
    int n = static_cast<int>(arr.size());
    if (n == 0) return 0;

    // Compute floor(log2) for all lengths up to n
    std::vector<int> logs(n + 1, 0);
    for (int i = 2; i <= n; ++i) {
        logs[i] = logs[i / 2] + 1;
    }

    int maxLog = logs[n] + 1;
    // Sparse table: st[i][j] = min of arr[i .. i + 2^j - 1]
    std::vector<std::vector<int>> st(n, std::vector<int>(maxLog));
    for (int i = 0; i < n; ++i) {
        st[i][0] = arr[i];
    }
    for (int j = 1; (1 << j) <= n; ++j) {
        int len = 1 << j;
        int half = 1 << (j - 1);
        for (int i = 0; i + len <= n; ++i) {
            st[i][j] = std::min(st[i][j - 1], st[i + half][j - 1]);
        }
    }

    long long total = 0;
    for (const auto& q : queries) {
        int L = q.first;
        int R = q.second;
        int len = R - L + 1;
        int j = logs[len];
        int minVal = std::min(st[L][j], st[R - (1 << j) + 1][j]);
        total += static_cast<long long>(minVal);
    }
    return total;
}
#include <cassert>
#include <vector>
#include <utility>

// Declare the function here (in practice it would be included from header)
long long rangeMinimumSum(const std::vector<int>& arr, const std::vector<std::pair<int,int>>& queries);

int main() {
    // Basic case
    std::vector<int> arr1 = {1, 3, 2, 5};
    std::vector<std::pair<int,int>> q1 = {{0, 3}, {1, 2}, {2, 3}};
    assert(rangeMinimumSum(arr1, q1) == (1 + 2 + 2));  // 5

    // Single element array
    std::vector<int> arr2 = {7};
    std::vector<std::pair<int,int>> q2 = {{0, 0}};
    assert(rangeMinimumSum(arr2, q2) == 7);

    // All equal elements
    std::vector<int> arr3 = {4, 4, 4};
    std::vector<std::pair<int,int>> q3 = {{0, 2}, {0, 0}, {1, 1}, {2, 2}};
    assert(rangeMinimumSum(arr3, q3) == 4 * 4);

    // Large range with negative numbers
    std::vector<int> arr4 = {-5, -1, -3, -2};
    std::vector<std::pair<int,int>> q4 = {{0, 0}, {1, 3}, {0, 3}};
    assert(rangeMinimumSum(arr4, q4) == (-5 + (-3) + (-5)));  // -13

    // Adjacent and overlapping queries
    std::vector<int> arr5 = {10, 20, 30, 40};
    std::vector<std::pair<int,int>> q5 = {{0, 1}, {1, 2}, {2, 3}, {0, 3}};
    assert(rangeMinimumSum(arr5, q5) == (10 + 20 + 30 + 10));  // 70

    // Many small queries (length 1) sums to sum of array
    std::vector<int> arr6 = {2, 4, 6, 8};
    std::vector<std::pair<int,int>> q6 = {{0, 0}, {1, 1}, {2, 2}, {3, 3}};
    assert(rangeMinimumSum(arr6, q6) == 2 + 4 + 6 + 8);

    // Empty queries list
    std::vector<int> arr7 = {1, 2};
    std::vector<std::pair<int,int>> q7 = {};
    assert(rangeMinimumSum(arr7, q7) == 0);

    // Edge case: n=2, query covering both ends
    std::vector<int> arr8 = {100, -1};
    std::vector<std::pair<int,int>> q8 = {{0, 1}, {1, 1}};
    assert(rangeMinimumSum(arr8, q8) == (-1) + (-1));

    return 0;
}
