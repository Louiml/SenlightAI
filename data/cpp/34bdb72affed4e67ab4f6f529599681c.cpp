/*
Given an undirected graph with vertices numbered 1 to `n` and `m` edges, write a C++ function `countCycleComponents(int n, const vector<pair<int,int>>& edges)` that returns the number of connected components that contain at least one cycle. A connected component is considered to have a cycle if it contains any cycle (a path that starts and ends at the same vertex with at least three edges and no repeated vertices except the start/end). The input graph may be disconnected and may contain parallel edges and self-loops. The function should operate on the given number of vertices (which may be up to 100,000) and edges (up to 100,000). The function should return the count of such components.
*/

#include <vector>
#include <functional>

// Counts the number of connected components that contain at least one cycle.
// n: number of vertices (1-indexed)
// edges: list of undirected edges as pairs (u, v). u and v in [1, n].
int countCycleComponents(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<int> degree(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        degree[i] = adj[i].size();
    }

    std::vector<bool> visited(n + 1, false);
    int result = 0;

    // Lambda to perform BFS/DFS and return component size and edge count.
    auto process_component = [&](int start) -> std::pair<int, int> {
        int vertex_count = 0;
        int edge_sum = 0;
        std::vector<int> stack;
        stack.push_back(start);
        visited[start] = true;
        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            vertex_count++;
            edge_sum += degree[u];
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    stack.push_back(v);
                }
            }
        }
        return {vertex_count, edge_sum / 2};
    };

    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            auto [vc, ec] = process_component(i);
            if (vc > 0 && ec >= vc) {
                result++;
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be declared above.
// int countCycleComponents(int n, const std::vector<std::pair<int,int>>& edges);

int main() {
    // Test 1: Two separate components: one triangle (cycle), one tree (no cycle)
    // Triangle: 1-2, 2-3, 3-1 ; Tree: 4-5
    std::vector<std::pair<int,int>> edges1 = {{1,2},{2,3},{3,1},{4,5}};
    assert(countCycleComponents(5, edges1) == 1);

    // Test 2: Single vertex with self-loop (cycle of length 1)
    std::vector<std::pair<int,int>> edges2 = {{1,1}};
    assert(countCycleComponents(1, edges2) == 1);

    // Test 3: Two parallel edges between same vertices (forms a cycle of length 2)
    std::vector<std::pair<int,int>> edges3 = {{1,2},{1,2}};
    assert(countCycleComponents(2, edges3) == 1);

    // Test 4: Disconnected, no cycles at all (all trees) 
    std::vector<std::pair<int,int>> edges4 = {{1,2},{2,3},{4,5}};
    assert(countCycleComponents(5, edges4) == 0);

    // Test 5: One large connected component with a cycle plus a tree
    // Cycle: 1-2,2-3,3-1 ; plus edge 3-4 (tree leaf); component has cycle
    std::vector<std::pair<int,int>> edges5 = {{1,2},{2,3},{3,1},{3,4}};
    assert(countCycleComponents(4, edges5) == 1);

    // Test 6: Empty graph
    std::vector<std::pair<int,int>> edges6;
    assert(countCycleComponents(3, edges6) == 0);

    // Test 7: Two separate cycles (two triangles) -> both components have cycles
    std::vector<std::pair<int,int>> edges7 = {{1,2},{2,3},{3,1},{4,5},{5,6},{6,4}};
    assert(countCycleComponents(6, edges7) == 2);

    // Test 8: Graph with vertices but isolated vertices only
    std::vector<std::pair<int,int>> edges8;
    assert(countCycleComponents(5, edges8) == 0); // no components with cycles, isolated vertices have no cycles

    // Test 9: Self loop on a vertex plus edge to another vertex -> component with cycle
    std::vector<std::pair<int,int>> edges9 = {{1,1},{1,2}};
    assert(countCycleComponents(2, edges9) == 1);

    // Test 10: Large vertex count with no edges
    assert(countCycleComponents(100000, edges6) == 0);

    return 0;
}

// The problem requires counting connected components that contain at least one cycle. A connected component with `V_c` vertices and `E_c` edges contains a cycle if and only if `E_c >= V_c` (since a tree has `E_c = V_c - 1`). Therefore, we can perform a DFS (or BFS) to find each connected component, count its vertices and edges, and then check if `E_c >= V_c`. Important edge cases: self-loops add an edge counted twice in the adjacency representation, but they still contribute one edge to the component's edge count; parallel edges also increase the edge count. Ensure that the edge count is computed correctly: each edge in the input contributes 1 to the total edge count of the component regardless of which vertex it connects. The DFS approach marks visited vertices, and for each unvisited vertex, we traverse the entire component, counting vertices and edges (where each undirected edge is counted once, e.g., by incrementing the edge counter whenever we encounter a neighbor that is not the parent, but careful with parallel edges; better to count edges during traversal via the adjacency list, but simpler: during DFS, for each vertex, iterate over its adjacency list and if the neighbor is unvisited, recurse; also count edges by incrementing an edge counter for each adjacency list entry? That would double-count edges. So instead, we can count edges after the component is identified by summing the degree of all vertices in the component and dividing by 2, but self-loops count twice in adjacency lists? Actually, a self-loop adds 2 to the degree sum; dividing by 2 gives 1 edge, which is correct. So we can collect vertices of the component during BFS/DFS, then count edges as `sum(degree[v] for v in component) / 2`. That is safe. Time complexity is O(n+m) for DFS, space O(n+m) for adjacency list and visited array. For n up to 100,000, this is fine. The function should be robust to large inputs and use efficient data structures.
