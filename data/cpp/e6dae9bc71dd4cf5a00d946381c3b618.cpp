// Write a C++ function `bool hasCycle(int V, const std::vector<std::vector<int>>& adj)` that determines whether an undirected graph contains a cycle. The graph is given by the number of vertices `V` (labeled from 0 to V-1) and an adjacency list where `adj[u]` contains all neighbors of vertex `u`. The function must return `true` if the graph has at least one cycle, and `false` otherwise. The graph may be disconnected, so your algorithm must explicitly check every connected component. Use depth-first search (DFS) with a parent pointer to avoid counting the immediate back-and-forth edge between a node and its parent as a cycle. Vertices are zero-indexed, and the adjacency list is guaranteed to be symmetric (since the graph is undirected). The function must be efficient for graphs with up to 10^5 vertices and edges, and you must handle isolated vertices correctly. Do not modify the input graph; use `const` references wherever appropriate.

The main approach is to perform a DFS traversal from each unvisited vertex, maintaining a `visited` boolean array and a `parent` vertex. For each neighbor `nbr` of the current vertex `src`: if `nbr` is not visited, recursively call DFS with `src` as the new parent. If during recursion a cycle is found, propagate `true` upward. If `nbr` is already visited and `nbr` is not the parent of `src`, then we have found a back edge, indicating a cycle (because in an undirected graph, any edge to an already visited non-parent node creates a cycle). For each connected component, we start DFS with a parent of -1 (or any sentinel value not equal to any vertex index). Edge cases include: an empty graph (V=0) has no cycle; a graph with no edges has no cycle; a self-loop (edge from a vertex to itself) would be detected because `nbr==src` and `nbr != parent` (since parent is not equal to src unless parent is src, but parent passed is the previous vertex, so for a self-loop, `nbr == src` and `nbr != parent` because parent cannot equal src unless there is a direct self-loop edge, but even then the first call has parent=-1, so it will be caught); parallel edges (two edges between the same pair of vertices) are treated as a cycle because during DFS, the second edge will see the neighbor as visited and not the parent. The time complexity is O(V+E) because each vertex and edge is visited once. The auxiliary space is O(V) for the visited array and recursion stack (in worst case O(V) depth for a chain).

#include <vector>

// Determine if an undirected graph contains a cycle using DFS.
// V: number of vertices (0 to V-1)
// adj: adjacency list (symmetric for undirected graph)
bool hasCycle(int V, const std::vector<std::vector<int>>& adj) {
    std::vector<bool> visited(V, false);

    // Recursive helper: checks for cycle starting from node 'src' with given 'parent'.
    // Returns true if a cycle is found in this DFS subtree.
    auto dfs = [&](auto&& self, int src, int parent) -> bool {
        visited[src] = true;

        for (int nbr : adj[src]) {
            if (!visited[nbr]) {
                if (self(self, nbr, src)) {
                    return true;
                }
            } else if (nbr != parent) {
                // Discovered a back edge to an already visited node that is not the parent.
                return true;
            }
        }
        return false;
    };

    // Check every connected component, using -1 as the parent of the root.
    for (int i = 0; i < V; ++i) {
        if (!visited[i]) {
            if (dfs(dfs, i, -1)) {
                return true;
            }
        }
    }
    return false;
}

#include <cassert>
#include <vector>

// Assume hasCycle is defined above (or include the solution header).

int main() {
    // Test 1: Empty graph (no vertices)
    std::vector<std::vector<int>> adj0;
    assert(hasCycle(0, adj0) == false);

    // Test 2: Single vertex with no edges
    std::vector<std::vector<int>> adj1(1);
    assert(hasCycle(1, adj1) == false);

    // Test 3: Two vertices with a single edge (no cycle)
    std::vector<std::vector<int>> adj2(2);
    adj2[0].push_back(1);
    adj2[1].push_back(0);
    assert(hasCycle(2, adj2) == false);

    // Test 4: Triangle (cycle)
    std::vector<std::vector<int>> adj3(3);
    adj3[0].push_back(1); adj3[1].push_back(0);
    adj3[1].push_back(2); adj3[2].push_back(1);
    adj3[2].push_back(0); adj3[0].push_back(2);
    assert(hasCycle(3, adj3) == true);

    // Test 5: Disconnected graph where one component has a cycle
    std::vector<std::vector<int>> adj4(5);
    adj4[0].push_back(1); adj4[1].push_back(0);
    adj4[1].push_back(2); adj4[2].push_back(1);
    adj4[2].push_back(0); adj4[0].push_back(2); // component 0-1-2 has triangle
    adj4[3].push_back(4); adj4[4].push_back(3); // component 3-4 is a tree
    assert(hasCycle(5, adj4) == true);

    // Test 6: Self-loop (edge from vertex to itself)
    std::vector<std::vector<int>> adj5(2);
    adj5[0].push_back(0);
    adj5[1].push_back(1); adj5[1].push_back(1); // another self-loop
    assert(hasCycle(2, adj5) == true);

    // Test 7: Parallel edges (duplicate edges) — treated as cycle
    std::vector<std::vector<int>> adj6(2);
    adj6[0].push_back(1); adj6[0].push_back(1);
    adj6[1].push_back(0); adj6[1].push_back(0);
    assert(hasCycle(2, adj6) == true);

    // Test 8: A path of 4 vertices (no cycle)
    std::vector<std::vector<int>> adj7(4);
    adj7[0].push_back(1); adj7[1].push_back(0);
    adj7[1].push_back(2); adj7[2].push_back(1);
    adj7[2].push_back(3); adj7[3].push_back(2);
    assert(hasCycle(4, adj7) == false);

    // Test 9: Star graph (center 0 connected to 1,2,3) — no cycle
    std::vector<std::vector<int>> adj8(4);
    adj8[0].push_back(1); adj8[1].push_back(0);
    adj8[0].push_back(2); adj8[2].push_back(0);
    adj8[0].push_back(3); adj8[3].push_back(0);
    assert(hasCycle(4, adj8) == false);

    // Test 10: Large-ish but simple — a cycle of 5 vertices
    std::vector<std::vector<int>> adj9(5);
    for (int i = 0; i < 5; ++i) {
        adj9[i].push_back((i+1)%5);
        adj9[(i+1)%5].push_back(i);
    }
    assert(hasCycle(5, adj9) == true);

    return 0;
}
