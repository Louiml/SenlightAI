/*
Write a C++ function `std::string bellmanFordShortestPath(const std::vector<Edge>& edges, int vertexCount, int source)` that, given a weighted directed graph represented by a vector of `Edge` structs (each containing `src`, `dst`, `wt`), returns a formatted string of the shortest path costs from the source node to every other node (nodes are labeled `1` through `vertexCount`). The graph may contain negative-weight edges, but the function must detect any negative-weight cycle reachable from the source. If a negative cycle exists, return the exact string `"Negative cycle detected"`. Otherwise, for each node `i` from 1 to `vertexCount` excluding the source, output one line in the format `"<source> -> <i> Cost: <minimum distance>"` in increasing order of `i`. All distances are integers; if a node is unreachable from the source, output its cost as `-1`. The function must handle edge cases: multiple edges between the same pair, self-loops, zero-weight cycles, and an empty graph (with `vertexCount >= 1`). You may assume the source is always a valid node (1 ≤ source ≤ vertexCount), and the input vector may contain duplicate or directed edges in any order.
*/
#include <string>
#include <vector>
#include <limits>
#include <sstream>

struct Edge {
    int src;
    int dst;
    int wt;
};

// Returns a string with shortest path costs from source to all nodes,
// or "Negative cycle detected" if a negative cycle is reachable.
std::string bellmanFordShortestPath(const std::vector<Edge>& edges,
                                    int vertexCount,
                                    int source) {
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(vertexCount + 1, INF);
    dist[source] = 0;

    // Relax all edges V-1 times
    for (int i = 0; i < vertexCount - 1; ++i) {
        bool changed = false;
        for (const Edge& e : edges) {
            if (dist[e.src] != INF &&
                dist[e.src] + e.wt < dist[e.dst]) {
                dist[e.dst] = dist[e.src] + e.wt;
                changed = true;
            }
        }
        if (!changed) break;
    }

    // Check for negative cycle
    for (const Edge& e : edges) {
        if (dist[e.src] != INF &&
            dist[e.src] + e.wt < dist[e.dst]) {
            return "Negative cycle detected";
        }
    }

    // Build the result string
    std::ostringstream oss;
    for (int v = 1; v <= vertexCount; ++v) {
        if (v == source) continue;
        int cost = (dist[v] == INF) ? -1 : dist[v];
        oss << source << " -> " << v << " Cost: " << cost << "\n";
    }
    return oss.str();
}
#include <cassert>
#include <string>
#include <vector>

// Assume Edge struct and bellmanFordShortestPath are defined above.
// For testing, include a copy of the solution or declare it.

int main() {
    // Test 1: Simple graph with positive weights
    std::vector<Edge> e1 = {{1,2,3}, {2,3,4}, {1,3,7}};
    assert(bellmanFordShortestPath(e1, 3, 1) == "1 -> 2 Cost: 3\n1 -> 3 Cost: 7\n");

    // Test 2: Negative edges but no cycle
    std::vector<Edge> e2 = {{1,2,-5}, {2,3,2}, {1,3,10}};
    assert(bellmanFordShortestPath(e2, 3, 1) == "1 -> 2 Cost: -5\n1 -> 3 Cost: -3\n");

    // Test 3: Negative cycle reachable
    std::vector<Edge> e3 = {{1,2,1}, {2,3,-2}, {3,2,1}};
    assert(bellmanFordShortestPath(e3, 3, 1) == "Negative cycle detected");

    // Test 4: Unreachable nodes
    std::vector<Edge> e4 = {{1,2,1}};
    assert(bellmanFordShortestPath(e4, 4, 1) == "1 -> 2 Cost: 1\n1 -> 3 Cost: -1\n1 -> 4 Cost: -1\n");

    // Test 5: Self-loop with zero weight
    std::vector<Edge> e5 = {{1,1,0}, {1,2,5}};
    assert(bellmanFordShortestPath(e5, 2, 1) == "1 -> 2 Cost: 5\n");

    // Test 6: Negative cycle not reachable from source (should not detect)
    std::vector<Edge> e6 = {{2,3,-1}, {3,2,-1}, {1,2,5}};
    assert(bellmanFordShortestPath(e6, 3, 1) == "1 -> 2 Cost: 5\n1 -> 3 Cost: -1\n");

    // Test 7: Empty edge list
    std::vector<Edge> e7 = {};
    assert(bellmanFordShortestPath(e7, 2, 2) == "2 -> 1 Cost: -1\n");

    // Test 8: Multiple edges and negative weights
    std::vector<Edge> e8 = {{1,2,10}, {1,2,-3}, {2,3,2}};
    assert(bellmanFordShortestPath(e8, 3, 1) == "1 -> 2 Cost: -3\n1 -> 3 Cost: -1\n");

    // Test 9: Source is the only node
    std::vector<Edge> e9 = {};
    assert(bellmanFordShortestPath(e9, 1, 1) == "");

    // Test 10: Graph with a two-node negative cycle
    std::vector<Edge> e10 = {{1,2,-1}, {2,1,-1}};
    assert(bellmanFordShortestPath(e10, 2, 1) == "Negative cycle detected");

    return 0;
}
// The problem requires implementing the Bellman-Ford algorithm with negative-cycle detection. The algorithm initializes a distance array `dist` to infinity (use a large sentinel like `INT_MAX` or `1e9`) and sets `dist[source] = 0`. Then, for `vertexCount - 1` iterations, it relaxes every edge: if `dist[u] + wt < dist[v]`, update `dist[v]`. After these passes, the distances are the minimum possible if no negative cycles exist. To detect a negative cycle, perform one more full pass over all edges; if any relaxation is still possible, a negative cycle exists and we return the error string. Since unreachable nodes remain at `INF`, we output `-1` for them. The algorithm correctly handles negative weights, self-loops, and multiple edges. Key edge cases: a negative cycle not reachable from the source should not trigger detection (the standard algorithm only detects cycles reachable via relaxation from source, which is correct); zero-weight cycles cause no change, so they are fine; an empty edge list still works, producing all unreachable except the source. Time complexity is O(V * E) for the relaxation loops plus O(E) for detection, so total O(V * E). Space complexity is O(V) for the distance array. The output is built as a string using `std::to_string` and newlines, ensuring deterministic order by node index.
