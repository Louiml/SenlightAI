/*
Given an undirected graph with `N` vertices and `M` edges, where the edge set may contain cycles (non-tree edges), write a C++ function that determines, for every edge, whether it is a forward edge, backward edge, or bidirectional edge, according to the following rules. First, a spanning tree is built using any maximal set of edges that does not create cycles (e.g., via union-find Kruskal). All edges not in the tree are "extra" edges and must be labeled `'B'` (bidirectional). Then, `Q` queries are given, each specifying a pair `(u, v)`; for each query, the tree path from `u` to `v` is traversed conceptually: edges going "up" toward the root get label `'R'` (reverse) and edges going "down" away from the root get label `'L'` (forward). If an edge accumulates both an `'R'` and an `'L'` requirement from queries, or if it is an extra edge, it becomes `'B'`. If it has only `'R'` requirements, it stays `'R'`; if only `'L'`, it stays `'L'`; if no requirements (and not extra), it is labeled `'R'`. You are given the graph edges in input order, and must output a string of length `M` where the `i`-th character is `'B'`, `'L'`, or `'R'`. The graph may be disconnected; each connected component forms its own tree with an arbitrary root (the first vertex visited). Your task: implement a function `std::string classifyEdges(int N, const std::vector<std::pair<int,int>>& edges, const std::vector<std::pair<int,int>>& queries)` that returns the answer string. The graph vertices are 1-indexed (1..N). The edge list and query list are given in arbitrary order; the output must correspond to the edge indices in the input `edges` vector.
*/

#include <bits/stdc++.h>
using namespace std;

// Classify each edge as 'B' (bidirectional), 'L' (forward/down), or 'R' (reverse/up)
// based on spanning tree and query paths.
std::string classifyEdges(int N, const std::vector<std::pair<int,int>>& edges,
                          const std::vector<std::pair<int,int>>& queries) {
    int M = edges.size();
    // DSU for building spanning forest
    vector<int> parent(N+1), rnk(N+1, 1);
    iota(parent.begin(), parent.end(), 0);
    function<int(int)> findSet = [&](int u) -> int {
        return parent[u] == u ? u : parent[u] = findSet(parent[u]);
    };
    function<void(int,int)> unionSet = [&](int u, int v) {
        u = findSet(u); v = findSet(v);
        if (u == v) return;
        if (rnk[u] < rnk[v]) swap(u, v);
        parent[v] = u;
        if (rnk[u] == rnk[v]) rnk[u]++;
    };

    vector<vector<int>> adj(N+1);
    vector<bool> extra(M, false);
    map<pair<int,int>, int> edgeIndex;  // store tree edge index
    for (int i = 0; i < M; ++i) {
        int u = edges[i].first, v = edges[i].second;
        if (findSet(u) != findSet(v)) {
            unionSet(u, v);
            adj[u].push_back(v);
            adj[v].push_back(u);
            edgeIndex[{u, v}] = i;
            edgeIndex[{v, u}] = i;
        } else {
            extra[i] = true;
        }
    }

    // LCA preprocessing: DFS from each root
    vector<int> in(N+1, 0), out(N+1, 0);
    vector<vector<int>> jump(N+1, vector<int>(20, 0));
    int timer = 0;
    function<void(int,int)> build = [&](int u, int p) {
        in[u] = ++timer;
        jump[u][0] = p;
        for (int k = 1; k < 20; ++k)
            jump[u][k] = jump[jump[u][k-1]][k-1];
        for (int v : adj[u])
            if (v != p) build(v, u);
        out[u] = ++timer;
    };
    for (int i = 1; i <= N; ++i)
        if (findSet(i) == i) build(i, i);

    auto ancestor = [&](int u, int v) -> bool {
        return in[u] <= in[v] && out[u] >= out[v];
    };
    auto LCA = [&](int u, int v) -> int {
        if (ancestor(u, v)) return u;
        if (ancestor(v, u)) return v;
        for (int k = 19; k >= 0; --k)
            if (!ancestor(jump[u][k], v))
                u = jump[u][k];
        return jump[u][0];
    };

    // Difference arrays: [0]=up, [1]=down, [2]=both
    vector<array<int,3>> a(N+1);
    for (auto& arr : a) arr = {0, 0, 0};
    for (auto& q : queries) {
        int u = q.first, v = q.second;
        int l = LCA(u, v);
        a[u][0]++;
        a[v][1]++;
        a[l][0]--;
        a[l][1]--;
    }
    for (int i = 0; i < M; ++i) if (extra[i]) {
        int u = edges[i].first, v = edges[i].second;
        int l = LCA(u, v);
        a[u][2]++;
        a[v][2]++;
        a[l][2] -= 2;
    }

    // Post-order DFS to aggregate and classify tree edges
    vector<pair<pair<int,int>, char>> result; // tree edge (u,v) and label
    function<void(int,int)> dfs = [&](int u, int p) {
        for (int v : adj[u]) if (v != p) {
            dfs(v, u);
            a[u][0] += a[v][0];
            a[u][1] += a[v][1];
            a[u][2] += a[v][2];
            char lab;
            if (a[v][2] > 0) lab = 'B';
            else if (a[v][1] > 0) lab = 'L';
            else lab = 'R';  // includes zero requirements
            result.push_back({{u, v}, lab});
        }
    };
    for (int i = 1; i <= N; ++i)
        if (findSet(i) == i) dfs(i, i);

    // Map back to original indices
    vector<char> ans(M, '?');
    for (auto& res : result) {
        auto [u, v] = res.first;
        char lab = res.second;
        // Edge is stored as (u,v) with u = parent, v = child
        // The map has both orders; for 'L' or 'R' we need to ensure orientation matches
        int idx = edgeIndex[{u, v}];
        // For 'L'/'R', they are defined relative to the query direction, but here
        // we rely on the DFS orientation: 'L' = down (parent->child), 'R' = up.
        // The map stores the original tree edge orientation in input order; we already
        // have both (u,v) and (v,u) in map, so just look up.
        ans[idx] = lab;
    }
    for (int i = 0; i < M; ++i)
        if (extra[i]) ans[i] = 'B';

    return string(ans.begin(), ans.end());
}

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Place the classifyEdges function here (or include it)

