Given a set of \(N\) points in a 2D plane (coordinates provided as two arrays of \(N\) integers) and a list of \(M\) fixed edges that must be included in the final spanning tree, write a C++ function `computeMinimumSpanningCost(int N, const std::vector<int>& xs, const std::vector<int>& ys, const std::vector<std::pair<int,int>>& fixedEdges)` that returns a `double` equal to the total weight of a minimum spanning tree where each of the \(M\) fixed edges has weight 0.0, and all other pair of distinct points has weight equal to the Euclidean distance between them. The function must handle \(N \ge 1\), and the fixed edges may contain duplicate pairs (but they should not be double-counted in the MST cost). The graph is complete, and the result must be computed with Prim's algorithm, starting from vertex 0. Use `double` for all distances and the final answer. The function should be self-contained (no reliance on global state) and reusable across multiple calls.

The problem is a standard minimum spanning tree (MST) problem on a complete graph with \(N\) vertices. The weight of an edge between vertices \(u\) and \(v\) is the Euclidean distance \(\sqrt{(x_u - x_v)^2 + (y_u - y_v)^2}\), except that any edge listed in `fixedEdges` has weight 0.0 (representing already-built connections). Since the graph is complete, we can run Prim's algorithm directly. We initialize `minWeight[v]` to infinity for all vertices, except `minWeight[0] = 0.0`. In each iteration, we pick the unvisited vertex with the smallest `minWeight`, add that weight to the total, mark it visited, and then update `minWeight` for all unvisited neighbors: for every \(v\), the best candidate weight to connect \(v` is the minimum over all already visited vertices of the corresponding edge weight. To handle the fixed edges efficiently, we can precompute a symmetric weight matrix `dist[u][v]` where for fixed edges we set the weight to 0.0, otherwise compute Euclidean distance. Since the graph is complete, iterating over all \(v\) for each chosen vertex is \(O(N)\) per iteration, giving \(O(N^2)\) total time. The space is \(O(N^2)\) for the weight matrix. Edge cases: \(N=1\) returns 0.0; duplicate fixed edges should have weight 0.0 (no extra cost); the MST cost is always non-negative because all weights are non-negative. Use `double` and compare with a tolerance if necessary, but the test uses exact equality for small integers and simple distances, so standard `==` is acceptable after ensuring no floating point drift (we can round to 6 decimal places in tests if needed).

#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

// Compute the total weight of a minimum spanning tree of a complete graph
// with N points (xs, ys) and M fixed edges that have weight 0.0.
// Returns the sum of edge weights in the MST.
double computeMinimumSpanningCost(int N,
                                  const std::vector<int>& xs,
                                  const std::vector<int>& ys,
                                  const std::vector<std::pair<int,int>>& fixedEdges) {
    // Build the weight matrix: dist[u][v] = 0.0 if (u,v) is a fixed edge,
    // otherwise Euclidean distance.
    const double INF = std::numeric_limits<double>::infinity();
    std::vector<std::vector<double>> dist(N, std::vector<double>(N, INF));

    // Fill non-fixed edges with Euclidean distances.
    for (int u = 0; u < N; ++u) {
        for (int v = u + 1; v < N; ++v) {
            double dx = xs[u] - xs[v];
            double dy = ys[u] - ys[v];
            double d = std::sqrt(dx*dx + dy*dy);
            dist[u][v] = d;
            dist[v][u] = d;
        }
    }

    // Overwrite fixed edges with weight 0.0 (they are always included).
    for (const auto& edge : fixedEdges) {
        int u = edge.first;
        int v = edge.second;
        dist[u][v] = 0.0;
        dist[v][u] = 0.0;
    }

    // Prim's algorithm starting from vertex 0.
    std::vector<bool> added(N, false);
    std::vector<double> minWeight(N, INF);
    minWeight[0] = 0.0;
    double total = 0.0;

    for (int iter = 0; iter < N; ++iter) {
        // Find the unvisited vertex with the smallest minWeight.
        int u = -1;
        for (int v = 0; v < N; ++v) {
            if (!added[v] && (u == -1 || minWeight[v] < minWeight[u])) {
                u = v;
            }
        }

        total += minWeight[u];
        added[u] = true;

        // Update minWeight for all unvisited neighbors (complete graph).
        for (int v = 0; v < N; ++v) {
            if (!added[v] && dist[u][v] < minWeight[v]) {
                minWeight[v] = dist[u][v];
            }
        }
    }

    return total;
}

#include <cassert>
#include <cmath>

