// Implement a C++ function `bool isBipartiteGraph(int V, const std::vector<std::vector<int>>& adj)` that determines whether an undirected graph (represented as an adjacency list with vertices numbered from `0` to `V-1`) is bipartite. A bipartite graph can have its vertices divided into two disjoint sets such that every edge connects a vertex in one set to a vertex in the other set (equivalently, it can be colored with exactly two colors so that no two adjacent vertices share the same color). The graph may consist of multiple connected components. The function should return `true` if the graph is bipartite, and `false` otherwise. Assume the adjacency list is valid (no self-loops or duplicate edges) and `V >= 1`.

The standard approach is to attempt a 2-coloring of the graph using BFS (or DFS). Start by initializing all vertices as uncolored (e.g., color `-1`). Iterate over all vertices; if a vertex is uncolored, run a BFS from it, assigning it color `0` (or `1`). For each neighbor of the current vertex during BFS: if the neighbor is uncolored, assign it the opposite color of the current vertex and enqueue it; if the neighbor has the same color as the current vertex, the graph is not bipartite and we return `false`. This BFS is repeated for each uncolored vertex to handle disconnected components. Edge cases include: a single vertex (trivially bipartite), a graph with no edges (bipartite), and even cycles (bipartite) vs odd cycles (not bipartite). Time complexity is \(O(V + E)\) since each vertex and edge is traversed once, and space complexity is \(O(V)\) for the color array and BFS queue.

#include <vector>
#include <queue>

// Determines if an undirected graph is bipartite using BFS-based 2-coloring.
// V: number of vertices, adj: adjacency list (size V).
// Returns true if the graph can be colored with two colors such that no two
// adjacent vertices share the same color, false otherwise.
bool isBipartiteGraph(int V, const std::vector<std::vector<int>>& adj) {
    // -1: uncolored, 0 or 1: two distinct colors
    std::vector<int> color(V, -1);
    
    // Helper lambda for BFS from a given source.
    // Returns true if the connected component containing 'source' is bipartite.
    auto bfs_check = [&](int source) -> bool {
        std::queue<int> q;
        q.push(source);
        color[source] = 0;
        
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            
            for (int neighbor : adj[current]) {
                if (color[neighbor] == -1) {
                    // Assign opposite color to neighbor
                    color[neighbor] = 1 - color[current];
                    q.push(neighbor);
                } else if (color[neighbor] == color[current]) {
                    // Adjacent vertices with same color -> not bipartite
                    return false;
                }
            }
        }
        return true;
    };
    
    // Check every connected component
    for (int i = 0; i < V; ++i) {
        if (color[i] == -1) {
            if (!bfs_check(i)) {
                return false;
            }
        }
    }
    return true;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty graph (no edges) with 3 vertices -> bipartite
    std::vector<std::vector<int>> adj1(3);
    assert(isBipartiteGraph(3, adj1) == true);

    // Test 2: Single vertex -> bipartite
    std::vector<std::vector<int>> adj2(1);
    assert(isBipartiteGraph(1, adj2) == true);

    // Test 3: Simple edge between 0 and 1 -> bipartite
    std::vector<std::vector<int>> adj3(2);
    adj3[0].push_back(1);
    adj3[1].push_back(0);
    assert(isBipartiteGraph(2, adj3) == true);

    // Test 4: Triangle (odd cycle) -> not bipartite
    std::vector<std::vector<int>> adj4(3);
    adj4[0].push_back(1); adj4[1].push_back(0);
    adj4[1].push_back(2); adj4[2].push_back(1);
    adj4[2].push_back(0); adj4[0].push_back(2);
    assert(isBipartiteGraph(3, adj4) == false);

    // Test 5: Even cycle (square) -> bipartite
    std::vector<std::vector<int>> adj5(4);
    adj5[0].push_back(1); adj5[1].push_back(0);
    adj5[1].push_back(2); adj5[2].push_back(1);
    adj5[2].push_back(3); adj5[3].push_back(2);
    adj5[3].push_back(0); adj5[0].push_back(3);
    assert(isBipartiteGraph(4, adj5) == true);

    // Test 6: Two disconnected components, one odd cycle -> not bipartite
    std::vector<std::vector<int>> adj6(5);
    adj6[0].push_back(1); adj6[1].push_back(0);
    adj6[1].push_back(2); adj6[2].push_back(1);
    adj6[2].push_back(0); adj6[0].push_back(2); // component 0-1-2 (triangle)
    adj6[3].push_back(4); adj6[4].push_back(3); // component 3-4 (edge)
    assert(isBipartiteGraph(5, adj6) == false);

    // Test 7: Disconnected components both bipartite -> true
    std::vector<std::vector<int>> adj7(4);
    adj7[0].push_back(1); adj7[1].push_back(0);
    adj7[2].push_back(3); adj7[3].push_back(2);
    assert(isBipartiteGraph(4, adj7) == true);

    // Test 8: Star graph (center 0, leaves 1,2,3) -> bipartite
    std::vector<std::vector<int>> adj8(4);
    adj8[0].push_back(1); adj8[1].push_back(0);
    adj8[0].push_back(2); adj8[2].push_back(0);
    adj8[0].push_back(3); adj8[3].push_back(0);
    assert(isBipartiteGraph(4, adj8) == true);

    // Test 9: Two vertices with multiple parallel edges (simulated duplicates) -> still bipartite
    std::vector<std::vector<int>> adj9(2);
    adj9[0].push_back(1); adj9[1].push_back(0);
    adj9[0].push_back(1); adj9[1].push_back(0);
    assert(isBipartiteGraph(2, adj9) == true);

    // Test 10: Chain of 5 vertices (path) -> bipartite
    std::vector<std::vector<int>> adj10(5);
    adj10[0].push_back(1); adj10[1].push_back(0);
    adj10[1].push_back(2); adj10[2].push_back(1);
    adj10[2].push_back(3); adj10[3].push_back(2);
    adj10[3].push_back(4); adj10[4].push_back(3);
    assert(isBipartiteGraph(5, adj10) == true);

    return 0;
}
