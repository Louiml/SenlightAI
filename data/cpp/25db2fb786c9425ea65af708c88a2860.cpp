Implement a C++ function `RangeAddRangeSum` that, given an array of `n` integers (1-indexed for convenience) and a list of `m` operations, supports two types of operations: type `1 x y v` adds `v` to every element in the subarray from index `x` to `y` inclusive, and type `2 x y` queries the sum of the subarray from index `x` to `y` inclusive. The function should return a `std::vector<long long>` of the results of all type-2 operations in the order they appear. The array values and update values may be negative, and `n` and `m` can both be up to `10^5`. Use a segment tree with lazy propagation to handle range updates and range sum queries efficiently. The function signature should be `std::vector<long long> RangeAddRangeSum(int n, const std::vector<std::tuple<int,int,int,long long>>& ops, const std::vector<long long>& initial)` where each tuple represents an operation: for type 1, the tuple is `(1, x, y, v)`, and for type 2, it is `(2, x, y, 0)` (the fourth field is ignored). The initial array is given as a 1-indexed vector of size `n+1` (index 0 unused). Provide a complete implementation without a `main` function.
#include <cassert>
#include <vector>
#include <tuple>

// Assume the solution above is included or defined before this main.

int main() {
    // Test 1: Basic update and query
    std::vector<long long> arr1 = {0, 1, 2, 3, 4, 5}; // 1-indexed: size 5
    std::vector<std::tuple<int,int,int,long long>> ops1 = {
        {1, 2, 4, 10},  // add 10 to [2,4]
        {2, 1, 5}       // query [1,5] => 1+12+13+14+5 = 45
    };
    auto res1 = RangeAddRangeSum(5, ops1, arr1);
    assert(res1.size() == 1 && res1[0] == 45);

    // Test 2: Negative values
    std::vector<long long> arr2 = {0, -5, -3, -2};
    std::vector<std::tuple<int,int,int,long long>> ops2 = {
        {2, 1, 3},  // sum = -10
        {1, 1, 2, -1},
        {2, 1, 3}   // sum = (-6)+(-4)+(-2) = -12
    };
    auto res2 = RangeAddRangeSum(3, ops2, arr2);
    assert(res2.size() == 2 && res2[0] == -10 && res2[1] == -12);

    // Test 3: Single point update and query after full range update
    std::vector<long long> arr3 = {0, 7, 7, 7};
    std::vector<std::tuple<int,int,int,long long>> ops3 = {
        {1, 1, 1, 3},  // arr[1]=10
        {2, 1, 1},     // 10
        {1, 3, 3, -2}, // arr[3]=5
        {2, 2, 3}      // 7+5=12
    };
    auto res3 = RangeAddRangeSum(3, ops3, arr3);
    assert(res3.size() == 2 && res3[0] == 10 && res3[1] == 12);

    // Test 4: Overlapping and nested updates
    std::vector<long long> arr4 = {0, 0, 0, 0, 0};
    std::vector<std::tuple<int,int,int,long long>> ops4 = {
        {1, 1, 5, 1}, // all +1 => 1,1,1,1,1
        {1, 2, 4, 2}, // adds 2 to [2,4] => 1,3,3,3,1
        {2, 1, 5}     // sum = 11
    };
    auto res4 = RangeAddRangeSum(5, ops4, arr4);
    assert(res4.size() == 1 && res4[0] == 11);

    // Test 5: Many updates and queries with large values (stress)
    const int N = 1000;
    std::vector<long long> arr5(N+1, 0);
    std::vector<std::tuple<int,int,int,long long>> ops5;
    // Add 1 to all elements from 1 to N
    ops5.emplace_back(1, 1, N, 1);
    // Then query all, should be N
    ops5.emplace_back(2, 1, N);
    // Add -1 to all elements from 1 to N (back to zero)
    ops5.emplace_back(1, 1, N, -1);
    ops5.emplace_back(2, 1, N);
    auto res5 = RangeAddRangeSum(N, ops5, arr5);
    assert(res5.size() == 2 && res5[0] == N && res5[1] == 0);

    return 0;
}
#include <vector>
#include <tuple>
#include <cstddef>

class SegmentTree {
public:
    SegmentTree(const std::vector<long long>& arr) {
        n = static_cast<int>(arr.size()) - 1; // arr is 1-indexed, index 0 unused
        tree.resize(4 * n + 5);
        lazy.resize(4 * n + 5, 0);
        build(1, 1, n, arr);
    }

    void add(int ql, int qr, long long val) { rangeAdd(1, 1, n, ql, qr, val); }
    long long query(int ql, int qr) { return rangeSum(1, 1, n, ql, qr); }

private:
    int n;
    std::vector<long long> tree;
    std::vector<long long> lazy;

    void build(int node, int l, int r, const std::vector<long long>& arr) {
        if (l == r) {
            tree[node] = arr[l];
            return;
        }
        int mid = (l + r) / 2;
        build(node * 2, l, mid, arr);
        build(node * 2 + 1, mid + 1, r, arr);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }

    void pushDown(int node, int l, int r) {
        if (lazy[node] != 0) {
            int mid = (l + r) / 2;
            // Apply to left child
            tree[node * 2] += lazy[node] * (mid - l + 1);
            lazy[node * 2] += lazy[node];
            // Apply to right child
            tree[node * 2 + 1] += lazy[node] * (r - mid);
            lazy[node * 2 + 1] += lazy[node];
            lazy[node] = 0;
        }
    }

    void rangeAdd(int node, int l, int r, int ql, int qr, long long val) {
        if (ql <= l && r <= qr) {
            tree[node] += val * (r - l + 1);
            lazy[node] += val;
            return;
        }
        pushDown(node, l, r);
        int mid = (l + r) / 2;
        if (ql <= mid) rangeAdd(node * 2, l, mid, ql, qr, val);
        if (qr > mid) rangeAdd(node * 2 + 1, mid + 1, r, ql, qr, val);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }

    long long rangeSum(int node, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tree[node];
        pushDown(node, l, r);
        int mid = (l + r) / 2;
        long long result = 0;
        if (ql <= mid) result += rangeSum(node * 2, l, mid, ql, qr);
        if (qr > mid) result += rangeSum(node * 2 + 1, mid + 1, r, ql, qr);
        return result;
    }
};

// Main function as per task specification
std::vector<long long> RangeAddRangeSum(
    int n,
    const std::vector<std::tuple<int,int,int,long long>>& ops,
    const std::vector<long long>& initial
) {
    SegmentTree st(initial);
    std::vector<long long> results;
    for (const auto& op : ops) {
        int type, x, y;
        long long v;
        std::tie(type, x, y, v) = op;
        if (type == 1) {
            st.add(x, y, v);
        } else {
            results.push_back(st.query(x, y));
        }
    }
    return results;
}
// The problem requires maintaining an array under range additions and range sum queries. A segment tree with lazy propagation is ideal because it supports both operations in `O(log n)` time each. Build the tree recursively: each node stores the sum of its segment and a lazy value representing pending additions. For an update, if the node's segment is fully inside the update range, apply the addition to the node's sum and add to its lazy value, then stop. Otherwise, push down any lazy value to children, recurse into overlapping children, and recompute the node's sum from children. For a query, similarly, if fully inside, return the node's sum; otherwise push down, recurse into overlapping children, and sum results. Important edge cases: negative values, updates that affect only one element, and nested updates that require proper lazy propagation to maintain correct sums. Time complexity is `O((n+m) log n)` and space `O(n)` for the tree.
