// Write a C++ function `int pathAndSubtreeQuery(int n, int m, int root, int mod, const std::vector<int>& values, const std::vector<std::pair<int,int>>& edges, const std::vector<std::tuple<int,int,int,int>>& queries)` that processes `m` operations on a rooted tree with `n` nodes (1-indexed). Each operation is given as `(op, x, y, k)`. Operations: `op=1`: add `k` to all values on the simple path from `x` to `y` (mod `mod`); `op=2`: return the sum of values on the path from `x` to `y` (mod `mod`); `op=3`: add `k` to all values in the subtree rooted at `x`; `op=4`: return the sum of values in the subtree rooted at `x`. All additions are done modulo `mod` (which can be any positive integer). The initial values are given in `values` (size `n`), and the tree edges are in `edges` (size `n-1`). The function should return the sum of all answers from operations of type 2 and 4, modulo `mod`. You may assume `mod` is positive and all inputs are valid. Use heavy-light decomposition and a Fenwick tree supporting range add and range sum queries.
#include <cassert>
#include <vector>
#include <tuple>

// The solution function is assumed to be defined above.

int main() {
    // Simple chain 1-2-3-4, root=1
    {
        int n = 4;
        std::vector<int> values = {1, 2, 3, 4};
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        std::vector<std::tuple<int,int,int,int>> queries;
        // op=2 query path 1-4 => sum 10
        queries.push_back({2, 1, 4, 0});
        // op=1 add 5 to path 1-3
        queries.push_back({1, 1, 3, 5});
        // op=2 query path 2-4 => new values: [6,7,8,4] => 7+8+4=19
        queries.push_back({2, 2, 4, 0});
        // op=3 add 10 to subtree of 2 (nodes 2,3,4) => [6,17,18,14]
        queries.push_back({3, 2, 0, 10});
        // op=4 query subtree of 2 => 17+18+14=49
        queries.push_back({4, 2, 0, 0});
        int result = pathAndSubtreeQuery(n, queries.size(), 1, 1000, values, edges, queries);
        assert(result == (10 + 19 + 49) % 1000);
    }

    // Star tree root=1 with leaves 2,3,4
    {
        int n = 4;
        std::vector<int> values = {5, 6, 7, 8};
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{1,4}};
        std::vector<std::tuple<int,int,int,int>> queries;
        // op=4 query subtree of root => all values 5+6+7+8=26
        queries.push_back({4, 1, 0, 0});
        // op=1 add 1 to path 2-3 (nodes 2,1,3)
        queries.push_back({1, 2, 3, 1});
        // op=2 query path 4-2 => values now [6,7,7,8] path 4->1->2 sum = 8+6+7=21
        queries.push_back({2, 4, 2, 0});
        int result = pathAndSubtreeQuery(n, queries.size(), 1, 1000, values, edges, queries);
        assert(result == (26 + 21) % 1000);
    }

    // Single node
    {
        int n = 1;
        std::vector<int> values = {10};
        std::vector<std::pair<int,int>> edges;
        std::vector<std::tuple<int,int,int,int>> queries;
        queries.push_back({4, 1, 0, 0}); // expect 10
        queries.push_back({1, 1, 1, 7});
        queries.push_back({2, 1, 1, 0}); // expect 17
        int result = pathAndSubtreeQuery(n, queries.size(), 1, 100, values, edges, queries);
        assert(result == (10 + 17) % 100);
    }

    // Modulo with negative handling (mod=1 all answers zero)
    {
        int n = 2;
        std::vector<int> values = {1, 2};
        std::vector<std::pair<int,int>> edges = {{1,2}};
        std::vector<std::tuple<int,int,int,int>> queries;
        queries.push_back({4, 1, 0, 0});
        queries.push_back({2, 1, 2, 0});
        int result = pathAndSubtreeQuery(n, queries.size(), 1, 1, values, edges, queries);
        assert(result == 0);
    }

    // Larger random-ish test (deterministic)
    {
        int n = 5;
        std::vector<int> values = {1, 2, 3, 4, 5};
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5}}; // chain
        std::vector<std::tuple<int,int,int,int>> queries;
        // Add 5 to whole tree
        queries.push_back({1, 1, 5, 5});
        // Query whole tree sum => initially 15 + 5*5 = 40
        queries.push_back({2, 1, 5, 0});
        // Subtree sum of node 2 => nodes 2..5 sum = (7+8+9+10)=34
        queries.push_back({4, 2, 0, 0});
        int result = pathAndSubtreeQuery(n, queries.size(), 1, 1000, values, edges, queries);
        assert(result == (40 + 34) % 1000);
    }

    return 0;
}
#include <vector>
#include <tuple>
#include <algorithm>

class FenwickRangeAddSum {
    int n, mod;
    std::vector<long long> bit1, bit2;

    void internalAdd(std::vector<long long>& bit, int idx, long long val) {
        for (; idx <= n; idx += idx & -idx) {
            bit[idx] = (bit[idx] + val) % mod;
        }
    }

    long long internalSum(const std::vector<long long>& bit, int idx) const {
        long long res = 0;
        for (; idx > 0; idx -= idx & -idx) {
            res = (res + bit[idx]) % mod;
        }
        return res;
    }

public:
    FenwickRangeAddSum(int n_, int mod_) : n(n_), mod(mod_), bit1(n_ + 1, 0), bit2(n_ + 1, 0) {}

