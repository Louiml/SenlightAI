// Implement a C++ function `countZeroRanges` that takes a vector of integers `initialValues`, a vector of range-add updates (each update given as `(l, r, delta)` with 0-based inclusive indices), and a list of queries `(ql, qr)`. For each query, return the number of positions in `[ql, qr]` whose current value is exactly zero after applying all updates that occur before that query (i.e., process updates sequentially; after each update you may be asked a query — for simplicity, assume all updates are given first and then all queries are given, so each query should reflect the state after all updates have been applied). The function should output a vector of integers, one per query, representing the count of zeros in the requested range. The initial array may contain any integers (including negatives), and updates may add positive or negative deltas. Values can grow large, so use 64‑bit integers. The array size `n` and both update and query counts can be up to 200,000, and all indices are valid. The solution must operate in O((n + updates + queries) log n) time.
We need a data structure that supports range add updates and range queries counting the number of minimums, but here the minimum must be exactly zero. The snippet provided implements a lazy segment tree that tracks the minimum value in each segment and how many occurrences of that minimum exist. Since the task asks for the count of zeros (not necessarily the minimum), we cannot directly use the snippet as‑is unless we assume no negative values (as the snippet does). However, updates can produce negative values, so we need a more general approach.

Instead of counting zeros directly, we can use a segment tree that stores for each node the minimum value in that segment and the count of positions attaining that minimum. Then for a query range, we compute the minimum value across the range and its count. If that minimum is zero, then the count of zeros equals the count of the minimum; otherwise, there are no zeros. This works because if the minimum is greater than zero, no zero exists, and if the minimum is exactly zero, then every zero is a minimum. If the minimum is negative, there are no zeros (since zeros are not the minimum). Thus, the snippet’s logic is actually sufficient even with negative values if we adjust the query interpretation: we only return the count when the minimum is zero; otherwise return zero.

We must implement a segment tree with lazy propagation for range addition. Each node stores `mn` (64‑bit) and `cnt`. The combine operation: take the smaller `mn`; if equal, sum counts; otherwise take the count of the smaller. Update: add delta to `mn` and to lazy. Push: propagate lazy to children and reset. Query: combine results from children, returning a pair (mn, cnt). Then for each query, if `mn == 0`, return `cnt`; otherwise return 0.

Edge cases: empty range (l > r) should return (INF, 0) so that combining logic works (since we only call query with valid ranges, but the recursion may produce empty ranges). The number of zeros can be up to the array size, so use `int` for counts (max 200,000) — safe. Use `long long` for values because updates can accumulate to large magnitudes. Initial build with all zeros is fine, but we need to set each initial value via range update of size 1 or by direct leaf assignment. Since the initial array is given, we build the tree by initializing leaves with the initial values.

Time complexity: build O(n), each update and query O(log n), total O((n + updates + queries) log n). Space O(n) for the segment tree arrays.
#include <vector>
#include <algorithm>
#include <cstdint>

struct Node {
    long long mn;
    int cnt;
    Node() : mn(0), cnt(1) {}
    Node(long long m, int c) : mn(m), cnt(c) {}
};

class LazySegTree {
private:
    int n;
    std::vector<Node> tree;
    std::vector<long long> lazy;

    Node combine(const Node& a, const Node& b) const {
        if (a.mn < b.mn) return Node(a.mn, a.cnt);
        if (b.mn < a.mn) return Node(b.mn, b.cnt);
        return Node(a.mn, a.cnt + b.cnt);
    }

    void build(int v, int tl, int tr, const std::vector<long long>& arr) {
        if (tl == tr) {
            tree[v] = Node(arr[tl], 1);
            return;
        }
        int tm = (tl + tr) / 2;
        build(2 * v, tl, tm, arr);
        build(2 * v + 1, tm + 1, tr, arr);
        tree[v] = combine(tree[2 * v], tree[2 * v + 1]);
    }

    void push(int v) {
        if (lazy[v] != 0) {
            tree[2 * v].mn += lazy[v];
            tree[2 * v + 1].mn += lazy[v];
            lazy[2 * v] += lazy[v];
            lazy[2 * v + 1] += lazy[v];
            lazy[v] = 0;
        }
    }

    void update(int v, int tl, int tr, int l, int r, long long val) {
        if (l > r) return;
        if (l == tl && r == tr) {
            tree[v].mn += val;
            lazy[v] += val;
            return;
        }
        push(v);
        int tm = (tl + tr) / 2;
        update(2 * v, tl, tm, l, std::min(r, tm), val);
        update(2 * v + 1, tm + 1, tr, std::max(l, tm + 1), r, val);
        tree[v] = combine(tree[2 * v], tree[2 * v + 1]);
    }

