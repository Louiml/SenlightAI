Given a directed graph with integer edge weights (which may be negative, but with no negative cycles) represented as an adjacency matrix, implement a function `floydWarshallReachability` that computes for all pairs of vertices `(i, j)` the length of the shortest path from `i` to `j`, and also returns a boolean matrix indicating reachability (i.e., whether a path exists). If no path exists, the distance should be reported as `std::numeric_limits<int>::max()`. The input is an `n x n` matrix where `matrix[i][j]` is the direct edge weight from `i` to `j`, or `INF` (a large constant like `1e9`) if there is no direct edge. The function must handle self-loops and zero-weight edges correctly, and must work for any `n >= 1`. The output should be a pair: first the all-pairs shortest distance matrix, second an all-pairs reachability matrix (bool). The implementation must be self-contained (no external graph libraries) and use the Floyd–Warshall algorithm.
#include <cassert>
#include <vector>

int main() {
    const int INF = 1000000000;

    // Test 1: Simple 3-node graph.
    std::vector<std::vector<int>> g1 = {
        {0, 2, INF},
        {INF, 0, 3},
        {INF, INF, 0}
    };
    auto res1 = floydWarshallReachability(g1);
    assert(res1.first[0][2] == 5);
    assert(res1.first[1][0] == INF);
    assert(res1.second[0][2] == true);
    assert(res1.second[1][0] == false);

    // Test 2: Disconnected nodes.
    std::vector<std::vector<int>> g2 = {
        {0, INF},
        {INF, 0}
    };
    auto res2 = floydWarshallReachability(g2);
    assert(res2.first[0][1] == INF);
    assert(res2.second[0][1] == false);
    assert(res2.second[0][0] == true);

    // Test 3: Negative edge (no negative cycle).
    std::vector<std::vector<int>> g3 = {
        {0, 5, INF},
        {INF, 0, -2},
        {INF, INF, 0}
    };
    auto res3 = floydWarshallReachability(g3);
    assert(res3.first[0][2] == 3);
    assert(res3.second[0][2] == true);

    // Test 4: Single node.
    std::vector<std::vector<int>> g4 = {{0}};
    auto res4 = floydWarshallReachability(g4);
    assert(res4.first[0][0] == 0);
    assert(res4.second[0][0] == true);

    // Test 5: Zero-weight edge.
    std::vector<std::vector<int>> g5 = {
        {0, 0, INF},
        {INF, 0, 0},
        {INF, INF, 0}
    };
    auto res5 = floydWarshallReachability(g5);
    assert(res5.first[0][2] == 0);
    assert(res5.second[0][2] == true);

    // Test 6: Self-loop with positive weight doesn't hurt.
    std::vector<std::vector<int>> g6 = {
        {0, 1},
        {1, 0}
    };
    auto res6 = floydWarshallReachability(g6);
    assert(res6.first[0][0] == 0);
    assert(res6.first[0][1] == 1);
    assert(res6.second[0][0] == true);

    // Test 7: Larger graph with multiple paths.
    std::vector<std::vector<int>> g7 = {
        {0, 3, INF, 7},
        {8, 0, 2, INF},
        {5, INF, 0, 1},
        {2, INF, INF, 0}
    };
    auto res7 = floydWarshallReachability(g7);
    // From 0 to 2: direct? no, but via 1: 0->1 (3) + 1->2 (2) = 5, or via 3: 0->3 (7)+3->2? no edge, so 5.
    assert(res7.first[0][2] == 5);
    // From 1 to 0: direct 8, or via 2->0: 2+5=7, so 7.
    assert(res7.first[1][0] == 7);
    // All are reachable.
    assert(res7.second[0][3] == true);
    assert(res7.second[1][3] == true);

    return 0;
}
#include <vector>
#include <limits>
#include <algorithm>

// Computes all-pairs shortest path distances and reachability using Floyd-Warshall.
// Input: n x n matrix where g[i][j] is edge weight or INF if no direct edge.
// Returns: pair of matrices: (dist, reachable), where dist[i][j] is shortest
//          path length (or INF if unreachable) and reachable[i][j] is true iff
//          there is a path from i to j.
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<bool>>>
floydWarshallReachability(const std::vector<std::vector<int>>& g) {
    const int n = static_cast<int>(g.size());
    const int INF = std::numeric_limits<int>::max() / 4; // avoid overflow

    // Initialize distance matrix with given edges, 0 on diagonal.
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, INF));
    for (int i = 0; i < n; ++i) {
        dist[i][i] = 0;
        for (int j = 0; j < n; ++j) {
            if (i != j && g[i][j] < INF) {
                dist[i][j] = g[i][j];
            }
        }
    }

    // Floyd-Warshall main loop.
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            if (dist[i][k] == INF) continue; // optimization
            for (int j = 0; j < n; ++j) {
                if (dist[k][j] == INF) continue; // optimization
                int candidate = dist[i][k] + dist[k][j];
                if (candidate < dist[i][j]) {
                    dist[i][j] = candidate;
                }
            }
        }
    }

    // Build reachability matrix.
    std::vector<std::vector<bool>> reachable(n, std::vector<bool>(n, false));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            reachable[i][j] = (dist[i][j] < INF);
        }
    }

    return {std::move(dist), std::move(reachable)};
}
// The Floyd–Warshall algorithm is a dynamic programming approach that computes shortest paths between all pairs of vertices in \(O(n^3)\) time and \(O(n^2)\) space. The core idea is to iteratively consider each vertex `k` as an intermediate point. For every pair `(i, j)`, we update the distance from `i` to `j` as the minimum of its current distance and the sum of distances from `i` to `k` and `k` to `j`. To handle large distances safely (avoiding integer overflow), we use a sentinel value for infinity (e.g., `INF = 1e9`) and check before addition that both `dist[i][k]` and `dist[k][j]` are not infinite. Reachability is derived from the final distances: `reachable[i][j] = (dist[i][j] < INF)`. Edge cases include: (1) `n == 1` where the only pair is the vertex itself; if `matrix[0][0]` is 0 (or non-negative), distance is 0 and reachable is true; (2) negative edges but no negative cycles—Floyd–Warshall still works; (3) self-loops: a self-loop with positive weight doesn't affect shortest path (since we start with 0), but a negative self-loop would imply a negative cycle, which we assume doesn't occur; (4) unreachable pairs: distance remains `INF` and reachability false. The main loop runs `for k`, `for i`, `for j`, giving \(O(n^3)\) time. Space is \(O(n^2)\) for the distance matrix (and another \(O(n^2)\) for reachability if we store separately, but we can reuse the distance matrix). We must ensure that diagonal entries `dist[i][i]` are set to 0 initially (since path of length 0 exists).
