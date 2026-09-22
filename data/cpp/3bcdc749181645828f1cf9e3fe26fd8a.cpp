/*
Write a C++ function named `breadthFirstTraversalOrder` that takes a non-negative integer `V` (number of vertices), a vector of pairs of integers `edges` (each pair represents an undirected edge between two vertices, with valid vertex indices in `[0, V-1]`), and a starting vertex `source`. The function should return a `std::vector<int>` containing the order in which vertices are visited during a breadth-first search (BFS) starting at `source`. The graph may contain cycles and disconnected vertices; vertices that are unreachable from `source` should not appear in the result. The adjacency list should be built in the order the edges are provided, and neighbors should be visited in that insertion order. The function must handle empty edge lists, a graph with a single vertex, edges that create loops (self-cycles), and duplicate edges gracefully (duplicates should be treated as separate entries in the adjacency list, but BFS visits each vertex once). Do not modify the input parameters; ensure the function is `const`-correct where applicable (e.g., use `const` references for inputs).
*/

#include <vector>
#include <queue>

// Return BFS traversal order starting from 'source' in an undirected graph.
// Vertices are 0-indexed. Unreachable vertices are not included.
std::vector<int> breadthFirstTraversalOrder(
    int V,
    const std::vector<std::pair<int, int>>& edges,
    int source)
{
    std::vector<int> result;
    if (V <= 0 || source < 0 || source >= V) {
        return result;
    }

    // Build adjacency list preserving edge insertion order.
    std::vector<std::vector<int>> adj(V);
    for (const auto& [u, v] : edges) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    std::vector<bool> visited(V, false);
    std::queue<int> q;

    visited[source] = true;
    q.push(source);

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
#include <utility>

// The solution function is assumed to be defined above (or included here).

int main() {
    // Basic graph from the snippet: 4 vertices, edges given.
    std::vector<std::pair<int, int>> edges1 = {{0,1},{0,2},{1,2},{2,0},{2,3},{3,3}};
    std::vector<int> result1 = breadthFirstTraversalOrder(4, edges1, 2);
    std::vector<int> expected1 = {2, 0, 3, 1}; // BFS from 2 visits 2, then 0,3, then 1 (depending on order)
    assert(result1 == expected1);

    // Single vertex, no edges.
    std::vector<std::pair<int, int>> edges2;
    std::vector<int> result2 = breadthFirstTraversalOrder(1, edges2, 0);
    assert(result2 == std::vector<int>{0});

    // Disconnected vertices: 3 vertices, only edge 0-1, start at 2.
    std::vector<std::pair<int, int>> edges3 = {{0,1}};
    std::vector<int> result3 = breadthFirstTraversalOrder(3, edges3, 2);
    assert(result3 == std::vector<int>{2});

    // Self-loop and duplicate edges: 2 vertices, edges (0,0), (0,1), (1,0), (0,1).
    std::vector<std::pair<int, int>> edges4 = {{0,0},{0,1},{1,0},{0,1}};
    std::vector<int> result4 = breadthFirstTraversalOrder(2, edges4, 0);
    std::vector<int> expected4 = {0, 1}; // 0 first, then 1 (since duplicate 0s are ignored after visited)
    assert(result4 == expected4);

    // Empty graph with V=0.
    std::vector<int> result5 = breadthFirstTraversalOrder(0, edges2, 0);
    assert(result5.empty());

    // Invalid source (out of range).
    std::vector<int> result6 = breadthFirstTraversalOrder(3, edges3, 5);
    assert(result6.empty());

    // Line graph: 0-1-2, start at 1.
    std::vector<std::pair<int, int>> edges7 = {{0,1},{1,2}};
    std::vector<int> result7 = breadthFirstTraversalOrder(3, edges7, 1);
    std::vector<int> expected7 = {1, 0, 2};
    assert(result7 == expected7);

    return 0;
}

// The solution uses the standard BFS algorithm. First, build an adjacency list as a `std::vector<std::vector<int>>` of size `V`, where for each undirected edge `(u, v)`, we push `v` to `adj[u]` and `u` to `adj[v]` in the order given. Then, maintain a `std::vector<bool> visited` of size `V` initialized to `false`. Use a queue (e.g., `std::queue<int>`) to process vertices. Mark `source` as visited, enqueue it, and then repeatedly dequeue a vertex, append it to the result vector, and for each neighbor in `adj[current]`, if it hasn't been visited, mark it visited and enqueue it. Important edge cases: if `V` is 0, return an empty vector; if `source` is out of range (not in `[0, V-1]`), return an empty vector (or handle robustly); if there are self-loops, they are treated as any other neighbor, but since we mark visited immediately, they won't cause re-enqueue; disconnected vertices are simply never visited. Time complexity: building adjacency is O(E) where E is number of edges, BFS visits each vertex and each edge once, so O(V+E). Space complexity: O(V+E) for adjacency list plus O(V) for visited and queue.
