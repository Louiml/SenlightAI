Write a C++ function `longestPathInDAG` that takes the number of vertices `V`, an adjacency list `adj` for a directed acyclic graph (DAG) where each edge `(u, v)` has a positive integer weight, and a source vertex `s`. The function must return a `std::vector<int>` where the element at index `i` is the length (sum of edge weights) of the *longest* path from `s` to vertex `i`. If a vertex is unreachable from `s`, store `INT_MAX` for that vertex. The graph is guaranteed to be acyclic, but may be disconnected. The function must handle edges weights up to `10^4` and `V` up to `10^5` efficiently using topological ordering.
#include <vector>
#include <stack>
#include <limits.h>
#include <cassert>

// Forward declaration to keep test self-contained (or include the solution header)
std::vector<int> longestPathInDAG(int V, const std::vector<std::pair<int, int>> adj[], int s);

// Simple helper to add an edge (for test only)
void addEdge(std::vector<std::pair<int, int>> adj[], int u, int v, int wt) {
    adj[u].push_back({v, wt});
}

int main() {
    // Test 1: Simple linear graph 0->1->2, weights 5 and 3
    int V1 = 3;
    std::vector<std::pair<int, int>> adj1[V1];
    addEdge(adj1, 0, 1, 5);
    addEdge(adj1, 1, 2, 3);
    std::vector<int> dist1 = longestPathInDAG(V1, adj1, 0);
    assert(dist1[0] == 0);
    assert(dist1[1] == 5);
    assert(dist1[2] == 8);

    // Test 2: Branching graph (same as snippet but for longest)
    int V2 = 6;
    std::vector<std::pair<int, int>> adj2[V2];
    addEdge(adj2, 0, 1, 2);
    addEdge(adj2, 0, 4, 1);
    addEdge(adj2, 1, 2, 3);
    addEdge(adj2, 2, 3, 6);
    addEdge(adj2, 4, 2, 2);
    addEdge(adj2, 4, 5, 4);
    addEdge(adj2, 5, 3, 1);
    std::vector<int> dist2 = longestPathInDAG(V2, adj2, 0);
    // Paths to 3: 0->1->2->3 = 2+3+6=11, 0->4->5->3 = 1+4+1=6, 0->4->2->3 = 1+2+6=9 → longest 11
    assert(dist2[0] == 0);
    assert(dist2[1] == 2);
    assert(dist2[2] == 5); // longest to 2: 0->1->2=5 vs 0->4->2=3
    assert(dist2[3] == 11);
    assert(dist2[4] == 1);
    assert(dist2[5] == 5); // only 0->4->5=5

    // Test 3: Source not reachable to some vertices (disconnected)
    int V3 = 4;
    std::vector<std::pair<int, int>> adj3[V3];
    addEdge(adj3, 0, 1, 2);
    addEdge(adj3, 2, 3, 7); // component with 2 and 3, but no edge from 0
    std::vector<int> dist3 = longestPathInDAG(V3, adj3, 0);
    assert(dist3[0] == 0);
    assert(dist3[1] == 2);
    assert(dist3[2] == INT_MAX);
    assert(dist3[3] == INT_MAX);

    // Test 4: Multiple edges to same vertex (choose max)
    int V4 = 3;
    std::vector<std::pair<int, int>> adj4[V4];
    addEdge(adj4, 0, 2, 4);
    addEdge(adj4, 1, 2, 3);
    addEdge(adj4, 0, 1, 1);
    std::vector<int> dist4 = longestPathInDAG(V4, adj4, 0);
    // 0->1->2 = 1+3=4, 0->2 = 4, both equal
    assert(dist4[0] == 0);
    assert(dist4[1] == 1);
    assert(dist4[2] == 4);

    // Test 5: Single vertex
    int V5 = 1;
    std::vector<std::pair<int, int>> adj5[V5];
    std::vector<int> dist5 = longestPathInDAG(V5, adj5, 0);
    assert(dist5[0] == 0);

    return 0;
}
#include <vector>
#include <stack>
#include <limits.h>

// Perform DFS-based topological sort, pushing vertices onto stack after visiting all neighbors.
void topoSortUtil(int v, const std::vector<std::pair<int, int>> adj[], std::vector<bool>& visited, std::stack<int>& st) {
    visited[v] = true;
    for (const auto& edge : adj[v]) {
        int neighbor = edge.first;
        if (!visited[neighbor]) {
            topoSortUtil(neighbor, adj, visited, st);
        }
    }
    st.push(v);
}

// Return longest path distances from source s in a DAG. Unreachable vertices get INT_MAX.
std::vector<int> longestPathInDAG(int V, const std::vector<std::pair<int, int>> adj[], int s) {
    std::vector<bool> visited(V, false);
    std::stack<int> st;

    // Topological sort for all vertices (handles disconnected graph)
    for (int i = 0; i < V; ++i) {
        if (!visited[i]) {
            topoSortUtil(i, adj, visited, st);
        }
    }

    // Initialize distances: source is 0, others INT_MAX (unreachable)
    std::vector<int> dist(V, INT_MAX);
    dist[s] = 0;

    // Process vertices in topological order
    while (!st.empty()) {
        int u = st.top();
        st.pop();

        // Only relax outgoing edges if u is reachable
        if (dist[u] != INT_MAX) {
            for (const auto& edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (dist[u] + weight > dist[v]) {
                    dist[v] = dist[u] + weight;
                }
            }
        }
    }

    return dist;
}
// The problem asks for the longest path from a single source in a DAG. Since the graph is acyclic, we can process vertices in topological order. For each vertex `u` in that order, if `u` is reachable (distance not `INT_MAX`), we relax all outgoing edges: for each edge `(u, v, w)`, we update `dist[v] = max(dist[v], dist[u] + w)`. This works because when we process `u`, all its predecessors have already been processed, so `dist[u]` is final. Initialize all distances to `INT_MAX` except `dist[s] = 0`. Unreachable vertices remain `INT_MAX`. Edge cases: (1) if the source has no outgoing edges, only `dist[s]` is 0; (2) a disconnected component with no path from `s` stays `INT_MAX`. Time complexity is `O(V + E)` for topological sort (DFS-based) plus `O(V + E)` for relaxation, totaling `O(V + E)`. Space complexity is `O(V)` for stack, visited array, and distance vector. Note that weights are positive, but the algorithm works for any weights since it’s a DAG; however, if negative weights existed, the longest path would be infinite if a cycle exists, but the problem guarantees acyclic. Here, we simply take max instead of min.
