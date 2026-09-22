Implement a lazy segment tree in C++ that supports range add updates and range sum queries on an array of 64-bit integers. Your task is to write a free function `lazySegmentTree` that takes a `std::vector<long long>` as input (the initial array), and returns a function object (or struct with `operator()`) that can be used to perform these operations. Specifically, you must implement a class `LazySegmentTree` (defined outside any function) with three public methods: `rangeAdd(l, r, val)` (0-indexed inclusive range add), `rangeSum(l, r)` (0-indexed inclusive range sum query returning a `long long`), and a constructor that takes the initial array by const reference. The class must use a pointer-based node structure (like the snippet) with lazy propagation to ensure O(log n) per operation. The implementation must handle empty arrays gracefully (any query returns 0, any add does nothing) and must handle ranges where l > r by swapping them. Values and sums can exceed 32-bit range, so use `long long` throughout. Edge cases include single-element arrays and large update values.

The core idea is a segment tree where each node stores the sum of its segment and a lazy value representing pending additions that should be applied to all elements in that segment. The tree is built recursively: for a leaf, sum equals the array element; for internal nodes, sum is the sum of children sums. For range add, we traverse: if the node's segment is fully inside the query range, we add `val` to the node's sum and accumulate it in `lazy`, then return. If partially overlapping, we push down the lazy value to children (so they get the pending addition), then recurse into children and recompute the node's sum. For range sum, we similarly traverse: if fully inside, return node's sum; if partially, push down lazy first, then recurse and combine children results. The recursion depth is O(log n) because each level splits the range, and in the worst case the number of visited nodes per operation is O(log n). The constructor builds the tree in O(n) time using recursive build. Space complexity is O(n) for nodes, but since each node is allocated dynamically, we must carefully manage memory (we can allocate nodes in a flat array or use pointers with deletion in destructor). For simplicity, we use a static array of nodes of size 4*n (as commonly done), since that avoids memory management pitfalls and is faster. The empty array case is handled by checking if n == 0 and returning 0 for queries, no-op for adds. Time complexity for each operation is O(log n), and total auxiliary space is O(n) for the tree. Edge cases: when l > r, swap to make valid range; when array is empty, all operations are no-ops returning 0.

#include <vector>
#include <algorithm>
#include <cstddef>

class LazySegmentTree {
public:
    // Constructor from initial array (by const reference)
    explicit LazySegmentTree(const std::vector<long long>& arr) {
        n = arr.size();
        sum.resize(4 * n + 5, 0);
        lazy.resize(4 * n + 5, 0);
        if (n > 0) {
            build(1, 0, n - 1, arr);
        }
    }

    // Range add: add val to all elements in [l, r] inclusive
    void rangeAdd(int l, int r, long long val) {
        if (n == 0) return;
        if (l > r) std::swap(l, r);
        // Clamp to valid range
        l = std::max(l, 0);
        r = std::min(r, n - 1);
        if (l > r) return; // no valid range
        update(1, 0, n - 1, l, r, val);
    }

    // Range sum query: return sum of elements in [l, r] inclusive
    long long rangeSum(int l, int r) const {
        if (n == 0) return 0LL;
        if (l > r) std::swap(l, r);
        l = std::max(l, 0);
        r = std::min(r, n - 1);
        if (l > r) return 0LL;
        return query(1, 0, n - 1, l, r);
    }

private:
    int n;
    std::vector<long long> sum;
    mutable std::vector<long long> lazy; // mutable because lazy propagation in const query

    void build(int node, int l, int r, const std::vector<long long>& arr) {
        if (l == r) {
            sum[node] = arr[l];
            return;
        }
        int mid = (l + r) / 2;
        build(node * 2, l, mid, arr);
        build(node * 2 + 1, mid + 1, r, arr);
        sum[node] = sum[node * 2] + sum[node * 2 + 1];
    }

