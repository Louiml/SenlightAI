// Write a C++ function that performs an iterative depth-first search (DFS) traversal on an undirected graph represented using an adjacency list. The function should take as parameters the number of vertices, a 2D vector representing the adjacency list, and a starting vertex index. It should return a vector of integers containing the vertices in the order they are first visited during the DFS. The graph may be disconnected, and the DFS should only traverse the connected component containing the starting vertex. The graph vertices are labeled from 0 to V-1. Handle edge cases such as a single vertex, a graph with no edges, and a starting vertex that is not connected to any other vertex. The function must not modify the graph and must be const-correct.
// The solution uses an explicit stack to avoid recursion depth issues and to match the iterative style of the original snippet. We maintain a boolean `visited` array of size V, initialized to `false`. We push the starting vertex onto the stack and mark it visited. Then, while the stack is not empty, we pop the top vertex, append it to the result vector, and iterate over all its neighbors. For each neighbor that has not been visited, we mark it visited and push it onto the stack. Because we push all unvisited neighbors at once, the traversal order might differ from recursive DFS depending on the neighbor order, but it is still a valid DFS. Important edge cases: (1) if the starting vertex is out of range, we should return an empty vector; (2) if the graph has no edges, the function returns just the starting vertex; (3) for a disconnected graph, only the component reachable from the start is visited. Time complexity is O(V + E) since each vertex is pushed and popped at most once, and each edge is examined twice (once for each endpoint). Space complexity is O(V) for the visited array and the stack.
#include <vector>
#include <stack>
#include <stdexcept>

// Perform iterative DFS on an undirected graph starting from a given vertex.
// Returns a vector of vertices in the order they are visited.
// Graph is represented as an adjacency list (vector of vectors).
// Assumes vertices are labeled 0 to V-1. If startVertex is out of range, returns empty vector.
std::vector<int> iterativeDFS(int numVertices, const std::vector<std::vector<int>>& adjList, int startVertex) {
    // Validate input
    if (numVertices <= 0 || startVertex < 0 || startVertex >= numVertices) {
        return {};
    }
    // Ensure adjacency list has at least numVertices entries
    if (adjList.size() < static_cast<size_t>(numVertices)) {
        return {};
    }

    std::vector<bool> visited(numVertices, false);
    std::vector<int> result;
    std::stack<int> s;

    s.push(startVertex);
    visited[startVertex] = true;

    while (!s.empty()) {
        int curr = s.top();
        s.pop();
        result.push_back(curr);

        // Iterate over neighbors in the given order
        for (int neighbor : adjList[curr]) {
            // Check bounds and visited status
            if (neighbor >= 0 && neighbor < numVertices && !visited[neighbor]) {
                visited[neighbor] = true;
                s.push(neighbor);
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above (iterativeDFS).
// Test cases:
int main() {
    // Test 1: Simple connected graph from the snippet
    int V1 = 5;
    std::vector<std::vector<int>> adj1 = {{1,2}, {0,2,3}, {0,1,4}, {1,4}, {2,3}};
    std::vector<int> res1 = iterativeDFS(V1, adj1, 0);
    assert(res1.size() == 5);
    // Check all vertices are visited exactly once (order may vary but set must be all)
    std::vector<bool> seen(5, false);
    for (int v : res1) { seen[v] = true; }
    for (bool b : seen) { assert(b); }

    // Test 2: Single vertex, no edges
    std::vector<std::vector<int>> adj2 = {{}};
    std::vector<int> res2 = iterativeDFS(1, adj2, 0);
    assert(res2 == std::vector<int>({0}));

    // Test 3: Disconnected graph, start from a vertex in one component
    int V3 = 5;
    std::vector<std::vector<int>> adj3 = {{1}, {0}, {3}, {2}, {}}; // component 0-1, component 2-3, isolated 4
    std::vector<int> res3 = iterativeDFS(V3, adj3, 0);
    assert(res3.size() == 2);
    // Must contain exactly {0,1} in any order
    bool has0 = false, has1 = false;
    for (int v : res3) { if (v == 0) has0 = true; if (v == 1) has1 = true; }
    assert(has0 && has1);

    // Test 4: Start from isolated vertex
    std::vector<int> res4 = iterativeDFS(V3, adj3, 4);
    assert(res4 == std::vector<int>({4}));

    // Test 5: Out-of-range start vertex
    std::vector<int> res5 = iterativeDFS(V3, adj3, 5);
    assert(res5.empty());

    // Test 6: Negative start vertex
    std::vector<int> res6 = iterativeDFS(V3, adj3, -1);
    assert(res6.empty());

    // Test 7: Zero vertices
    std::vector<std::vector<int>> adj7 = {};
    std::vector<int> res7 = iterativeDFS(0, adj7, 0);
    assert(res7.empty());

    // Test 8: Graph with self-loop (0->0)
    std::vector<std::vector<int>> adj8 = {{0}};
    std::vector<int> res8 = iterativeDFS(1, adj8, 0);
    assert(res8 == std::vector<int>({0}));

    // Test 9: Graph with parallel edges (duplicate neighbors)
    std::vector<std::vector<int>> adj9 = {{1,1}, {0,0}};
    std::vector<int> res9 = iterativeDFS(2, adj9, 0);
    assert(res9.size() == 2);
    bool has0_9 = false, has1_9 = false;
    for (int v : res9) { if (v == 0) has0_9 = true; if (v == 1) has1_9 = true; }
    assert(has0_9 && has1_9);

    // Test 10: Larger linear graph (0-1-2-3)
    std::vector<std::vector<int>> adj10 = {{1}, {0,2}, {1,3}, {2}};
    std::vector<int> res10 = iterativeDFS(4, adj10, 0);
    assert(res10.size() == 4);
    // DFS order from 0 with neighbor list order: 1, then from 1: 2 (since 0 visited), then from 2: 3
    assert(res10 == std::vector<int>({0,1,2,3}));

    return 0;
}
