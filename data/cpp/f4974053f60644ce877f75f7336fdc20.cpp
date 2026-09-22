You are given a directed graph with `n` vertices and `m` edges, two special vertices `a` and `b`, and an integer `k`. Additionally, there are `k` “teleport” events, but each event only matters modulo 2: the effective parity of `k` (i.e., `k % 2`) is used as the target Sprague–Grundy value for a game. For each vertex `u`, define its Grundy number `g[u]` in the usual way: if `u` has out‑degree 0, then `g[u] = parity` (which is `k % 2`); otherwise `g[u] = mex{ g[v] : v is a direct successor of u }`. Compute the Grundy numbers for the original graph and also for the reversed graph (i.e., edges reversed). If `g[a] == g_rev[b]` (where `g_rev` is computed on the reversed graph), output `"NO"`; otherwise output `"YES"`. The input format is: first line `n m a b`, second line a string (ignore it), then `m` lines of edges `u v` (1‑based), then a line with `x y k` (ignore `x` and `y`), then `k` lines each with four integers (ignore all of them). Your task is to write a C++ function `std::string decideGame(int n, int m, int a, int b, int k, const std::vector<std::pair<int,int>>& edges)` that returns `"YES"` or `"NO"` according to the rule above. The graph may have multiple edges and cycles; if a vertex is in a cycle, the standard topological processing will not cover it – in that case, assume the Grundy value for any vertex not processed by the algorithm (i.e., remaining after Kahn’s algorithm) is `0`. Edge cases: `n` can be 0 (then return `"NO"` if `a == b`? but `a` and `b` are 1‑based so if `n=0` they are invalid, so handle gracefully by returning `"NO"`), and `k` can be 0.
The problem is a direct application of computing Grundy numbers (mex) on a directed graph, but with a twist: leaf nodes (out‑degree 0) get the value `k % 2` instead of 0, and for nodes not reachable by the reverse topological order (i.e., part of cycles or not processed because they depend on cycles), we assign 0. The algorithm uses Kahn’s algorithm in reverse: we start with all nodes that have out‑degree 0 in the current graph. For each such node `u`, we set its Grundy value as: if it truly has no outgoing edges in the original graph, then `g[u] = parity`; otherwise (it was processed because its outgoing neighbors were all computed) we compute the mex of the values of its direct successors. Note: in the given code, the check `if (adj[u].empty())` is done when popping from the queue, but since a node could have outgoing edges that were all processed, `adj[u]` is not empty, so it goes to the else branch and computes mex. This works because we only process nodes after all their outgoing neighbors have been processed (since we decrement in‑degree of predecessors when a successor is processed, and we start with nodes with out‑degree 0). The reversal of edges is used to compute in‑degrees in the reversed graph, which are out‑degrees in original. After computing both `g` for original and `g_rev` for reversed, we compare `g[a-1]` and `g_rev[b-1]`. Since indices are 1‑based in input, we convert to 0‑based. If equal, return `"NO"` else `"YES"`. Time complexity is O(n + m) for each call to the Grundy computation (two calls), but each call uses a set per node which can be O(deg log deg) – total O(n + m log max_deg). Space is O(n + m). Edge cases: if `n==0`, we cannot have valid `a` or `b`, so return `"NO"`. If `k` is even, parity = 0; if odd, parity = 1. The string and the extra `k` lines are ignored because they don't affect the result.
#include <bits/stdc++.h>

// Computes Grundy numbers for a directed graph.
// adj: adjacency list of the graph
// rev_adj: reversed adjacency list (for Kahn's algorithm)
// parity: value assigned to leaves (out-degree 0)
// Returns vector of Grundy numbers. Nodes not processed (cycles) get 0.
static std::vector<int> computeGrundy(const std::vector<std::vector<int>>& adj,
                                      const std::vector<std::vector<int>>& rev_adj,
                                      int parity) {
    int n = (int)adj.size();
    std::vector<int> outdeg(n);
    std::queue<int> q;
    for (int i = 0; i < n; ++i) {
        outdeg[i] = (int)adj[i].size();
        if (outdeg[i] == 0) {
            q.push(i);
        }
    }

    std::vector<int> g(n, 0);
    std::vector<int> processed(n, 0);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        processed[u] = 1;

        if (adj[u].empty()) {
            g[u] = parity;
        } else {
            std::set<int> vals;
            for (int v : adj[u]) {
                vals.insert(g[v]);
            }
            int mex = 0;
            while (vals.count(mex)) ++mex;
            g[u] = mex;
        }

        // Update predecessors in original graph (which are successors in rev_adj)
        for (int pred : rev_adj[u]) {
            outdeg[pred]--;
            if (outdeg[pred] == 0) {
                q.push(pred);
            }
        }
    }

    // Any node not processed (part of cycle) remains 0.
    return g;
}

