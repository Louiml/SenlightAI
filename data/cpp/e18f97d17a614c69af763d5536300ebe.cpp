Write a C++ function `findMaxPathInDAG` that takes as input the number of vertices `n`, a vector of directed edges `edges` (each edge is a pair of integers from 0 to n-1), and a modulus `mod`. The function should return a pair of integers `(maxPathLength, pathCount)`, where `maxPathLength` is the maximum number of vertices that can be visited along any directed path in the graph (considering vertex counts, not edge weights), and `pathCount` is the number of distinct paths (counting different start/end nodes and intermediate vertices) that achieve this maximum length, modulo `mod`. Since the graph may contain cycles, first condense the graph into its strongly connected components (SCCs). Each SCC becomes a single node with a weight equal to the number of original vertices in it. Then, on the resulting directed acyclic graph (DAG) of SCCs, compute the longest path in terms of total vertex count, and count how many distinct paths attain that longest length. The count should be taken modulo `mod` at every addition. If the graph is empty (no vertices), return `(0, 1)`. Assume the input graph is directed and may have parallel edges and self-loops.

// The problem can be solved using Tarjan's strongly connected components algorithm to condense the graph. First, build adjacency lists for the original graph. Run Tarjan's to assign each vertex an SCC ID and compute the size of each SCC (number of original vertices). Then, construct a new graph where nodes are SCCs, and for every original edge `(u, v)` with `col[u] != col[v]`, add an edge from `col[u]` to `col[v]` in the DAG, but ensure no duplicate edges (use a set or map of pairs). Compute indegrees for each SCC node. Perform a topological sort (Kahn's algorithm) on the DAG. For each SCC node, maintain `dp[i]` = maximum total vertex count of a path ending at that SCC, and `cont[i]` = number of such paths modulo `mod`. Initialize for all nodes with indegree zero: `dp[i] = sz[i]`, `cont[i] = 1`. When relaxing an edge `u -> v`: if `dp[v] < dp[u] + sz[v]`, update `dp[v] = dp[u] + sz[v]` and `cont[v] = cont[u]`; if equal, add `cont[v] = (cont[v] + cont[u]) % mod`. After processing all nodes, find the maximum `dp` value across all SCCs and sum the corresponding `cont` values modulo `mod`. Edge cases: empty graph (return `(0,1)`), cycles (handled by SCC condensation), multiple paths with same length (must sum counts), and modulus possibly being 1 (then counts become 0). Time complexity is O(V + E) for Tarjan and topological processing, with an additional O(E) for duplicate edge check (using a map or unordered_set). Space complexity is O(V + E).

#include <vector>
#include <map>
#include <queue>
#include <utility>