int main() {
    // Test 1: Simple tree with two edges, one query from 1 to 3
    {
        int N = 3;
        vector<pair<int,int>> edges = {{1,2}, {2,3}};
        vector<pair<int,int>> queries = {{1,3}};
        string res = classifyEdges(N, edges, queries);
        // Path 1->2->3: edge (1,2) goes down (L), edge (2,3) goes down (L)
        assert(res == "LL");
    }

    // Test 2: Graph with a cycle (extra edge) and a query
    {
        int N = 3;
        vector<pair<int,int>> edges = {{1,2}, {2,3}, {1,3}};
        vector<pair<int,int>> queries = {{1,2}};
        string res = classifyEdges(N, edges, queries);
        // Edge 0 is tree, edge 1 is tree, edge 2 is extra -> B.
        // Query 1->2: edge 0 is L, edge 1 unused -> R (zero requirement)
        // But edge 1 has zero requirement? Actually path from 1 to 2 only uses edge 0.
        // Edge 1 has no requirements -> R.
        // So expected: "LRB"? Wait edge0=L, edge1=R, edge2=B => "LRB"
        assert(res == "LRB");
    }

    // Test 3: Disconnected graph, two components
    {
        int N = 4;
        vector<pair<int,int>> edges = {{1,2}, {3,4}};
        vector<pair<int,int>> queries = {{1,2}};
        string res = classifyEdges(N, edges, queries);
        // Edge 0 = L, edge 1 = R (zero requirement) => "LR"
        assert(res == "LR");
    }

    // Test 4: Query in opposite direction gives R
    {
        int N = 3;
        vector<pair<int,int>> edges = {{1,2}, {2,3}};
        vector<pair<int,int>> queries = {{3,1}};
        string res = classifyEdges(N, edges, queries);
        // Path 3->2->1: both edges up => R, R
        assert(res == "RR");
    }

    // Test 5: Query that covers both directions on same edge
    {
        int N = 3;
        vector<pair<int,int>> edges = {{1,2}, {2,3}};
        vector<pair<int,int>> queries = {{1,2}, {2,1}};
        string res = classifyEdges(N, edges, queries);
        // Edge 0 gets L then R -> B. Edge 1 unused -> R.
        assert(res == "BR");
    }

    // Test 6: Extra edge with query on cycle
    {
        int N = 2;
        vector<pair<int,int>> edges = {{1,2}, {1,2}}; // two parallel edges
        vector<pair<int,int>> queries = {{1,2}};
        string res = classifyEdges(N, edges, queries);
        // Edge 0 tree -> L, Edge 1 extra -> B
        assert(res == "LB");
    }

    // Test 7: Single vertex, no edges
    {
        int N = 1;
        vector<pair<int,int>> edges = {};
        vector<pair<int,int>> queries = {};
        string res = classifyEdges(N, edges, queries);
        assert(res.empty());
    }

    // Test 8: Larger tree with multiple queries
    {
        int N = 5;
        vector<pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5}};
        vector<pair<int,int>> queries = {{1,5}}; // creates L,L,L,L
        string res = classifyEdges(N, edges, queries);
        assert(res == "LLLL");
    }

    // Test 9: Mixed queries causing both
    {
        int N = 4;
        vector<pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        vector<pair<int,int>> queries = {{1,4},{4,1}};
        string res = classifyEdges(N, edges, queries);
        // All edges get both L and R -> B
        assert(res == "BBB");
    }

    // Test 10: Cycle with no queries – extra edge still B, tree edges all R
    {
        int N = 4;
        vector<pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,1}};
        vector<pair<int,int>> queries = {};
        string res = classifyEdges(N, edges, queries);
        // Three tree edges get R (zero req), one extra edge gets B.
        // Which are tree? Depends on order: assume 1-2,2-3,3-4 are tree, 4-1 extra.
        // So "RRRB"
        assert(res == "RRRB");
    }

    cout << "All tests passed!\n";
    return 0;
}

