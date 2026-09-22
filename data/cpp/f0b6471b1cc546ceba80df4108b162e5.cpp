Write a C++ function `std::vector<int> bfsTraversal(const std::vector<std::vector<int>>& adjList, int start)` that performs a breadth-first search (BFS) on an undirected graph represented as an adjacency list (where each inner vector contains the neighbor indices of a vertex). The graph has vertices numbered from `0` to `adjList.size() - 1`. The function should return a vector of vertex indices in the order they are first visited during BFS, starting from the given `start` vertex. The graph is guaranteed to be connected (i.e., all vertices are reachable from the start). Assume the input is valid: `adjList` is non-empty, `start` is within bounds, and there are no duplicate edges or self-loops. The traversal must visit neighbors in the exact order they appear in the adjacency list.
#include <cassert>
#include <vector>

// The solution function is defined above (include before this main).
// For testing, we assume it is included here.

int main() {
    // Test 1: Simple linear graph 0-1-2-3
    std::vector<std::vector<int>> g1 = {{1}, {0, 2}, {1, 3}, {2}};
    assert(bfsTraversal(g1, 0) == std::vector<int>({0, 1, 2, 3}));

    // Test 2: Star graph centered at 0 with leaves 1,2,3 (order given)
    std::vector<std::vector<int>> g2 = {{1, 2, 3}, {0}, {0}, {0}};
    assert(bfsTraversal(g2, 0) == std::vector<int>({0, 1, 2, 3}));

    // Test 3: Graph from the original snippet (0 to 9) starting at 0
    std::vector<std::vector<int>> g3 = {
        {1, 3},
        {0, 2, 4},
        {1, 5},
        {0, 4, 6},
        {1, 3, 5, 7},
        {2, 4, 8},
        {3, 7},
        {4, 6, 8},
        {5, 7},
        {}
    };
    // Note: Vertex 9 is isolated? In original snippet it had edges. But here we add no edges.
    // Actually original had 9 connected to 8? No, edges list didn't include 9 with others.
    // Let's correct to make connected: add edge 8-9.
    g3[8].push_back(9);
    g3[9].push_back(8);
    std::vector<int> expected3 = {0, 1, 3, 2, 4, 6, 5, 7, 8, 9};
    assert(bfsTraversal(g3, 0) == expected3);

    // Test 4: Single vertex graph
    std::vector<std::vector<int>> g4 = {{}};
    assert(bfsTraversal(g4, 0) == std::vector<int>({0}));

    // Test 5: Two vertices connected
    std::vector<std::vector<int>> g5 = {{1}, {0}};
    assert(bfsTraversal(g5, 1) == std::vector<int>({1, 0}));

    // Test 6: Graph where neighbor order matters
    std::vector<std::vector<int>> g6 = {{2, 1}, {0, 3}, {0, 3}, {1, 2}};
    // BFS from 0: visit 0, then 2 then 1, then from 2 visit 3, then from 1 visit nothing new
    assert(bfsTraversal(g6, 0) == std::vector<int>({0, 2, 1, 3}));

    return 0;
}
#include <vector>
#include <queue>
#include <cstddef>

// Perform breadth-first traversal starting from 'start' on an undirected graph.
// Returns vertex indices in visit order.
std::vector<int> bfsTraversal(const std::vector<std::vector<int>>& adjList, int start) {
    std::size_t n = adjList.size();
    std::vector<bool> visited(n, false);
    std::vector<int> result;
    std::queue<int> q;

    visited[start] = true;
    q.push(start);
    result.push_back(start);

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        // Visit neighbors in the order they appear in the adjacency list.
        for (int neighbor : adjList[cur]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
                result.push_back(neighbor);
            }
        }
    }

    return result;
}
// The algorithm uses a standard BFS approach: maintain a queue of vertices to visit, and a boolean array (or `std::unordered_set` for generality, but since vertices are integers from 0 to N-1, a vector of bool is efficient) to track visited vertices. Start by marking `start` as visited, enqueuing it, and adding it to the result. Then, while the queue is not empty, dequeue the front vertex, iterate through its adjacency list in order, and for each unvisited neighbor, mark it visited, enqueue it, and append it to the result. This ensures that vertices are discovered in increasing distance from the start, and within the same distance, in the order they appear in the adjacency lists. Edge cases: if the graph is connected, the loop will visit all vertices; if the start vertex has no neighbors (but graph is connected, so this only happens if the graph has one vertex), the result will contain just the start. Time complexity is O(V + E) where V is the number of vertices and E is the total number of edges, because each vertex is enqueued and dequeued at most once, and each edge is examined once from each endpoint. Space complexity is O(V) for the queue, visited array, and result vector.
