/*
Write a C++ function that, given a directed or undirected graph represented by an adjacency-list-like structure using `std::vector<std::vector<int>>` (where vertices are numbered from 0 to n-1 and each inner vector contains the neighbors of the corresponding vertex), and given a starting vertex index, returns a `std::vector<int>` that represents the order in which vertices are first visited by a depth-first search (DFS) traversal. The DFS must follow the neighbors in the exact order they appear in the adjacency list. If the graph is disconnected, the traversal must continue from the smallest unvisited vertex index after finishing the component containing the start vertex. The function should handle an empty graph (n = 0) by returning an empty vector. The function must be named `depth_first_search_order` and should accept the graph as `const std::vector<std::vector<int>>&` and the start vertex as an `int`. The function must not modify the input graph.
*/
#include <vector>

// Perform a depth-first search starting from 'start', appending visited vertices to 'result'.
// The 'visited' vector tracks which vertices have been discovered.
void dfs_visit(const std::vector<std::vector<int>>& graph, int vertex,
               std::vector<bool>& visited, std::vector<int>& result) {
    visited[vertex] = true;
    result.push_back(vertex);
    for (int neighbor : graph[vertex]) {
        if (!visited[neighbor]) {
            dfs_visit(graph, neighbor, visited, result);
        }
    }
}

// Returns the DFS traversal order for the given graph, starting from 'start'.
// If the graph is disconnected, continues from the smallest unvisited vertex.
std::vector<int> depth_first_search_order(const std::vector<std::vector<int>>& graph,
                                          int start) {
    if (graph.empty()) return {};

    std::vector<bool> visited(graph.size(), false);
    std::vector<int> result;

    // If start is valid, begin there; otherwise, we'll handle all vertices in loop.
    if (start >= 0 && start < static_cast<int>(graph.size())) {
        dfs_visit(graph, start, visited, result);
    }

    // Visit any remaining components, starting from the smallest index.
    for (int v = 0; v < static_cast<int>(graph.size()); ++v) {
        if (!visited[v]) {
            dfs_visit(graph, v, visited, result);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple directed path 0->1->2
    std::vector<std::vector<int>> g1 = {{1}, {2}, {}};
    assert(depth_first_search_order(g1, 0) == std::vector<int>({0, 1, 2}));

    // Test 2: Disconnected graph: 0-1, 2 isolated, start at 1
    std::vector<std::vector<int>> g2 = {{1}, {0}, {}};
    assert(depth_first_search_order(g2, 1) == std::vector<int>({1, 0, 2}));

    // Test 3: Undirected triangle 0-1-2-0, start at 2
    std::vector<std::vector<int>> g3 = {{1, 2}, {0, 2}, {0, 1}};
    assert(depth_first_search_order(g3, 2) == std::vector<int>({2, 0, 1}));

    // Test 4: Empty graph
    std::vector<std::vector<int>> g4;
    assert(depth_first_search_order(g4, 0).empty());

    // Test 5: Single vertex
    std::vector<std::vector<int>> g5 = {{}};
    assert(depth_first_search_order(g5, 0) == std::vector<int>({0}));

    // Test 6: Graph with isolated vertex and a component, start at 0, but 0 isolated
    std::vector<std::vector<int>> g6 = {{}, {2}, {1}};
    assert(depth_first_search_order(g6, 0) == std::vector<int>({0, 1, 2}));

    // Test 7: Start vertex out of range – should still traverse all from 0
    std::vector<std::vector<int>> g7 = {{1}, {}};
    assert(depth_first_search_order(g7, 5) == std::vector<int>({0, 1}));

    // Test 8: Star graph centered at 0 with many leaves, start at 0
    std::vector<std::vector<int>> g8 = {{1, 2, 3}, {}, {}, {}};
    assert(depth_first_search_order(g8, 0) == std::vector<int>({0, 1, 2, 3}));

    // Test 9: Two disjoint edges, start at 3
    std::vector<std::vector<int>> g9 = {{1}, {0}, {3}, {2}};
    assert(depth_first_search_order(g9, 3) == std::vector<int>({3, 2, 0, 1}));

    // Test 10: Larger graph with multiple components, start at 2
    std::vector<std::vector<int>> g10 = {{1}, {0}, {3, 4}, {}, {}};
    assert(depth_first_search_order(g10, 2) == std::vector<int>({2, 3, 4, 0, 1}));

    return 0;
}
// The solution uses an iterative or recursive DFS with a visited array of booleans sized to the number of vertices. Start by marking the start vertex as visited and push it onto the result. Then recursively or iteratively visit all unvisited neighbors in the order they appear in the adjacency list. After finishing the component, iterate through all vertex indices from 0 to n-1 and for any unvisited vertex, perform DFS from it and append its visits to the result. This ensures full traversal of disconnected graphs. Edge cases include an empty graph (return empty), a start vertex out of range (should be treated as no valid start, but the task assumes valid start; still, the function can handle it by starting the global sweep from 0 if start is invalid). Time complexity is O(V + E) because each vertex and edge is processed once. Space complexity is O(V) for the visited array and recursion stack (or explicit stack for iterative). The result vector also takes O(V) space. Const correctness is maintained by taking the graph by const reference and not modifying it.
