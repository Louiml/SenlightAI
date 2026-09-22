// Write a C++ function that takes an undirected graph represented by an integer `n` (number of vertices, labeled 1 through n) and a vector of integer pairs `edges` (each pair representing an undirected edge), and returns a `std::set<int>` containing all articulation points (also known as cut vertices) of the graph. An articulation point is a vertex whose removal increases the number of connected components in the graph. The graph may be disconnected, contain self-loops, or contain parallel edges; the function must handle these correctly. The vertices are 1-indexed, and the graph can have up to \(10^5\) vertices and \(2 \times 10^5\) edges.
// The solution uses Tarjan’s algorithm for finding articulation points via Depth-First Search (DFS). Each vertex keeps two discovery times: `dfs_num` (the order in which it is first visited) and `dfs_low` (the smallest discovery time reachable from the vertex using at most one back edge). For a vertex `u` with child `v` in the DFS tree, `u` is an articulation point if `dfs_low[v] >= dfs_num[u]` and `u` is not the root. For the root of a DFS tree, it is an articulation point if it has more than one child in the DFS tree. Self-loops are ignored because they do not affect connectivity; parallel edges are handled naturally because the DFS will still correctly compute low-link values, and the parent check only skips the immediate parent edge (so parallel edges to the parent are still considered as back edges, but this does not incorrectly mark the parent as an articulation point because the condition uses `>=` and the parent's discovery time is strict). The graph may be disconnected, so we run DFS from every unvisited vertex. The algorithm runs in \(O(V + E)\) time and uses \(O(V + E)\) space for the adjacency list, plus \(O(V)\) for the auxiliary arrays, where `V = n` and `E = edges.size()`.
#include <vector>
#include <set>
#include <algorithm>

/**
 * Finds all articulation points in an undirected graph.
 *
 * @param n Number of vertices (1-indexed, vertices 1..n).
 * @param edges Vector of undirected edges as pairs (u, v).
 * @return A std::set<int> containing all articulation points.
 */
std::set<int> findArticulationPoints(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list.
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        // Ignore self-loops; they do not affect articulation.
        if (u == v) continue;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<int> dfs_num(n + 1, 0);
    std::vector<int> dfs_low(n + 1, 0);
    int timer = 0;
    std::set<int> articulation_points;

    // Tarjan DFS.
    std::function<void(int, int)> tarjan = [&](int node, int parent) {
        dfs_num[node] = dfs_low[node] = ++timer;
        int children = 0;
        for (int child : adj[node]) {
            if (child == parent) continue; // Skip direct parent edge.
            if (dfs_num[child] == 0) {
                // Tree edge.
                tarjan(child, node);
                dfs_low[node] = std::min(dfs_low[node], dfs_low[child]);
                if (parent != -1 && dfs_low[child] >= dfs_num[node]) {
                    articulation_points.insert(node);
                }
                ++children;
            } else {
                // Back edge (or parallel edge to parent already skipped).
                dfs_low[node] = std::min(dfs_low[node], dfs_num[child]);
            }
        }
        // Root of the DFS tree is articulation if it has more than one child.
        if (parent == -1 && children > 1) {
            articulation_points.insert(node);
        }
    };

    // Run DFS for every connected component.
    for (int i = 1; i <= n; ++i) {
        if (dfs_num[i] == 0) {
            tarjan(i, -1);
        }
    }

    return articulation_points;
}
#include <cassert>
#include <set>
#include <vector>
#include <utility>

int main() {
    // Simple triangle: no articulation points.
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
        std::set<int> result = findArticulationPoints(3, edges);
        assert(result.empty());
    }

    // Star graph with center 1: center is articulation.
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{1,4}};
        std::set<int> result = findArticulationPoints(4, edges);
        assert(result == std::set<int>({1}));
    }

    // Line graph 1-2-3-4: vertices 2 and 3 are articulation points.
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        std::set<int> result = findArticulationPoints(4, edges);
        assert(result == std::set<int>({2,3}));
    }

    // Disconnected graph: two separate edges, no articulation points.
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{3,4}};
        std::set<int> result = findArticulationPoints(4, edges);
        assert(result.empty());
    }

    // Single isolated vertex: no articulation points.
    {
        std::vector<std::pair<int,int>> edges;
        std::set<int> result = findArticulationPoints(1, edges);
        assert(result.empty());
    }

    // Self-loop and parallel edges should not affect.
    {
        std::vector<std::pair<int,int>> edges = {{1,1},{1,2},{1,2},{2,3}};
        std::set<int> result = findArticulationPoints(3, edges);
        // Vertex 1 is articulation (removal disconnects 2-3 from nothing? Actually 2-3 remains connected, so 1 is articulation because the graph becomes {2,3} and {no 1}).
        assert(result == std::set<int>({1,2})); // Wait: removing 1 leaves {2,3} connected, so 1 is articulation. Removing 2 leaves {1,3} disconnected? {1} and {3} are disconnected, so 2 is articulation. So {1,2}.
    }

    // Two-node graph with single edge: neither is articulation (graph becomes two isolated vertices, but since each is alone, it's still connected? Removing 1 leaves {2} which is connected, so not articulation).
    {
        std::vector<std::pair<int,int>> edges = {{1,2}};
        std::set<int> result = findArticulationPoints(2, edges);
        assert(result.empty());
    }

    // Larger cycle with chord: no articulation points.
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5},{5,1},{1,3}};
        std::set<int> result = findArticulationPoints(5, edges);
        assert(result.empty());
    }

    // Root with one child is not articulation.
    {
        std::vector<std::pair<int,int>> edges = {{1,2}};
        std::set<int> result = findArticulationPoints(2, edges);
        assert(result.empty());
    }

    // Disconnected with one component a star and another a single vertex.
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{4}}; // edge {4} invalid? Use {4,5}? Let's do n=5: {1,2},{1,3},{4,5}
        std::vector<std::pair<int,int>> edges2 = {{1,2},{1,3},{4,5}};
        std::set<int> result = findArticulationPoints(5, edges2);
        assert(result == std::set<int>({1}));
    }

    return 0;
}
