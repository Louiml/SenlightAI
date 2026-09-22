You are given an array of length `n` (1-indexed from 0 to n-1) initially filled with zeros. You need to process `q` queries of two types:  
- `1 x y v`: set all elements from index `x` to `y` (inclusive, 0-indexed) to the value `v`.  
- `2 x y`: compute the average value of the elements in the range `[x, y]`. The average must be printed as a reduced fraction `p/q` (in lowest terms). If the average is an integer, print only that integer without a denominator. Write a C++ function `void solveQueries(int n, const vector<tuple<int,int,int,int>>& queries, vector<string>& output)` that processes these queries and fills `output` with the result for each type-2 query, in order. The function must handle up to `n = 100,000` and `q = 100,000`, with values of `v` up to 10^9. Assume all queries are valid (0 ≤ x ≤ y < n). Print results as strings (integers or fractions in lowest terms).
We need a data structure that supports range assignment and range sum queries. A lazy propagation segment tree is ideal. Each node stores the sum of its segment. A lazy value of `-1` indicates no pending assignment; otherwise it holds the value to assign to the entire segment. When we need to assign a range, we recursively set the sum to `val * length` and mark lazy for children. When querying, we propagate lazy values down before visiting children. For each type-2 query, we compute `sum = query(x,y)` and `rangeLen = y - x + 1`. We then reduce the fraction `sum/rangeLen` by dividing by their greatest common divisor. If the fraction denominator becomes 1, we output only the numerator. Time complexity: each query is O(log n), and initialization is O(n) for the segment tree built in O(1) per node (here we call init once). Space complexity O(n). Edge cases: n=1, full-range assignments, overlapping lazy updates, and large values require `long long` (int64).
#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

class LazySegmentTree {
    int n;
    vector<int64> tree, lazy;
public:
    LazySegmentTree(int size) : n(size) {
        tree.resize(4 * n, 0);
        lazy.resize(4 * n, -1);
    }

    void push(int node, int l, int r) {
        if (lazy[node] != -1) {
            tree[node] = lazy[node] * (r - l + 1);
            if (l != r) {
                lazy[node * 2] = lazy[node];
                lazy[node * 2 + 1] = lazy[node];
            }
            lazy[node] = -1;
        }
    }

    void update(int node, int l, int r, int ql, int qr, int64 val) {
        push(node, l, r);
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            tree[node] = val * (r - l + 1);
            if (l != r) {
                lazy[node * 2] = val;
                lazy[node * 2 + 1] = val;
            }
            return;
        }
        int mid = (l + r) / 2;
        update(node * 2, l, mid, ql, qr, val);
        update(node * 2 + 1, mid + 1, r, ql, qr, val);
        tree[node] = tree[node * 2] + tree[node * 2 + 1];
    }

    int64 query(int node, int l, int r, int ql, int qr) {
        push(node, l, r);
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        return query(node * 2, l, mid, ql, qr) +
               query(node * 2 + 1, mid + 1, r, ql, qr);
    }
};

int64 gcd64(int64 a, int64 b) {
    while (b != 0) {
        int64 t = b;
        b = a % b;
        a = t;
    }
    return a;
}

void solveQueries(int n, const vector<tuple<int,int,int,int>>& queries, vector<string>& output) {
    LazySegmentTree seg(n);
    for (const auto& q : queries) {
        int type = get<0>(q);
        int x = get<1>(q);
        int y = get<2>(q);
        if (type == 1) {
            int64 v = get<3>(q);
            seg.update(1, 0, n - 1, x, y, v);
        } else { // type == 2
            int64 sum = seg.query(1, 0, n - 1, x, y);
            int64 len = y - x + 1;
            int64 g = gcd64(sum, len);
            int64 num = sum / g;
            int64 den = len / g;
            if (den == 1) {
                output.push_back(to_string(num));
            } else {
                output.push_back(to_string(num) + "/" + to_string(den));
            }
        }
    }
}
#include <cassert>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

// Include the solution function here (copied for standalone test)

int main() {
    // Test 1: Basic operations
    int n1 = 5;
    vector<tuple<int,int,int,int>> q1 = {
        {2, 0, 4, 0},   // avg of zeros -> 0
        {1, 1, 3, 6},   // set [1,3] to 6
        {2, 0, 4, 0},   // sum = 18, avg = 18/5 -> 18/5
        {2, 1, 3, 0},   // avg = 6
        {2, 0, 0, 0},   // avg = 0
        {1, 0, 4, 2},   // set all to 2
        {2, 0, 4, 0}    // avg = 2
    };
    vector<string> out1;
    solveQueries(n1, q1, out1);
    vector<string> expected1 = {"0", "18/5", "6", "0", "2"};
    assert(out1 == expected1);

    // Test 2: Single element array
    int n2 = 1;
    vector<tuple<int,int,int,int>> q2 = {
        {2, 0, 0, 0},
        {1, 0, 0, 7},
        {2, 0, 0, 0}
    };
    vector<string> out2;
    solveQueries(n2, q2, out2);
    vector<string> expected2 = {"0", "7"};
    assert(out2 == expected2);

    // Test 3: Full range assignment and multiple updates
    int n3 = 4;
    vector<tuple<int,int,int,int>> q3 = {
        {1, 0, 3, 10},
        {2, 0, 3, 0},   // avg = 10
        {1, 1, 2, 4},
        {2, 0, 3, 0},   // sum = 10+4+4+10=28, avg=7
        {2, 1, 2, 0},   // avg = 4
        {2, 0, 0, 0},   // avg = 10
        {2, 3, 3, 0}    // avg = 10
    };
    vector<string> out3;
    solveQueries(n3, q3, out3);
    vector<string> expected3 = {"10", "7", "4", "10", "10"};
    assert(out3 == expected3);

    // Test 4: Large values and reduction
    int n4 = 3;
    vector<tuple<int,int,int,int>> q4 = {
        {1, 0, 2, 1000000000},
        {2, 0, 2, 0},   // avg = 1000000000
        {1, 0, 1, 5},
        {2, 0, 2, 0}    // sum = 5+5+1e9 = 1000000010, len=3, gcd=1 -> 1000000010/3
    };
    vector<string> out4;
    solveQueries(n4, q4, out4);
    vector<string> expected4 = {"1000000000", "1000000010/3"};
    assert(out4 == expected4);

    // Test 5: Zero values and mixed updates
    int n5 = 6;
    vector<tuple<int,int,int,int>> q5 = {
        {1, 2, 4, 0},
        {2, 0, 5, 0},   // all zeros -> 0
        {1, 0, 5, 3},
        {2, 0, 5, 0},   // avg = 3
        {1, 0, 0, 9},
        {2, 0, 5, 0}    // sum = 9+3*5=24, avg=4
    };
    vector<string> out5;
    solveQueries(n5, q5, out5);
    vector<string> expected5 = {"0", "3", "4"};
    assert(out5 == expected5);

    return 0;
}
