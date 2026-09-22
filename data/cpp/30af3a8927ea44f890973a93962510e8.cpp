Write a C++ function that takes a fixed-size 9x9 adjacency matrix (representing an undirected weighted graph with vertices numbered 0 through 8) and a source vertex, and returns a `std::vector<std::pair<int,int>>` containing the shortest-path distances from the source to every other vertex. The vector must be ordered by vertex index (index 0 corresponds to vertex 0, index 1 to vertex 1, etc.), and each pair should be `(vertex, distance)`. The graph is guaranteed to be connected (every vertex reachable from any source). The adjacency matrix uses 0 to indicate no direct edge, and positive integers for weights. Write the function as `std::vector<std::pair<int,int>> shortestDistances(const int graph[9][9], int src)`. No `main` function should be included in the solution; only the function implementation.
The correct algorithm is Dijkstra's algorithm, which computes single-source shortest paths in a weighted graph with non-negative weights. The provided snippet incorrectly implements Prim's algorithm (for minimum spanning tree) while labeling it as Dijkstra, so we must implement the actual Dijkstra. We initialize a distance array of size 9, setting `dist[src] = 0` and all other distances to infinity. We also maintain a boolean array `visited` to mark vertices whose shortest distance is finalized. In each of the 9 iterations, we select the unvisited vertex with the minimum tentative distance (using a linear scan, which is fine for 9 vertices). We mark it visited, and for each neighbor `v` where `graph[u][v] != 0`, we relax the edge: if `dist[u] + graph[u][v] < dist[v]`, update `dist[v]`. Edge cases: if `src` is out of range (0-8), we can return an empty vector or handle gracefully; since the problem guarantees valid input, we assume it. Also, because the graph is connected, all distances will be finite after the algorithm. Time complexity: \(O(V^2)\) since we do a linear scan for the minimum in each of V iterations, plus relaxation over all edges (worst-case O(V^2) checks per iteration), giving \(O(V^2)\) overall. Space complexity: \(O(V)\) for the distance and visited arrays, plus the output vector of size V-1. The algorithm correctly handles zero-weight edges (though not present here) and avoids revisiting finalized vertices.
#include <vector>
#include <limits>
#include <algorithm>

// Compute shortest distances from source vertex in a 9-vertex undirected weighted graph.
// graph is adjacency matrix; 0 means no edge. Returns vector of (vertex, distance) for all vertices except source.
std::vector<std::pair<int,int>> shortestDistances(const int graph[9][9], int src) {
    const int V = 9;
    const int INF = std::numeric_limits<int>::max();
    
    std::vector<int> dist(V, INF);
    std::vector<bool> visited(V, false);
    
    dist[src] = 0;
    
    for (int count = 0; count < V - 1; ++count) {
        // Find unvisited vertex with minimum distance
        int u = -1;
        int minDist = INF;
        for (int i = 0; i < V; ++i) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        
        if (u == -1) break; // No reachable unvisited vertex
        
        visited[u] = true;
        
        // Relax edges from u
        for (int v = 0; v < V; ++v) {
            if (graph[u][v] != 0 && !visited[v]) {
                int newDist = dist[u] + graph[u][v];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                }
            }
        }
    }
    
    std::vector<std::pair<int,int>> result;
    for (int i = 0; i < V; ++i) {
        if (i != src) {
            result.push_back({i, dist[i]});
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be included before this main.

int main() {
    int graph[9][9] = {
        {0, 4, 0, 0, 0, 0, 0, 8, 0},
        {4, 0, 8, 0, 0, 0, 0, 11, 0},
        {0, 8, 0, 7, 0, 4, 0, 0, 2},
        {0, 0, 7, 0, 9, 14, 0, 0, 0},
        {0, 0, 0, 9, 0, 10, 0, 0, 0},
        {0, 0, 4, 14, 10, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 2, 0, 1, 6},
        {8, 11, 0, 0, 0, 0, 1, 0, 7},
        {0, 0, 2, 0, 0, 0, 6, 7, 0}
    };
    
    // Test from source 0
    auto result0 = shortestDistances(graph, 0);
    // Expected: (1,4), (2,12), (3,19), (4,21), (5,11), (6,9), (7,8), (8,14)
    assert(result0.size() == 8);
    assert(result0[0] == std::make_pair(1, 4));
    assert(result0[1] == std::make_pair(2, 12));
    assert(result0[2] == std::make_pair(3, 19));
    assert(result0[3] == std::make_pair(4, 21));
    assert(result0[4] == std::make_pair(5, 11));
    assert(result0[5] == std::make_pair(6, 9));
    assert(result0[6] == std::make_pair(7, 8));
    assert(result0[7] == std::make_pair(8, 14));
    
    // Test from source 4
    auto result4 = shortestDistances(graph, 4);
    // Expected: (0,21), (1,17), (2,9), (3,9), (5,10), (6,12), (7,13), (8,11)
    assert(result4.size() == 8);
    assert(result4[0] == std::make_pair(0, 21));
    assert(result4[1] == std::make_pair(1, 17));
    assert(result4[2] == std::make_pair(2, 9));
    assert(result4[3] == std::make_pair(3, 9));
    assert(result4[4] == std::make_pair(5, 10));
    assert(result4[5] == std::make_pair(6, 12));
    assert(result4[6] == std::make_pair(7, 13));
    assert(result4[7] == std::make_pair(8, 11));
    
    return 0;
}
