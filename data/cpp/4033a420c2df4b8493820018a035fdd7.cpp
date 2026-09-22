/*
Write a C++ function that, given a directed graph represented by an adjacency list, a starting vertex `start`, and a total number of vertices `V` (vertices numbered 1..V), performs a Breadth-First Search (BFS) starting from `start`. The function must return a vector containing the vertices in the exact order they are first visited during the BFS traversal, using the adjacency list in the order edges were added. The graph is directed, so edges are one-way. The function should handle disconnected graphs by only visiting reachable vertices from `start`. If `start` is out of the valid range (1..V), return an empty vector. The function must be `const` correct, not modify the input graph, and work correctly even if the graph has no edges or only one vertex.
*/

#include <vector>
#include <queue>

// Perform BFS on a directed graph from a given start vertex.
// Graph is represented as adjacency list (vector of vectors). Vertices are 1-indexed.
// Returns a vector of vertices in the order they are first visited.
std::vector<int> bfsTraversal(int V, int start, const std::vector<std::vector<int>>& adj) {
    std::vector<int> result;
    if (V <= 0 || start < 1 || start > V) {
        return result; // Invalid input, return empty
    }

    std::vector<bool> visited(V + 1, false); // 1-indexed vertices
    std::queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        result.push_back(current);

        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above. Test it here.
int main() {
    // Test case 1: Simple directed graph 1->2, 1->3, 2->4
    std::vector<std::vector<int>> adj1 = {{}, {2, 3}, {4}, {}, {}}; // index 0 unused
    assert(bfsTraversal(4, 1, adj1) == std::vector<int>({1, 2, 3, 4}));

    // Test case 2: Disconnected graph, start at 1 in node 1 only
    std::vector<std::vector<int>> adj2 = {{}, {2}, {}, {4}, {}};
    assert(bfsTraversal(4, 1, adj2) == std::vector<int>({1, 2}));

    // Test case 3: Single vertex, no edges
    std::vector<std::vector<int>> adj3 = {{}};
    assert(bfsTraversal(1, 1, adj3) == std::vector<int>({1}));

    // Test case 4: Start has no outgoing edges, only itself
    std::vector<std::vector<int>> adj4 = {{}, {2}, {}};
    assert(bfsTraversal(2, 2, adj4) == std::vector<int>({2}));

    // Test case 5: Invalid start (0)
    std::vector<std::vector<int>> adj5 = {{}, {2}, {}};
    assert(bfsTraversal(2, 0, adj5) == std::vector<int>({}));

    // Test case 6: Start out of range (greater than V)
    std::vector<std::vector<int>> adj6 = {{}, {2}, {}};
    assert(bfsTraversal(2, 3, adj6) == std::vector<int>({}));

    // Test case 7: Graph with cycle 1->2, 2->1
    std::vector<std::vector<int>> adj7 = {{}, {2}, {1}};
    assert(bfsTraversal(2, 1, adj7) == std::vector<int>({1, 2}));

    // Test case 8: Multiple edges to same neighbor (should only visit once)
    std::vector<std::vector<int>> adj8 = {{}, {2, 2, 3}, {}, {}};
    assert(bfsTraversal(3, 1, adj8) == std::vector<int>({1, 2, 3}));

    // Test case 9: Graph with no edges at all, start=1
    std::vector<std::vector<int>> adj9 = {{}, {}, {}, {}};
    assert(bfsTraversal(3, 1, adj9) == std::vector<int>({1}));

    // Test case 10: Larger graph with branching
    std::vector<std::vector<int>> adj10 = {{}, {2, 3}, {4, 5}, {6}, {}, {}, {}, {}};
    // BFS order from 1: 1,2,3,4,5,6
    assert(bfsTraversal(6, 1, adj10) == std::vector<int>({1, 2, 3, 4, 5, 6}));

    return 0;
}

// The solution uses a standard iterative BFS with a queue. We initialize a boolean visited array of size V+1 (to allow 1-based indexing) all set to false. Push the start vertex onto the queue, mark it visited, and append it to the result vector. Then, in a loop, pop the front vertex, iterate over its adjacency list, and for each unvisited neighbor, mark it visited, push it to the queue, and add it to the result. This ensures each vertex is visited at most once and in level-order order (by increasing distance from start). Edge cases: if `start` is outside [1, V], return empty immediately. If the start vertex has no outgoing edges or the graph is disconnected, the function still works because we only explore reachable nodes. The adjacency list is taken as a constant reference to avoid copying. Complexity: O(V + E) time where V is number of vertices and E is number of edges, since each vertex and edge is processed once. Space: O(V) for the visited array and the queue (queue can hold up to V vertices in worst case), plus O(V) for the result vector.
