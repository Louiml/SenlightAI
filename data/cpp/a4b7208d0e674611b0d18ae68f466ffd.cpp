// Write a C++ function `bool isBridge(const std::vector<std::vector<int>>& adj, int s, int e)` that determines whether the undirected edge between vertices `s` and `e` is a bridge in the graph. The graph is represented as an adjacency list (each `adj[i]` holds the neighbors of vertex `i`). The function must return `true` if removing the edge disconnects the graph (i.e., there is no alternative path between `s` and `e` after removing the edge), and `false` otherwise. The graph has no self-loops, may be disconnected, and may contain duplicate edges (though you can treat duplicates as multiple parallel edges—removing only one instance of the edge is required). If the edge does not exist in the adjacency lists, treat it as **not** a bridge (return `false`). Vertices are numbered from `0` to `V-1`, where `V = adj.size()`.

// The simplest correct approach: temporarily remove the edge (one occurrence) from both adjacency lists, then perform a DFS (or BFS) from vertex `s`. If after removal vertex `e` is still reachable from `s`, then there is an alternative path, so the edge is not a bridge; otherwise it is a bridge. Since the graph is undirected, we must remove the edge from both directions. Important edge cases: (1) if the edge is not present in either adjacency list, return `false` immediately; (2) if `s == e` (self-loop) – but problem says no self-loops, yet guard anyway: if `s == e`, removing a self-loop always leaves the graph connected, so return `false`; (3) if the graph is already disconnected, but `s` and `e` are in different components originally, then removing the edge still leaves them disconnected, but the edge itself is not a bridge (since they were never connected via that edge), so return `false`? Actually careful: an edge is a bridge if removing it increases the number of connected components. If `s` and `e` were already in different components before removal, the edge wouldn’t exist connecting them, so that case can't happen. So we don't need to handle that. The algorithm takes `O(V + E)` time for the DFS (worst-case) and `O(V)` auxiliary space for the visited array. Removing an element from a vector adjacency list can be done by searching for the value, which takes `O(degree)` time; overall time is `O(V + E)`. We only need to remove one occurrence, not all duplicates.

#include <vector>
#include <queue>

// Determines if edge (s, e) is a bridge in an undirected graph.
// adj is the adjacency list; V = adj.size().
bool isBridge(const std::vector<std::vector<int>>& adj, int s, int e) {
    if (s == e) return false; // self-loop cannot be a bridge
    
    // Make local copy of adjacency list to avoid modifying input.
    std::vector<std::vector<int>> local = adj;
    
    // Remove one occurrence of edge (s, e) from both lists.
    bool found_s = false, found_e = false;
    for (auto it = local[s].begin(); it != local[s].end(); ++it) {
        if (*it == e) {
            local[s].erase(it);
            found_s = true;
            break;
        }
    }
    for (auto it = local[e].begin(); it != local[e].end(); ++it) {
        if (*it == s) {
            local[e].erase(it);
            found_e = true;
            break;
        }
    }
    // If edge didn't exist, it's not a bridge.
    if (!found_s || !found_e) return false;
    
    // BFS/DFS from s to see if e is still reachable.
    std::vector<bool> visited(local.size(), false);
    std::queue<int> q;
    q.push(s);
    visited[s] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : local[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return !visited[e];
}

#include <cassert>
#include <vector>

// Assume isBridge is defined above.

int main() {
    // Example 1 from prompt: 4 vertices, edges: 0-1, 1-2, 2-3. Edge 1-2 is bridge.
    std::vector<std::vector<int>> g1 = {{1}, {0,2}, {1,3}, {2}};
    assert(isBridge(g1, 1, 2) == true);
    assert(isBridge(g1, 0, 1) == true); // also bridge
    assert(isBridge(g1, 2, 3) == true); // also bridge

    // Example 2: 5 vertices, edges: 1-2, 2-0, 1-0, 3-4, 3-0. Edge 2-0 is not a bridge.
    std::vector<std::vector<int>> g2 = {{2,1,3}, {2,0}, {1,0}, {4,0}, {3}};
    assert(isBridge(g2, 2, 0) == false);

    // Test edge not present.
    std::vector<std::vector<int>> g3 = {{1}, {0,2}, {1}};
    assert(isBridge(g3, 0, 2) == false);

    // Test triangle (cycle): no edge is a bridge.
    std::vector<std::vector<int>> g4 = {{1,2}, {0,2}, {0,1}};
    assert(isBridge(g4, 0, 1) == false);
    assert(isBridge(g4, 1, 2) == false);
    assert(isBridge(g4, 0, 2) == false);

    // Single vertex with self-loop (though not typical).
    std::vector<std::vector<int>> g5 = {{0}};
    assert(isBridge(g5, 0, 0) == false);

    // Duplicate edges: two parallel edges between 0 and 1. Removing one still leaves connection.
    std::vector<std::vector<int>> g6 = {{1,1}, {0,0}};
    assert(isBridge(g6, 0, 1) == false);

    return 0;
}
