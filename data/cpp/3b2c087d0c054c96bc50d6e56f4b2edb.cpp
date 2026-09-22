/*
Write a C++ function that, given an integer vector `v` indexed from 1 to `n` (where `v[0]` is unused), and three integers `n`, `a`, and `b` with `1 <= a < b <= n`, returns the sum of the elements `v[a] + v[a+1] + ... + v[b-1]` (i.e., the sum from index `a` up to but not including index `b`). The function should be robust for `n` up to 2,000,000 and values up to 1,000,000,000, and it must not modify the input vector. The input vector is guaranteed to have at least `n+1` elements (where element 0 is ignored), and there is no need to validate the indices. You may assume all values fit within a 64-bit signed integer.
*/
#include <vector>
#include <cstdint>

/**
 * Returns the sum of elements v[a] + v[a+1] + ... + v[b-1].
 * The vector v is 1-indexed, meaning v[0] is ignored.
 * Precondition: 1 <= a < b <= v.size()-1.
 */
long long sumSubarray(const std::vector<long long>& v, int a, int b) {
    long long total = 0;
    for (int i = a; i < b; ++i) {
        total += v[i];
    }
    return total;
}
#include <cassert>
#include <vector>
#include <cstdint>

// Declaration of the function being tested
long long sumSubarray(const std::vector<long long>& v, int a, int b);

int main() {
    // Basic test from the snippet: n=5, v[1..4] = {1,3,2,4}, a=2, b=4 => sum = v[2]+v[3] = 3+2 = 5
    std::vector<long long> v1 = {0, 1, 3, 2, 4, 0}; // size 6, indices 0..5, but n=5
    assert(sumSubarray(v1, 2, 4) == 5);

    // Single element range: v[1]=10, a=1, b=2
    std::vector<long long> v2 = {0, 10};
    assert(sumSubarray(v2, 1, 2) == 10);

    // Entire range from index 1 to n-1: v = {0, 5, -3, 7}, n=4, a=1, b=4 => sum = 5 + (-3) + 7 = 9
    std::vector<long long> v3 = {0, 5, -3, 7, 0};
    assert(sumSubarray(v3, 1, 4) == 9);

    // Larger values and range to test potential overflow: all 1e9, length 1000000
    std::vector<long long> v4(1000001, 0);
    for (int i = 1; i <= 1000000; ++i) v4[i] = 1000000000LL;
    assert(sumSubarray(v4, 1, 1000001) == 1000000000000000LL); // 1e15

    // Negative values
    std::vector<long long> v5 = {0, -5, -10, -3};
    assert(sumSubarray(v5, 2, 4) == -13); // -10 + -3

    // a=1, b=2 (single first element)
    std::vector<long long> v6 = {0, 42, 7};
    assert(sumSubarray(v6, 1, 2) == 42);

    return 0;
}
// The problem is a classic prefix-sum or direct summation task. The simplest approach is to iterate from `a` to `b-1` and accumulate the values, which takes `O(b-a)` time and `O(1)` extra space. However, since `n` can be up to 2,000,000, this is still perfectly fine (worst case ~2 million operations). An alternative is to precompute a prefix sum array once, allowing each query in `O(1)`, but since there is only a single query in the task, the direct loop is sufficient and simpler. Edge cases include when `a` and `b` are adjacent (then `b-a = 1` and we sum exactly one element), and when `a` is 1 and `b` is `n` (then we sum everything from index 1 to `n-1`). Since `v` is treated as 1-indexed, we must be careful to start the loop at `i = a` and stop at `i < b`. We use `long long` for the accumulator to avoid overflow, as values can be up to 1e9 and there can be up to ~2e6 elements, so the sum can be up to ~2e15, which fits in a 64-bit integer. Time complexity: `O(b-a)` for the direct loop, worst-case `O(n)`. Space complexity: `O(1)` auxiliary.