// Main solution function: decides YES/NO based on Grundy comparison.
std::string decideGame(int n, int m, int a, int b, int k,
                       const std::vector<std::pair<int,int>>& edges) {
    if (n <= 0) return "NO";
    // Convert to 0-based
    --a; --b;

    std::vector<std::vector<int>> adj(n);
    std::vector<std::vector<int>> rev_adj(n);
    for (const auto& e : edges) {
        int u = e.first - 1;
        int v = e.second - 1;
        if (u < 0 || u >= n || v < 0 || v >= n) continue;
        adj[u].push_back(v);
        rev_adj[v].push_back(u);
    }

    int parity = k % 2;
    std::vector<int> g = computeGrundy(adj, rev_adj, parity);

    // Build reversed graph (swap edges)
    std::vector<std::vector<int>> rev_adj_rev(n);
    std::vector<std::vector<int>> adj_rev(n); // reversed graph's adjacency
    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            adj_rev[v].push_back(u); // reversed edge
            rev_adj_rev[u].push_back(v); // reversed graph's reverse
        }
    }
    std::vector<int> g_rev = computeGrundy(adj_rev, rev_adj_rev, parity);

    if (g[a] == g_rev[b]) return "NO";
    return "YES";
}
#include <cassert>
#include <string>
#include <vector>

// The solution function is defined above. We only call it here.
int main() {
    // Test 1: Simple edge case n=1, no edges, a=b=1, k=0 -> both leaves, parity=0 -> equal -> NO
    assert(decideGame(1, 0, 1, 1, 0, {}) == "NO");
    // Test 2: n=2, one edge 1->2, a=1,b=2,k=0. Leaf 2 gets 0, node 1 gets mex{0}=1. Reversed graph: edge 2->1, leaf 1 gets 0, node 2 gets mex{0}=1. Compare g[1]=1 and g_rev[2]=1 -> equal -> NO
    assert(decideGame(2, 1, 1, 2, 0, {{1,2}}) == "NO");
    // Test 3: n=2, one edge 1->2, a=1,b=1,k=0. g[1]=1, g_rev[1]=0 (since in reversed graph 1 is leaf) -> not equal -> YES
    assert(decideGame(2, 1, 1, 1, 0, {{1,2}}) == "YES");
    // Test 4: n=3, edges: 1->2, 2->3, 3 is leaf. k=1 so parity=1. Leaf 3 gets 1, node2 mex{1}=0, node1 mex{0}=1. Reversed graph: edges 2->1,3->2, leaf1 gets1, node2 mex{1}=0, node3 mex{0}=1. a=1,b=3: g[1]=1, g_rev[3]=1 -> equal -> NO
    assert(decideGame(3, 2, 1, 3, 1, {{1,2},{2,3}}) == "NO");
    // Test 5: n=2, cycle 1<->2 (two edges). Both nodes have outdegree>0, never processed -> g all 0, same for reversed -> both 0 -> equal -> NO
    assert(decideGame(2, 2, 1, 2, 0, {{1,2},{2,1}}) == "NO");
    // Test 6: n=2, cycle, a=1,b=1 -> g[1]=0, g_rev[1]=0 -> NO
    assert(decideGame(2, 2, 1, 1, 0, {{1,2},{2,1}}) == "NO");
    // Test 7: n=0 -> NO
    assert(decideGame(0, 0, 0, 0, 0, {}) == "NO");
    // Test 8: n=3, edges 1->2, 1->3, 2 and 3 leaves. k=0 parity=0. g[2]=g[3]=0, g[1]=mex{0}=1. Reversed graph: edges 2->1,3->1, leaves 2,3 get0, node1 mex{0}=1. a=1,b=2: g[1]=1, g_rev[2]=0 -> YES
    assert(decideGame(3, 2, 1, 2, 0, {{1,2},{1,3}}) == "YES");
    // Test 9: Self-loop on node 1: outdegree=1 but the only neighbor is itself, never processed -> g=0. Reversed same -> a=1,b=1 -> equal -> NO
    assert(decideGame(2, 1, 1, 1, 5, {{1,1}}) == "NO");
    // Test 10: n=2, edge 1->2, k=1 parity=1. Leaf 2 gets1, node1 mex{1}=0. Reversed: leaf1 gets1, node2 mex{1}=0. a=1,b=2: g[1]=0, g_rev[2]=0 -> equal -> NO
    assert(decideGame(2, 1, 1, 2, 1, {{1,2}}) == "NO");

    return 0;
}