// Find the maximum path length (vertex count) and number of such paths in a directed graph.
// n: number of vertices (0 .. n-1)
// edges: list of directed edges represented as (from, to)
// mod: modulus for counting paths
// Returns pair (maxPathLength, pathCount) where pathCount is modulo mod.
std::pair<int, int> findMaxPathInDAG(int n, const std::vector<std::pair<int,int>>& edges, int mod) {
    if (n == 0) {
        return {0, 1}; // empty graph: one empty path (length 0)
    }

    // Build adjacency for Tarjan
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
    }

    // Tarjan's SCC
    std::vector<int> dfn(n, 0), low(n, 0);
    std::vector<int> st;
    std::vector<bool> inStack(n, false);
    std::vector<int> col(n, -1);
    std::vector<int> sz;
    int timeStamp = 0, compCnt = 0;

    // Lambda for Tarjan DFS
    std::function<void(int)> tarjan = [&](int u) {
        dfn[u] = low[u] = ++timeStamp;
        st.push_back(u);
        inStack[u] = true;
        for (int v : adj[u]) {
            if (dfn[v] == 0) {
                tarjan(v);
                low[u] = std::min(low[u], low[v]);
            } else if (inStack[v]) {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
        if (low[u] == dfn[u]) {
            int countSize = 0;
            while (true) {
                int v = st.back();
                st.pop_back();
                inStack[v] = false;
                col[v] = compCnt;
                countSize++;
                if (v == u) break;
            }
            sz.push_back(countSize);
            compCnt++;
        }
    };

    for (int i = 0; i < n; ++i) {
        if (dfn[i] == 0) {
            tarjan(i);
        }
    }

    // Build DAG edges (with deduplication)
    std::vector<std::vector<int>> dag(compCnt);
    std::vector<int> indeg(compCnt, 0);
    std::map<std::pair<int,int>, bool> seen;

    for (int u = 0; u < n; ++u) {
        for (int v : adj[u]) {
            int cu = col[u], cv = col[v];
            if (cu != cv && !seen[{cu, cv}]) {
                seen[{cu, cv}] = true;
                dag[cu].push_back(cv);
                indeg[cv]++;
            }
        }
    }

    // Topological order (Kahn)
    std::queue<int> q;
    std::vector<int> dp(compCnt, 0);      // max path length ending at this component
    std::vector<int> cont(compCnt, 0);    // number of such paths modulo mod

    for (int i = 0; i < compCnt; ++i) {
        if (indeg[i] == 0) {
            q.push(i);
            dp[i] = sz[i];
            cont[i] = 1 % mod;
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : dag[u]) {
            // If we can get a longer path through u
            if (dp[v] < dp[u] + sz[v]) {
                dp[v] = dp[u] + sz[v];
                cont[v] = cont[u];
            } else if (dp[v] == dp[u] + sz[v]) {
                cont[v] = (cont[v] + cont[u]) % mod;
            }
            indeg[v]--;
            if (indeg[v] == 0) {
                q.push(v);
                // dp[v] is already set appropriately (if not, it would be updated later)
            }
        }
    }

    // Find maximum dp and sum corresponding cont
    int maxLen = 0;
    int maxCount = 0;
    for (int i = 0; i < compCnt; ++i) {
        if (dp[i] > maxLen) {
            maxLen = dp[i];
            maxCount = cont[i] % mod;
        } else if (dp[i] == maxLen) {
            maxCount = (maxCount + cont[i]) % mod;
        }
    }

    return {maxLen, maxCount};
}

#include <cassert>
#include <vector>
#include <utility>

// Function declaration (from solution above)
std::pair<int,int> findMaxPathInDAG(int n, const std::vector<std::pair<int,int>>& edges, int mod);

int main() {
    // Test 1: Simple chain 0->1->2
    assert(findMaxPathInDAG(3, {{0,1},{1,2}}, 100) == std::make_pair(3, 1));

    // Test 2: Two parallel longest paths of length 2 vertices each
    // 0->1 and 0->2 (both length 2)
    assert(findMaxPathInDAG(3, {{0,1},{0,2}}, 100) == std::make_pair(2, 2));

    // Test 3: Cycle 0<->1, becomes one SCC of size 2, no edges left
    assert(findMaxPathInDAG(2, {{0,1},{1,0}}, 100) == std::make_pair(2, 1));

    // Test 4: Self-loop and edge, SCC size 2 and then to 2
    // 0->0, 0->1, 1->2: SCC {0,1} size 2, then 2 size 1 => path length 3
    assert(findMaxPathInDAG(3, {{0,0},{0,1},{1,2}}, 100) == std::make_pair(3, 1));

    // Test 5: Two ways to reach longest path: 0->2 and 1->2, but 0 and 1 both length 2, path length 3? Actually 0->2 gives length 2, 1->2 length 2, max=2 with count 2
    assert(findMaxPathInDAG(3, {{0,2},{1,2}}, 100) == std::make_pair(2, 2));

    // Test 6: Complex: 0->1, 0->2, 1->3, 2->3 => longest path length 3 (0-1-3 and 0-2-3) count 2
    assert(findMaxPathInDAG(4, {{0,1},{0,2},{1,3},{2,3}}, 100) == std::make_pair(3, 2));

    // Test 7: Empty graph
    assert(findMaxPathInDAG(0, {}, 10) == std::make_pair(0, 1));

    // Test 8: Single vertex no edges
    assert(findMaxPathInDAG(1, {}, 7) == std::make_pair(1, 1));

    // Test 9: Mod reduction: two paths of length 3, mod 2 => count 0
    assert(findMaxPathInDAG(4, {{0,1},{0,2},{1,3},{2,3}}, 2) == std::make_pair(3, 0));
}
