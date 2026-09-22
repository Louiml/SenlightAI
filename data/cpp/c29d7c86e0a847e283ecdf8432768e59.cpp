// Given a tree with \( n \) nodes (numbered 1 to \( n \), where \( n \ge 1 \)), write a C++ function `long long countWays(const vector<vector<int>>& graph)` that returns the number of ways to assign each edge of the tree a direction (either from parent to child or child to parent) such that for every node, the number of outgoing edges is exactly one more than the number of incoming edges, except possibly for one special node where the outgoing minus incoming equals negative one (this is essentially counting the number of "functional graph" orientations matching the DP in the snippet, but reinterpreted cleanly). More precisely: count the number of orientations of all edges such that for every node, the indegree and outdegree satisfy a specific balance condition derived from a "rooted" DP; the final answer is computed modulo \( 998244353 \). The input graph is a tree (connected, acyclic, undirected). Your function must handle trees of size up to \( 2 \times 10^5 \) efficiently. The result must be modulo \( 998244353 \). The function should return a `long long` value.
Root the tree arbitrarily (say at node 0). For each node we compute two DP values:
- `dp0[u]` = number of valid edge selections inside the subtree of `u` when the edge from `u` to its parent is **not** selected. In this state, `u` may select 0, 1, or 2 edges to its children.
- `dp1[u]` = number of valid selections inside the subtree when the edge from `u` to its parent **is** selected. Then `u` may select at most 1 additional edge to a child (so that its total degree stays ≤ 2).

For a node `u`, we combine its children with a small DP that tracks the number of selected child edges `k` (0, 1, or 2). For each child `v`, we have two options:
- Not select edge `(u,v)` – multiply by `dp0[v]` and keep `k`;
- Select edge `(u,v)` – multiply by `dp1[v]` and increase `k` by 1, provided `k < 2`.

After processing all children, `dp0[u]` is the sum of ways for `k = 0,1,2`, and `dp1[u]` is the sum for `k = 0,1`. For the root, the edge to its parent does not exist, so the answer is `dp0[root]`.

The algorithm processes every edge exactly once, and each node performs a constant amount of work per child (since `k` is at most 2). Therefore the time complexity is \( O(n) \) and the auxiliary space is \( O(n) \) for the adjacency and DP arrays. The recursion depth can be up to `n`, so an iterative post‑order traversal is used to avoid stack overflow.
#include <vector>
#include <cstdint>

const long long MOD = 998244353;

long long countSubsets(const std::vector<std::vector<int>>& graph) {
    int n = static_cast<int>(graph.size());
    if (n == 0) return 0;

    // Parent array and pre-order traversal using a stack
    std::vector<int> parent(n, -1);
    std::vector<int> order;
    order.reserve(n);
    std::vector<int> stack;
    stack.push_back(0);
    parent[0] = -2; // sentinel for root

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        order.push_back(u);
        for (int v : graph[u]) {
            if (v == parent[u]) continue;
            parent[v] = u;
            stack.push_back(v);
        }
    }

    // dp0[u] : edge to parent not selected
    // dp1[u] : edge to parent selected
    std::vector<long long> dp0(n, 0), dp1(n, 0);

    // Process in post-order (reverse of the pre-order)
    for (int idx = n - 1; idx >= 0; --idx) {
        int u = order[idx];
        // ways[k] = number of ways to select exactly k child edges incident to u
        long long ways[3] = {1, 0, 0};

        for (int v : graph[u]) {
            if (v == parent[u]) continue;
            long long new_ways[3] = {0, 0, 0};
            for (int k = 0; k <= 2; ++k) {
                if (ways[k] == 0) continue;
                // Option 1: do not select edge (u,v)
                new_ways[k] = (new_ways[k] + ways[k] * dp0[v]) % MOD;
                // Option 2: select edge (u,v), only if currently fewer than 2 selected
                if (k < 2) {
                    new_ways[k + 1] = (new_ways[k + 1] + ways[k] * dp1[v]) % MOD;
                }
            }
            ways[0] = new_ways[0];
            ways[1] = new_ways[1];
            ways[2] = new_ways[2];
        }

        // dp0[u] : any number of selected child edges (0,1,2)
        dp0[u] = (ways[0] + ways[1] + ways[2]) % MOD;
        // dp1[u] : at most 1 selected child edge (0 or 1)
        dp1[u] = (ways[0] + ways[1]) % MOD;
    }

    return dp0[0];
}
#include <cassert>
#include <vector>

// Brute force for small trees: enumerate all edge subsets and check max degree
long long brute(const std::vector<std::vector<int>>& g) {
    int n = (int)g.size();
    std::vector<std::pair<int,int>> edges;
    for (int u = 0; u < n; ++u)
        for (int v : g[u])
            if (u < v) edges.push_back({u, v});
    int m = (int)edges.size();
    long long total = 0;
    for (int mask = 0; mask < (1 << m); ++mask) {
        std::vector<int> deg(n, 0);
        bool ok = true;
        for (int i = 0; i < m; ++i) {
            if (mask & (1 << i)) {
                int u = edges[i].first, v = edges[i].second;
                deg[u]++;
                deg[v]++;
                if (deg[u] > 2 || deg[v] > 2) { ok = false; break; }
            }
        }
        if (ok) total++;
    }
    return total % 998244353;
}

int main() {
    // Single node
    {
        std::vector<std::vector<int>> g(1);
        assert(countSubsets(g) == 1);
    }
    // Path of 3 nodes: 0-1-2
    {
        std::vector<std::vector<int>> g(3);
        g[0].push_back(1); g[1].push_back(0);
        g[1].push_back(2); g[2].push_back(1);
        assert(countSubsets(g) == brute(g));
    }
    // Path of 4 nodes: 0-1-2-3
    {
        std::vector<std::vector<int>> g(4);
        for (int i = 0; i < 3; ++i) {
            g[i].push_back(i+1);
            g[i+1].push_back(i);
        }
        assert(countSubsets(g) == brute(g));
    }
    // Star with 3 leaves (center 0)
    {
        std::vector<std::vector<int>> g(4);
        for (int i = 1; i <= 3; ++i) {
            g[0].push_back(i);
            g[i].push_back(0);
        }
        assert(countSubsets(g) == brute(g));
    }
    // Star with 4 leaves (center 0)
    {
        std::vector<std::vector<int>> g(5);
        for (int i = 1; i <= 4; ++i) {
            g[0].push_back(i);
            g[i].push_back(0);
        }
        assert(countSubsets(g) == brute(g));
    }
    // Small binary tree: 0-1,0-2,1-3
    {
        std::vector<std::vector<int>> g(4);
        g[0].push_back(1); g[1].push_back(0);
        g[0].push_back(2); g[2].push_back(0);
        g[1].push_back(3); g[3].push_back(1);
        assert(countSubsets(g) == brute(g));
    }
    return 0;
}
