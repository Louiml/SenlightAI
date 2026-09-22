/*
Implement a C++ function `std::vector<int> findArticulationPoints(const std::vector<std::vector<int>>& adjacencyList)` that takes an undirected graph represented by an adjacency list (vertices numbered 0 to n-1) and returns a sorted vector containing all articulation points (also called cut vertices). An articulation point is a vertex whose removal increases the number of connected components of the graph. The graph may be disconnected, and isolated vertices (with no edges) are not articulation points. The function must handle graphs with up to 10,000 vertices efficiently. The input adjacency list is assumed to be valid, with each edge appearing in both vertices' lists exactly once.
*/
#include <vector>
#include <algorithm>
#include <set>

// Find all articulation points in an undirected graph given by adjacency list.
std::vector<int> findArticulationPoints(const std::vector<std::vector<int>>& adj) {
    int n = static_cast<int>(adj.size());
    std::vector<int> disc(n, -1), low(n, -1), parent(n, -1);
    std::vector<bool> isArt(n, false);
    int time = 0;

    // Recursive helper using lambda for DFS.
    std::function<void(int)> dfs = [&](int u) {
        disc[u] = low[u] = time++;
        int children = 0;
        for (int v : adj[u]) {
            if (disc[v] == -1) { // unvisited → tree edge
                parent[v] = u;
                children++;
                dfs(v);
                low[u] = std::min(low[u], low[v]);
                if (parent[u] == -1 && children > 1)
                    isArt[u] = true;
                if (parent[u] != -1 && low[v] >= disc[u])
                    isArt[u] = true;
            } else if (v != parent[u]) { // back edge (ignore direct parent)
                low[u] = std::min(low[u], disc[v]);
            }
        }
    };

    // Run DFS for all connected components.
    for (int i = 0; i < n; ++i) {
        if (disc[i] == -1) {
            dfs(i);
        }
    }

    std::vector<int> result;
    for (int i = 0; i < n; ++i) {
        if (isArt[i]) result.push_back(i);
    }
    std::sort(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared here.

int main() {
    // Test 1: Simple chain 0-1-2 → articulation point 1
    std::vector<std::vector<int>> g1 = {{1}, {0,2}, {1}};
    assert(findArticulationPoints(g1) == std::vector<int>{1});

    // Test 2: Triangle (cycle) → no articulation points
    std::vector<std::vector<int>> g2 = {{1,2}, {0,2}, {0,1}};
    assert(findArticulationPoints(g2).empty());

    // Test 3: Star with center 0 connected to 1,2,3 → center is articulation
    std::vector<std::vector<int>> g3 = {{1,2,3}, {0}, {0}, {0}};
    assert(findArticulationPoints(g3) == std::vector<int>{0});

    // Test 4: Disconnected graph with a bridge 0-1 and isolated 2
    std::vector<std::vector<int>> g4 = {{1}, {0}, {}};
    assert(findArticulationPoints(g4) == std::vector<int>{0,1});

    // Test 5: Single isolated vertex → no articulation
    std::vector<std::vector<int>> g5 = {{}};
    assert(findArticulationPoints(g5).empty());

    // Test 6: Two vertices with one edge → both are articulation
    std::vector<std::vector<int>> g6 = {{1}, {0}};
    assert(findArticulationPoints(g6) == std::vector<int>{0,1});

    // Test 7: More complex graph: 0-1-2-3-0 (cycle) with an extra edge 0-3, 1-3
    // Actually a complete graph of 4 nodes → no articulation
    std::vector<std::vector<int>> g7 = {{1,2,3}, {0,2,3}, {0,1,3}, {0,1,2}};
    assert(findArticulationPoints(g7).empty());

    // Test 8: Graph with articulation 2 and 3: 0-1-2-3 and 2-4
    std::vector<std::vector<int>> g8 = {{1}, {0,2}, {1,3,4}, {2}, {2}};
    assert(findArticulationPoints(g8) == std::vector<int>{2});

    // Test 9: Self-loop and parallel edge (should be ignored) on vertex 0 connected to 1
    std::vector<std::vector<int>> g9 = {{0,1,1}, {0,0}}; // 0 self-loop, parallel edges
    assert(findArticulationPoints(g9) == std::vector<int>{0,1});

    // Test 10: Larger graph with multiple articulation points
    std::vector<std::vector<int>> g10 = {{1}, {0,2}, {1,3,4}, {2,5}, {2}, {3,6}, {5}};
    // Articulation: 1,2,3,5 (bridges in a chain) but 2 also connects to 4
    // Actually: 0-1-2-3-5-6 and 2-4. Articulation points: 1,2,3,5
    assert(findArticulationPoints(g10) == std::vector<int>({1,2,3,5}));

    return 0;
}
// The solution uses a depth-first search (DFS) based approach with discovery time and low-link values, which is a classic method for finding articulation points in undirected graphs. The algorithm maintains three arrays: `disc[v]` (discovery time), `low[v]` (the lowest discovery time reachable from v via back edges), and `parent[v]` to track the DFS tree. For each vertex `v`, we examine its neighbors `u`. If `u` is undiscovered, we recursively DFS on `u`, then update `low[v] = min(low[v], low[u])`. If `v` is the root of the DFS tree, it is an articulation point if it has more than one child. For non-root vertices, `v` is an articulation point if there exists a child `u` such that `low[u] >= disc[v]`, meaning no back edge from the subtree of `u` connects to an ancestor of `v`. Since the graph may be disconnected, we run DFS from every unvisited vertex, treating each as a separate tree root. Isolated vertices (no edges) have no children and are never marked. We must also avoid counting the same vertex multiple times by using a boolean or set. The time complexity is O(V + E) since each vertex and edge is visited once, and the auxiliary space is O(V) for the arrays. Edge cases include self-loops (which don't affect articulation) and parallel edges (which are harmless).