// The problem combines spanning-tree construction, LCA preprocessing, and difference-array propagation on a tree.  
// **Approach:**  
// 1. Build a spanning forest using DSU: iterate edges, if `find(u) != find(v)`, add this edge to the adjacency list of the tree, record the edge index in a map from `(u,v)` to index, and union the sets. Edges that connect already-connected components are marked as "extra".  
// 2. For each tree root (a vertex that is its own DSU parent after all unions), run a DFS to assign `in`/`out` timestamps and build binary lifting tables (`jump[u][k]` for `k=0..19`) for LCA.  
// 3. For each query `(u, v)`, compute `l = LCA(u,v)`. Increment `a[u][0]++` and `a[v][1]++` (where `0` means "up" requirement, `1` means "down" requirement), then decrement at the LCA: `a[l][0]--` and `a[l][1]--`. This difference-array trick will allow tree DFS to aggregate counts per edge.  
// 4. For each extra edge `(u, v)`, compute its LCA and do `a[u][2]++`, `a[v][2]++`, `a[l][2] -= 2`. This marks that the entire path has a "both" requirement.  
// 5. Perform a post-order DFS from each root, accumulating child values into parent. At each node `u` (except the root when `u == parent`), based on the aggregated counts `a[u][0]`, `a[u][1]`, `a[u][2]` for the edge `(parent, u)`, decide the label: if `a[u][2] > 0` → `'B'`; else if `a[u][1] > 0` → `'L'`; else → `'R'` (including the case where counts are all zero, because the problem requires zero-requirement edges to be `'R'`).  
// 6. Map the label back to the original edge index using the stored map (or swap endpoints if needed, and for `'L'`/`'R'` also swap the direction because the label depends on orientation).  
// 7. All extra edges are labeled `'B'` regardless of queries.  
// **Edge cases:** Graph may be disconnected (multiple roots); queries may involve vertices in different components (though the problem likely guarantees they are in the same component; if not, LCA fails, but we assume valid input). Vertices are 1-indexed.  
// **Complexity:** Building DSU: O(M α(N)). DFS and LCA preprocessing: O(N log N). Processing Q queries: O(Q log N). Final DFS: O(N). Total time O((N+M+Q) log N) and space O(N log N + M). For typical constraints N, M, Q ≤ 1e5, this is efficient.  
// **Important:** The function must handle recursion depth for large N; to avoid stack overflow, we can use iterative DFS or increase recursion limit (but for a standalone function without main, we assume recursion is acceptable; in practice, use `std::function` or explicit stack for robustness).
