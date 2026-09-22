Implement a C++ function `rangeSumWithRangeAdd` that processes a sequence of two types of operations on an initially zero-initialized 1-indexed array of size `n`: (1) an "update" operation that adds a given integer value to every element in a closed interval `[l, r]`, and (2) a "query" operation that returns the current sum of all elements in a closed interval `[l, r]`. The function should take the array size `n`, the number of operations `m`, and a vector of operations (each represented as a struct or tuple containing an operation type `op` (0 for update, 1 for query), interval endpoints `l` and `r`, and for updates an additional `value`; for queries the value field is unused and should be ignored). The function must return a vector of `long long` containing the results of all query operations in the order they appear. Use a segment tree with lazy propagation to support efficient range additions and range sum queries. The input intervals are guaranteed to satisfy `1 <= l <= r <= n`, and all values fit in a 64-bit signed integer. The function must handle up to `n = 100000` and `m = 100000` operations efficiently.

// The core challenge is to support both range addition and range sum queries in \(O(\log n)\) time per operation. A segment tree with lazy propagation is appropriate. Each node stores the sum of its segment, and a lazy value representing pending additions that have not been propagated to children. For an update on `[l, r]`, we recursively traverse the tree: when a node's segment is fully inside the update range, we add `value * (segment length)` to its sum and add `value` to its lazy tag, then stop. If the segment partially overlaps, we first push any existing lazy tag to children, then recurse on overlapping children, and finally recompute the node's sum from children. For a query on `[l, r]`, we similarly push lazy tags before recursing, and if the node's segment is fully inside the query range, we return its stored sum; otherwise we recurse on overlapping children and sum their results. Lazy propagation ensures that we don't update every element individually, achieving the required logarithmic time. Edge cases include overlapping updates (multiple additions on same range, which sum naturally via lazy tags), queries that span the entire array, and intervals that are single points. Complexity: each operation visits \(O(\log n)\) nodes, so total time is \(O(m \log n)\), and space is \(O(n)\) for the tree arrays (we use 4 times the array size).

#include <vector>
#include <cstdint>

// Operation structure: op=0 for range add, op=1 for range sum query.
// For queries, value is ignored.
struct Operation {
    int op;
    int l;
    int r;
    long long value;
};

// Internal segment tree class with lazy propagation.
class RangeAddSumTree {
private:
    int n;
    std::vector<long long> sum;
    std::vector<long long> lazy;

    void push(int node, int left, int right) {
        if (lazy[node] != 0) {
            sum[node] += lazy[node] * (right - left + 1);
            if (left != right) {
                lazy[node * 2] += lazy[node];
                lazy[node * 2 + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    void update(int node, int left, int right, int ql, int qr, long long val) {
        if (ql > right || qr < left) return;
        if (ql <= left && right <= qr) {
            sum[node] += val * (right - left + 1);
            if (left != right) {
                lazy[node * 2] += val;
                lazy[node * 2 + 1] += val;
            }
            return;
        }
        push(node, left, right);
        int mid = (left + right) / 2;
        update(node * 2, left, mid, ql, qr, val);
        update(node * 2 + 1, mid + 1, right, ql, qr, val);
        sum[node] = sum[node * 2] + sum[node * 2 + 1];
    }

    long long query(int node, int left, int right, int ql, int qr) {
        if (ql > right || qr < left) return 0;
        push(node, left, right);
        if (ql <= left && right <= qr) return sum[node];
        int mid = (left + right) / 2;
        return query(node * 2, left, mid, ql, qr) +
               query(node * 2 + 1, mid + 1, right, ql, qr);
    }

public:
    RangeAddSumTree(int size) : n(size), sum(4 * size + 5, 0), lazy(4 * size + 5, 0) {}

    void rangeAdd(int l, int r, long long val) {
        update(1, 1, n, l, r, val);
    }

    long long rangeSum(int l, int r) {
        return query(1, 1, n, l, r);
    }
};

// Main solution function: processes operations and returns query results.
std::vector<long long> rangeSumWithRangeAdd(int n, const std::vector<Operation>& operations) {
    RangeAddSumTree tree(n);
    std::vector<long long> results;

    for (const auto& op : operations) {
        if (op.op == 0) {
            tree.rangeAdd(op.l, op.r, op.value);
        } else { // op.op == 1
            results.push_back(tree.rangeSum(op.l, op.r));
        }
    }

    return results;
}

#include <cassert>
#include <vector>
#include <cstdint>

// Forward declaration of the solution function (already defined above).
// Operation structure is defined above.
std::vector<long long> rangeSumWithRangeAdd(int n, const std::vector<Operation>& operations);

int main() {
    // Test 1: single update then query full range.
    std::vector<Operation> ops1 = {{0, 1, 5, 10}, {1, 1, 5, 0}};
    auto res1 = rangeSumWithRangeAdd(5, ops1);
    assert(res1.size() == 1 && res1[0] == 50);

    // Test 2: multiple overlapping updates and queries.
    std::vector<Operation> ops2 = {
        {0, 1, 3, 5},   // add 5 to [1,3]
        {0, 2, 4, 7},   // add 7 to [2,4]
        {1, 1, 1, 0},   // query [1,1] -> 5
        {1, 2, 3, 0},   // query [2,3] -> (5+7)+(5+7)=24
        {1, 4, 4, 0},   // query [4,4] -> 7
        {1, 1, 4, 0}    // query [1,4] -> 5+12+12+7=36
    };
    auto res2 = rangeSumWithRangeAdd(4, ops2);
    assert(res2.size() == 4);
    assert(res2[0] == 5);
    assert(res2[1] == 24);
    assert(res2[2] == 7);
    assert(res2[3] == 36);

    // Test 3: single-element array with multiple updates.
    std::vector<Operation> ops3 = {
        {0, 1, 1, 100},
        {0, 1, 1, -30},
        {1, 1, 1, 0},
        {0, 1, 1, 5},
        {1, 1, 1, 0}
    };
    auto res3 = rangeSumWithRangeAdd(1, ops3);
    assert(res3.size() == 2);
    assert(res3[0] == 70);
    assert(res3[1] == 75);

    // Test 4: update on entire array, then query subranges.
    std::vector<Operation> ops4 = {
        {0, 1, 6, 3},
        {1, 1, 6, 0},
        {1, 2, 5, 0},
        {1, 3, 3, 0}
    };
    auto res4 = rangeSumWithRangeAdd(6, ops4);
    assert(res4.size() == 3);
    assert(res4[0] == 18);
    assert(res4[1] == 12);
    assert(res4[2] == 3);

    // Test 5: no operations -> empty result.
    std::vector<Operation> ops5;
    auto res5 = rangeSumWithRangeAdd(3, ops5);
    assert(res5.empty());

    // Test 6: interleaved updates and queries, value 0 updates.
    std::vector<Operation> ops6 = {
        {0, 2, 4, 0},
        {1, 1, 5, 0},
        {0, 1, 5, 2},
        {1, 1, 5, 0}
    };
    auto res6 = rangeSumWithRangeAdd(5, ops6);
    assert(res6.size() == 2);
    assert(res6[0] == 0);
    assert(res6[1] == 10);

    return 0;
}
