You are given an undirected graph with `n` nodes, numbered from 1 to `n`. The graph is connected and has no self-loops or multiple edges. Write a C++ function `int farthestNodeCount(int n, const vector<vector<int>>& edges)` that, given the number of nodes and a list of undirected edges (`edges[i]` is a vector of two integers `[a, b]` meaning `a` and `b` are connected), returns the number of nodes that are at the maximum shortest distance from node 1. That is, find the largest shortest-path distance from node 1 to any other node, then count how many nodes (excluding node 1 itself) are at that exact maximum distance. For example, if the farthest distance is 3 and exactly 4 nodes are at distance 3 from node 1, the answer is 4. The input graph will have at least one edge, and `n >= 2`.
// The problem reduces to finding the shortest distances from a single source (node 1) in an unweighted undirected graph. Use Dijkstra's algorithm with a priority queue (or BFS since edge weights are uniform; here we follow the snippet's Dijkstra approach). Initialize a distance vector with a large value (e.g., `INT_MAX`), set `distance[1] = 0`, and use a min-heap (or priority queue with negative distances to simulate min-heap) to process nodes in order of increasing distance. For each popped node, if a shorter distance is found for a neighbor, update it and push the neighbor. After computing all distances, find the maximum value among `distance[2..n]` and count how many entries equal that maximum. Edge cases: graph is connected, so all nodes are reachable; node 1 itself is excluded from the count; duplicates or large distances are handled automatically. Time complexity is `O((n + E) log n)` due to priority queue operations, and space complexity is `O(n + E)` for the adjacency list and distance array.
#include <vector>
#include <queue>
#include <limits>

// Returns the number of nodes (excluding node 1) at the maximum shortest distance from node 1.
int farthestNodeCount(int n, const std::vector<std::vector<int>>& edges) {
    // Build adjacency list (1-indexed)
    std::vector<std::vector<int>> graph(n + 1);
    for (const auto& edge : edges) {
        graph[edge[0]].push_back(edge[1]);
        graph[edge[1]].push_back(edge[0]);
    }

    // Dijkstra from node 1
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(n + 1, INF);
    dist[1] = 0;

    // Min-heap: (distance, node)
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
    pq.push({0, 1});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;  // stale entry

        for (int v : graph[u]) {
            if (dist[v] > d + 1) {
                dist[v] = d + 1;
                pq.push({dist[v], v});
            }
        }
    }

    // Find maximum distance among nodes 2..n and count occurrences
    int maxDist = 0;
    for (int i = 2; i <= n; ++i) {
        if (dist[i] > maxDist) maxDist = dist[i];
    }

    int count = 0;
    for (int i = 2; i <= n; ++i) {
        if (dist[i] == maxDist) ++count;
    }
    return count;
}
#include <cassert>
#include <vector>

int main() {
    // Simple chain: 1-2-3-4, farthest is node 4 at distance 3 (only 1 node)
    assert(farthestNodeCount(4, {{1,2},{2,3},{3,4}}) == 1);

    // Star: center 1 connected to 2,3,4,5 -> all at distance 1, max distance = 1, count = 4
    assert(farthestNodeCount(5, {{1,2},{1,3},{1,4},{1,5}}) == 4);

    // Two farthest nodes: 1-2-3 and 1-2-4? Actually 1 connected to 2, 2 to 3 and 4 -> max dist=2, nodes 3 and 4 both at distance 2
    assert(farthestNodeCount(4, {{1,2},{2,3},{2,4}}) == 2);

    // Single edge: n=2, 1-2, max distance=1, count=1
    assert(farthestNodeCount(2, {{1,2}}) == 1);

    // Slightly larger: 1-2, 2-3, 3-4, and 1-5, 5-4 -> distances: node2=1, node3=2, node4=2, node5=1. max=2, count=2 (nodes 3 and 4)
    assert(farthestNodeCount(5, {{1,2},{2,3},{3,4},{1,5},{5,4}}) == 2);

    // All nodes directly connected to 1 except one extra branch
    // 1 connected to 2,3,4; also 2-5; plus 4-6-7 -> distances: 2:1,3:1,4:1,5:2,6:2,7:3 -> max=3, count=1
    assert(farthestNodeCount(7, {{1,2},{1,3},{1,4},{2,5},{4,6},{6,7}}) == 1);

    // Duplicate edge check (though problem says none, robust)
    assert(farthestNodeCount(3, {{1,2},{2,3},{1,2}}) == 1); // still a chain

    // Already a cycle: 1-2-3-1 plus 3-4 -> distances: 2:1,3:1,4:2 -> max=2, count=1
    assert(farthestNodeCount(4, {{1,2},{2,3},{3,1},{3,4}}) == 1);

    // Two separate branches of different lengths: 1-2-3-4 (dist3) and 1-5-6 (dist2) -> max=3, count=1
    assert(farthestNodeCount(6, {{1,2},{2,3},{3,4},{1,5},{5,6}}) == 1);

    // Balanced: 1-2-3 and 1-4-5 -> both deepest at distance2, count=2 (nodes 3 and 5)
    assert(farthestNodeCount(5, {{1,2},{2,3},{1,4},{4,5}}) == 2);

    return 0;
}