    void rangeAdd(int l, int r, long long val) {
        val %= mod;
        if (val < 0) val += mod;
        internalAdd(bit1, l, val);
        internalAdd(bit1, r + 1, (mod - val) % mod);
        internalAdd(bit2, l, val * (l - 1) % mod);
        internalAdd(bit2, r + 1, (mod - val * r % mod) % mod);
    }

    long long prefixSum(int idx) const {
        if (idx <= 0) return 0;
        long long left = (internalSum(bit1, idx) * idx) % mod;
        long long right = internalSum(bit2, idx);
        return (left - right + mod) % mod;
    }

    long long rangeSum(int l, int r) const {
        if (l > r) return 0;
        return (prefixSum(r) - prefixSum(l - 1) + mod) % mod;
    }
};

int pathAndSubtreeQuery(
    int n, int m, int root, int mod,
    const std::vector<int>& values,
    const std::vector<std::pair<int,int>>& edges,
    const std::vector<std::tuple<int,int,int,int>>& queries) {

    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // HLD preprocessing
    std::vector<int> parent(n + 1, 0), depth(n + 1, 0), size(n + 1, 0), heavy(n + 1, 0);
    std::vector<int> dfn(n + 1), top(n + 1), order(n + 1);
    int timer = 0;

    auto dfs1 = [&](auto&& self, int u, int p) -> void {
        parent[u] = p;
        depth[u] = depth[p] + 1;
        size[u] = 1;
        int maxSub = 0;
        for (int v : adj[u]) {
            if (v == p) continue;
            self(self, v, u);
            size[u] += size[v];
            if (size[v] > maxSub) {
                maxSub = size[v];
                heavy[u] = v;
            }
        }
    };
    dfs1(dfs1, root, 0);

    auto dfs2 = [&](auto&& self, int u, int h) -> void {
        dfn[u] = ++timer;
        order[timer] = u;
        top[u] = h;
        if (heavy[u]) self(self, heavy[u], h);
        for (int v : adj[u]) {
            if (v == parent[u] || v == heavy[u]) continue;
            self(self, v, v);
        }
    };
    dfs2(dfs2, root, root);

    // Initialize Fenwick with initial values
    FenwickRangeAddSum bit(n, mod);
    for (int i = 1; i <= n; ++i) {
        bit.rangeAdd(dfn[i], dfn[i], values[i - 1]);
    }

    long long totalAnswer = 0;

    // Helper lambda for path operations
    auto splitPath = [&](int u, int v, std::vector<std::pair<int,int>>& intervals) {
        intervals.clear();
        while (top[u] != top[v]) {
            if (depth[top[u]] < depth[top[v]]) std::swap(u, v);
            intervals.emplace_back(dfn[top[u]], dfn[u]);
            u = parent[top[u]];
        }
        if (depth[u] > depth[v]) std::swap(u, v);
        intervals.emplace_back(dfn[u], dfn[v]);
    };

    for (const auto& [op, x, y, k] : queries) {
        if (op == 1) {
            std::vector<std::pair<int,int>> intervals;
            splitPath(x, y, intervals);
            for (const auto& [l, r] : intervals) {
                bit.rangeAdd(l, r, k);
            }
        } else if (op == 2) {
            std::vector<std::pair<int,int>> intervals;
            splitPath(x, y, intervals);
            long long sum = 0;
            for (const auto& [l, r] : intervals) {
                sum = (sum + bit.rangeSum(l, r)) % mod;
            }
            totalAnswer = (totalAnswer + sum) % mod;
        } else if (op == 3) {
            int l = dfn[x];
            int r = dfn[x] + size[x] - 1;
            bit.rangeAdd(l, r, k);
        } else { // op == 4
            int l = dfn[x];
            int r = dfn[x] + size[x] - 1;
            totalAnswer = (totalAnswer + bit.rangeSum(l, r)) % mod;
        }
    }

    return static_cast<int>(totalAnswer);
}
// We need to support path updates/queries and subtree updates/queries on a static tree. Heavy-Light Decomposition (HLD) maps each node to a contiguous segment in a DFS order, making path operations decomposable into `O(log n)` intervals, while subtree operations become a single interval because subtree nodes are contiguous in DFS order. For range add and range sum, we use two Fenwick trees (Binary Indexed Trees): one storing `∆` and another storing `∆*i` to support range add and prefix sum query in `O(log n)`. The path update/query uses HLD to break the path into several intervals, applying each interval's operation. Subtree operations directly use `dfn[x]` to `dfn[x]+siz[x]-1`. Since all operations are modulo `mod`, we must handle negative values carefully. Edge cases: `mod` may be 1 (then every result is 0, but we still need to avoid division by zero — careful with modular arithmetic), path endpoints can be same, and the root itself. Time complexity: each operation is `O(log^2 n)` for path ops (because we process `O(log n)` intervals and each interval Fenwick update/query is `O(log n)`), and `O(log n)` for subtree ops. Overall `O((n + m) log^2 n)` time and `O(n)` space. We must ensure that the Fenwick update for range add uses `(mod - k%mod)` for range subtraction to avoid negative values.
