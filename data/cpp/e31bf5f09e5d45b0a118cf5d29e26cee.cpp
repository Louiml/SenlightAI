/*
Implement a C++ function `CircularRangeMinimum` that processes a circular array of integers of length N and supports two types of operations on a given initial array: "range add" (add a value to every element in a contiguous circular interval, inclusive of both ends) and "range minimum query" (return the minimum value in a contiguous circular interval, inclusive of both ends). The function should take the initial vector, then a list of operations (each either `{type=1, l, r, val}` for add or `{type=2, l, r, val=0}` for query, where indices are 0-based and if `l > r` the interval wraps around the end of the array), and return a vector of long long integers containing the results of all query operations in order. The array length N ≤ 100000, number of operations M ≤ 100000, and all values fit in 64-bit signed integers. The function must be efficient enough to handle worst-case inputs within typical time limits.
*/

#include <vector>
#include <algorithm>
#include <cstdint>

// Segment tree with lazy propagation for range add and range minimum query.
class RangeAddMinSegmentTree {
public:
    RangeAddMinSegmentTree(const std::vector<long long>& arr) {
        n = arr.size();
        min_val.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
        build(1, 0, n - 1, arr);
    }

    // Add val to all elements in [l, r] (inclusive).
    void range_add(int l, int r, long long val) {
        if (l > r) return;
        update(1, 0, n - 1, l, r, val);
    }

    // Return minimum value in [l, r] (inclusive).
    long long range_min(int l, int r) {
        if (l > r) return LLONG_MAX;
        return query(1, 0, n - 1, l, r);
    }

private:
    int n;
    std::vector<long long> min_val;
    std::vector<long long> lazy;

    void build(int idx, int st, int ed, const std::vector<long long>& arr) {
        if (st == ed) {
            min_val[idx] = arr[st];
            return;
        }
        int mid = (st + ed) >> 1;
        int lft = idx << 1;
        int rgt = lft | 1;
        build(lft, st, mid, arr);
        build(rgt, mid + 1, ed, arr);
        min_val[idx] = std::min(min_val[lft], min_val[rgt]);
    }

    void apply(int idx, long long val) {
        min_val[idx] += val;
        lazy[idx] += val;
    }

    void push(int idx) {
        if (lazy[idx] != 0) {
            int lft = idx << 1;
            int rgt = lft | 1;
            apply(lft, lazy[idx]);
            apply(rgt, lazy[idx]);
            lazy[idx] = 0;
        }
    }

    void update(int idx, int st, int ed, int l, int r, long long val) {
        if (st == l && ed == r) {
            apply(idx, val);
            return;
        }
        push(idx);
        int mid = (st + ed) >> 1;
        int lft = idx << 1;
        int rgt = lft | 1;
        if (r <= mid) {
            update(lft, st, mid, l, r, val);
        } else if (l > mid) {
            update(rgt, mid + 1, ed, l, r, val);
        } else {
            update(lft, st, mid, l, mid, val);
            update(rgt, mid + 1, ed, mid + 1, r, val);
        }
        min_val[idx] = std::min(min_val[lft], min_val[rgt]);
    }

    long long query(int idx, int st, int ed, int l, int r) {
        if (st == l && ed == r) {
            return min_val[idx];
        }
        push(idx);
        int mid = (st + ed) >> 1;
        int lft = idx << 1;
        int rgt = lft | 1;
        if (r <= mid) {
            return query(lft, st, mid, l, r);
        } else if (l > mid) {
            return query(rgt, mid + 1, ed, l, r);
        } else {
            return std::min(query(lft, st, mid, l, mid),
                            query(rgt, mid + 1, ed, mid + 1, r));
        }
    }
};

// Process operations on a circular array.
// operations: vector of {type, l, r, val}
//   type 1 = add val to [l, r] circular inclusive
//   type 2 = query minimum in [l, r] circular inclusive, val ignored
// Returns vector of query results.
std::vector<long long> CircularRangeMinimum(
    const std::vector<long long>& initial,
    const std::vector<std::vector<long long>>& operations) {
    int n = initial.size();
    RangeAddMinSegmentTree tree(initial);
    std::vector<long long> results;

    for (const auto& op : operations) {
        int type = static_cast<int>(op[0]);
        int l = static_cast<int>(op[1]);
        int r = static_cast<int>(op[2]);
        long long val = op[3];

        if (type == 1) {
            if (l <= r) {
                tree.range_add(l, r, val);
            } else {
                tree.range_add(l, n - 1, val);
                tree.range_add(0, r, val);
            }
        } else { // type == 2
            long long res;
            if (l <= r) {
                res = tree.range_min(l, r);
            } else {
                res = std::min(tree.range_min(l, n - 1),
                               tree.range_min(0, r));
            }
            results.push_back(res);
        }
    }
    return results;
}

#include <cassert>
#include <vector>
#include <cstdint>
#include <iostream>

// Declaration of the solution function (must match the provided implementation)
std::vector<long long> CircularRangeMinimum(
    const std::vector<long long>& initial,
    const std::vector<std::vector<long long>>& operations);

