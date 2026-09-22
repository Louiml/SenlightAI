Given two 1-indexed arrays `a` and `b` of length `n` (1 ≤ n ≤ 10^5), and `q` queries (1 ≤ q ≤ 10^5), each query provides a closed interval `[l, r]`. For each query, you must determine whether it is possible to pair each element `i` in `[l, r]` with a distinct integer `k` also in `[l, r]` such that `b[k] - a[i]` is non‑negative, and the sum over all pairs of `(b[k] - a[i])` is zero. If such a perfect matching exists, output the maximum possible total sum of `(b[k] - a[i])` over any valid pairing (this maximum will be non‑negative). If no valid pairing exists, output `-1`. Write a function `vector<long long> solveQueries(const vector<int>& a, const vector<int>& b, const vector<pair<int,int>>& queries)` that processes all queries and returns the answer for each, in order.

#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (e.g., copy from above)

int main() {
    // Test 1: simple valid case
    {
        vector<int> a = {1, 2, 3};
        vector<int> b = {3, 2, 1};  // diff: 2,0,-2
        vector<pair<int,int>> queries = {{0,2}};
        auto res = solveQueries(a, b, queries);
        assert(res.size() == 1);
        assert(res[0] == 2);  // max prefix sum is 2
    }
    // Test 2: invalid total not zero
    {
        vector<int> a = {1, 1};
        vector<int> b = {2, 3};  // diff: 1,2
        vector<pair<int,int>> queries = {{0,1}};
        auto res = solveQueries(a, b, queries);
        assert(res[0] == -1);
    }
    // Test 3: prefix negative
    {
        vector<int> a = {5, 0};
        vector<int> b = {0, 5};  // diff: -5,5
        vector<pair<int,int>> queries = {{0,1}};
        auto res = solveQueries(a, b, queries);
        assert(res[0] == -1);  // minPrefix is -5
    }
    // Test 4: single element with diff 0
    {
        vector<int> a = {7};
        vector<int> b = {7};
        vector<pair<int,int>> queries = {{0,0}};
        auto res = solveQueries(a, b, queries);
        assert(res[0] == 0);
    }
    // Test 5: multiple queries
    {
        vector<int> a = {4, 6, 9};
        vector<int> b = {6, 6, 7};  // diff: 2,0,-2
        vector<pair<int,int>> queries = {{0,0}, {1,2}, {0,2}};
        auto res = solveQueries(a, b, queries);
        assert(res.size() == 3);
        assert(res[0] == 2);
        assert(res[1] == -1);  // diff: 0,-2 total -2 not zero
        assert(res[2] == 2);
    }
    // Test 6: all zero diffs
    {
        vector<int> a = {3, 3, 3};
        vector<int> b = {3, 3, 3};
        vector<pair<int,int>> queries = {{0,2}};
        auto res = solveQueries(a, b, queries);
        assert(res[0] == 0);
    }
    // Test 7: long range with balanced positive/negative
    {
        vector<int> a = {10, 0, 5, 1};
        vector<int> b = {5, 5, 5, 6};  // diff: -5,5,0,5
        vector<pair<int,int>> queries = {{0,3}};
        auto res = solveQueries(a, b, queries);
        assert(res[0] == -1);  // total = 5 not zero
    }
    // Test 8: valid but max prefix > total
    {
        vector<int> a = {0, 5, 0};
        vector<int> b = {5, 0, 0};  // diff: 5,-5,0
        vector<pair<int,int>> queries = {{0,2}};
        auto res = solveQueries(a, b, queries);
        assert(res[0] == 5);  // max prefix is 5
    }

    cout << "All tests passed!" << endl;
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll INF = 1e18;

struct Node {
    ll minPrefix, maxPrefix, total;  // empty prefix is included
    ll minSuffix, maxSuffix;
    Node() : minPrefix(INF), maxPrefix(-INF), total(0), minSuffix(INF), maxSuffix(-INF) {}
    Node(ll val) : minPrefix(min(0LL, val)), maxPrefix(max(0LL, val)), total(val),
                   minSuffix(min(0LL, val)), maxSuffix(max(0LL, val)) {}
};

Node mergeNodes(const Node& L, const Node& R) {
    Node res;
    res.total = L.total + R.total;
    res.minPrefix = min(L.minPrefix, L.total + R.minPrefix);
    res.maxPrefix = max(L.maxPrefix, L.total + R.maxPrefix);
    res.minSuffix = min(R.minSuffix, R.total + L.minSuffix);
    res.maxSuffix = max(R.maxSuffix, R.total + L.maxSuffix);
    return res;
}

class SegmentTree {
    int n;
    vector<Node> tree;
    vector<ll> arr;
public:
    SegmentTree(const vector<ll>& data) : n(data.size()), arr(data) {
        tree.resize(4*n);
        build(1, 0, n-1);
    }
    void build(int node, int l, int r) {
        if (l == r) {
            tree[node] = Node(arr[l]);
        } else {
            int mid = (l+r)/2;
            build(node*2, l, mid);
            build(node*2+1, mid+1, r);
            tree[node] = mergeNodes(tree[node*2], tree[node*2+1]);
        }
    }
    Node query(int node, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return Node(); // empty node
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l+r)/2;
        Node left = query(node*2, l, mid, ql, qr);
        Node right = query(node*2+1, mid+1, r, ql, qr);
        return mergeNodes(left, right);
    }
    Node query(int l, int r) {
        return query(1, 0, n-1, l, r);
    }
};

