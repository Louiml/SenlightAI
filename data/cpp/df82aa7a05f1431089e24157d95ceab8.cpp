// Given a sequence of directed edges representing a simplified cargo transportation network, where each edge connects two nodes (identified by non-negative integers) and has a non-negative integer capacity, write a C++ function named `computeMaxFlow` that returns the maximum amount of flow that can be sent from a specified source node to a specified sink node. The function must implement the Edmonds-Karp algorithm (BFS-based Ford-Fulkerson) and handle multiple edges between the same pair of nodes by merging their capacities. Nodes are numbered from 0 to N-1, where N is inferred from the maximum node ID present in the edge list plus one. The input consists of a vector of triples `(from, to, capacity)`, the source node ID, and the sink node ID. The output is a single integer representing the maximum flow value. If no path exists, the result is 0. The solution must be self-contained, using only standard C++ headers, and must not rely on any external libraries beyond the standard template library.
#include <cassert>
#include <tuple>
#include <vector>

// The solution function is assumed to be declared above.
// (In a real test, include the solution header or paste the function here.)

int main() {
    // Simple chain: 0 -> 1 -> 2, capacities 3 and 2.
    std::vector<std::tuple<int, int, int>> edges1 = {{0,1,3}, {1,2,2}};
    assert(computeMaxFlow(edges1, 0, 2) == 2);

    // Parallel edges: 0->1 twice with capacities 5 and 7, 1->2 with 4.
    std::vector<std::tuple<int, int, int>> edges2 = {{0,1,5}, {0,1,7}, {1,2,4}};
    assert(computeMaxFlow(edges2, 0, 2) == 4); // bottleneck is 4.

    // No path from source to sink.
    std::vector<std::tuple<int, int, int>> edges3 = {{0,1,5}, {2,3,1}};
    assert(computeMaxFlow(edges3, 0, 3) == 0);

    // Source == sink should be 0 (degenerate case).
    assert(computeMaxFlow(edges1, 1, 1) == 0);

    // Larger network: source 0, sink 3.
    // Edges: 0->1 (10), 0->2 (5), 1->2 (15), 1->3 (10), 2->3 (10)
    std::vector<std::tuple<int, int, int>> edges4 = {{0,1,10}, {0,2,5}, {1,2,15}, {1,3,10}, {2,3,10}};
    assert(computeMaxFlow(edges4, 0, 3) == 15); // Two paths: 0-1-3 (10) and 0-2-3 (5)

    // Self-loop should be ignored: 0->0 with 100, and 0->1 with 5.
    std::vector<std::tuple<int, int, int>> edges5 = {{0,0,100}, {0,1,5}};
    assert(computeMaxFlow(edges5, 0, 1) == 5);

    // Multiple edges, including reverse direction (0->1 and 1->0) - should be independent.
    std::vector<std::tuple<int, int, int>> edges6 = {{0,1,3}, {1,0,2}};
    assert(computeMaxFlow(edges6, 0, 1) == 3); // Only forward edge counts for 0->1 flow.

    // Empty edge list.
    std::vector<std::tuple<int, int, int>> edges7;
    assert(computeMaxFlow(edges7, 0, 1) == 0); // N=0, returns 0.

    // Single node with no edges, source==sink.
    assert(computeMaxFlow(edges7, 0, 0) == 0);

    return 0;
}
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

/**
 * Compute the maximum flow from 'source' to 'sink' in a directed graph
 * described by edges (from, to, capacity). Parallel edges are merged by
 * summing capacities. Uses Edmonds-Karp (BFS-based augmenting path).
 *
 * @param edges List of triples (from, to, capacity). Node IDs are non-negative.
 * @param source Source node ID.
 * @param sink   Sink node ID.
 * @return Maximum flow value as an integer.
 */
