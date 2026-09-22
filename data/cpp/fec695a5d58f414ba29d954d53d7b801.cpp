Write a C++ function `vector<int> bfsShortestPaths(int node, int source, const vector<vector<int>>& adjacency)` that, given an undirected unweighted graph with `node` vertices labeled 0 to `node-1`, a source vertex `source`, and an adjacency matrix (where `adjacency[i][j]` is 1 if there is an edge between i and j, otherwise 0), returns a vector of length `node` where the i-th element is the shortest path distance (number of edges) from `source` to vertex i, and -1 if vertex i is unreachable. The function should not modify the input adjacency matrix.

// The problem is a standard Breadth-First Search (BFS) on an unweighted graph because BFS explores vertices in order of increasing distance from the source. Start by initializing a distance array of size `node` with -1, marking all vertices as unvisited. Set the distance of the source to 0 and push it into a queue. While the queue is not empty, pop the front vertex `u`. For each neighbor `v` (where `adjacency[u][v] == 1`), if `v` has not been visited (i.e., `dist[v] == -1`), set `dist[v] = dist[u] + 1` and push `v` into the queue. This ensures that the first time a vertex is reached, it is via the shortest path. Important edge cases: the source may be unreachable to some vertices (those remain -1); the adjacency matrix may be symmetric, but the code only reads it; the graph could be disconnected; there could be self-loops (but the code treats them as edges, which would not affect distances because the source is already visited; self-loops from other vertices are skipped). Time complexity is O(node^2) because we iterate over all `node` columns for each popped vertex, which is O(node) per vertex, and there are at most `node` vertices, so O(node^2). Space complexity is O(node) for the distance array and queue, plus O(node^2) for the input matrix that is passed by reference.

#include <vector>
#include <queue>
#include <algorithm>

// Compute shortest path distances from a source vertex in an undirected unweighted graph.
// adjacency is an NxN matrix where adjacency[i][j] == 1 indicates an edge.
// Returns a vector of distances; -1 means unreachable.
std::vector<int> bfsShortestPaths(int node, int source, const std::vector<std::vector<int>>& adjacency) {
    std::vector<int> dist(node, -1);
    std::queue<int> q;

    dist[source] = 0;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < node; ++v) {
            // Check if there is an edge and v is unvisited
            if (adjacency[u][v] == 1 && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist;
}

#include <cassert>
#include <vector>

// Assume the solution function is defined above.
int main() {
    // Test 1: Simple path 0-1-2
    std::vector<std::vector<int>> adj1 = {
        {0,1,0},
        {1,0,1},
        {0,1,0}
    };
    auto d1 = bfsShortestPaths(3, 0, adj1);
    assert(d1[0] == 0);
    assert(d1[1] == 1);
    assert(d1[2] == 2);

    // Test 2: Disconnected graph, source 0
    std::vector<std::vector<int>> adj2 = {
        {0,1,0,0},
        {1,0,0,0},
        {0,0,0,1},
        {0,0,1,0}
    };
    auto d2 = bfsShortestPaths(4, 0, adj2);
    assert(d2[0] == 0);
    assert(d2[1] == 1);
    assert(d2[2] == -1);
    assert(d2[3] == -1);

    // Test 3: Fully connected (complete graph) with 4 vertices
    std::vector<std::vector<int>> adj3(4, std::vector<int>(4,1));
    // Zero out diagonal (no self loops)
    for (int i = 0; i < 4; ++i) adj3[i][i] = 0;
    auto d3 = bfsShortestPaths(4, 2, adj3);
    assert(d3[0] == 1);
    assert(d3[1] == 1);
    assert(d3[2] == 0);
    assert(d3[3] == 1);

    // Test 4: Single vertex graph
    std::vector<std::vector<int>> adj4 = {{0}};
    auto d4 = bfsShortestPaths(1, 0, adj4);
    assert(d4[0] == 0);

    // Test 5: Two isolated vertices
    std::vector<std::vector<int>> adj5 = {
        {0,0},
        {0,0}
    };
    auto d5 = bfsShortestPaths(2, 0, adj5);
    assert(d5[0] == 0);
    assert(d5[1] == -1);

    // Test 6: Larger graph with multiple paths, BFS should get shortest
    std::vector<std::vector<int>> adj6 = {
        {0,1,1,0,0},
        {1,0,0,1,0},
        {1,0,0,1,1},
        {0,1,1,0,1},
        {0,0,1,1,0}
    };
    auto d6 = bfsShortestPaths(5, 0, adj6);
    // Distances: 0->0=0, 0->1=1, 0->2=1, 0->3=2 (via 1 or 2), 0->4=2 (via 2 or 3)
    assert(d6[0] == 0);
    assert(d6[1] == 1);
    assert(d6[2] == 1);
    assert(d6[3] == 2);
    assert(d6[4] == 2);

    // Test 7: Source is not vertex 0 and graph is a simple line
    std::vector<std::vector<int>> adj7 = {
        {0,1,0,0},
        {1,0,1,0},
        {0,1,0,1},
        {0,0,1,0}
    };
    auto d7 = bfsShortestPaths(4, 1, adj7);
    assert(d7[0] == 1);
    assert(d7[1] == 0);
    assert(d7[2] == 1);
    assert(d7[3] == 2);

    // Test 8: Graph with self-loop (should not cause issues)
    std::vector<std::vector<int>> adj8 = {
        {1,1,0},
        {1,1,1},
        {0,1,1}
    };
    auto d8 = bfsShortestPaths(3, 0, adj8);
    assert(d8[0] == 0);
    assert(d8[1] == 1);
    assert(d8[2] == 2);

    // Test 9: Empty graph connectivity (all zeros) with multiple vertices
    std::vector<std::vector<int>> adj9 = {
        {0,0,0},
        {0,0,0},
        {0,0,0}
    };
    auto d9 = bfsShortestPaths(3, 2, adj9);
    assert(d9[0] == -1);
    assert(d9[1] == -1);
    assert(d9[2] == 0);

    return 0;
}
