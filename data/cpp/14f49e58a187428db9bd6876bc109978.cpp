Given a complete weighted undirected graph on vertices labeled 1 through n (where n ≥ 2), with pairwise edge weights provided as input, write a C++ function `std::vector<long long> adjustedDistances(const std::vector<std::vector<long long>>& graph)` that returns, for each vertex, the sum of the shortest path distance from a designated "special pair" (the two vertices connected by the globally minimum-weight edge) plus `(n-1)` times that minimum edge weight. More precisely, after identifying the minimum edge weight `w_min` connecting vertices `a` and `b`, subtract `w_min` from every edge weight (so the min edge becomes zero). Then, for every other vertex `i`, reduce the weight of its edges to both `a` and `b` to at most twice the smallest incident edge weight of `i`. Compute the shortest path distances from both `a` and `b` simultaneously (initializing both distances to 0) in this modified graph. Return for each vertex `v` the value `dist[v] + (n-1) * w_min`.

// The solution identifies the globally minimum edge (w_min) between vertices a and b. Subtracting w_min from all edges preserves the relative shortest path structure (all path lengths reduce by exactly w_min per edge), making the minimum edge weight zero. Then, to ensure that vertices not adjacent to a or b can reach the special pair cheaply, we clamp each vertex's edges to a and b to at most twice its smallest incident edge weight; this is a heuristic optimization that does not break correctness because those edges can only improve distances. After this preprocessing, we run Dijkstra's algorithm or a simple O(n²) shortest-path relaxation from the two sources a and b simultaneously (both start with distance 0). Since the graph is complete and dense (n up to about 2000), the O(n²) Dijkstra implementation is appropriate. Processing n vertices, each relaxation loop over all neighbors costs O(n²) total. The final answer for each vertex is its shortest distance from either a or b plus (n-1) * w_min, which accounts for the original weights. Edge cases: when n = 2, the only edge is the minimum, and all distances become (n-1)*w_min. The graph is guaranteed complete and symmetric. Time complexity is O(n²) and space complexity is O(n²) for the adjacency matrix.

#include <vector>
#include <algorithm>
#include <limits>

