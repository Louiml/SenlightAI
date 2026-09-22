/*
Implement a C++ function that, given an undirected unweighted graph represented as a number of vertices `N` and a list of `M` edges, finds the lowest common ancestor (LCA) of two specified vertices `v1` and `v2` in a spanning tree of the graph. The graph is guaranteed to be connected. The function should take as parameters the number of vertices `N`, the number of edges `M`, a vector of edge pairs, and the two query vertices `v1` and `v2`. It must build the graph, perform a depth-first search (DFS) from vertex `0` to construct a spanning tree (recording parent and depth for each vertex), and then find the LCA by bringing the two vertices to the same depth and moving both upward simultaneously. Return the LCA vertex number.
*/

#include <vector>
#include <queue>
#include <algorithm>

/**
 * Finds the lowest common ancestor (LCA) of two vertices in a spanning tree
 * of a connected undirected graph.
 *
 * @param N Number of vertices (0..N-1)
 * @param M Number of edges
 * @param edges Vector of pairs (u, v) representing undirected edges
 * @param v1 First query vertex
 * @param v2 Second query vertex
 * @return The LCA vertex number
 */
int lowestCommonAncestor(int N, int M, const std::vector<std::pair<int,int>>& edges, int v1, int v2) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(N);
    for (const auto& edge : edges) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }

    // Parent and depth arrays for spanning tree via DFS from 0
    std::vector<int> parent(N, -1);
    std::vector<int> depth(N, -1);

    // DFS using explicit stack to avoid recursion depth issues
    std::vector<int> stack;
    stack.push_back(0);
    depth[0] = 0;
    parent[0] = 0; // root's parent is itself for convenience

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        for (int v : adj[u]) {
            if (depth[v] == -1) {
                depth[v] = depth[u] + 1;
                parent[v] = u;
                stack.push_back(v);
            }
        }
    }

    // Bring v1 and v2 to same depth
    while (depth[v1] > depth[v2]) {
        v1 = parent[v1];
    }
    while (depth[v2] > depth[v1]) {
        v2 = parent[v2];
    }

    // Move both up until they meet
    while (v1 != v2) {
        v1 = parent[v1];
        v2 = parent[v2];
    }

    return v1;
}

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or link it)
int lowestCommonAncestor(int, int, const std::vector<std::pair<int,int>>&, int, int);

int main() {
    // Test 1: Simple chain 0-1-2-3
    std::vector<std::pair<int,int>> edges1 = {{0,1},{1,2},{2,3}};
    assert(lowestCommonAncestor(4, 3, edges1, 1, 3) == 1); // LCA of 1 and 3 is 1
    assert(lowestCommonAncestor(4, 3, edges1, 2, 3) == 2); // LCA of 2 and 3 is 2
    assert(lowestCommonAncestor(4, 3, edges1, 0, 3) == 0); // Root is ancestor

    // Test 2: Star graph, center 0 connected to 1,2,3
    std::vector<std::pair<int,int>> edges2 = {{0,1},{0,2},{0,3}};
    assert(lowestCommonAncestor(4, 3, edges2, 1, 2) == 0); // LCA through center
    assert(lowestCommonAncestor(4, 3, edges2, 2, 2) == 2); // Same vertex

    // Test 3: Graph with extra edge creating a cycle (0-1-2-0 plus 2-3)
    std::vector<std::pair<int,int>> edges3 = {{0,1},{1,2},{0,2},{2,3}};
    // DFS from 0 might produce tree 0-1-2-3 (or 0-2-3, but parent assignment is deterministic via adjacency order)
    // Verify that LCA is correctly found for any valid tree; here we check basic cases
    assert(lowestCommonAncestor(4, 4, edges3, 1, 3) >= 0); // Should not crash and return a valid vertex
    int lca = lowestCommonAncestor(4, 4, edges3, 1, 3);
    // The LCA must be on the path between 1 and 3 in the spanning tree; since root is 0, either 0 or 1 or 2 is valid.
    assert(lca == 0 || lca == 1 || lca == 2);

    // Test 4: Larger tree-like graph
    std::vector<std::pair<int,int>> edges4 = {{0,1},{0,2},{1,3},{1,4},{2,5},{2,6}};
    assert(lowestCommonAncestor(7, 6, edges4, 3, 4) == 1);
    assert(lowestCommonAncestor(7, 6, edges4, 3, 5) == 0);
    assert(lowestCommonAncestor(7, 6, edges4, 5, 6) == 2);

    // Test 5: Disconnected? But problem says connected, so no need.

    return 0;
}

// The problem requires constructing a tree from the connected graph via DFS starting at vertex `0`. During DFS, we assign each visited vertex a parent (initially -1) and a depth (initially -1). We only traverse edges to unvisited vertices, effectively creating a spanning tree. The root (vertex 0) has depth 0 and parent -1 (or itself). After DFS, both target vertices are guaranteed to be visited because the graph is connected. To find the LCA, we compare depths: if one is deeper, we move it up to its parent until both depths are equal. Then we alternately move both vertices up until they meet. Edge cases: `v1 == v2` returns that vertex immediately; root as one of the vertices works because its parent is itself (or we stop when depths equal and then check equality). Time complexity is O(N + M) for building and DFS, plus O(depth) for LCA, which in worst case is O(N). Space complexity is O(N^2) if using an adjacency matrix, but we can optimize to O(N + M) using an adjacency list. However, the original snippet used a matrix; we'll use adjacency list for efficiency. The solution must handle vertices numbered from 0 to N-1.
