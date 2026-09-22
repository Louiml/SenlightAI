Write a C++ function `bool hasCycle(const std::vector<std::vector<int>>& adj)` that detects whether a directed graph, represented by an adjacency list where `adj[u]` contains the indices of nodes reachable from node `u`, contains at least one cycle. The graph has nodes numbered `0` to `n-1`, where `n` is the size of the outer vector. The function must return `true` if a cycle exists and `false` otherwise. The graph may be disconnected, and nodes with no outgoing edges (sinks) are allowed. Implement the cycle detection using a recursive depth-first search with a three‑state coloring (unvisited, in current recursion stack, fully processed). Handle self‑loops and multi‑edges correctly.

#include <cassert>
#include <functional>
#include <vector>

// (The solution function is assumed to be defined above.)

int main() {
    // Test 1: simple cycle A->B->A
    std::vector<std::vector<int>> g1 = {{1}, {0}};
    assert(hasCycle(g1) == true);

    // Test 2: DAG (no cycle)
    std::vector<std::vector<int>> g2 = {{1}, {}};
    assert(hasCycle(g2) == false);

    // Test 3: self-loop
    std::vector<std::vector<int>> g3 = {{0}};
    assert(hasCycle(g3) == true);

    // Test 4: disconnected, one component with cycle
    std::vector<std::vector<int>> g4 = {{1}, {0}, {}};
    assert(hasCycle(g4) == true);

    // Test 5: empty graph
    std::vector<std::vector<int>> g5;
    assert(hasCycle(g5) == false);

    // Test 6: multi-edge and cycle
    std::vector<std::vector<int>> g6 = {{1, 1}, {2}, {0}};
    assert(hasCycle(g6) == true);

    // Test 7: multi-edge and no cycle
    std::vector<std::vector<int>> g7 = {{1, 1}, {}};
    assert(hasCycle(g7) == false);

    // Test 8: longer chain with back edge to start
    std::vector<std::vector<int>> g8 = {{1}, {2}, {3}, {1}};
    assert(hasCycle(g8) == true);

    // Test 9: chain without cycle
    std::vector<std::vector<int>> g9 = {{1}, {2}, {3}, {}};
    assert(hasCycle(g9) == false);

    // Test 10: two cycles in separate components
    std::vector<std::vector<int>> g10 = {{1}, {0}, {3}, {2}};
    assert(hasCycle(g10) == true);
}

#include <vector>

// Detect if a directed graph (adjacency list) contains a cycle.
bool hasCycle(const std::vector<std::vector<int>>& adj) {
    int n = static_cast<int>(adj.size());
    std::vector<int> state(n, 0); // 0 = unvisited, 1 = visiting, 2 = visited

    // Recursive DFS helper
    std::function<bool(int)> dfs = [&](int u) -> bool {
        state[u] = 1; // mark as visiting
        for (int v : adj[u]) {
            if (state[v] == 0) {
                if (dfs(v)) return true;
            } else if (state[v] == 1) {
                return true; // back edge → cycle
            }
        }
        state[u] = 2; // mark as fully processed
        return false;
    };

    for (int i = 0; i < n; ++i) {
        if (state[i] == 0 && dfs(i)) {
            return true;
        }
    }
    return false;
}

// The solution uses DFS with a color array: `0` = unvisited, `1` = currently being visited (in the current recursion stack), `2` = fully processed. For each unvisited node, we start DFS. When visiting a node `u`, mark it as `1`. For each neighbor `v`, if `v` is unvisited, recursively DFS; if `v` is marked `1`, a cycle is found (because we reached a node already on the current recursion stack). After processing all neighbors, mark `u` as `2`. We check every unvisited node to handle disconnected components. Self‑loops are automatically detected because `v == u` will be marked `1` when processing the neighbor list. Time complexity is O(V + E) since each node and edge is processed once. Space complexity is O(V) for the color array and the implicit recursion stack (which in the worst case can be O(V) deep). No special handling is needed for multi‑edges because they don’t affect cycle detection.
