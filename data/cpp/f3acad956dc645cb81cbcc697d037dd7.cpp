Given a fixed-size array `arr` of `n` positive integers (1-indexed) and a sequence of `m` operations, implement a C++ function `processRangeAddPointQuery` that simulates the provided segment tree logic. The operations are of two types:  
- Type 1: `1 a b x` — add the integer `x` to every element in the inclusive range `[a, b]` of the array.  
- Type 2: `2 i` — output the current value of the element at position `i` (1-indexed).  
Your function should take the initial array vector `arr` (its size `n` is given by `arr.size()`), and a vector of operations (each operation is a vector of integers as described), and return a vector of `long long` values containing the outputs of all type 2 operations in the order they appear. You must implement the exact lazy-propagation-free segment tree approach from the snippet: each node stores the sum of lazy additions applied to its entire interval, and a point query is computed by summing the values from the leaf node up to the root. Do not use any standard library segment tree or Fenwick tree; implement the recursive tree manually. Assume input values fit in `long long`, and `n` can be up to `100000`, `m` up to `100000`. Ensure your implementation is self-contained and does not rely on global variables.
// The core idea is a segment tree that supports range updates and point queries using a "lazy" value stored at each node representing the total addition applied to the whole segment. The tree is built recursively: for each node, we store its interval boundaries and a `sum` field (initially 0) that accumulates additions. The array values themselves are stored at leaves in a `pos` map from original index to node index. For a range update `Add(index, a, b, l, r, value)`, if the current node’s interval exactly matches `[a,b]`, we add `value` to its `sum` and stop. Otherwise, we split the range into left/right children using the mid-point, handling three cases: fully left, fully right, or split. For a point query at position `i`, we start from the leaf node and sum the `sum` values of all ancestors up to the root (including the leaf). This yields the total added value to that position. The initial array values are not stored in the tree except at initialization, but we need to add them to the query result. Since the snippet does not add initial values in `Output`, the provided code would output only accumulated additions; to match expected behavior, our function must add the original `arr[i-1]` to the accumulated sum. Time complexity: building tree O(n), each operation O(log n) for both update and query. Space complexity O(n) for tree arrays.
#include <vector>
#include <cstdint>
#include <algorithm>

struct SegmentNode {
    int left, right;
    long long sum;
};

class RangeAddPointQuery {
    int n;
    std::vector<SegmentNode> tree;
    std::vector<int> pos; // maps original index to leaf node index

    void build(int node, int l, int r) {
        tree[node].left = l;
        tree[node].right = r;
        if (l == r) {
            pos[l] = node;
            tree[node].sum = 0; // initial array values handled separately
            return;
        }
        int mid = (l + r) >> 1;
        build(node << 1, l, mid);
        build((node << 1) + 1, mid + 1, r);
        tree[node].sum = 0;
    }

    long long query(int node) const {
        // sum all lazy values from leaf up to root
        long long res = 0;
        int cur = node;
        while (cur != 0) {
            res += tree[cur].sum;
            cur >>= 1;
        }
        return res;
    }

    void add(int node, int ql, int qr, int l, int r, long long value) {
        if (ql == l && qr == r) {
            tree[node].sum += value;
            return;
        }
        int mid = (l + r) >> 1;
        if (qr <= mid) {
            add(node << 1, ql, qr, l, mid, value);
        } else if (ql > mid) {
            add((node << 1) + 1, ql, qr, mid + 1, r, value);
        } else {
            add(node << 1, ql, mid, l, mid, value);
            add((node << 1) + 1, mid + 1, qr, mid + 1, r, value);
        }
    }

public:
    RangeAddPointQuery(const std::vector<long long>& arr) {
        n = static_cast<int>(arr.size());
        if (n == 0) return;
        tree.resize(4 * n + 5);
        pos.resize(n + 1);
        build(1, 1, n);
    }