int main() {
    // Test 1: Basic non-wrapping operations
    std::vector<long long> arr1 = {1, 2, 3, 4, 5};
    std::vector<std::vector<long long>> ops1 = {
        {2, 0, 2, 0},   // min of [0,2] => 1
        {1, 1, 3, 10},  // add 10 to [1,3] => arr = [1,12,13,14,5]
        {2, 0, 4, 0},   // min of all => 1
        {2, 1, 3, 0}    // min of [1,3] => 12
    };
    auto res1 = CircularRangeMinimum(arr1, ops1);
    assert(res1.size() == 3);
    assert(res1[0] == 1);
    assert(res1[1] == 1);
    assert(res1[2] == 12);

    // Test 2: Wrapping operations
    std::vector<long long> arr2 = {5, 3, 7, 2, 6};
    std::vector<std::vector<long long>> ops2 = {
        {2, 3, 1, 0},   // wrap around: min of [3,4] and [0,1] => min(2,6,5,3)=2
        {1, 4, 2, 1},   // add 1 to [4,0,1,2] => arr = [6,4,8,2,7]
        {2, 4, 2, 0},   // min again => min(7,6,4,8)=4
        {1, 0, 4, -3},  // add -3 to all => arr = [3,1,5,-1,4]
        {2, 0, 4, 0}    // min all => -1
    };
    auto res2 = CircularRangeMinimum(arr2, ops2);
    assert(res2.size() == 3);
    assert(res2[0] == 2);
    assert(res2[1] == 4);
    assert(res2[2] == -1);

    // Test 3: Single element array
    std::vector<long long> arr3 = {42};
    std::vector<std::vector<long long>> ops3 = {
        {2, 0, 0, 0},   // => 42
        {1, 0, 0, 5},   // => 47
        {2, 0, 0, 0}    // => 47
    };
    auto res3 = CircularRangeMinimum(arr3, ops3);
    assert(res3.size() == 2);
    assert(res3[0] == 42);
    assert(res3[1] == 47);

    // Test 4: Negative and large values
    std::vector<long long> arr4 = {-1000000000000LL, 2000000000000LL};
    std::vector<std::vector<long long>> ops4 = {
        {2, 0, 1, 0},                   // min => -1e12
        {1, 0, 0, 500000000000LL},      // arr = [-5e11, 2e12]
        {2, 1, 1, 0},                   // min => 2e12
        {1, 1, 0, -3000000000000LL},    // add to both (wrap) => arr = [-3.5e12, -1e12]
        {2, 0, 1, 0}                    // min => -3.5e12
    };
    auto res4 = CircularRangeMinimum(arr4, ops4);
    assert(res4.size() == 3);
    assert(res4[0] == -1000000000000LL);
    assert(res4[1] == 2000000000000LL);
    assert(res4[2] == -3500000000000LL);

    // Test 5: Repeated updates and queries
    std::vector<long long> arr5 = {0, 0, 0, 0};
    std::vector<std::vector<long long>> ops5 = {
        {1, 0, 3, 1},   // all become 1
        {1, 1, 2, -2},  // arr = [1,-1,-1,1]
        {2, 0, 2, 0},   // min => -1
        {2, 2, 3, 0},   // min => -1
        {2, 0, 3, 0}    // min => -1
    };
    auto res5 = CircularRangeMinimum(arr5, ops5);
    assert(res5.size() == 3);
    assert(res5[0] == -1);
    assert(res5[1] == -1);
    assert(res5[2] == -1);

    // Test 6: No wrap but touching edges
    std::vector<long long> arr6 = {10, 20, 30, 40};
    std::vector<std::vector<long long>> ops6 = {
        {2, 0, 0, 0},   // 10
        {2, 3, 3, 0},   // 40
        {1, 0, 3, 100}, // all +100 => [110,120,130,140]
        {2, 0, 3, 0}    // 110
    };
    auto res6 = CircularRangeMinimum(arr6, ops6);
    assert(res6.size() == 3);
    assert(res6[0] == 10);
    assert(res6[1] == 40);
    assert(res6[2] == 110);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The core requirement is segment tree with lazy propagation supporting range addition and range minimum query, but on a circular array. The circular interval is handled by either performing a single range update/query if `l <= r`, or splitting into two non-wrapping intervals: `[l, N-1]` and `[0, r]` when `l > r`. The segment tree stores the minimum value for each segment. Lazy propagation is necessary because pure point updates would be too slow for range additions. The `update_node` function applies an addition to a node and stores it in the lazy array; `update_lazy` pushes pending additions to children before descending. `insert` performs range addition recursively, and `query` returns minimum recursively, both using lazy propagation to ensure correctness. Important edge cases: N=1 (any circular interval is just that single element), empty intervals should not occur (l and r are always valid indices), large values that require `long long` to avoid overflow, and wrapping intervals where l > r must be split correctly without double-counting the boundary between r and l. Time complexity is O((N + M) log N) since each operation touches O(log N) nodes. Space complexity is O(N) for the segment tree arrays (4*N each for minimum and lazy, using long long).
