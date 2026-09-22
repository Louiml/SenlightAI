// Given a tree with `n` nodes (numbered 1 to `n`) and `n-1` undirected edges, implement a C++ function that processes a sequence of `q` queries. Each query specifies an edge index `e` (1-based), an operation type `t` (either 1 or 2), and a value `x`. For type 1, add `x` to all nodes except those in the subtree of the deeper endpoint when the edge is considered as rooted from the side of the `e`th edge's first endpoint. For type 2, add `x` to all nodes except those in the subtree of the deeper endpoint when the edge is rooted from the side of the second endpoint. After all queries, return a vector `result` where `result[i-1]` is the final value assigned to node `i`. All values are initially zero, and updates accumulate. The function signature is `std::vector<long long> processTree(int n, const std::vector<std::pair<int,int>>& edges, const std::vector<std::tuple<int,int,int>>& queries)`. The function should handle `n` up to `2e5` and `q` up to `2e5` efficiently, and values can be large, so use 64-bit integers.

The core idea is to flatten the tree into an Euler tour so that each subtree becomes a contiguous interval in an array. We perform a DFS from node 1 (root) to compute `in[u]` (entry time) and `out[u]` (exit time) for every node. This allows any subtree update to be a range addition on the Euler array. For each query, we need to decide which side of the edge is "excluded" based on the operation type. Since the edge connects `u` and `v`, exactly one of them is the parent of the other in the root‑based tree (because the tree is rooted at 1). If `in[u] < in[v]`, then `u` is the parent of `v` (since in a DFS, the parent is visited before the child). For type 1, we add `x` to all nodes except those in the subtree of the child; this is equivalent to adding `x` to the entire Euler array and subtracting `x` from the child's subtree interval. If `in[u] > in[v]`, then `v` is the parent of `u`, so for type 1 we simply add `x` to the subtree of `u` (because the excluded part is the subtree of the deeper endpoint, which is `u`). Symmetric logic applies for type 2. We use a lazy segment tree to support range addition and point query. After processing all queries, we query each node's position in the Euler array and store the result. Time complexity is `O((n+q) log n)` due to DFS and segment tree operations, with `O(n)` auxiliary space for the tree and segment tree arrays.

#include <bits/stdc++.h>
using namespace std;

// Process tree edge-update queries and return final node values.
std::vector<long long> processTree(
    int n,
    const std::vector<std::pair<int,int>>& edges,
    const std::vector<std::tuple<int,int,int>>& queries
) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // Euler tour
    std::vector<int> in(n + 1), out(n + 1);
    int timer = 0;
    std::function<void(int,int)> dfs = [&](int u, int p) {
        in[u] = timer++;
        for (int v : adj[u]) {
            if (v != p) dfs(v, u);
        }
        out[u] = timer - 1;
    };
    dfs(1, -1);

    // Lazy segment tree for range addition and point query
    std::vector<long long> tree(4 * n, 0), lazy(4 * n, 0);

    // Propagate pending lazy values
    auto propagate = [&](int node, int l, int r) {
        if (lazy[node] == 0) return;
        tree[node] += lazy[node] * (r - l + 1);
        if (l != r) {
            lazy[2*node] += lazy[node];
            lazy[2*node+1] += lazy[node];
        }
        lazy[node] = 0;
    };

    // Range add
    std::function<void(int,int,int,int,int,long long)> update =
        [&](int node, int l, int r, int ql, int qr, long long val) {
            propagate(node, l, r);
            if (qr < l || ql > r) return;
            if (ql <= l && r <= qr) {
                lazy[node] += val;
                propagate(node, l, r);
                return;
            }
            int mid = (l + r) / 2;
            update(2*node, l, mid, ql, qr, val);
            update(2*node+1, mid+1, r, ql, qr, val);
            tree[node] = tree[2*node] + tree[2*node+1];
        };
    auto range_add = [&](int l, int r, long long val) {
        if (l > r) return;
        update(1, 0, n-1, l, r, val);
    };

    // Point query
    std::function<long long(int,int,int,int)> query =
        [&](int node, int l, int r, int pos) -> long long {
            propagate(node, l, r);
            if (l == r) return tree[node];
            int mid = (l + r) / 2;
            if (pos <= mid) return query(2*node, l, mid, pos);
            else return query(2*node+1, mid+1, r, pos);
        };
    auto point_query = [&](int pos) -> long long {
        return query(1, 0, n-1, pos);
    };

    // Process queries
    for (const auto& q : queries) {
        int t = std::get<0>(q);
        int e = std::get<1>(q) - 1; // zero-based edge index
        int x = std::get<2>(q);
        int u = edges[e].first;
        int v = edges[e].second;

        if (t == 1) {
            if (in[u] < in[v]) {
                // u is parent, v is child; add to all except subtree of v
                range_add(0, n-1, x);
                range_add(in[v], out[v], -x);
            } else {
                // v is parent, u is child; add only to subtree of u
                range_add(in[u], out[u], x);
            }
        } else { // t == 2
            if (in[u] > in[v]) {
                // v is parent, u is child; add to all except subtree of u
                range_add(0, n-1, x);
                range_add(in[u], out[u], -x);
            } else {
                // u is parent, v is child; add only to subtree of v
                range_add(in[v], out[v], x);
            }
        }
    }

    // Collect results
    std::vector<long long> result(n);
    for (int i = 1; i <= n; ++i) {
        result[i-1] = point_query(in[i]);
    }
    return result;
}

