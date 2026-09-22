Write a standalone C++ function `segmentTreeRangeSum` that takes an integer array `a` of length `n`, a list of operations, and returns a vector of results. Each operation is either a point update `(0, idx, val)` meaning set `a[idx] = val`, or a range sum query `(1, l, r)` meaning return the sum of elements from index `l` to `r` inclusive. The operations are provided as a vector of tuples `(type, x, y)`, where for type 0, `x` is the index and `y` is the new value; for type 1, `x` is the left bound and `y` is the right bound. Your function should return a vector of integers containing the results of all range sum queries in the order they appear. Assume 0-based indexing, `n ≥ 1`, and all indices and values are within the range of a 32-bit signed integer. The function must handle arbitrary interleaving of updates and queries and cannot rebuild the segment tree for each query.

// The core requirement is to support two operations on a static array that can be dynamically modified: point updates and range sum queries. A segment tree is ideal because it allows both operations in \(O(\log n)\) time, with a precomputation time of \(O(n)\) and an auxiliary space of \(O(n)\). The tree is stored in an array of size \(4n\) (or \(3n\) as in the snippet, but use \(4n\) for safety). Each node represents the sum of a contiguous segment of the original array. Build recursively: leaf nodes store individual array elements, and internal nodes store the sum of their two children. For a query, if the node's segment is completely outside the query range, return 0; if completely inside, return the stored sum; otherwise, recursively query both children and add their results. For an update, recursively descend to the target index (comparing the index to the node's segment bounds), update the leaf, and then recompute the internal node's sum from its children on the way back up. Edge cases include querying a single element (both bounds equal), updating the same index multiple times, and queries that cover the entire array. The complexity is \(O(n)\) to build the tree once, and each update or query is \(O(\log n)\). The total time complexity for \(q\) operations is \(O(n + q \log n)\), and space is \(O(n)\).

#include<bits/stdc++.h>
using namespace std;

// Builds a segment tree for range sums from array a (0-based).
// tree[node] stores the sum of segment [l..r] of a.
void buildSegTree(vector<int>& tree, const vector<int>& a, int node, int l, int r) {
    if (l == r) {
        tree[node] = a[l];
        return;
    }
    int mid = (l + r) / 2;
    buildSegTree(tree, a, 2 * node, l, mid);
    buildSegTree(tree, a, 2 * node + 1, mid + 1, r);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

// Returns sum of a[x..y] inclusive.
int rangeSum(const vector<int>& tree, int node, int l, int r, int x, int y) {
    if (r < x || l > y) return 0;
    if (l >= x && r <= y) return tree[node];
    int mid = (l + r) / 2;
    return rangeSum(tree, 2 * node, l, mid, x, y) +
           rangeSum(tree, 2 * node + 1, mid + 1, r, x, y);
}

// Sets a[idx] = val and updates the tree accordingly.
void pointUpdate(vector<int>& tree, int node, int l, int r, int idx, int val) {
    if (idx < l || idx > r) return;
    if (l == r) {
        tree[node] = val;
        return;
    }
    int mid = (l + r) / 2;
    pointUpdate(tree, 2 * node, l, mid, idx, val);
    pointUpdate(tree, 2 * node + 1, mid + 1, r, idx, val);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

// Main solution function. operations: vector of tuples (type, x, y).
// type 0: update a[x] = y; type 1: query sum from x to y inclusive.
vector<int> segmentTreeRangeSum(const vector<int>& a, const vector<tuple<int,int,int>>& operations) {
    int n = (int)a.size();
    vector<int> tree(4 * n, 0);
    buildSegTree(tree, a, 1, 0, n - 1);

    vector<int> results;
    for (const auto& op : operations) {
        int type, x, y;
        tie(type, x, y) = op;
        if (type == 0) {
            pointUpdate(tree, 1, 0, n - 1, x, y);
        } else {
            results.push_back(rangeSum(tree, 1, 0, n - 1, x, y));
        }
    }
    return results;
}

int main() {
    // Test 1: basic queries only
    vector<int> a1 = {1, 2, 3, 4, 5};
    vector<tuple<int,int,int>> ops1 = {{1,0,4}, {1,1,3}, {1,2,2}};
    vector<int> res1 = segmentTreeRangeSum(a1, ops1);
    assert(res1.size() == 3);
    assert(res1[0] == 15);
    assert(res1[1] == 9);
    assert(res1[2] == 3);

    // Test 2: updates then queries
    vector<int> a2 = {10, 20, 30, 40};
    vector<tuple<int,int,int>> ops2 = {{0,1,25}, {1,0,3}, {0,3,0}, {1,0,3}};
    vector<int> res2 = segmentTreeRangeSum(a2, ops2);
    assert(res2.size() == 2);
    assert(res2[0] == 105); // 10+25+30+40
    assert(res2[1] == 65);  // 10+25+30+0

    // Test 3: repeated updates on same index
    vector<int> a3 = {5,5,5};
    vector<tuple<int,int,int>> ops3 = {{0,0,1}, {0,0,2}, {0,0,3}, {1,0,2}};
    vector<int> res3 = segmentTreeRangeSum(a3, ops3);
    assert(res3.size() == 1);
    assert(res3[0] == 11); // 3+5+5

    // Test 4: single element array
    vector<int> a4 = {7};
    vector<tuple<int,int,int>> ops4 = {{1,0,0}, {0,0,9}, {1,0,0}};
    vector<int> res4 = segmentTreeRangeSum(a4, ops4);
    assert(res4.size() == 2);
    assert(res4[0] == 7);
    assert(res4[1] == 9);

    // Test 5: empty operations
    vector<int> a5 = {1,2,3};
    vector<tuple<int,int,int>> ops5;
    vector<int> res5 = segmentTreeRangeSum(a5, ops5);
    assert(res5.empty());

    return 0;
}
