Write a C++ function `long long minimumAfterRangeAdditions(int n, const vector<long long>& initial, const vector<tuple<int,int,long long>>& operations)` that simulates applying a sequence of range increment operations to an array of size `n` (1-indexed). Each operation `(l, r, delta)` adds `delta` to every element from index `l` to `r` inclusive, where `delta` can be negative. After each operation, if the minimum value in the entire updated array becomes negative, the function must immediately return `-1` (indicating failure). If all operations complete without any element ever becoming negative, the function should return `0`. The initial array contains non-negative values, and the total number of operations may be up to `100000`. Use an efficient segment tree with lazy propagation to handle range updates and global minimum queries. The function must be self-contained and not rely on any global state.
// The core challenge is to support: (1) range addition (including negative deltas) and (2) querying the global minimum after every update, with early termination if that minimum is negative. A segment tree with lazy propagation is the standard solution. Each tree node stores the minimum value in its segment. For range updates, we apply the delta to the node’s minimum and accumulate the delta in a lazy `add` field. When a query or update descends into a node, we push the lazy value to its children. After each update, the root node’s `Min` field gives the global minimum. If it is less than zero, return `-1` immediately. Edge cases include: operations where `l == r`, operations with zero delta (which do not change anything but still must be processed), and ensuring that negative results are checked after each operation, not just at the end. Time complexity is O((n + m) log n) where m is the number of operations, and space is O(n) for the tree.
#include <vector>
#include <tuple>
#include <algorithm>

// Segment tree with lazy propagation for range addition and global minimum query.
class LazyMinSegTree {
    int n;
    std::vector<long long> minVal, lazy;
    
    void build(int node, int l, int r, const std::vector<long long>& arr) {
        if (l == r) {
            minVal[node] = arr[l];
            return;
        }
        int mid = (l + r) / 2;
        build(node*2, l, mid, arr);
        build(node*2+1, mid+1, r, arr);
        minVal[node] = std::min(minVal[node*2], minVal[node*2+1]);
    }
    
    void apply(int node, long long delta) {
        minVal[node] += delta;
        lazy[node] += delta;
    }
    
    void push(int node) {
        if (lazy[node] == 0) return;
        apply(node*2, lazy[node]);
        apply(node*2+1, lazy[node]);
        lazy[node] = 0;
    }
    
    void range_add(int node, int l, int r, int ql, int qr, long long delta) {
        if (ql <= l && r <= qr) {
            apply(node, delta);
            return;
        }
        push(node);
        int mid = (l + r) / 2;
        if (ql <= mid) range_add(node*2, l, mid, ql, qr, delta);
        if (qr > mid) range_add(node*2+1, mid+1, r, ql, qr, delta);
        minVal[node] = std::min(minVal[node*2], minVal[node*2+1]);
    }
    
public:
    LazyMinSegTree(const std::vector<long long>& arr) {
        n = static_cast<int>(arr.size());
        minVal.assign(4*n, 0);
        lazy.assign(4*n, 0);
        if (n > 0) build(1, 1, n, arr);
    }
    
    void rangeAdd(int l, int r, long long delta) {
        range_add(1, 1, n, l, r, delta);
    }
    
    long long globalMin() const {
        return minVal[1];
    }
};

// Apply operations; if any global minimum becomes negative, return -1. Otherwise 0.
long long minimumAfterRangeAdditions(int n, const std::vector<long long>& initial,
                                     const std::vector<std::tuple<int,int,long long>>& operations) {
    // Note: initial is 1-indexed: initial[0] corresponds to index 1.
    LazyMinSegTree seg(initial);
    for (const auto& op : operations) {
        int l, r;
        long long delta;
        std::tie(l, r, delta) = op;
        seg.rangeAdd(l, r, delta);
        if (seg.globalMin() < 0) {
            return -1;
        }
    }
    return 0;
}
#include <cassert>
#include <vector>
#include <tuple>
#include <iostream>

// Include the solution function here (or link separately).
// For self-contained test, paste the solution code above.

int main() {
    // Test 1: Simple addition, all non-negative.
    std::vector<long long> init1 = {1, 2, 3, 4, 5};
    std::vector<std::tuple<int,int,long long>> ops1 = {{2, 4, 2}};
    assert(minimumAfterRangeAdditions(5, init1, ops1) == 0);

    // Test 2: Negative result appears after an operation.
    std::vector<long long> init2 = {5, 5, 5};
    std::vector<std::tuple<int,int,long long>> ops2 = {{1, 3, -3}, {1, 3, -3}};
    assert(minimumAfterRangeAdditions(3, init2, ops2) == -1);

    // Test 3: Operation with zero delta does not cause negative.
    std::vector<long long> init3 = {0};
    std::vector<std::tuple<int,int,long long>> ops3 = {{1, 1, 0}};
    assert(minimumAfterRangeAdditions(1, init3, ops3) == 0);

    // Test 4: Multiple updates where min never negative but could be tricky.
    std::vector<long long> init4 = {10, 20, 30};
    std::vector<std::tuple<int,int,long long>> ops4 = {{1, 2, -5}, {2, 3, -5}, {1, 1, -5}};
    // After op1: [5,15,30] min 5. After op2: [5,10,25] min 5. After op3: [0,10,25] min 0. All non-negative.
    assert(minimumAfterRangeAdditions(3, init4, ops4) == 0);

    // Test 5: Single negative delta that makes one element negative.
    std::vector<long long> init5 = {3, 4, 5};
    std::vector<std::tuple<int,int,long long>> ops5 = {{2, 2, -5}};
    assert(minimumAfterRangeAdditions(3, init5, ops5) == -1);

    // Test 6: Larger array with disjoint updates.
    std::vector<long long> init6 = {100, 100, 100, 100, 100};
    std::vector<std::tuple<int,int,long long>> ops6 = {{1, 2, -50}, {4, 5, -50}};
    // After op1: [50,50,100,100,100] min 50. After op2: [50,50,100,50,50] min 50.
    assert(minimumAfterRangeAdditions(5, init6, ops6) == 0);

    // Test 7: Exactly hits zero, still not negative.
    std::vector<long long> init7 = {7, 7, 7};
    std::vector<std::tuple<int,int,long long>> ops7 = {{1, 3, -7}};
    assert(minimumAfterRangeAdditions(3, init7, ops7) == 0);

    std::cout << "All tests passed.\n";
    return 0;
}