vector<long long> solveQueries(const vector<int>& a, const vector<int>& b, const vector<pair<int,int>>& queries) {
    int n = a.size();
    vector<ll> diff(n);
    for (int i = 0; i < n; ++i) diff[i] = (ll)b[i] - (ll)a[i];
    SegmentTree st(diff);
    vector<ll> ans;
    for (const auto& q : queries) {
        int l = q.first, r = q.second;
        Node res = st.query(l, r);
        if (res.total == 0 && res.minPrefix >= 0) {
            ans.push_back(res.maxPrefix);
        } else {
            ans.push_back(-1);
        }
    }
    return ans;
}

// This is equivalent to checking whether the multiset of differences `b[i] - a[i]` over `[l, r]` can be partitioned into pairs where one non‑negative and one non‑positive sum to zero. The key observation: if we sort the differences, a valid pairing exists exactly when the prefix sums of the sorted differences are all non‑negative and the total sum is zero, because we can greedily match the largest positive with the most negative. However, we need to handle range queries efficiently without sorting each subarray. The trick is to use a segment tree that stores, for each segment, the minimum prefix sum, maximum prefix sum, total sum, and the same for suffixes, but computed on the **cumulative** sequence (i.e., using the running sums of `b[i]-a[i]`). Specifically, define `d[i] = b[i] - a[i]`. For a query `[l, r]`, the condition simplifies to: `sum_{i=l}^{r} d[i] == 0` **and** for every prefix of the subarray `[l, r]`, the cumulative sum is ≥ 0. That is, if we take cumulative sums starting from `l`, all must be non‑negative, and the final total must be zero. The maximum possible sum of a valid pairing is exactly the maximum prefix sum (which will be ≥ 0). We can compute these using a segment tree that stores for each node: `minPrefix` (minimum cumulative sum from the left end, where an empty prefix counts as 0), `maxPrefix` (maximum cumulative sum), `total` (sum of all elements), and similarly `minSuffix`/`maxSuffix`. Merging two segments A and B: `total = A.total + B.total`, `minPrefix = min(A.minPrefix, A.total + B.minPrefix)`, `maxPrefix = max(A.maxPrefix, A.total + B.maxPrefix)`, `minSuffix = min(B.minSuffix, B.total + A.minSuffix)`, `maxSuffix = max(B.maxSuffix, B.total + A.maxSuffix)`. For a leaf with value `v`, the empty prefix is 0, so `minPrefix = min(0, v)`, `maxPrefix = max(0, v)`, `minSuffix = min(0, v)`, `maxSuffix = max(0, v)`, `total = v`. For a query, we combine the nodes covering `[l, r]` in order. The answer is `-1` if `total != 0` or `minPrefix < 0`; otherwise it is `maxPrefix`. Time: building the tree O(n), each query O(log n) with node merging; total O((n+q) log n). Space: O(n).