// Given a complete weighted undirected graph (n >= 2) as an adjacency matrix,
// returns for each vertex v: dist[v] + (n-1) * w_min, where w_min is the
// minimum edge weight, and dist is the shortest path from the two vertices
// connected by that minimum edge, after applying the described preprocessing.
std::vector<long long> adjustedDistances(const std::vector<std::vector<long long>>& graph) {
    const long long INF = std::numeric_limits<long long>::max() / 4;
    const int n = static_cast<int>(graph.size());

    // Find the globally minimum edge and its endpoints.
    long long w_min = INF;
    int a = -1, b = -1;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (graph[i][j] < w_min) {
                w_min = graph[i][j];
                a = i;
                b = j;
            }
        }
    }

    // Copy graph and subtract w_min from every edge weight.
    std::vector<std::vector<long long>> mt = graph;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) mt[i][j] -= w_min;
        }
    }

    // For every vertex other than a and b, clamp edges to a and b to at most
    // twice the smallest incident edge of that vertex.
    for (int i = 0; i < n; ++i) {
        if (i == a || i == b) continue;
        long long mm = INF;
        for (int j = 0; j < n; ++j) {
            if (j == i) continue;
            mm = std::min(mm, mt[i][j]);
        }
        long long limit = 2 * mm;
        mt[i][a] = std::min(mt[i][a], limit);
        mt[i][b] = std::min(mt[i][b], limit);
        mt[a][i] = std::min(mt[a][i], limit);
        mt[b][i] = std::min(mt[b][i], limit);
    }

    // Dijkstra from both a and b simultaneously.
    std::vector<long long> dist(n, INF);
    std::vector<bool> vis(n, false);
    dist[a] = 0;
    dist[b] = 0;

    for (int t = 0; t < n; ++t) {
        // Find the unvisited vertex with the smallest distance.
        int u = -1;
        long long best = INF;
        for (int i = 0; i < n; ++i) {
            if (!vis[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        vis[u] = true;
        for (int v = 0; v < n; ++v) {
            if (!vis[v] && mt[u][v] < INF) {
                dist[v] = std::min(dist[v], dist[u] + mt[u][v]);
            }
        }
    }

    // Add (n-1) * w_min to each distance.
    std::vector<long long> result(n);
    long long add = static_cast<long long>(n - 1) * w_min;
    for (int i = 0; i < n; ++i) {
        result[i] = dist[i] + add;
    }
    return result;
}

#include <cassert>
#include <vector>

// The function to test
std::vector<long long> adjustedDistances(const std::vector<std::vector<long long>>& graph);

int main() {
    // Test 1: n = 2, single edge weight 5.
    {
        std::vector<std::vector<long long>> graph = {
            {0, 5},
            {5, 0}
        };
        auto res = adjustedDistances(graph);
        assert(res.size() == 2);
        assert(res[0] == 5);
        assert(res[1] == 5);
    }

    // Test 2: n = 3, triangle with weights 1, 2, 3.
    {
        std::vector<std::vector<long long>> graph = {
            {0, 1, 2},
            {1, 0, 3},
            {2, 3, 0}
        };
        auto res = adjustedDistances(graph);
        // Min edge = 1 (between 0-1). Subtract 1 from all edges -> edges: 0,1,2.
        // For vertex 2, smallest incident edge is 1 (to vertex 0), clamp edges to 0 and 1 to 2.
        // Dist from either 0 or 1: vertex 0 -> 0, vertex 1 -> 0, vertex 2 -> 2.
        // Add (n-1)*1 = 2 -> [2, 2, 4].
        assert(res[0] == 2);
        assert(res[1] == 2);
        assert(res[2] == 4);
    }

    // Test 3: n = 4, star where center 0 connects to others with weight 10, others connected with weight 100.
    {
        std::vector<std::vector<long long>> graph = {
            {0, 10, 10, 10},
            {10, 0, 100, 100},
            {10, 100, 0, 100},
            {10, 100, 100, 0}
        };
        auto res = adjustedDistances(graph);
        // Min edge = 10 (0-1). Subtract 10: edges from 0 become 0, others become 90.
        // For vertex 2: smallest incident edge is 0 (to 0), clamp edges to 0 and 1 to 0.
        // Dist from 0 or 1: vertex 0 -> 0, vertex 1 -> 0, vertex 2 -> 0 (via clamp), vertex 3 -> 0.
        // Add (n-1)*10 = 30 -> [30,30,30,30].
        assert(res[0] == 30);
        assert(res[1] == 30);
        assert(res[2] == 30);
        assert(res[3] == 30);
    }

    // Test 4: n = 4, all equal weights 7.
    {
        std::vector<std::vector<long long>> graph = {
            {0, 7, 7, 7},
            {7, 0, 7, 7},
            {7, 7, 0, 7},
            {7, 7, 7, 0}
        };
        auto res = adjustedDistances(graph);
        // w_min = 7, subtract -> all edges 0. Dist all 0. Add (n-1)*7 = 21.
        assert(res[0] == 21 && res[1] == 21 && res[2] == 21 && res[3] == 21);
    }

    // Test 5: n = 5, asymmetric but complete, ensure correctness.
    {
        std::vector<std::vector<long long>> graph = {
            {0, 3, 8, 9, 10},
            {3, 0, 4, 7, 6},
            {8, 4, 0, 5, 2},
            {9, 7, 5, 0, 1},
            {10, 6, 2, 1, 0}
        };
        auto res = adjustedDistances(graph);
        // Min edge = 1 between vertices 3 and 4. Subtract 1.
        // Original shortest distances from {3,4} after subtraction:
        // v0: to 4 via 2? Better: 0-1=3, 1-3=7 -> 10, but 0-2=8, 2-4=2 -> 10. After min edge 1 subtracted, edges become: 0-1=2, 1-3=6, 0-2=7, 2-4=1, etc.
        // Clamp for each non-special vertex: for v0, mm = min(2,7,8,9)=2, clamp edges to 3 and 4 to 4.
        // After clamping, dist from {3,4} to v0 is 4 (via direct clamp). So dist[0]=4.
        // For v1, mm = min(2,3,6,5)=2, clamp to 3 and 4 to 4. dist[1]=4.
        // For v2, mm = min(7,3,4,1)=1, clamp to 3 and 4 to 2. dist[2]=2.
        // Special v3=0, v4=0.
        // Add (n-1)*1 = 4 -> [8,8,6,4,4].
        assert(res[0] == 8);
        assert(res[1] == 8);
        assert(res[2] == 6);
        assert(res[3] == 4);
        assert(res[4] == 4);
    }

    return 0;
}
