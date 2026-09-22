/*
You are given a sequence of `n` sorted non-overlapping intervals, where each interval `i` (for `1 <= i <= n`) has a starting point equal to the cumulative sum of all previous intervals plus 1, and a length equal to `a[i]`. In other words, the intervals are consecutive over the positive integers, with the first interval covering `[1, a[1]]`, the second covering `[a[1]+1, a[1]+a[2]]`, and so on. You will then be given `m` queries, each asking for a positive integer `x`. For each query, you must find and return the index `i` of the interval that contains `x` (i.e., `x` is within the range of interval `i`). Write a C++ function `std::vector<int> findIntervalIndices(const std::vector<long long>& lengths, const std::vector<long long>& queries)` that takes the vector of interval lengths and the vector of query values, and returns a vector of the interval indices (1-based) corresponding to each query. Assume all inputs are positive and the total sum of lengths fits within `long long`.
*/
#include <vector>
#include <algorithm>

// Given sorted consecutive interval lengths, find the 1-based index of the interval
// containing each query value x. The intervals partition the positive integers.
std::vector<int> findIntervalIndices(const std::vector<long long>& lengths, const std::vector<long long>& queries) {
    int n = static_cast<int>(lengths.size());
    std::vector<long long> prefix(n);
    prefix[0] = lengths[0];
    for (int i = 1; i < n; ++i) {
        prefix[i] = prefix[i-1] + lengths[i];
    }

    std::vector<int> result;
    result.reserve(queries.size());
    for (long long x : queries) {
        // Binary search for the first prefix value >= x
        auto it = std::lower_bound(prefix.begin(), prefix.end(), x);
        // Distance from begin gives 0-based index; add 1 for 1-based result
        result.push_back(static_cast<int>(it - prefix.begin()) + 1);
    }
    return result;
}
#include <cassert>
#include <vector>

// Function under test
std::vector<int> findIntervalIndices(const std::vector<long long>& lengths, const std::vector<long long>& queries);

int main() {
    // Test 1: Simple three intervals
    std::vector<long long> lengths1 = {3, 2, 4};
    std::vector<long long> queries1 = {1, 3, 4, 5, 6, 9, 7};
    std::vector<int> expected1 = {1, 1, 2, 2, 3, 3, 3};
    assert(findIntervalIndices(lengths1, queries1) == expected1);

    // Test 2: Single interval
    std::vector<long long> lengths2 = {10};
    std::vector<long long> queries2 = {1, 10, 5};
    std::vector<int> expected2 = {1, 1, 1};
    assert(findIntervalIndices(lengths2, queries2) == expected2);

    // Test 3: Many intervals of length 1
    std::vector<long long> lengths3 = {1, 1, 1, 1};
    std::vector<long long> queries3 = {1, 2, 3, 4};
    std::vector<int> expected3 = {1, 2, 3, 4};
    assert(findIntervalIndices(lengths3, queries3) == expected3);

    // Test 4: Query equal to a boundary
    std::vector<long long> lengths4 = {2, 3, 1};
    // prefix: [2, 5, 6]
    std::vector<long long> queries4 = {2, 5, 6};
    std::vector<int> expected4 = {1, 2, 3};
    assert(findIntervalIndices(lengths4, queries4) == expected4);

    // Test 5: Large values and many queries
    std::vector<long long> lengths5 = {1000000000LL, 500000000LL, 2000000000LL};
    std::vector<long long> queries5 = {1, 1000000000LL, 1000000001LL, 1500000000LL, 3500000000LL};
    std::vector<int> expected5 = {1, 1, 2, 2, 3};
    assert(findIntervalIndices(lengths5, queries5) == expected5);

    // Test 6: Empty queries
    std::vector<long long> lengths6 = {5};
    std::vector<long long> queries6 = {};
    std::vector<int> expected6 = {};
    assert(findIntervalIndices(lengths6, queries6) == expected6);

    return 0;
}
// The key is to transform the interval lengths into a prefix sum array where the cumulative sum up to position `i` gives the end point of interval `i`. For example, if lengths are `[3, 2, 4]`, prefix sums are `[3, 5, 9]`. Then interval `1` covers `[1,3]`, interval `2` covers `[4,5]`, and interval `3` covers `[6,9]`. For any query `x`, we need to find the smallest index `i` such that `prefix[i] >= x`. Since the prefix sums are strictly increasing (because lengths are positive), we can use binary search (lower_bound). The answer is the 1-based index of that prefix sum. Edge cases: if the query is exactly equal to a prefix sum, it belongs to that interval, not the next one. All queries and lengths are positive, so we don't need to handle zero or negative. Complexity: building the prefix sum takes `O(n)` time and `O(n)` space for the prefix array. Each query is answered in `O(log n)` time, giving overall `O(n + m log n)` time and `O(n)` auxiliary space.