    Node query(int v, int tl, int tr, int l, int r) const {
        if (l > r) return Node(INT64_MAX, 0);
        if (l == tl && r == tr) return tree[v];
        push(v); // non-const in a const context; we'll make it mutable or make query non-const
        int tm = (tl + tr) / 2;
        Node left = query(2 * v, tl, tm, l, std::min(r, tm));
        Node right = query(2 * v + 1, tm + 1, tr, std::max(l, tm + 1), r);
        return combine(left, right);
    }

public:
    LazySegTree(const std::vector<long long>& arr) {
        n = (int)arr.size();
        tree.assign(4 * n, Node());
        lazy.assign(4 * n, 0);
        build(1, 0, n - 1, arr);
    }

    void rangeAdd(int l, int r, long long val) {
        update(1, 0, n - 1, l, r, val);
    }

    // Returns count of zeros in [l, r]
    int countZero(int l, int r) {
        Node res = query(1, 0, n - 1, l, r);
        return (res.mn == 0) ? res.cnt : 0;
    }
};

// Main solution function: given initial values, updates, and queries, return counts of zeros
std::vector<int> countZeroRanges(const std::vector<long long>& initialValues,
                                 const std::vector<std::tuple<int,int,long long>>& updates,
                                 const std::vector<std::pair<int,int>>& queries) {
    LazySegTree st(initialValues);
    for (const auto& [l, r, delta] : updates) {
        st.rangeAdd(l, r, delta);
    }
    std::vector<int> answer;
    answer.reserve(queries.size());
    for (const auto& [l, r] : queries) {
        answer.push_back(st.countZero(l, r));
    }
    return answer;
}
#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

// Include the solution function here (assuming it's above)

int main() {
    // Test 1: Basic
    vector<long long> init1 = {0, 0, 0};
    vector<tuple<int,int,long long>> upd1 = {{0, 1, 5}}; // after update: [5,5,0]
    vector<pair<int,int>> q1 = {{0, 2}, {2, 2}, {0, 1}};
    vector<int> res1 = countZeroRanges(init1, upd1, q1);
    assert(res1 == vector<int>({1, 1, 0}));

    // Test 2: Negative values can appear
    vector<long long> init2 = {3, -2, 1, 0};
    vector<tuple<int,int,long long>> upd2 = {{0, 1, -3}, {2, 3, 0}};
    // after updates: [0, -5, 1, 0]
    vector<pair<int,int>> q2 = {{0, 0}, {0, 3}, {1, 2}, {2, 3}};
    vector<int> res2 = countZeroRanges(init2, upd2, q2);
    assert(res2 == vector<int>({1, 2, 0, 1}));

    // Test 3: All zeros, no updates
    vector<long long> init3 = {0, 0, 0, 0};
    vector<tuple<int,int,long long>> upd3;
    vector<pair<int,int>> q3 = {{0, 3}, {1, 2}, {0, 0}};
    vector<int> res3 = countZeroRanges(init3, upd3, q3);
    assert(res3 == vector<int>({4, 2, 1}));

    // Test 4: No zeros at all
    vector<long long> init4 = {1, 2, 3};
    vector<tuple<int,int,long long>> upd4 = {{0, 2, 10}};
    vector<pair<int,int>> q4 = {{0, 2}, {0, 0}, {1, 2}};
    vector<int> res4 = countZeroRanges(init4, upd4, q4);
    assert(res4 == vector<int>({0, 0, 0}));

    // Test 5: Large values and many updates
    vector<long long> init5 = {1000000, -1000000, 500000};
    vector<tuple<int,int,long long>> upd5 = {{0, 0, -1000000}, {1, 1, 1000000}, {2, 2, -500000}};
    vector<pair<int,int>> q5 = {{0, 2}, {0, 1}, {2, 2}};
    vector<int> res5 = countZeroRanges(init5, upd5, q5);
    assert(res5 == vector<int>({3, 2, 1}));

    // Test 6: Overlapping updates
    vector<long long> init6 = {0, 0, 0, 0};
    vector<tuple<int,int,long long>> upd6 = {{0, 3, 1}, {1, 2, -1}, {0, 0, -1}};
    // after first: [1,1,1,1]; after second: [1,0,0,1]; after third: [0,0,0,1]
    vector<pair<int,int>> q6 = {{0, 3}, {1, 2}, {0, 2}};
    vector<int> res6 = countZeroRanges(init6, upd6, q6);
    assert(res6 == vector<int>({3, 2, 3}));

    return 0;
}
