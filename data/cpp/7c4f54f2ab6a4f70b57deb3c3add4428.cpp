/*
Write a C++ function `bool hasCycleInDirectedGraph(int n, const std::vector<std::pair<int,int>>& edges)` that determines whether a directed graph with vertices numbered from 1 to `n` contains a cycle. The graph is provided as a list of directed edges, where each edge is a pair `(u, v)` meaning there is a directed edge from vertex `u` to vertex `v`. The graph may be disconnected and may contain self-loops or multiple edges between the same pair of vertices; any self-loop (u==v) automatically indicates a cycle. Return `true` if the graph contains at least one directed cycle, otherwise return `false`. The vertices that do not appear in any edge are still considered part of the graph (i.e., isolated vertices) and must be handled gracefully.
*/
#include <vector>
#include <unordered_map>
#include <list>

// Depth-first search to detect cycle in a directed graph.
// Returns true if a cycle is found starting from 'node'.
bool dfsCycleCheck(int node,
                   std::unordered_map<int, bool>& visited,
                   std::unordered_map<int, bool>& dfsVisited,
                   const std::unordered_map<int, std::list<int>>& adj) {
    visited[node] = true;
    dfsVisited[node] = true;

    for (int neighbor : adj.at(node)) {
        if (!visited[neighbor]) {
            if (dfsCycleCheck(neighbor, visited, dfsVisited, adj)) {
                return true;
            }
        } else if (dfsVisited[neighbor]) {
            // Back edge to a node on the current recursion stack => cycle
            return true;
        }
    }

    dfsVisited[node] = false;
    return false;
}

// Determines if a directed graph with vertices 1..n contains a cycle.
bool hasCycleInDirectedGraph(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list
    std::unordered_map<int, std::list<int>> adj;
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        adj[u].push_back(v);
        // Ensure vertices that have no edges but are in range are represented
        // (Will be handled by the loop over 1..n)
    }

    std::unordered_map<int, bool> visited;
    std::unordered_map<int, bool> dfsVisited;

    // Check for cycle in each connected component
    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            if (dfsCycleCheck(i, visited, dfsVisited, adj)) {
                return true;
            }
        }
    }
    return false;
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Empty graph with isolated vertices: no cycle
    assert(hasCycleInDirectedGraph(3, {}) == false);

    // Simple acyclic chain: 1->2->3
    assert(hasCycleInDirectedGraph(3, {{1,2}, {2,3}}) == false);

    // Simple cycle: 1->2, 2->1
    assert(hasCycleInDirectedGraph(2, {{1,2}, {2,1}}) == true);

    // Self-loop
    assert(hasCycleInDirectedGraph(2, {{1,1}}) == true);

    // Disconnected component with a cycle, one isolated vertex
    assert(hasCycleInDirectedGraph(4, {{1,2}, {2,3}, {3,1}}) == true);

    // Multiple edges between same pair (still acyclic if no back edge)
    assert(hasCycleInDirectedGraph(2, {{1,2}, {1,2}}) == false);

    // Larger graph with a cycle hidden deeper
    assert(hasCycleInDirectedGraph(5, {{1,2}, {2,3}, {3,4}, {4,2}, {4,5}}) == true);

    // DAG with multiple branches
    assert(hasCycleInDirectedGraph(5, {{1,2}, {1,3}, {2,4}, {3,4}, {4,5}}) == false);

    // Graph with no edges but vertices up to 5
    assert(hasCycleInDirectedGraph(5, {}) == false);

    // Single vertex with self-loop
    assert(hasCycleInDirectedGraph(1, {{1,1}}) == true);

    // Single vertex with no edge
    assert(hasCycleInDirectedGraph(1, {}) == false);

    return 0;
}
// The algorithm uses depth-first search (DFS) on each unvisited vertex (covering all connected components). During DFS, maintain two boolean maps: `visited` (marks vertices that have been fully processed or are currently in the recursion stack) and `dfsvisited` (marks vertices that are currently on the recursion stack of the ongoing DFS). When exploring a neighbor of the current node: if the neighbor is not yet visited, recursively check it; if the neighbor is already visited and also marked in `dfsvisited`, then a cycle is detected because we have reached a vertex that is still on the current DFS path. After processing all neighbors of a node, remove it from `dfsvisited` (set to false) to correctly handle branching paths. The process is repeated for every vertex from 1 to `n` that has not been visited, to cover disconnected components. Edge cases include: empty edge list, a single vertex with no edges (no cycle), a self-loop edge (u==v) which will immediately trigger the cycle condition, multiple edges between the same vertices (which do not cause a false positive because they are just duplicate adjacency entries; the cycle detection logic remains correct), and graphs with isolated vertices (which are processed as individual components and correctly return false). Time complexity is O(V + E) because each vertex and each edge is processed once. Space complexity is O(V) for the visited maps, plus O(V + E) for the adjacency list representation.
