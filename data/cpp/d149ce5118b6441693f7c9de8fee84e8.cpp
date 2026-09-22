// Write a C++ function named `hasCycle` that takes an integer `V` representing the number of vertices (labeled `0` to `V-1`) and a vector of adjacency lists `graph` (where `graph[i]` contains the neighbors of vertex `i`), and returns a `bool` indicating whether the directed graph contains at least one cycle. The graph may be disconnected, may contain self-loops (an edge from a vertex to itself), and may contain parallel edges (duplicate edges between the same pair of vertices). The function must correctly detect cycles in all such cases. Use iterative or recursive DFS with explicit recursion tracking (like the provided snippet) to distinguish back edges from cross edges. The function signature must be `bool hasCycle(int V, const std::vector<std::vector<int>>& graph)`. Do not include a `main` function in the solution; only provide the function and its supporting code.

The problem is to detect a directed cycle in a graph. The standard approach is a depth-first search (DFS) that tracks two states per vertex: whether it has been visited at all, and whether it is currently in the recursion stack (i.e., on the path from the starting vertex of the current DFS). When traversing an edge from `u` to `v`, if `v` is already in the current recursion stack, then we have found a back edge, which indicates a cycle. If `v` has been fully processed (not in recursion stack), it cannot form a cycle through this edge. Because the graph may be disconnected, we must run DFS from every unvisited vertex. Self-loops are correctly detected because when exploring an edge from `u` to `u`, the neighbor `u` is already in the recursion stack (since we set it before exploring its neighbors), so it returns `true`. Parallel edges don't affect correctness because visiting the same neighbor twice without it being in recursion is harmless (it's already visited and not in recursion, so we skip it). Edge cases include: an empty graph (`V=0` returns false), a single vertex with no edges (false), a single vertex with a self-loop (true), and a graph that is a DAG (false). Time complexity is `O(V + E)` where `E` is the total number of edges, because each vertex and edge is processed once. Space complexity is `O(V)` for the visited and recursion-stack arrays, plus the recursion depth that can be up to `O(V)` in the worst case (in a chain). No additional auxiliary data structures are needed.

#include <vector>

// Checks if a directed graph contains a cycle using DFS with recursion stack tracking.
bool dfsCycleCheck(int node, const std::vector<std::vector<int>>& graph, std::vector<bool>& visited, std::vector<bool>& inRecursion) {
    visited[node] = true;
    inRecursion[node] = true;

    for (int neighbor : graph[node]) {
        if (!visited[neighbor]) {
            if (dfsCycleCheck(neighbor, graph, visited, inRecursion)) {
                return true;
            }
        } else if (inRecursion[neighbor]) {
            // Neighbor is in current DFS path -> back edge -> cycle
            return true;
        }
    }

    inRecursion[node] = false; // Remove from current path
    return false;
}

// Returns true if the directed graph has at least one cycle.
bool hasCycle(int V, const std::vector<std::vector<int>>& graph) {
    std::vector<bool> visited(V, false);
    std::vector<bool> inRecursion(V, false);

    for (int i = 0; i < V; ++i) {
        if (!visited[i] && dfsCycleCheck(i, graph, visited, inRecursion)) {
            return true;
        }
    }
    return false;
}

#include <cassert>
#include <vector>

// Assume the solution function hasCycle is defined above.

int main() {
    // Test 1: Simple 3-cycle
    std::vector<std::vector<int>> g1(3);
    g1[0].push_back(1);
    g1[1].push_back(2);
    g1[2].push_back(0);
    assert(hasCycle(3, g1) == true);

    // Test 2: DAG (no cycle)
    std::vector<std::vector<int>> g2(3);
    g2[0].push_back(1);
    g2[1].push_back(2);
    assert(hasCycle(3, g2) == false);

    // Test 3: Self-loop
    std::vector<std::vector<int>> g3(2);
    g3[0].push_back(0);
    assert(hasCycle(2, g3) == true);

    // Test 4: Disconnected components, one with cycle
    std::vector<std::vector<int>> g4(5);
    g4[0].push_back(1);
    g4[2].push_back(3);
    g4[3].push_back(2); // cycle in component {2,3}
    assert(hasCycle(5, g4) == true);

    // Test 5: Parallel edges and a back edge
    std::vector<std::vector<int>> g5(3);
    g5[0].push_back(1);
    g5[0].push_back(1); // duplicate
    g5[1].push_back(2);
    g5[2].push_back(1); // back edge -> cycle
    assert(hasCycle(3, g5) == true);

    // Test 6: Single vertex no edges
    std::vector<std::vector<int>> g6(1);
    assert(hasCycle(1, g6) == false);

    // Test 7: Empty graph (V=0)
    std::vector<std::vector<int>> g7(0);
    assert(hasCycle(0, g7) == false);

    // Test 8: Graph with only cross edges (DAG)
    std::vector<std::vector<int>> g8(4);
    g8[0].push_back(1);
    g8[0].push_back(2);
    g8[1].push_back(3);
    g8[2].push_back(3);
    assert(hasCycle(4, g8) == false);

    // Test 9: Two separate cycles
    std::vector<std::vector<int>> g9(6);
    g9[0].push_back(1);
    g9[1].push_back(0);
    g9[2].push_back(3);
    g9[3].push_back(4);
    g9[4].push_back(2);
    assert(hasCycle(6, g9) == true);

    // Test 10: Edge from a vertex to its already processed neighbor (no cycle)
    std::vector<std::vector<int>> g10(3);
    g10[0].push_back(1);
    g10[0].push_back(2);
    g10[1].push_back(2); // cross edge, not a cycle
    assert(hasCycle(3, g10) == false);

    return 0;
}