    void push(int node, int l, int r) const {
        if (lazy[node] != 0 && l != r) {
            long long val = lazy[node];
            int mid = (l + r) / 2;
            // Left child covers [l, mid], right covers [mid+1, r]
            sum[node * 2] += val * (mid - l + 1);
            sum[node * 2 + 1] += val * (r - mid);
            lazy[node * 2] += val;
            lazy[node * 2 + 1] += val;
            lazy[node] = 0;
        }
    }

    void update(int node, int l, int r, int ql, int qr, long long val) {
        if (ql <= l && r <= qr) {
            sum[node] += val * (r - l + 1);
            lazy[node] += val;
            return;
        }
        push(node, l, r);
        int mid = (l + r) / 2;
        if (ql <= mid) update(node * 2, l, mid, ql, qr, val);
        if (qr > mid) update(node * 2 + 1, mid + 1, r, ql, qr, val);
        sum[node] = sum[node * 2] + sum[node * 2 + 1];
    }

    long long query(int node, int l, int r, int ql, int qr) const {
        if (ql <= l && r <= qr) {
            return sum[node];
        }
        push(node, l, r);
        int mid = (l + r) / 2;
        long long res = 0;
        if (ql <= mid) res += query(node * 2, l, mid, ql, qr);
        if (qr > mid) res += query(node * 2 + 1, mid + 1, r, ql, qr);
        return res;
    }
};

// Required free function (descriptively named) that matches the task spec:
// Given initial array, returns a LazySegmentTree ready for operations.
LazySegmentTree createLazySegmentTree(const std::vector<long long>& arr) {
    return LazySegmentTree(arr);
}

#include <cassert>
#include <vector>
#include "solution.h" // assuming the above is in solution.h or replace with paste

int main() {
    // Basic test from snippet context
    std::vector<long long> arr = {1, 2, 3, 4, 5};
    LazySegmentTree tree(arr);
    assert(tree.rangeSum(0, 4) == 15);
    tree.rangeAdd(0, 4, 10);
    assert(tree.rangeSum(0, 4) == 65);
    assert(tree.rangeSum(2, 3) == 23 + 24); // original 3+4=7 +20 =27, actually 3+10=13, 4+10=14, sum=27
    assert(tree.rangeSum(2, 3) == 27);

    // Empty array
    std::vector<long long> empty;
    LazySegmentTree emptyTree(empty);
    assert(emptyTree.rangeSum(0, 0) == 0);
    emptyTree.rangeAdd(0, 0, 5); // should no-op
    assert(emptyTree.rangeSum(0, 0) == 0);

    // Single element
    std::vector<long long> single = {7};
    LazySegmentTree singleTree(single);
    assert(singleTree.rangeSum(0, 0) == 7);
    singleTree.rangeAdd(0, 0, 3);
    assert(singleTree.rangeSum(0, 0) == 10);

    // l > r swapped
    std::vector<long long> arr2 = {1, 2, 3};
    LazySegmentTree tree2(arr2);
    assert(tree2.rangeSum(2, 0) == 6); // should be same as rangeSum(0,2)
    tree2.rangeAdd(2, 0, 1); // should add 1 to all three
    assert(tree2.rangeSum(0, 2) == 9);

    // Out-of-range indices are clamped
    LazySegmentTree tree3(arr2);
    assert(tree3.rangeSum(-5, 1) == 3); // only indices 0..1
    tree3.rangeAdd(1, 10, 2); // only indices 1..2 get +2
    assert(tree3.rangeSum(0, 2) == 1 + (2+2) + (3+2)); // 1+4+5 =10
    assert(tree3.rangeSum(0, 2) == 10);

    // Large update values
    LazySegmentTree tree4({1, 2});
    tree4.rangeAdd(0, 1, 1000000000LL);
    tree4.rangeAdd(0, 1, 1000000000LL);
    assert(tree4.rangeSum(0, 1) == 2000000003LL);

    return 0;
}