    void rangeAdd(int l, int r, long long value) {
        if (l > r) return;
        add(1, l, r, 1, n, value);
    }

    long long pointQuery(int index, const std::vector<long long>& arr) const {
        if (index < 1 || index > n) return 0;
        return arr[index - 1] + query(pos[index]);
    }
};

// Main function to process operations
std::vector<long long> processRangeAddPointQuery(const std::vector<long long>& arr,
                                                 const std::vector<std::vector<long long>>& operations) {
    RangeAddPointQuery solver(arr);
    std::vector<long long> results;
    for (const auto& op : operations) {
        if (op[0] == 1) {
            solver.rangeAdd(static_cast<int>(op[1]), static_cast<int>(op[2]), op[3]);
        } else if (op[0] == 2) {
            results.push_back(solver.pointQuery(static_cast<int>(op[1]), arr));
        }
    }
    return results;
}
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution code here (or assume it's above)

int main() {
    // Test 1: Basic updates and queries
    std::vector<long long> arr1 = {1, 2, 3, 4, 5};
    std::vector<std::vector<long long>> ops1 = {{1, 1, 3, 10}, {2, 1}, {2, 3}, {2, 4}};
    auto res1 = processRangeAddPointQuery(arr1, ops1);
    assert(res1.size() == 3);
    assert(res1[0] == 11); // 1+10
    assert(res1[1] == 13); // 3+10
    assert(res1[2] == 4);  // no update to index 4

    // Test 2: Update overlapping ranges
    std::vector<long long> arr2 = {0, 0, 0};
    std::vector<std::vector<long long>> ops2 = {{1, 1, 2, 5}, {1, 2, 3, 7}, {2, 2}};
    auto res2 = processRangeAddPointQuery(arr2, ops2);
    assert(res2.size() == 1);
    assert(res2[0] == 12); // 5+7 from index 2 (both updates include it)

    // Test 3: Larger values and negative (though snippet uses positive, we allow negative)
    std::vector<long long> arr3 = {10, 20, 30};
    std::vector<std::vector<long long>> ops3 = {{1, 1, 1, -5}, {2, 1}, {1, 2, 3, 100}, {2, 3}};
    auto res3 = processRangeAddPointQuery(arr3, ops3);
    assert(res3.size() == 2);
    assert(res3[0] == 5);  // 10-5
    assert(res3[1] == 130); // 30+100

    // Test 4: Single element array
    std::vector<long long> arr4 = {42};
    std::vector<std::vector<long long>> ops4 = {{2, 1}, {1, 1, 1, 8}, {2, 1}};
    auto res4 = processRangeAddPointQuery(arr4, ops4);
    assert(res4.size() == 2);
    assert(res4[0] == 42);
    assert(res4[1] == 50);

    // Test 5: Many operations stress sanity (small)
    std::vector<long long> arr5 = {1, 2, 3, 4, 5, 6};
    std::vector<std::vector<long long>> ops5 = {
        {1, 2, 5, 1}, {2, 3}, {1, 1, 6, 2}, {2, 1}, {2, 6}, {1, 3, 3, 10}, {2, 3}
    };
    auto res5 = processRangeAddPointQuery(arr5, ops5);
    assert(res5.size() == 4);
    // index3: 3 + (1 from first) + (2 from second) + (10 from third) = 16
    // index1: 1 + 2 = 3
    // index6: 6 + 1 + 2 = 9
    // index3 again: same as before = 16
    assert(res5[0] == 3 + 1 + 2 + 10); // 16
    assert(res5[1] == 1 + 2); // 3
    assert(res5[2] == 6 + 1 + 2); // 9
    assert(res5[3] == 16);

    // Test 6: No operations
    std::vector<long long> arr6 = {5, 5, 5};
    std::vector<std::vector<long long>> ops6 = {};
    auto res6 = processRangeAddPointQuery(arr6, ops6);
    assert(res6.empty());

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