int computeMaxFlow(const std::vector<std::tuple<int, int, int>> &edges, int source, int sink) {
    // Determine number of nodes: max node ID + 1.
    int max_node = -1;
    for (const auto &e : edges) {
        max_node = std::max(max_node, std::max(std::get<0>(e), std::get<1>(e)));
    }
    int N = (max_node < 0) ? 0 : max_node + 1;
    if (N == 0 || source == sink) return 0;

    // Build adjacency matrix for merged capacities.
    std::vector<std::vector<int>> capacity(N, std::vector<int>(N, 0));
    for (const auto &[u, v, cap] : edges) {
        capacity[u][v] += cap;
    }

    // Residual capacities (we modify a copy of capacity).
    std::vector<std::vector<int>> residual = capacity;

    int max_flow = 0;
    const int INF = std::numeric_limits<int>::max();

    // BFS to find augmenting path in residual graph.
    while (true) {
        std::vector<int> parent(N, -1);
        std::vector<bool> visited(N, false);
        std::queue<int> q;
        q.push(source);
        visited[source] = true;

        while (!q.empty() && !visited[sink]) {
            int u = q.front(); q.pop();
            for (int v = 0; v < N; ++v) {
                if (!visited[v] && residual[u][v] > 0) {
                    visited[v] = true;
                    parent[v] = u;
                    if (v == sink) break;
                    q.push(v);
                }
            }
        }

        if (!visited[sink]) break; // No more augmenting paths.

        // Find bottleneck (minimum residual capacity) on the found path.
        int bottleneck = INF;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            bottleneck = std::min(bottleneck, residual[u][v]);
        }

        // Augment flow along the path.
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residual[u][v] -= bottleneck;
            residual[v][u] += bottleneck; // Reverse edge for residual graph.
        }

        max_flow += bottleneck;
    }

    return max_flow;
}
// The problem is a standard maximum flow problem where the network may have parallel edges (multiple edges between the same pair of nodes) that must be combined into a single edge with summed capacity. The Edmonds-Karp algorithm uses BFS to find shortest augmenting paths (in terms of number of edges) in the residual graph, repeatedly augmenting flow along these paths until no more augmenting paths exist.
//
// **Algorithm:**
// 1. Determine the number of nodes N as `max_node_id + 1` (or 0 if no edges, but source/sink will still be valid IDs).
// 2. Build an adjacency matrix `capacity[N][N]` (or a vector of vectors) initialized to 0. For each edge `(u, v, cap)`, add `cap` to `capacity[u][v]` to merge parallel edges.
// 3. Initialize `max_flow = 0`.
// 4. While BFS from source to sink in the residual graph (where for each pair (u,v), residual capacity is `capacity[u][v] - flow[u][v]`) finds a path:
//    - Track the parent of each visited node and the bottleneck capacity (minimum residual capacity) along the path.
//    - Augment flow along the path: for each edge (u,v) in the path, add bottleneck to flow[u][v] and subtract from flow[v][u] (or equivalently maintain residual capacities directly).
//    - Add bottleneck to max_flow.
// 5. Return max_flow.
//
// **Edge cases:**
// - No edges: flow is 0.
// - Multiple parallel edges: merged capacities.
// - Self-loops (edge from node to itself): they do not affect flow, but merging requires adding to capacity[u][u]; BFS should ignore self-loops since they never improve the path (but merging does not hurt).
// - Source equals sink: maximum flow is infinite in theory, but for this task we treat it as 0 since no movement needed; the BFS would find a path of length 0, but we should handle by returning 0 (or the "infinite" value, but to be safe return 0 as per typical convention for no-flow-required). The task does not specify, so we return 0 to keep it simple.
// - Disconnected source/sink: BFS fails, return 0.
// - Large capacities may overflow int? We use `long long` to be safe, but the task likely expects `int`. We'll use `int` and note it in comments; but for robustness, we could use `int64_t`. We'll keep `int` for simplicity.
//
// **Complexity:**
// - Time: O(V * E^2) in worst case for Edmonds-Karp, where V is number of nodes and E is number of edges (after merging). For a typical competitive programming problem, this is acceptable for small graphs. BFS is O(V+E) and at most O(V*E) augmentations.
// - Space: O(V^2) for the adjacency matrix. For sparse graphs, adjacency list would be more efficient, but matrix simplifies merging.
//
// **Implementation details:**
// - Use `std::vector<std::vector<int>>` for capacities.
// - BFS uses a queue, a `parent` array, and computes bottleneck by traversing parent from sink to source.
// - Use `const` for input parameters where possible.
