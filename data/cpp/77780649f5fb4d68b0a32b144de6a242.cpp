// Write a C++ function `int countConnectedComponents(const std::vector<std::vector<int>>& adjacencyList)` that takes an undirected graph represented as an adjacency list (where vertices are indexed from 0 to `n-1`, and each inner vector contains the neighbor indices) and returns the number of connected components in the graph. The graph may be disconnected, may contain isolated vertices (vertices with no edges), and may have self-loops (an edge from a vertex to itself). The function must not modify the input graph and must handle an empty graph (a vector with zero vertices) by returning 0.
// The problem reduces to counting the number of connected components in an undirected graph. The standard approach is to perform a depth-first search (DFS) or breadth-first search from every unvisited vertex. Each time we start a DFS from an unvisited vertex, we have discovered a new connected component; increment the component counter and mark all vertices reachable from that starting vertex as visited. This exactly matches the snippet’s logic. Important edge cases: (1) An empty graph (size 0) should return 0. (2) An isolated vertex (with an empty adjacency list) forms its own component and must be counted. (3) Self-loops do not change connectivity (they just cause the same vertex to be revisited but since it’s already visited, they are ignored). (4) The graph may have vertices with indices from 0 to n-1, and all indices are valid. The algorithm runs in O(V + E) time because each vertex and each edge is processed once. Space complexity is O(V) for the visited array and the recursion stack in the worst case (a chain of vertices). In the reference solution, we use an iterative DFS to avoid deep recursion stacks, but the recursive version is acceptable; given the task’s likely constraints, recursion is fine.
#include <vector>

// Count the number of connected components in an undirected graph.
// The graph is given as an adjacency list; vertices are indexed 0..size-1.
int countConnectedComponents(const std::vector<std::vector<int>>& adjacencyList) {
    const int n = static_cast<int>(adjacencyList.size());
    if (n == 0) {
        return 0;
    }

    std::vector<bool> visited(n, false);
    int components = 0;

    // Iterative DFS to avoid deep recursion.
    for (int start = 0; start < n; ++start) {
        if (visited[start]) {
            continue;
        }
        ++components;

        // Simple stack-based DFS
        std::vector<int> stack;
        stack.push_back(start);
        visited[start] = true;

        while (!stack.empty()) {
            const int u = stack.back();
            stack.pop_back();

            for (const int v : adjacencyList[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    stack.push_back(v);
                }
            }
        }
    }

    return components;
}
#include <cassert>
#include <vector>

// Declaration for the function under test (include the solution code above).
int countConnectedComponents(const std::vector<std::vector<int>>& adjacencyList);

int main() {
    // Test 1: Empty graph
    assert(countConnectedComponents({}) == 0);

    // Test 2: Single vertex, no edges
    assert(countConnectedComponents({{}}) == 1);

    // Test 3: Two isolated vertices
    assert(countConnectedComponents({{}, {}}) == 2);

    // Test 4: Two vertices connected by an edge
    assert(countConnectedComponents({{1}, {0}}) == 1);

    // Test 5: Triangle (3 vertices fully connected)
    assert(countConnectedComponents({{1,2}, {0,2}, {0,1}}) == 1);

    // Test 6: Disconnected graph: two separate edges
    assert(countConnectedComponents({{1}, {0}, {3}, {2}}) == 2);

    // Test 7: Graph with self-loops and isolated vertex
    assert(countConnectedComponents({{0}, {}} ) == 2); // vertex 0 has self-loop, vertex 1 isolated

    // Test 8: Graph with self-loops and connected component
    assert(countConnectedComponents({{0,1}, {0,1}, {2}}) == 2); // component {0,1}, isolated {2}

    // Test 9: Larger graph: 5 vertices, edges form one cycle plus an isolated vertex
    std::vector<std::vector<int>> graph = {{1,4}, {0,2}, {1,3}, {2,4}, {0,3}, {}};
    assert(countConnectedComponents(graph) == 2);

    // Test 10: Chain of 4 vertices all connected
    assert(countConnectedComponents({{1}, {0,2}, {1,3}, {2}}) == 1);

    return 0;
}
