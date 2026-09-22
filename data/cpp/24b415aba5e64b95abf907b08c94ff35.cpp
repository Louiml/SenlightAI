/*
Write a C++ function `void dfsTraversal(int start, const std::vector<std::vector<int>>& adjacencyList, std::vector<int>& traversalOrder)` that performs a depth-first search (DFS) starting from a given vertex `start` on an undirected graph represented as an adjacency list. The function must fill the `traversalOrder` vector with the vertices in the exact order they are first visited by the recursive DFS algorithm (pre-order). The graph may be disconnected, but the function only needs to traverse the component reachable from `start`. Assume vertex labels are integers from `0` to `V-1`. The function must be `const`-correct: the adjacency list is passed as `const` reference and not modified. Edge cases include a single-vertex graph, a graph with cycles (which must be handled without infinite recursion), and a start vertex that is isolated (no edges).
*/

#include <vector>
#include <functional>

// Perform DFS from 'start' and store the pre-order traversal in 'traversalOrder'.
// adjacencyList is a const reference to an undirected graph's adjacency list.
// The function fills 'traversalOrder' with the vertices in the order they are first visited.
void dfsTraversal(int start, const std::vector<std::vector<int>>& adjacencyList, std::vector<int>& traversalOrder) {
    int numVertices = static_cast<int>(adjacencyList.size());
    if (numVertices == 0) return;

    std::vector<bool> visited(numVertices, false);

    std::function<void(int)> dfsHelper = [&](int current) {
        visited[current] = true;
        traversalOrder.push_back(current);

        for (int neighbor : adjacencyList[current]) {
            if (!visited[neighbor]) {
                dfsHelper(neighbor);
            }
        }
    };

    dfsHelper(start);
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above this main.
int main() {
    // Test 1: simple connected graph from the snippet
    std::vector<std::vector<int>> graph1(5);
    graph1[0] = {1, 2};
    graph1[1] = {0, 4};
    graph1[2] = {0, 3};
    graph1[3] = {2, 4};
    graph1[4] = {1, 3};
    std::vector<int> order1;
    dfsTraversal(0, graph1, order1);
    assert(order1 == std::vector<int>({0, 1, 4, 3, 2})); // depends on neighbor order

    // Test 2: single vertex graph
    std::vector<std::vector<int>> graph2(1);
    std::vector<int> order2;
    dfsTraversal(0, graph2, order2);
    assert(order2 == std::vector<int>({0}));

    // Test 3: isolated start vertex (no edges)
    std::vector<std::vector<int>> graph3(3);
    graph3[0] = {1};
    graph3[1] = {0};
    // vertex 2 is isolated
    std::vector<int> order3;
    dfsTraversal(2, graph3, order3);
    assert(order3 == std::vector<int>({2}));

    // Test 4: graph with a cycle: triangle 0-1-2-0
    std::vector<std::vector<int>> graph4(3);
    graph4[0] = {1, 2};
    graph4[1] = {0, 2};
    graph4[2] = {1, 0};
    std::vector<int> order4;
    dfsTraversal(0, graph4, order4);
    assert(order4 == std::vector<int>({0, 1, 2}));

    // Test 5: linear chain 0-1-2-3
    std::vector<std::vector<int>> graph5(4);
    graph5[0] = {1};
    graph5[1] = {0, 2};
    graph5[2] = {1, 3};
    graph5[3] = {2};
    std::vector<int> order5;
    dfsTraversal(0, graph5, order5);
    assert(order5 == std::vector<int>({0, 1, 2, 3}));

    return 0;
}

// The solution follows the standard recursive DFS approach. Maintain a `visited` boolean array (or vector) of size `V` (obtained from `adjacencyList.size()`), initially all `false`. Start the recursion from the given `start` vertex. In the recursive helper, mark the current vertex as visited, append it to `traversalOrder`, then iterate over all neighbors of the current vertex from the adjacency list; for each neighbor that is not yet visited, recursively call the helper on that neighbor. This ensures each vertex in the reachable component is visited exactly once, and cycles are handled because we check the `visited` flag before recursing. For an isolated start vertex, the loop over its (empty) adjacency list does nothing, so only the start vertex is added. The order of traversal depends on the order of neighbors in the adjacency list (which is given as input). Time complexity is O(V + E) for the visited component, where V is the number of vertices and E is the number of edges in the reachable component, because each vertex is visited once and each edge is examined once from its endpoints. Space complexity is O(V) for the visited array and the recursion stack (in the worst case, the recursion depth can be V for a linear graph).
