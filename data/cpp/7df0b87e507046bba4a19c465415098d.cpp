Write a C++ function `connectedComponentSizeAndVertices(int numVertices, const std::vector<std::pair<int,int>>& edges)` that returns a `std::pair<int, std::vector<int>>` where the first element is the number of vertices in the connected component containing vertex `1` (vertices are numbered from `1` to `numVertices`), and the second element is a sorted vector of the vertex numbers (1-indexed) in that component. The input graph is undirected and unweighted. The function should handle the case where the graph has zero edges (each vertex is its own component), and also handle cases where the graph may be disconnected or contain cycles. The vertices in the returned vector must be in ascending order. The graph is assumed to have at least one vertex, and edges are given as pairs of distinct vertices (no self-loops). The function must not modify its inputs.
The problem reduces to depth-first search (DFS) or breadth-first search (BFS) starting from vertex 1. We need to traverse all vertices reachable from vertex 1 through edges. Since the graph is represented by an edge list, we can build an adjacency list (or an adjacency matrix if easier, but adjacency list is more efficient for sparse graphs). Start DFS from vertex 1, mark visited vertices, and count them. Also collect the visited vertex numbers. Because the traversal visits vertices in the order of adjacency, to guarantee the result vector is sorted, either collect them and then sort, or use a `std::set` to store visited vertices, which naturally maintains order. Time complexity: Building adjacency list takes O(V+E), DFS visits each vertex and edge once, so O(V+E). Sorting the collected vertices takes O(K log K) where K is the component size, but since K ≤ V, overall O(V+E + V log V) if we sort, or we can avoid sorting by using an ordered set with O(K log K) insertion. For simplicity, we'll collect and sort. Space complexity: O(V+E) for adjacency list and visited array, plus O(K) for the result.

Edge cases: (1) Single vertex, no edges → component size 1, contains vertex 1. (2) Vertex 1 isolated but other connected components exist → size 1, contains only vertex 1. (3) Directed? No, undirected. (4) Self-loops? Problem states edges are between distinct vertices, but we can still handle self-loops defensively (they don't affect connectivity). (5) Disconnected graph where vertex 1 is part of a component with several vertices.
#include <vector>
#include <algorithm>
#include <utility>

// Returns the number of vertices in the connected component containing vertex 1
// and the sorted list of vertex numbers in that component (1-indexed).
std::pair<int, std::vector<int>> connectedComponentSizeAndVertices(
    int numVertices,
    const std::vector<std::pair<int, int>>& edges) {
    
    // Build adjacency list (1-indexed vertices internally)
    std::vector<std::vector<int>> adj(numVertices + 1);
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    std::vector<bool> visited(numVertices + 1, false);
    std::vector<int> componentVertices;
    
    // Depth-first search from vertex 1 using recursion
    std::function<void(int)> dfs = [&](int node) {
        visited[node] = true;
        componentVertices.push_back(node);
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfs(neighbor);
            }
        }
    };
    
    dfs(1);
    
    // Sort the collected vertex numbers
    std::sort(componentVertices.begin(), componentVertices.end());
    
    return {static_cast<int>(componentVertices.size()), componentVertices};
}
#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is defined above (or included here)

int main() {
    // Test 1: Simple connected graph with 4 vertices
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        auto result = connectedComponentSizeAndVertices(n, edges);
        assert(result.first == 4);
        assert(result.second == std::vector<int>({1,2,3,4}));
    }
    
    // Test 2: Disconnected graph, vertex 1 isolated
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges = {{2,3},{4,5}};
        auto result = connectedComponentSizeAndVertices(n, edges);
        assert(result.first == 1);
        assert(result.second == std::vector<int>({1}));
    }
    
    // Test 3: Vertex 1 connected to a subset, other component exists
    {
        int n = 6;
        std::vector<std::pair<int,int>> edges = {{1,3},{1,5},{3,5},{2,4},{2,6}};
        auto result = connectedComponentSizeAndVertices(n, edges);
        assert(result.first == 3);
        assert(result.second == std::vector<int>({1,3,5}));
    }
    
    // Test 4: Cycle with vertex 1
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
        auto result = connectedComponentSizeAndVertices(n, edges);
        assert(result.first == 3);
        assert(result.second == std::vector<int>({1,2,3}));
    }
    
    // Test 5: Single vertex, no edges
    {
        int n = 1;
        std::vector<std::pair<int,int>> edges = {};
        auto result = connectedComponentSizeAndVertices(n, edges);
        assert(result.first == 1);
        assert(result.second == std::vector<int>({1}));
    }
    
    // Test 6: Edges with non-sorted input, multiple edges (parallel edges)
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{2,1},{3,1},{1,2},{4,3},{3,4}};
        auto result = connectedComponentSizeAndVertices(n, edges);
        assert(result.first == 4);
        assert(result.second == std::vector<int>({1,2,3,4}));
    }
    
    // Test 7: Vertex 1 connected to a chain but not to all
    {
        int n = 7;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{4,5},{5,6},{6,7}};
        auto result = connectedComponentSizeAndVertices(n, edges);
        assert(result.first == 3);
        assert(result.second == std::vector<int>({1,2,3}));
    }
    
    return 0;
}