int main() {
    // Test 1: Single point -> MST cost = 0
    {
        int N = 1;
        std::vector<int> xs = {0};
        std::vector<int> ys = {0};
        std::vector<std::pair<int,int>> edges;
        double ans = computeMinimumSpanningCost(N, xs, ys, edges);
        assert(std::fabs(ans - 0.0) < 1e-9);
    }

    // Test 2: Two points, distance 3 (triangle with coordinates (0,0) and (3,0))
    {
        int N = 2;
        std::vector<int> xs = {0, 3};
        std::vector<int> ys = {0, 0};
        std::vector<std::pair<int,int>> edges;
        double ans = computeMinimumSpanningCost(N, xs, ys, edges);
        assert(std::fabs(ans - 3.0) < 1e-9);
    }

    // Test 3: Three points forming equilateral triangle side 2 (distance sqrt(2^2+0^2)=2? actually (0,0), (2,0), (1,sqrt(3)) gives sides 2)
    // Use simple coordinates: (0,0), (2,0), (0,2) -> distances 2, 2, sqrt(8) ~ 2.828 => MST chooses two edges of 2 each -> total 4
    {
        int N = 3;
        std::vector<int> xs = {0, 2, 0};
        std::vector<int> ys = {0, 0, 2};
        std::vector<std::pair<int,int>> edges;
        double ans = computeMinimumSpanningCost(N, xs, ys, edges);
        assert(std::fabs(ans - 4.0) < 1e-9);
    }

    // Test 4: Fixed edge forces 0 cost, reducing total
    {
        int N = 3;
        std::vector<int> xs = {0, 3, 0};
        std::vector<int> ys = {0, 0, 4};
        // Distances: (0,1)=3, (0,2)=4, (1,2)=5 -> without fixed edges, MST would pick 3 and 4 => total 7
        // With fixed edge (0,1) and (0,2) both zero, MST cost = 0 (all connected)
        std::vector<std::pair<int,int>> edges = {{0,1}, {0,2}};
        double ans = computeMinimumSpanningCost(N, xs, ys, edges);
        assert(std::fabs(ans - 0.0) < 1e-9);
    }

    // Test 5: Fixed edge reduces cost compared to no fixed edges
    {
        int N = 3;
        std::vector<int> xs = {0, 3, 0};
        std::vector<int> ys = {0, 0, 4};
        // Without fixed: MST total 7. With fixed (0,1): still need to connect vertex 2, best edge is (0,2) cost 4 -> total 4
        std::vector<std::pair<int,int>> edges = {{0,1}};
        double ans = computeMinimumSpanningCost(N, xs, ys, edges);
        assert(std::fabs(ans - 4.0) < 1e-9);
    }

    // Test 6: Duplicate fixed edges should not affect cost
    {
        int N = 2;
        std::vector<int> xs = {0, 5};
        std::vector<int> ys = {0, 0};
        std::vector<std::pair<int,int>> edges = {{0,1}, {0,1}};
        double ans = computeMinimumSpanningCost(N, xs, ys, edges);
        assert(std::fabs(ans - 0.0) < 1e-9);
    }

    // Test 7: Four points, all pairwise distances (0,0), (1,0), (0,1), (1,1) -> MST total 3 (edges: (0,1)=1, (0,2)=1, (1,3)=1)
    {
        int N = 4;
        std::vector<int> xs = {0, 1, 0, 1};
        std::vector<int> ys = {0, 0, 1, 1};
        std::vector<std::pair<int,int>> edges;
        double ans = computeMinimumSpanningCost(N, xs, ys, edges);
        assert(std::fabs(ans - 3.0) < 1e-9);
    }

    // Test 8: Fixed edge makes a cycle? Since MST is a tree, adding a fixed edge doesn't force a cycle, but we just use it.
    // Simple: N=2 with fixed edge -> cost 0
    {
        int N = 2;
        std::vector<int> xs = {0, 10};
        std::vector<int> ys = {0, 0};
        std::vector<std::pair<int,int>> edges = {{0,1}};
        double ans = computeMinimumSpanningCost(N, xs, ys, edges);
        assert(std::fabs(ans - 0.0) < 1e-9);
    }

    // Test 9: Larger test, random but simple line (0,0),(1,1),(2,2) -> distances sqrt(2), sqrt(2) ~ 1.414 each -> total ~2.828
    {
        int N = 3;
        std::vector<int> xs = {0, 1, 2};
        std::vector<int> ys = {0, 1, 2};
        std::vector<std::pair<int,int>> edges;
        double ans = computeMinimumSpanningCost(N, xs, ys, edges);
        double expected = 2 * std::sqrt(2.0);
        assert(std::fabs(ans - expected) < 1e-9);
    }

    // Test 10: All points same coordinates -> distances 0
    {
        int N = 5;
        std::vector<int> xs(N, 3);
        std::vector<int> ys(N, 4);
        std::vector<std::pair<int,int>> edges;
        double ans = computeMinimumSpanningCost(N, xs, ys, edges);
        assert(std::fabs(ans - 0.0) < 1e-9);
    }

    return 0;
}
