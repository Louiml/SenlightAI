Write a C++ function that, given an undirected graph represented by its number of vertices `n` (vertices numbered 0 to n-1), an edge list, and a starting vertex `src`, returns the number of vertices in the connected component that contains `src`. The graph may be disconnected, may contain isolated vertices (vertices with no edges), and may contain self-loops or parallel edges (which should be ignored for counting purposes). You may assume `n` is a positive integer and all edge endpoints are valid vertex indices. Your function should accept the graph as a vector of vectors (adjacency list) and should not modify the input.
The problem is a standard connected-component size computation using Breadth-First Search (BFS). We initialize a visited array of size `n` (all false). We enqueue the source vertex, mark it visited, and set a counter to 0. Then we repeatedly dequeue a vertex, increment the counter, and for each neighbor, if it hasn’t been visited, mark it visited and enqueue it. The BFS continues until the queue is empty. Since BFS explores the entire connected component reachable from the source, the counter at the end equals the component size. Edge cases: if `src` is isolated (no edges), the queue processes only that vertex, returning 1. Self-loops and parallel edges are handled naturally because visited checks prevent double-counting and re-processing. If `n` is large, we must avoid recursion (BFS is iterative) to prevent stack overflow. Time complexity is O(V + E) where V is the number of vertices and E is the number of edges in the component (or total edges if we scan adjacency lists). Space complexity is O(V) for the visited array and queue.
#include <vector>
#include <queue>

// Count the number of vertices in the connected component of 'src'
// in an undirected graph given as an adjacency list.
// The graph is not modified. Vertices are 0..n-1.
int connectedComponentSize(const std::vector<std::vector<int>>& adj, int src) {
    int n = static_cast<int>(adj.size());
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    
    q.push(src);
    visited[src] = true;
    int componentSize = 0;
    
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        ++componentSize;
        
        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    
    return componentSize;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple connected graph with 4 vertices and 3 edges
    std::vector<std::vector<int>> graph1 = {{1,2},{0,3},{0},{1}};
    assert(connectedComponentSize(graph1, 0) == 4);
    assert(connectedComponentSize(graph1, 1) == 4);
    assert(connectedComponentSize(graph1, 3) == 4);

    // Test 2: Disconnected graph with isolated vertex
    std::vector<std::vector<int>> graph2 = {{1},{0},{},{}};
    assert(connectedComponentSize(graph2, 0) == 2);
    assert(connectedComponentSize(graph2, 2) == 1);
    assert(connectedComponentSize(graph2, 3) == 1);

    // Test 3: Single vertex no edges
    std::vector<std::vector<int>> graph3 = {{}};
    assert(connectedComponentSize(graph3, 0) == 1);

    // Test 4: Self-loop and parallel edges
    std::vector<std::vector<int>> graph4 = {{1,1,0},{2,0},{1}};
    assert(connectedComponentSize(graph4, 0) == 3);
    assert(connectedComponentSize(graph4, 1) == 3);
    assert(connectedComponentSize(graph4, 2) == 3);

    // Test 5: Two separate components
    std::vector<std::vector<int>> graph5 = {{1},{0},{3},{2}};
    assert(connectedComponentSize(graph5, 0) == 2);
    assert(connectedComponentSize(graph5, 1) == 2);
    assert(connectedComponentSize(graph5, 2) == 2);
    assert(connectedComponentSize(graph5, 3) == 2);

    return 0;
}
