// Implement a C++ function `long long rangeAddRangeSum(const vector<int>& initial, const vector<tuple<int,int,int>>& operations)` where each operation is either a range-add (type 0: add a value to all elements in a closed interval `[l,r]`) or a range-sum query (type 1: return the sum of elements in `[l,r]`). The function must process all operations in order and return the sum of all query results. The initial array is 0-indexed, and all intervals are valid (0 ≤ l ≤ r < n). Use a lazy segment tree supporting range add and range sum. Handle negative values correctly. Time complexity O((n + ops) log n), space O(n).

#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

// Declare the solution function (already defined above)
long long rangeAddRangeSum(const vector<int>& initial, const vector<tuple<int,int,int,int>>& operations);

int main() {
    // Simple case: initial [1,2,3], add 5 to [0,1], query [0,2] -> sum should be 1+5 + 2+5 + 3 = 16
    assert(rangeAddRangeSum({1,2,3}, {
        {0,0,1,5},
        {1,0,2,0}
    }) == 16);

    // No operations: sum is 0 (no queries)
    assert(rangeAddRangeSum({4,5,6}, {}) == 0);

    // Only queries, no updates
    assert(rangeAddRangeSum({10,20,30,40}, {
        {1,1,2,0},
        {1,0,3,0}
    }) == (20+30) + (10+20+30+40));

    // Negative values and updates
    assert(rangeAddRangeSum({-5,0,5}, {
        {0,0,1,-3},
        {1,0,2,0}
    }) == (-8) + (-3) + 5);

    // Range covering entire array and multiple updates
    assert(rangeAddRangeSum({1,1,1,1}, {
        {0,0,3,2},
        {0,0,3,-1},
        {1,0,3,0}
    }) == (1+2-1)*4);

    // Single element
    assert(rangeAddRangeSum({7}, {
        {0,0,0,3},
        {1,0,0,0}
    }) == 10);

    // Invalid ranges (l > r) should be ignored gracefully (function handles)
    assert(rangeAddRangeSum({1,2}, {{0,1,0,5},{1,0,1,0}}) == 3); // update ignored, query returns 1+2

    // Type field only 0 or 1
    assert(rangeAddRangeSum({0,0,0}, {
        {0,0,1,4},
        {1,0,2,0},
        {1,1,2,0}
    }) == 4 + 4);

    return 0;
}

#include <vector>
#include <tuple>
#include <cstdint>

class LazySegTree {
    int n;
    std::vector<long long> sum;
    std::vector<long long> lazy;

    void apply(int node, long long val, int seg_len) {
        sum[node] += val * seg_len;
        lazy[node] += val;
    }

    void push(int node, int left, int right) {
        if (lazy[node] != 0) {
            int mid = (left + right) / 2;
            apply(node*2, lazy[node], mid - left + 1);
            apply(node*2+1, lazy[node], right - mid);
            lazy[node] = 0;
        }
    }

    void build(int node, int left, int right, const std::vector<int>& arr) {
        if (left == right) {
            sum[node] = arr[left];
            return;
        }
        int mid = (left + right) / 2;
        build(node*2, left, mid, arr);
        build(node*2+1, mid+1, right, arr);
        sum[node] = sum[node*2] + sum[node*2+1];
    }

    void update(int node, int left, int right, int ql, int qr, long long val) {
        if (ql <= left && right <= qr) {
            apply(node, val, right - left + 1);
            return;
        }
        push(node, left, right);
        int mid = (left + right) / 2;
        if (ql <= mid) update(node*2, left, mid, ql, qr, val);
        if (qr > mid) update(node*2+1, mid+1, right, ql, qr, val);
        sum[node] = sum[node*2] + sum[node*2+1];
    }

    long long query(int node, int left, int right, int ql, int qr) {
        if (ql <= left && right <= qr) {
            return sum[node];
        }
        push(node, left, right);
        int mid = (left + right) / 2;
        long long res = 0;
        if (ql <= mid) res += query(node*2, left, mid, ql, qr);
        if (qr > mid) res += query(node*2+1, mid+1, right, ql, qr);
        return res;
    }

public:
    LazySegTree(const std::vector<int>& arr) : n(arr.size()) {
        sum.resize(4*n, 0);
        lazy.resize(4*n, 0);
        if (n > 0) build(1, 0, n-1, arr);
    }

    void range_add(int l, int r, long long val) {
        if (n == 0 || l > r) return;
        update(1, 0, n-1, l, r, val);
    }

    long long range_sum(int l, int r) {
        if (n == 0 || l > r) return 0;
        return query(1, 0, n-1, l, r);
    }
};

// operations: each tuple is (type, l, r, val). type=0: add val to [l,r]; type=1: query sum of [l,r], val unused.
long long rangeAddRangeSum(const std::vector<int>& initial, const std::vector<std::tuple<int,int,int,int>>& operations) {
    LazySegTree tree(initial);
    long long answer = 0;
    for (const auto& op : operations) {
        int type = std::get<0>(op);
        int l = std::get<1>(op);
        int r = std::get<2>(op);
        int val = std::get<3>(op);
        if (type == 0) {
            tree.range_add(l, r, val);
        } else {
            answer += tree.range_sum(l, r);
        }
    }
    return answer;
}

// We need to support two types of operations on a static array: range addition (add a constant to every element in a contiguous subarray) and range sum (compute the sum of a contiguous subarray). A standard approach is a segment tree with lazy propagation. Each node stores the sum of its segment and a lazy add value that is pending to be applied to its entire segment. When performing a range update, we fully cover the segment if the node's interval is inside the update interval, apply the add to the node's sum and lazy value, and return. Otherwise, we push any pending lazy value to children before recursing, then update the node's sum from children. For range sum, we similarly push lazy when we need to go deeper, and accumulate sums from fully covered segments. This yields O(log n) per operation. Edge cases: empty array? The problem guarantees n ≥ 1, but we can handle n=0 by returning 0. Negative values are handled naturally with long long to avoid overflow. The function accumulates all query sums into a long long result. Implementation: build the tree from the initial array, then iterate through operations, branching on type. All operations are valid, so no bounds checking needed beyond the segment tree logic.
