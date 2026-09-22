// Write a C++ function `int treeDistance(int n, const std::vector<std::pair<int,int>>& edges, int start, int target)` that builds an undirected tree (or connected graph, but assume the input always forms a tree for this exercise) from `n` vertices labeled `1` to `n` and `edges` containing `n-1` undirected edges. The function should compute the shortest path distance (i.e., the number of edges) from vertex `start` to vertex `target` using Depth-First Search (DFS) on the adjacency list. If start or target are invalid (outside 1..n), return -1. If they are the same vertex, return 0. The graph is guaranteed to be connected and acyclic, so DFS will visit all vertices exactly once.
The core algorithm is a standard DFS traversal of the graph represented by an adjacency list. We maintain a `depth` array where `depth[u]` stores the distance from the `start` vertex to `u`. We initialize `depth[start]=0` and set `visited[start]=true`. Then recursively visit each neighbor `v` of `u`; if `v` is not visited, set `depth[v]=depth[u]+1` and recurse. Since the graph is a tree, there is exactly one simple path between any two vertices, so the depth of `target` after the full DFS equals the shortest path distance. Edge cases: invalid vertex labels (outside 1..n) should return -1. If `start==target`, immediate return 0. The input may be empty (n=0) or n=1 with no edges, in which case if start==target return 0, else -1. The DFS must be iterative or recursive but must avoid stack overflow for large n (though we can assume n ≤ 10^5). Time complexity is O(n + m) = O(n) for a tree, and space complexity is O(n) for adjacency list and visited/depth arrays.
#include <vector>
#include <functional>
#include <algorithm>

// Compute the shortest path distance between start and target in an undirected tree.
// Returns -1 if either vertex is out of range [1, n].
int treeDistance(int n, const std::vector<std::pair<int,int>>& edges, int start, int target) {
    // Validate vertex labels.
    if (start < 1 || start > n || target < 1 || target > n) {
        return -1;
    }
    if (start == target) {
        return 0;
    }
    if (n == 0) {
        return -1;
    }

    // Build adjacency list.
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        // Assume valid edge endpoints, but guard if not.
        if (u < 1 || u > n || v < 1 || v > n) {
            continue; // ignore invalid edges (optional robustness)
        }
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<bool> visited(n + 1, false);
    std::vector<int> depth(n + 1, 0);

    // DFS from start.
    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                depth[v] = depth[u] + 1;
                dfs(v);
            }
        }
    };

    dfs(start);

    // If target was visited, return its depth; else (should not happen in connected tree) -1.
    return visited[target] ? depth[target] : -1;
}
#include <cassert>
#include <vector>
#include <utility>

int treeDistance(int, const std::vector<std::pair<int,int>>&, int, int); // declaration

int main() {
    // Build a small tree: n=5, edges: (1,2), (2,3), (2,4), (4,5)
    std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{2,4},{4,5}};
    assert(treeDistance(5, edges, 1, 1) == 0);
    assert(treeDistance(5, edges, 1, 2) == 1);
    assert(treeDistance(5, edges, 1, 3) == 2);
    assert(treeDistance(5, edges, 3, 5) == 3);
    assert(treeDistance(5, edges, 4, 5) == 1);
    assert(treeDistance(5, edges, 5, 5) == 0);

    // Invalid start or target
    assert(treeDistance(5, edges, 0, 2) == -1);
    assert(treeDistance(5, edges, 1, 6) == -1);

    // Single vertex tree with no edges
    std::vector<std::pair<int,int>> no_edges;
    assert(treeDistance(1, no_edges, 1, 1) == 0);
    assert(treeDistance(1, no_edges, 1, 2) == -1);

    // Two-vertex tree
    std::vector<std::pair<int,int>> two_edges = {{1,2}};
    assert(treeDistance(2, two_edges, 1, 2) == 1);
    assert(treeDistance(2, two_edges, 2, 1) == 1);
}
