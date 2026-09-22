// Write a C++ function `int findDiameterEdge(const std::vector<std::pair<int,int>>& edges, int n)` that, given a tree with `n` vertices (numbered 1 to n) represented by its `n-1` undirected edges, returns the integer edge index (0-based) of any edge that lies on the diameter of the tree. The diameter is the longest path (in terms of number of edges) between any two vertices. You may assume the input is always a valid tree (connected, acyclic, n ≥ 2). The function should determine the diameter and return the index of one edge that is part of at least one diameter path. If multiple edges are on any diameter, any valid one is acceptable. Do not use global variables or interactive querying; implement the algorithm directly.
#include <cassert>
#include <vector>
#include <utility>

// The function under test is declared here (include the solution code above)
int findDiameterEdge(const std::vector<std::pair<int,int>>& edges, int n);

int main() {
    // Test 1: Simple 3-node path 1-2-3, diameter edge can be either (1,2) or (2,3)
    {
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}};
        int idx = findDiameterEdge(edges, 3);
        assert(idx == 0 || idx == 1);
    }

    // Test 2: Star with center 1 and leaves 2,3,4, diameter is between any two leaves
    {
        std::vector<std::pair<int,int>> edges = {{1,2}, {1,3}, {1,4}};
        int idx = findDiameterEdge(edges, 4);
        // Any edge is on a diameter (leaf to center), so all are valid
        assert(idx >= 0 && idx < 3);
    }

    // Test 3: 5-node path 1-2-3-4-5, diameter edges are all edges, return any
    {
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}, {3,4}, {4,5}};
        int idx = findDiameterEdge(edges, 5);
        assert(idx >= 0 && idx < 4);
    }

    // Test 4: Tree with a long path: 1-2, 2-3, 3-4, and branch 3-5. Diameter is 1-2-3-4 (length 3) or 1-2-3-5 (also length 3). Edges (1,2), (2,3) definitely on all diameters.
    {
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}, {3,4}, {3,5}};
        int idx = findDiameterEdge(edges, 5);
        assert(idx == 0 || idx == 1); // (1,2) or (2,3) must be on any diameter
    }

    // Test 5: Two-node tree, the only edge is the diameter
    {
        std::vector<std::pair<int,int>> edges = {{1,2}};
        int idx = findDiameterEdge(edges, 2);
        assert(idx == 0);
    }

    // Test 6: Tree where diameter is not obvious: 1-2, 2-3, 3-4, 2-5, 5-6. Longest path 4-3-2-5-6 length 4, so edges (2,3), (3,4), (2,5), (5,6) on diameter, but (1,2) is not.
    {
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}, {3,4}, {2,5}, {5,6}};
        int idx = findDiameterEdge(edges, 6);
        // Valid indices are 1,2,3,4 (0-based) but not 0 (the edge 1-2)
        assert(idx >= 1 && idx <= 4);
    }

    // Test 7: Symmetric tree: 1-2, 1-3, 2-4, 2-5, 3-6, 3-7. Diameter is between leaves 4 and 6 (or 5 and 7), length 4. Edge (1,2) or (1,3) must be on any diameter.
    {
        std::vector<std::pair<int,int>> edges = {{1,2}, {1,3}, {2,4}, {2,5}, {3,6}, {3,7}};
        int idx = findDiameterEdge(edges, 7);
        assert(idx == 0 || idx == 1); // any diameter includes either (1,2) or (1,3)
    }

    return 0;
}
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>

// Find the index (0-based) of an edge that lies on a diameter of the tree.
// edges: vector of (u, v) pairs, vertices numbered 1..n. Returns an edge index.
int findDiameterEdge(const std::vector<std::pair<int,int>>& edges, int n) {
    // Build adjacency list: for each vertex, store (neighbor, edge_index)
    std::vector<std::vector<std::pair<int,int>>> adj(n + 1);
    for (int i = 0; i < static_cast<int>(edges.size()); ++i) {
        int u = edges[i].first;
        int v = edges[i].second;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    // BFS helper: returns the farthest node from 'start' and fills parent_node/edge
    // parent_node[v] = parent of v in BFS tree (0 if none)
    // parent_edge[v] = edge index used to reach v (0 if none)
    auto bfs = [&](int start, std::vector<int>& parent_node, std::vector<int>& parent_edge) {
        std::vector<int> dist(n + 1, -1);
        std::queue<int> q;
        dist[start] = 0;
        parent_node[start] = 0;
        parent_edge[start] = -1;
        q.push(start);
        int farthest = start;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (dist[u] > dist[farthest]) {
                farthest = u;
            }
            for (const auto& [v, idx] : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent_node[v] = u;
                    parent_edge[v] = idx;
                    q.push(v);
                }
            }
        }
        return farthest;
    };

    // First BFS from node 1 to find one endpoint A of a diameter
    std::vector<int> parent_node_a(n + 1), parent_edge_a(n + 1);
    int nodeA = bfs(1, parent_node_a, parent_edge_a);

    // Second BFS from A to find the other endpoint B, and the parent info from A
    std::vector<int> parent_node_b(n + 1), parent_edge_b(n + 1);
    int nodeB = bfs(nodeA, parent_node_b, parent_edge_b);

    // Trace from B back to A, collecting the edge indices on the diameter path
    std::vector<int> path_edges;
    int current = nodeB;
    while (current != nodeA) {
        int edge_idx = parent_edge_b[current];
        path_edges.push_back(edge_idx);
        current = parent_node_b[current];
    }

    // Return the first edge on the diameter path (any is valid)
    return path_edges.front();
}
// The core problem is to find an edge that belongs to the tree's diameter. The classic approach is: (1) Run a BFS/DFS from an arbitrary node (say node 1) to find the farthest node A. (2) Run a BFS/DFS from A to find the farthest node B and simultaneously record the parent of each node along the paths. The path from A to B is a diameter (this is a standard property of trees). (3) To find an edge on that diameter, we can trace back from B to A using the parent array, collecting edge IDs, then pick the first edge on that path. Since we store edges with original indices, when traversing from B to A we can record the edge index that connects each child to its parent. Then the diameter path is a sequence of edges; any one of them is valid. Edge cases: if the tree has exactly 2 nodes, the single edge is the diameter edge, and the BFS will correctly find it. If there are multiple diameters, any edge on the chosen diameter works, and the algorithm returns one such edge. Time complexity is O(n) for two BFS traversals plus O(length of diameter) for backtracking, which is O(n) overall. Space is O(n) for adjacency, parent, distances, and visited arrays.
