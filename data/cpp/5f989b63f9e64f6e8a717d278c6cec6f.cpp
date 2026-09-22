// Write a standalone C++ function `countReachableNodes` that takes a directed graph represented as an adjacency list (using `std::vector<std::vector<int>>`) and a starting vertex index, and returns the number of nodes reachable from that starting vertex via directed edges. The graph may have cycles, isolated vertices, and multiple edges. The starting vertex itself is always considered reachable. The function must be `const`-correct (take the graph by `const` reference) and must not modify the input graph. The graph vertices are indexed from `0` to `V-1`, where `V` is the number of vertices. Assume all vertex indices are valid (no vertices outside the range).

The problem reduces to a standard graph traversal: Depth-First Search (DFS) or Breadth-First Search (BFS) can be used to explore all nodes reachable from the given start. We initialize a boolean visited array of size `V` to `false`. We mark the starting node as visited and recursively visit all its unvisited neighbors. Because the graph may contain cycles, the visited array prevents infinite recursion and double counting. After the traversal completes, we count the number of `true` entries in the visited array, which equals the number of reachable nodes (including the start). Edge cases: 
- If the graph is empty (V = 0), the function returns 0 (simple loop over visited returns 0). 
- If the starting vertex is isolated (no outgoing edges), the function returns 1. 
- Multiple edges and self-loops are handled naturally by checking the visited array before recursing. 
Time complexity is O(V + E) where E is the total number of edges in the graph, because each vertex is visited at most once and each edge is examined once from each of its source vertices. Space complexity is O(V) for the visited array plus the recursion stack depth (in worst case O(V) for a linear chain graph).

#include <vector>
#include <functional>

// Count the number of vertices reachable from a given start vertex in a directed graph.
// The graph is represented as an adjacency list: graph[u] contains all vertices v such that there is an edge u -> v.
// The start vertex itself is counted as reachable.
int countReachableNodes(const std::vector<std::vector<int>>& graph, int start) {
    int numVertices = static_cast<int>(graph.size());
    std::vector<bool> visited(numVertices, false);

    // Depth-first search using a lambda to capture visited by reference.
    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (int v : graph[u]) {
            if (!visited[v]) {
                dfs(v);
            }
        }
    };

    // Trigger DFS from the start vertex (if graph is non-empty).
    if (numVertices > 0 && start >= 0 && start < numVertices) {
        dfs(start);
    }

    // Count visited nodes.
    int reachableCount = 0;
    for (bool isVisited : visited) {
        if (isVisited) {
            ++reachableCount;
        }
    }
    return reachableCount;
}

#include <cassert>
#include <vector>

// The solution function is expected to be defined above this main.
// (In a standalone test, the function is included here.)

int main() {
    // Test 1: Simple linear graph 0->1->2.
    std::vector<std::vector<int>> g1 = {{1}, {2}, {}};
    assert(countReachableNodes(g1, 0) == 3);
    assert(countReachableNodes(g1, 1) == 2);

    // Test 2: Graph with a cycle and isolated vertex.
    std::vector<std::vector<int>> g2 = {{1}, {0, 2}, {2, 1}, {}};
    // Starting from 0: reachable {0,1,2} (3 vertices); 3 is isolated and not reachable.
    assert(countReachableNodes(g2, 0) == 3);
    // Starting from 3: only itself.
    assert(countReachableNodes(g2, 3) == 1);

    // Test 3: Self-loop and multiple edges.
    std::vector<std::vector<int>> g3 = {{0, 0, 1}, {1, 1}};
    assert(countReachableNodes(g3, 0) == 2);
    assert(countReachableNodes(g3, 1) == 1);

    // Test 4: Empty graph (no vertices).
    std::vector<std::vector<int>> g4;
    assert(countReachableNodes(g4, 0) == 0);

    // Test 5: Disconnected components.
    std::vector<std::vector<int>> g5 = {{2}, {}, {1}, {0}};
    // From 0: 0->2->1; 3 is isolated from this component.
    assert(countReachableNodes(g5, 0) == 3);
    // From 3: 3->0->2->1, so all 4 reachable.
    assert(countReachableNodes(g5, 3) == 4);

    // Test 6: Single vertex with no edges.
    std::vector<std::vector<int>> g6 = {{}};
    assert(countReachableNodes(g6, 0) == 1);

    return 0;
}
