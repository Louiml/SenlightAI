/*
Write a C++ function `bool isCactusLike(int n, const std::vector<std::pair<int,int>>& edges)` that determines whether a directed graph with vertices numbered `0` to `n-1` satisfies two conditions: (1) the graph is strongly connected (there is exactly one strongly connected component containing all vertices), and (2) every edge belongs to exactly one cycle in the graph. The graph is simple (no self-loops or multi-edges) and provided as a list of directed edges. The function should return `true` if both conditions hold, otherwise `false`. You may assume the graph is weakly connected (the underlying undirected graph is connected) to simplify input handling.
*/

#include <vector>
#include <cstring>

// Determine if a directed graph is strongly connected and every edge belongs to exactly one cycle.
bool isCactusLike(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
    }

    // Tarjan's algorithm state
    std::vector<int> dfn(n, 0), low(n, 0);
    std::vector<bool> inStack(n, false);
    int timer = 0;
    int sccCount = 0;
    bool ok = true;
    std::vector<int> stk;

    // DFS from a vertex
    std::function<void(int)> dfs = [&](int u) {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        inStack[u] = true;
        int exitCount = 0; // number of distinct cycle exits from u

        for (int v : adj[u]) {
            if (!ok) return;
            if (!dfn[v]) {
                dfs(v);
                if (low[v] < dfn[u]) {
                    // subtree of v has a back edge reaching above u
                    exitCount++;
                    if (exitCount >= 2) { ok = false; return; }
                }
                low[u] = std::min(low[u], low[v]);
            } else if (inStack[v]) {
                // back edge to ancestor v
                low[u] = std::min(low[u], dfn[v]);
                exitCount++;
                if (exitCount >= 2) { ok = false; return; }
                // If v is not itself an SCC root (already part of a cycle), then this back edge creates a non-cactus structure
                if (dfn[v] != low[v]) { ok = false; return; }
            }
        }

        if (low[u] == dfn[u]) {
            // root of an SCC
            sccCount++;
            while (true) {
                int x = stk.back();
                stk.pop_back();
                inStack[x] = false;
                if (x == u) break;
            }
        }
    };

    dfs(0);

    // Must be exactly one SCC and no violation found
    return ok && sccCount == 1;
}

#include <cassert>
#include <vector>
#include <utility>

bool isCactusLike(int n, const std::vector<std::pair<int,int>>& edges); // declaration from solution

int main() {
    // Single vertex, no edges: trivially strongly connected and no cycles -> true
    assert(isCactusLike(1, {}));

    // Simple cycle 0->1->2->0: each edge on exactly one cycle -> true
    assert(isCactusLike(3, {{0,1},{1,2},{2,0}}));

    // Two cycles sharing an edge: 0->1,1->2,2->0,2->3,3->0. Edge 2->0 is on two cycles -> false
    assert(!isCactusLike(4, {{0,1},{1,2},{2,0},{2,3},{3,0}}));

    // Strongly connected but a chord creates multiple cycles: 0->1,1->2,2->0,0->2. Edge 0->2 creates second cycle -> false
    assert(!isCactusLike(3, {{0,1},{1,2},{2,0},{0,2}}));

    // Not strongly connected: 0->1,1->2 -> false
    assert(!isCactusLike(3, {{0,1},{1,2}}));

    // Two separate cycles with a connecting edge? Not strongly connected: 0->1,1->0,2->3,3->2,0->2 -> false
    assert(!isCactusLike(4, {{0,1},{1,0},{2,3},{3,2},{0,2}}));

    // A single cycle with an extra tail (not strongly connected): 0->1,1->2,2->0,0->3 -> false
    assert(!isCactusLike(4, {{0,1},{1,2},{2,0},{0,3}}));

    // A "figure-eight" with two cycles sharing a vertex (not an edge): 0->1,1->2,2->0,0->3,3->4,4->0 -> true? Actually each edge is on exactly one cycle and strongly connected -> true
    assert(isCactusLike(5, {{0,1},{1,2},{2,0},{0,3},{3,4},{4,0}}));

    // Self-loop (not allowed by simple graph but test if function fails gracefully) -> false
    assert(!isCactusLike(1, {{0,0}}));

    // Graph with 2 vertices and two reversed edges forming a 2-cycle: strongly connected, each edge on exactly one cycle -> true
    assert(isCactusLike(2, {{0,1},{1,0}}));
}

// The problem is a variant of identifying "cactus" directed graphs, where each edge lies on exactly one directed cycle. The key is to use Tarjan's algorithm for strongly connected components (SCCs), but we need extra logic to ensure each edge is on exactly one cycle. The standard Tarjan DFS maintains `dfn` (discovery time) and `low` (lowest reachable ancestor). For each back edge (u→v where v is currently on the stack), we increment a counter for u, since u has a back edge to an ancestor. For each tree edge (u→v where v is a child), after recursing, we check if `low[v] < dfn[u]` — this means v's subtree contains a back edge that reaches above u, so u is part of a cycle that goes through that subtree. The number of distinct "cycle-exit" paths from u (back edges or subtree low values reaching above u) must be at most 1; if it's 2 or more, then u would be on more than one cycle, violating the cactus property. Also, for a back edge to a vertex v that is already part of an SCC (i.e., `dfn[v] != low[v]`), it means that back edge creates an additional cycle not consistent with cactus. After the DFS, we must ensure there is exactly one SCC (i.e., the graph is strongly connected). The algorithm runs in O(V+E) time and O(V) auxiliary stack space for the DFS, plus O(E) for edge storage. Edge cases: a single vertex with no edges is trivially strongly connected and has no cycles, so it satisfies the condition (every edge trivially belongs to exactly one cycle vacuously). A graph with multiple edges or self-loops is not a cactus, but the input is guaranteed simple.
