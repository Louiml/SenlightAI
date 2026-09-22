// Write a C++ function named `hasNegativeWeightCycle` that takes an integer `n` (the number of vertices in a directed graph, labeled from 0 to n-1) and a vector of edges, where each edge is represented as a vector of three integers `{u, v, weight}`. The function should return `true` if the graph contains at least one negative-weight cycle, and `false` otherwise. The graph may be disconnected, may contain self-loops, parallel edges, and edges with zero or positive weights. Vertices that are unreachable from vertex 0 should be ignored for the initial path relaxation, but they may still participate in negative cycles. Assume the input graph is finite and each edge weight is within the range of a 32-bit signed integer (excluding `INT_MAX` itself). Implement the Bellman-Ford algorithm to detect negative cycles.
#include <cassert>
#include <vector>

// The solution function is already included above (in a full program it would be linked).

int main() {
    // No negative cycle, simple triangle.
    std::vector<std::vector<int>> edges1 = {{0,1,5},{1,2,3},{2,0,2}};
    assert(hasNegativeWeightCycle(3, edges1) == false);

    // Negative cycle of length 1 (self-loop).
    std::vector<std::vector<int>> edges2 = {{0,0,-1}};
    assert(hasNegativeWeightCycle(1, edges2) == true);

    // Negative cycle reachable from 0: 0->1 (1), 1->2 (-5), 2->1 (1) gives cycle -4.
    std::vector<std::vector<int>> edges3 = {{0,1,1},{1,2,-5},{2,1,1},{2,0,10}};
    assert(hasNegativeWeightCycle(3, edges3) == true);

    // Disconnected graph: vertex 0 isolated, negative cycle on vertex 1-2 unreachable from 0.
    std::vector<std::vector<int>> edges4 = {{1,2,-2},{2,1,-2}};
    assert(hasNegativeWeightCycle(3, edges4) == false);

    // No edges at all.
    std::vector<std::vector<int>> edges5 = {};
    assert(hasNegativeWeightCycle(3, edges5) == false);

    // Positive cycle only.
    std::vector<std::vector<int>> edges6 = {{0,1,1},{1,2,2},{2,0,3}};
    assert(hasNegativeWeightCycle(3, edges6) == false);

    // Negative edge but no cycle.
    std::vector<std::vector<int>> edges7 = {{0,1,-2},{1,2,3}};
    assert(hasNegativeWeightCycle(3, edges7) == false);

    // Parallel edges, one creating a negative cycle via two-step.
    std::vector<std::vector<int>> edges8 = {{0,1,10},{0,1,-5},{1,0,-1}};
    // Cycle 0->1 (-5) then 1->0 (-1) = -6, negative cycle.
    assert(hasNegativeWeightCycle(2, edges8) == true);

    // Vertex 0 has no outgoing edges, but there is a negative cycle reachable? No, not reachable.
    std::vector<std::vector<int>> edges9 = {{0,0,0},{1,2,-1},{2,1,-1}};
    assert(hasNegativeWeightCycle(3, edges9) == false);

    // Negative cycle involving vertex 0 directly.
    std::vector<std::vector<int>> edges10 = {{0,2,4},{2,0,-3},{2,1,2},{1,2,-1}};
    // Cycle 0->2 (4) then 2->0 (-3) = 1, not negative; cycle 1->2 (-1) then 2->1 (2)=1; no negative cycle.
    assert(hasNegativeWeightCycle(3, edges10) == false);

    return 0;
}
#include <vector>
#include <limits>
#include <cstddef>

// Returns true if the graph contains a negative-weight cycle reachable from vertex 0.
bool hasNegativeWeightCycle(int n, const std::vector<std::vector<int>>& edges) {
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(n, INF);
    dist[0] = 0;

    const std::size_t edgeCount = edges.size();

    // Perform n relaxation passes. The n-th pass detects negative cycles.
    for (int pass = 0; pass < n; ++pass) {
        bool relaxed = false;
        for (std::size_t j = 0; j < edgeCount; ++j) {
            int u = edges[j][0];
            int v = edges[j][1];
            int weight = edges[j][2];
            // Only relax if u is reachable and the sum doesn't overflow.
            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                relaxed = true;
            }
        }
        // If this is the last pass (pass == n-1) and we relaxed, negative cycle exists.
        if (pass == n - 1 && relaxed) {
            return true;
        }
        // Optional early exit: if no relaxation in early passes, no negative cycle.
        if (!relaxed && pass < n - 1) {
            break; // Early termination: distances converged, no cycle.
        }
    }
    return false;
}
// The standard approach is to run Bellman-Ford from a single source (vertex 0) to compute the shortest distances to all reachable vertices. Initialize `dist[0] = 0` and all other distances to `INT_MAX`. Then perform exactly `n` relaxation passes over all edges. In each pass, for every edge `(u, v, w)`, if `dist[u]` is not `INT_MAX` and `dist[v] > dist[u] + w`, update `dist[v]`. After `n-1` passes, the shortest paths (if no negative cycle exists) would have converged. A negative cycle exists if and only if a relaxation is still possible during the `n`-th pass. Therefore, set a flag `relaxed` to false at the start of each pass, set it true whenever any edge relaxes, and after the `n`-th pass return whether `relaxed` is true. Important edge cases: (1) Unreachable vertices from vertex 0 have `INT_MAX` distance; skip them to avoid overflow when adding weights, since `INT_MAX + negative` would underflow. (2) Self-loops with negative weight directly form a negative cycle of length 1 and will be detected because `dist[u] > dist[u] + w` when `w < 0`. (3) If the graph has no vertices reachable from 0, no relaxations occur, and the function returns false (unless there is a negative cycle entirely unreachable from 0, which Bellman-Ford from a single source would not detect; this task limits detection to cycles reachable from 0, which matches the given snippet's behavior). Time complexity is O(n * E) where E is the number of edges; space complexity is O(n) for the distance array.
