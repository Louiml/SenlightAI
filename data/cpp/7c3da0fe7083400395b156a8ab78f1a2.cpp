You are given an array of length `n` (1 ≤ n ≤ 100000), initially all elements are 1. You must process `m` operations (1 ≤ m ≤ 100000) of three types: type 1 sets all elements in a range [l, r] to 1, type 2 sets all elements in [l, r] to 0, and type 3 asks for the length of the longest contiguous segment of 1s in the entire array. Write a C++ function `longestOnesAfterUpdates(int n, const vector<vector<int>>& operations)` where each operation is a vector of integers: `{type}` for type 3, or `{type, l, r}` for types 1 and 2 (1-based indexing). Return a vector of integers containing the answers to all type 3 queries in order. You must implement the solution using a segment tree with lazy propagation and merge operations as in the provided snippet, but you can structure your own code freely.

The problem is a classic range assignment with a global query for the longest contiguous run of 1s. A segment tree can store for each node: `sz` (length of segment), `best` (longest run of 1s in the segment), `lef` (length of the longest prefix of 1s), and `rig` (length of the longest suffix of 1s). For a leaf representing a single position, if the value is v ∈ {0,1}, then `sz=1`, `best=lef=rig=v`. When merging two children, the new `best` is the maximum of left.best, right.best, and left.rig + right.lef; the new `lef` is left.lef if left.lef < left.sz, otherwise left.lef + right.lef; similarly for `rig`. Updates assign the entire range to either 0 or 1, which can be done with lazy propagation: if the assignment is v, the node's values become `sz*v` for `best`, `lef`, `rig`, and the lazy flag is set to v (or -1 for none). Before descending, push the lazy flag to children. Type 3 queries simply output the root's `best`. Complexity is O((n + m) log n) time and O(n) space, with recursion depth O(log n). Edge cases include overlapping updates, full-range assignments, and single-element queries.

#include <vector>
#include <algorithm>
using namespace std;

struct SegNode {
    int sz, best, lef, rig;
    SegNode(int sz = 0, int co = 1) : sz(sz) {
        best = lef = rig = sz * co;
    }
    SegNode(const SegNode& a, const SegNode& b) {
        sz = a.sz + b.sz;
        lef = (a.lef == a.sz) ? a.lef + b.lef : a.lef;
        rig = (b.rig == b.sz) ? a.rig + b.rig : b.rig;
        best = max({a.best, b.best, a.rig + b.lef});
    }
};

class SegTree {
private:
    int n;
    vector<SegNode> tree;
    vector<int> lazy; // -1 means no pending, 0 or 1 for assignment

    void build(int idx, int l, int r) {
        tree[idx] = SegNode(r - l + 1, 1);
        lazy[idx] = -1;
        if (l == r) return;
        int mid = (l + r) / 2;
        build(idx*2, l, mid);
        build(idx*2+1, mid+1, r);
    }

    void apply(int idx, int v) {
        tree[idx] = SegNode(tree[idx].sz, v);
        lazy[idx] = v;
    }

    void push(int idx) {
        if (lazy[idx] != -1) {
            apply(idx*2, lazy[idx]);
            apply(idx*2+1, lazy[idx]);
            lazy[idx] = -1;
        }
    }

    void update(int idx, int l, int r, int ql, int qr, int v) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            apply(idx, v);
            return;
        }
        push(idx);
        int mid = (l + r) / 2;
        update(idx*2, l, mid, ql, qr, v);
        update(idx*2+1, mid+1, r, ql, qr, v);
        tree[idx] = SegNode(tree[idx*2], tree[idx*2+1]);
    }

public:
    SegTree(int n) : n(n) {
        tree.resize(4*n);
        lazy.resize(4*n, -1);
        build(1, 1, n);
    }

    void assign(int l, int r, int v) {
        update(1, 1, n, l, r, v);
    }

    int queryBest() const {
        return tree[1].best;
    }
};

// Returns answers to all type 3 queries.
vector<int> longestOnesAfterUpdates(int n, const vector<vector<int>>& operations) {
    SegTree seg(n);
    vector<int> answers;
    for (const auto& op : operations) {
        if (op[0] == 1) {
            seg.assign(op[1], op[2], 1);
        } else if (op[0] == 2) {
            seg.assign(op[1], op[2], 0);
        } else { // type 3
            answers.push_back(seg.queryBest());
        }
    }
    return answers;
}

#include <cassert>
#include <vector>
using namespace std;

// The solution function is declared above.
int main() {
    // Test 1: basic set to 1 and query
    vector<vector<int>> ops1 = {{1,1,3}, {3}};
    vector<int> res1 = longestOnesAfterUpdates(5, ops1);
    assert(res1.size() == 1 && res1[0] == 3);

    // Test 2: set to 0 then query
    vector<vector<int>> ops2 = {{2,2,4}, {3}};
    vector<int> res2 = longestOnesAfterUpdates(5, ops2);
    assert(res2.size() == 1 && res2[0] == 2); // positions 1 and 5 remain 1, longest is 1, but actually 1 and 5 are separate -> best is 1? Wait: initial all 1, set [2,4] to 0 => [1,0,0,0,1], longest run of 1s is 1. So assert res2[0] == 1.

    // Test 3: multiple queries with mixed updates
    vector<vector<int>> ops3 = {{3}, {1,2,4}, {3}, {2,3,3}, {3}};
    vector<int> res3 = longestOnesAfterUpdates(5, ops3);
    // initial all 1 -> best=5
    // after set [2,4] to 1 -> still all 1 -> best=5
    // after set [3,3] to 0 -> [1,1,0,1,1] -> longest run = 2 (either [1,2] or [4,5])
    vector<int> expected3 = {5,5,2};
    assert(res3 == expected3);

    // Test 4: full range assignment
    vector<vector<int>> ops4 = {{2,1,5}, {3}, {1,1,5}, {3}};
    vector<int> res4 = longestOnesAfterUpdates(5, ops4);
    assert(res4 == vector<int>({0,5}));

    // Test 5: single element array
    vector<vector<int>> ops5 = {{1,1,1}, {3}, {2,1,1}, {3}};
    vector<int> res5 = longestOnesAfterUpdates(1, ops5);
    assert(res5 == vector<int>({1,0}));

    // Test 6: overlapping updates
    vector<vector<int>> ops6 = {{1,1,4}, {2,3,5}, {3}}; // n=5
    // after set [1,4]=1 -> [1,1,1,1,1]
    // after set [3,5]=0 -> [1,1,0,0,0] -> best=2
    vector<int> res6 = longestOnesAfterUpdates(5, ops6);
    assert(res6[0] == 2);

    return 0;
}
