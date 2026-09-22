/*
You are given an array of `n` integers (1-indexed in the problem statement, but your function will use 0-indexed parameters) and a sequence of `q` operations. Operation type 1 is `update(idx, val)`: set the element at index `idx` (0-indexed) to `val`. Operation type 2 is `range_sum(l, r)`: return the sum of elements from index `l` to `r` inclusive (0-indexed). Write a C++ function `std::vector<long long> process_queries(const std::vector<long long>& initial_array, const std::vector<std::vector<long long>>& queries)` that processes all queries in order and returns the results of all range-sum queries as a vector of long long values. The input array size `n` can be up to `2e5` and number of queries `q` can be up to `2e5`. All values and sums fit in 64-bit signed integers. Use a segment tree for efficiency, since naive range queries would be too slow.
*/
#include <vector>
#include <cstdint>

// Process point updates and range sum queries using an iterative segment tree.
// initial_array: 0-indexed initial array of size n
// queries: each element is [type, arg1, arg2]; type 1 -> update(arg1, arg2), type 2 -> sum from arg1 to arg2 inclusive
std::vector<long long> process_queries(const std::vector<long long>& initial_array,
                                       const std::vector<std::vector<long long>>& queries) {
    const long long n = static_cast<long long>(initial_array.size());
    std::vector<long long> st(2 * n, 0);

    // Build: set leaves and then compute internal nodes
    for (long long i = 0; i < n; ++i) {
        st[i + n] = initial_array[i];
    }
    for (long long i = n - 1; i > 0; --i) {
        st[i] = st[2 * i] + st[2 * i + 1];
    }

    std::vector<long long> results;
    results.reserve(queries.size()); // Reserve to avoid reallocations

    for (const auto& q : queries) {
        if (q[0] == 1) {
            // Update: set element at index q[1] (0-indexed) to q[2]
            long long idx = q[1];
            long long val = q[2];
            long long pos = idx + n;
            st[pos] = val;
            pos /= 2;
            while (pos > 0) {
                st[pos] = st[2 * pos] + st[2 * pos + 1];
                pos /= 2;
            }
        } else {
            // Query: sum from index q[1] to q[2] inclusive (0-indexed)
            long long l = q[1] + n;
            long long r = q[2] + n;
            long long sum = 0;
            while (l <= r) {
                if (l & 1) {
                    sum += st[l];
                    ++l;
                }
                if (!(r & 1)) {
                    sum += st[r];
                    --r;
                }
                l >>= 1;
                r >>= 1;
            }
            results.push_back(sum);
        }
    }

    return results;
}
#include <cassert>
#include <vector>

// The solution function is declared above, but for the test we include it here.
// In a real separate file, include the solution header.

int main() {
    // Test 1: Basic example from the snippet (after converting to 0-indexed)
    {
        std::vector<long long> arr = {1, 2, 3, 4, 5};
        std::vector<std::vector<long long>> queries = {
            {2, 0, 2},   // sum 1+2+3 = 6
            {1, 1, 10},  // set index 1 to 10 -> arr = {1,10,3,4,5}
            {2, 0, 4},   // sum 1+10+3+4+5 = 23
            {2, 1, 1},   // sum 10
            {2, 4, 4}    // sum 5
        };
        std::vector<long long> expected = {6, 23, 10, 5};
        assert(process_queries(arr, queries) == expected);
    }

    // Test 2: Empty array
    {
        std::vector<long long> arr = {};
        std::vector<std::vector<long long>> queries = {
            {2, 0, 0} // invalid but should not crash; but we assume valid input
        };
        // Instead, just test no range queries on empty
        std::vector<std::vector<long long>> q2 = {};
        assert(process_queries(arr, q2).empty());
    }

    // Test 3: Single element
    {
        std::vector<long long> arr = {7};
        std::vector<std::vector<long long>> queries = {
            {2, 0, 0},   // 7
            {1, 0, -3},  // arr = [-3]
            {2, 0, 0}    // -3
        };
        std::vector<long long> expected = {7, -3};
        assert(process_queries(arr, queries) == expected);
    }

    // Test 4: Large range with negative numbers
    {
        std::vector<long long> arr = {-5, -1, -10, 0, 3};
        std::vector<std::vector<long long>> queries = {
            {2, 0, 4},   // -5-1-10+0+3 = -13
            {2, 1, 3},   // -1-10+0 = -11
            {1, 2, 100}, // arr = [-5,-1,100,0,3]
            {2, 0, 4}    // -5-1+100+0+3 = 97
        };
        std::vector<long long> expected = {-13, -11, 97};
        assert(process_queries(arr, queries) == expected);
    }

    // Test 5: Alternate updates and queries
    {
        std::vector<long long> arr = {1, 2, 3};
        std::vector<std::vector<long long>> queries = {
            {2, 0, 0},   // 1
            {1, 0, 10},  // arr = {10,2,3}
            {2, 0, 2},   // 15
            {1, 2, 5},   // arr = {10,2,5}
            {2, 1, 2}    // 2+5 = 7
        };
        std::vector<long long> expected = {1, 15, 7};
        assert(process_queries(arr, queries) == expected);
    }

    return 0;
}
// The core requirement is to support point updates and range sum queries efficiently. A Fenwick tree or segment tree both work, but a segment tree is specified by the original snippet. The segment tree is built over an array of size `n` stored in a flat array of size `2*n` (the standard iterative segment tree layout). For each index `i` from 0 to n-1, the leaf at position `i+n` holds `arr[i]`. Then for `i` from `n-1` down to 1, the parent node at `i` stores the sum of its two children at `2*i` and `2*i+1`. For an update, set the leaf at `idx+n` to `val` and then move upwards, updating each parent as the sum of its two children until reaching the root. For a range query `[l, r]` inclusive, use the standard iterative approach: `l += n`, `r += n`. While `l <= r`, if `l` is odd, add `st[l]` to the result and increment `l`; if `r` is even, add `st[r]` to the result and decrement `r`; then shift both `l` and `r` up by dividing by 2. This works because each level of the tree covers disjoint intervals, and the loop accumulates the sums of the disjoint segments that exactly cover `[l, r]`. Edge cases include updates out of bounds (assume valid input), range with `l > r` (not expected), and empty arrays (should handle gracefully by returning empty vector). Time complexity: build is O(n), each update and query is O(log n), total O((n+q) log n). Space complexity is O(n) for the segment tree.
