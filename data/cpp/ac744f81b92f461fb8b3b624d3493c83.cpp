// Write a C++ function that, given a directed weighted graph represented as an adjacency list, a source vertex, and a destination vertex, returns the shortest path from source to destination as a vector of vertex indices (inclusive of both endpoints). The function must detect negative-weight cycles reachable from the source and return an empty vector if one exists. The graph is provided as a `vector<vector<array<int,2>>>` where index `u` holds edges `{v, w}` meaning an edge from `u` to `v` with weight `w`. Vertices are 0-indexed. The graph may contain up to 1000 vertices and 10000 edges, with weights in the range [-1000, 1000]. If no path exists between source and destination, return an empty vector. The function signature must be: `vector<int> shortestPath(const vector<vector<array<int,2>>>& graph, int src, int dst)`. Use the Bellman-Ford algorithm.
// The solution uses Bellman-Ford because it handles negative edge weights (unlike Dijkstra) and can detect negative cycles. The algorithm initializes all distances to infinity except the source (0). Then it relaxes all edges `V-1` times, where `V` is the number of vertices. After that, it checks all edges once more: if any edge can still be relaxed (i.e., `dist[u] + w < dist[v]`), then a negative cycle exists, and the function returns an empty vector (since the shortest path is undefined).
//
// To reconstruct the path, we track a predecessor array `parent` that stores the vertex from which each vertex was optimally reached. After Bellman-Ford completes (without negative cycle), we start from `dst`, repeatedly follow `parent` until we reach `src` (or until predecessor becomes -1). Since the problem guarantees a path exists if the predecessor chain reaches src, we reverse the collected vertices to get the correct order. Important edge cases: if `src == dst`, the path is just `{src}`; if there is no path (i.e., `dist[dst]` remains infinity), return empty vector; if a negative cycle is reachable from source, return empty vector. Time complexity is O(V·E), space complexity is O(V) for distances and predecessors, plus O(path length) for the output.
#include <vector>
#include <array>
#include <limits>
#include <algorithm>

// Returns shortest path from src to dst, or empty vector if negative cycle or unreachable.
std::vector<int> shortestPath(const std::vector<std::vector<std::array<int,2>>>& graph, int src, int dst) {
    const int V = static_cast<int>(graph.size());
    const int INF = std::numeric_limits<int>::max();

    std::vector<int> dist(V, INF);
    std::vector<int> parent(V, -1);
    dist[src] = 0;

    // Relax all edges V-1 times
    for (int i = 0; i < V - 1; ++i) {
        bool updated = false;
        for (int u = 0; u < V; ++u) {
            if (dist[u] == INF) continue;
            for (const auto& edge : graph[u]) {
                int v = edge[0];
                int w = edge[1];
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    updated = true;
                }
            }
        }
        if (!updated) break; // early exit if no changes
    }

    // Check for negative cycles
    for (int u = 0; u < V; ++u) {
        if (dist[u] == INF) continue;
        for (const auto& edge : graph[u]) {
            int v = edge[0];
            int w = edge[1];
            if (dist[v] > dist[u] + w) {
                return {}; // negative cycle reachable from src
            }
        }
    }

    if (dist[dst] == INF) return {}; // unreachable

    // Reconstruct path from dst back to src
    std::vector<int> path;
    for (int cur = dst; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
        if (cur == src) break;
    }
    std::reverse(path.begin(), path.end());
    if (path.front() != src) return {}; // no path to src
    return path;
}
#include <cassert>
#include <vector>
#include <array>

std::vector<int> shortestPath(const std::vector<std::vector<std::array<int,2>>>& graph, int src, int dst);

int main() {
    // Test 1: Simple positive-weight graph
    std::vector<std::vector<std::array<int,2>>> g1(3);
    g1[0] = {{1, 1}, {2, 5}};
    g1[1] = {{2, 1}};
    assert(shortestPath(g1, 0, 2) == std::vector<int>({0, 1, 2}));

    // Test 2: Negative edge but no cycle
    std::vector<std::vector<std::array<int,2>>> g2(3);
    g2[0] = {{1, 2}};
    g2[1] = {{2, -3}};
    assert(shortestPath(g2, 0, 2) == std::vector<int>({0, 1, 2}));

    // Test 3: Negative cycle reachable
    std::vector<std::vector<std::array<int,2>>> g3(2);
    g3[0] = {{1, -1}};
    g3[1] = {{0, -1}};
    assert(shortestPath(g3, 0, 0).empty());

    // Test 4: Unreachable destination
    std::vector<std::vector<std::array<int,2>>> g4(2);
    g4[0] = {{1, 5}};
    assert(shortestPath(g4, 1, 0).empty());

    // Test 5: src == dst
    std::vector<std::vector<std::array<int,2>>> g5(1);
    assert(shortestPath(g5, 0, 0) == std::vector<int>({0}));

    // Test 6: Graph from prompt (0->2 shortest path is 0->1->2 = 6+5=11, but 0->3->2 = 7-3=4)
    std::vector<std::vector<std::array<int,2>>> g6(5);
    g6[0] = {{1, 6}, {3, 7}};
    g6[1] = {{2, 5}, {3, 8}, {4, -4}};
    g6[2] = {{1, -2}};
    g6[3] = {{2, -3}, {4, 9}};
    g6[4] = {{0, 2}, {2, 7}};
    assert(shortestPath(g6, 0, 2) == std::vector<int>({0, 3, 2}));

    // Test 7: Multiple possible paths, choose shortest
    std::vector<std::vector<std::array<int,2>>> g7(4);
    g7[0] = {{1, 10}, {2, 2}};
    g7[1] = {{3, 1}};
    g7[2] = {{3, 5}};
    assert(shortestPath(g7, 0, 3) == std::vector<int>({0, 2, 3}));

    // Test 8: Negative cycle not reachable from src (should still work)
    std::vector<std::vector<std::array<int,2>>> g8(3);
    g8[0] = {{1, 1}};
    g8[1] = {{2, 1}};
    g8[2] = {{2, -1}}; // self-loop negative on vertex 2, but unreachable from 0? Actually reachable, so expect empty.
    // Let's make it unreachable: vertex 2 only connected to itself
    std::vector<std::vector<std::array<int,2>>> g8b(3);
    g8b[0] = {{1, 1}};
    g8b[1] = {{0, 1}}; // positive cycle
    g8b[2] = {{2, -1}}; // negative self-loop on 2
    assert(shortestPath(g8b, 0, 1) == std::vector<int>({0, 1}));
    assert(shortestPath(g8b, 0, 2).empty()); // unreachable

    return 0;
}
