// Write a C++ function `int networkDelayTime(const std::vector<std::vector<int>>& times, int n, int k)` that computes the time it takes for a signal to reach all `n` nodes in a network. The network is represented as a directed weighted graph where each element of `times` is `{u, v, w}` meaning a signal sent from node `u` reaches node `v` after `w` milliseconds. The signal starts at node `k` (1-indexed) at time 0. If a node cannot be reached, return -1; otherwise return the minimum time required for the signal to reach every node (the maximum shortest-path distance from `k` to any reachable node). All node labels are in the range `[1, n]`, edge weights are non-negative, and the graph may contain cycles or multiple edges.

The problem is a classic single-source shortest path problem on a directed graph with non-negative edge weights, best solved using Dijkstra's algorithm. We initialize a distance array `dist` of size `n` with a large sentinel value (e.g., `1e9`), set `dist[k-1] = 0`, and push `{0, k-1}` into a min-heap priority queue. While the heap is non-empty, we pop the node with the smallest accumulated time. If the popped time is greater than the current best distance (which can happen due to stale heap entries), we skip processing. Otherwise, for each outgoing edge `(v, w)` from the current node, if `time + w < dist[v]`, we update `dist[v]` and push `{dist[v], v}`. After the loop, the answer is the maximum value in `dist`. If that maximum is still the sentinel, at least one node is unreachable, so return -1. Edge cases include: `n == 1` (answer is 0 if `k` is the only node, or -1 if graph is empty but `k` still refers to a node—but here `k` must be valid, and since there's only one node it's always reachable), disconnected components, and negative weights (not in constraints, so Dijkstra is safe). Time complexity is O((V+E) log V) due to heap operations, where V = n and E = number of edges. Space complexity is O(V + E) for adjacency list and O(V) for the distance array and heap.

#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

// Compute the time for a signal to reach all nodes in a directed weighted graph.
// Returns -1 if any node is unreachable from the source node k.
int networkDelayTime(const std::vector<std::vector<int>>& times, int n, int k) {
    // Build adjacency list: node -> (neighbor, weight)
    std::vector<std::vector<std::pair<int, int>>> adj(n);
    for (const auto& edge : times) {
        int u = edge[0] - 1;
        int v = edge[1] - 1;
        int w = edge[2];
        adj[u].push_back({v, w});
    }

    // Min-heap: {accumulated time, node}
    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>> pq;

    const int INF = 1e9;
    std::vector<int> dist(n, INF);
    dist[k - 1] = 0;
    pq.push({0, k - 1});

    while (!pq.empty()) {
        int time = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        // Stale entry: skip
        if (time > dist[node]) continue;

        for (const auto& [neighbor, weight] : adj[node]) {
            int newTime = time + weight;
            if (newTime < dist[neighbor]) {
                dist[neighbor] = newTime;
                pq.push({newTime, neighbor});
            }
        }
    }

    int maxTime = *std::max_element(dist.begin(), dist.end());
    return (maxTime == INF) ? -1 : maxTime;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic case from problem statement
    std::vector<std::vector<int>> times1 = {{2,1,1}, {2,3,1}, {3,4,1}};
    assert(networkDelayTime(times1, 4, 2) == 2);

    // Test 2: Single node
    std::vector<std::vector<int>> times2 = {};
    assert(networkDelayTime(times2, 1, 1) == 0);

    // Test 3: Unreachable node
    std::vector<std::vector<int>> times3 = {{1,2,1}};
    assert(networkDelayTime(times3, 3, 1) == -1);

    // Test 4: Disconnected graph but source reaches all reachable only
    std::vector<std::vector<int>> times4 = {{1,2,2}, {2,3,5}, {1,3,10}};
    assert(networkDelayTime(times4, 3, 1) == 7); // 1->2->3 is faster than direct

    // Test 5: Multiple edges and cycles
    std::vector<std::vector<int>> times5 = {{1,2,1}, {2,1,3}, {2,3,1}, {3,2,2}};
    assert(networkDelayTime(times5, 3, 1) == 2);

    // Test 6: Larger network with parallel edges
    std::vector<std::vector<int>> times6 = {{1,2,3}, {1,2,1}, {2,3,4}, {3,4,2}};
    assert(networkDelayTime(times6, 4, 1) == 7); // 1->2 (1) + 2->3 (4) + 3->4 (2)

    // Test 7: Source cannot reach itself (but node labels are valid, so this should still work)
    std::vector<std::vector<int>> times7 = {{1,1,5}}; // self-loop
    assert(networkDelayTime(times7, 2, 1) == -1); // node 2 unreachable

    return 0;
}
