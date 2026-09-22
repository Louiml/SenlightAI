/*
Write a C++ function that, given a vector of integers and a series of point updates and range queries, returns the minimum value and the count of occurrences of that minimum within each queried subarray. The function should accept the initial array, a list of operations, and process them in order: an operation of type `1 i x` updates the element at index `i` (0-based) to value `x`; an operation of type `2 l r` queries the subarray indices `[l, r]` inclusive (0-based) and returns a pair `{minimum_value, count_of_minimum}` for that subarray after any prior updates. The function should return a vector of pairs, one for each query operation, in the order they appear.
*/
#include <vector>
#include <limits>
#include <algorithm>

using std::vector;
using std::pair;
using std::make_pair;

class SegmentTree {
private:
    vector<pair<int, int>> tree;
    int size;

    void build(const vector<int>& arr, int node, int left, int right) {
        if (right - left == 1) {
            if (left < (int)arr.size()) {
                tree[node] = {arr[left], 1};
            } else {
                tree[node] = {std::numeric_limits<int>::max(), 0};
            }
            return;
        }
        int mid = (left + right) / 2;
        build(arr, 2 * node + 1, left, mid);
        build(arr, 2 * node + 2, mid, right);
        merge(node);
    }

    void merge(int node) {
        const auto& left = tree[2 * node + 1];
        const auto& right = tree[2 * node + 2];
        if (left.first < right.first) {
            tree[node] = left;
        } else if (left.first > right.first) {
            tree[node] = right;
        } else {
            tree[node] = {left.first, left.second + right.second};
        }
    }

    void update(int idx, int value, int node, int left, int right) {
        if (right - left == 1) {
            tree[node] = {value, 1};
            return;
        }
        int mid = (left + right) / 2;
        if (idx < mid) {
            update(idx, value, 2 * node + 1, left, mid);
        } else {
            update(idx, value, 2 * node + 2, mid, right);
        }
        merge(node);
    }

    pair<int, int> query(int l, int r, int node, int left, int right) const {
        if (l >= right || r <= left) {
            return {std::numeric_limits<int>::max(), 0};
        }
        if (l <= left && right <= r) {
            return tree[node];
        }
        int mid = (left + right) / 2;
        pair<int, int> leftRes = query(l, r, 2 * node + 1, left, mid);
        pair<int, int> rightRes = query(l, r, 2 * node + 2, mid, right);
        if (leftRes.first < rightRes.first) {
            return leftRes;
        } else if (leftRes.first > rightRes.first) {
            return rightRes;
        } else {
            return {leftRes.first, leftRes.second + rightRes.second};
        }
    }

public:
    explicit SegmentTree(const vector<int>& arr) {
        size = 1;
        while (size < (int)arr.size()) {
            size <<= 1;
        }
        tree.assign(2 * size, {std::numeric_limits<int>::max(), 0});
        build(arr, 0, 0, size);
    }

    void set_value(int idx, int value) {
        update(idx, value, 0, 0, size);
    }

    pair<int, int> get(int l, int r) const {
        // l and r are inclusive 0-based range
        return query(l, r + 1, 0, 0, size);
    }
};

// Process operations and return query results.
// operations: each element is {type, a, b}
// type=1: update index a to value b
// type=2: query range [a, b] inclusive
vector<pair<int, int>> minWithCount(const vector<int>& initial,
                                   const vector<vector<int>>& operations) {
    SegmentTree st(initial);
    vector<pair<int, int>> results;
    for (const auto& op : operations) {
        if (op[0] == 1) {
            st.set_value(op[1], op[2]);
        } else {
            results.push_back(st.get(op[1], op[2]));
        }
    }
    return results;
}
#include <cassert>
#include <vector>

// The solution function is declared above.
int main() {
    // Basic single query on initial array
    vector<pair<int, int>> res1 = minWithCount({5, 2, 3, 2, 1}, {{2, 0, 4}});
    assert(res1.size() == 1);
    assert(res1[0] == std::make_pair(1, 1));

    // Query subrange with duplicates
    vector<pair<int, int>> res2 = minWithCount({4, 2, 2, 3, 2}, {{2, 1, 4}});
    assert(res2[0] == std::make_pair(2, 3)); // indices 1,2,4 have value 2

    // Point update changes minimum and count
    vector<pair<int, int>> res3 = minWithCount({1, 5, 3, 5}, {{1, 0, 5}, {2, 0, 3}});
    assert(res3.size() == 1);
    assert(res3[0] == std::make_pair(3, 1)); // after update: {5,5,3,5}, min=3

    // Update creates new minimum
    vector<pair<int, int>> res4 = minWithCount({10, 10, 10}, {{1, 2, 4}, {2, 0, 2}});
    assert(res4[0] == std::make_pair(4, 1)); // after update: {10,10,4}

    // Multiple queries with updates interspersed
    vector<pair<int, int>> res5 = minWithCount({2, 2, 2, 2},
                                               {{2, 0, 3}, {1, 1, 1}, {2, 0, 3}, {2, 1, 1}});
    assert(res5.size() == 3);
    assert(res5[0] == std::make_pair(2, 4));
    assert(res5[1] == std::make_pair(1, 1)); // after update {2,1,2,2}, min=1
    assert(res5[2] == std::make_pair(1, 1)); // single element at index 1

    // Single element array
    vector<pair<int, int>> res6 = minWithCount({7}, {{2, 0, 0}, {1, 0, 0}, {2, 0, 0}});
    assert(res6.size() == 2);
    assert(res6[0] == std::make_pair(7, 1));
    assert(res6[1] == std::make_pair(0, 1));

    return 0;
}
// The core idea is to use a segment tree where each node stores a pair `{minimum value, count of occurrences of that minimum}` for its segment. The tree is built from the initial array by recursively computing for each node the combined result from its two children: if the left child's minimum is smaller, propagate it; if the right child's minimum is smaller, propagate that; otherwise, both minima are equal, so propagate the value and sum the counts. Point updates traverse to the leaf, set the new value, and then recompute the node's pair upward using the same merge logic. Range queries recursively collect results from fully covered segments and merge them on the fly using the same rule, starting with a neutral pair `{INT_MAX, 0}` (representing “no elements”) where merging any pair with it returns the other pair. The function should handle an empty query range gracefully (though the problem likely guarantees valid ranges) by returning `{INT_MAX, 0}`. Edge cases include duplicate values, single-element segments, and updates that change the minimum count. Time complexity: building takes `O(n)`, each update or query takes `O(log n)`, so total for `m` operations is `O(m log n)`. Space complexity is `O(n)` for the segment tree.