#include <bits/stdc++.h>
#include <cassert>
std::vector<long long> processTree(
    int n,
    const std::vector<std::pair<int,int>>& edges,
    const std::vector<std::tuple<int,int,int>>& queries
);

int main() {
    // Test 1: Simple chain of 3 nodes
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        std::vector<std::tuple<int,int,int>> queries = {
            {1, 1, 5},  // edge 1 (1-2), type 1: add 5 to all except subtree of 2 when rooted at 1
            {2, 2, 2}   // edge 2 (2-3), type 2: add 2 to all except subtree of 3 when rooted from side of 3
        };
        auto res = processTree(n, edges, queries);
        // After first: node1=5, node2=0, node3=0
        // After second: edge 2 between 2-3, type 2: since in[2]<in[3], v=3 is child, add to all except subtree of 3 => node1+=2, node2+=2, node3+=0
        // Final: node1=7, node2=2, node3=0
        assert(res[0] == 7);
        assert(res[1] == 2);
        assert(res[2] == 0);
    }

    // Test 2: Star with center 1 and leaves 2,3
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3}};
        std::vector<std::tuple<int,int,int>> queries = {
            {1, 1, 10},  // edge 1 (1-2): add 10 to all except subtree of 2 -> node1,3 get 10
            {2, 2, 3}    // edge 2 (1-3): type2: add 3 to subtree of 3 (since in[1]<in[3], child=3)
        };
        auto res = processTree(n, edges, queries);
        assert(res[0] == 10); // node1
        assert(res[1] == 0);  // node2
        assert(res[2] == 13); // node3 (10+3)
    }

    // Test 3: No queries
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{1,2}};
        std::vector<std::tuple<int,int,int>> queries = {};
        auto res = processTree(n, edges, queries);
        assert(res[0] == 0);
        assert(res[1] == 0);
    }

    // Test 4: Larger tree with accumulated updates
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{2,4},{2,5}};
        std::vector<std::tuple<int,int,int>> queries = {
            {1, 1, 1},  // edge 1 (1-2): add 1 to all except subtree of 2 -> nodes 1,3
            {2, 2, 2},  // edge 2 (1-3): type2: add 2 to subtree of 3
            {1, 3, 3},  // edge 3 (2-4): type1: add 3 to subtree of 4 (since 2 is parent, 4 child)
            {2, 4, 4}   // edge 4 (2-5): type2: add to all except subtree of 5 -> nodes 1,3,2,4
        };
        auto res = processTree(n, edges, queries);
        // Node1: after q1=1, q2=0 (since type2 adds to subtree of 3), q3=0, q4=4 => total 5
        // Node2: after q1=0 (excluded), q2=0, q3=0, q4=4 => total 4
        // Node3: after q1=1, q2=2, q3=0, q4=4 => total 7
        // Node4: after q1=0, q2=0, q3=3, q4=4 => total 7
        // Node5: after q1=0, q2=0, q3=0, q4=0 => total 0
        assert(res[0] == 5);
        assert(res[1] == 4);
        assert(res[2] == 7);
        assert(res[3] == 7);
        assert(res[4] == 0);
    }

    // Test 5: Edge cases with type 2 where parent-child relation reversed
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}}; // chain 1-2-3-4
        std::vector<std::tuple<int,int,int>> queries = {
            {2, 1, 5}  // edge 1 (1-2), type2: since in[1]<in[2], child=2 => add 5 to subtree of 2 (nodes 2,3,4)
        };
        auto res = processTree(n, edges, queries);
        assert(res[0] == 0);
        assert(res[1] == 5);
        assert(res[2] == 5);
        assert(res[3] == 5);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
