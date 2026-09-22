Write a C++ function `int shortestDistance(int start, int end, const std::vector<std::vector<int>>& adjacencyList)` that returns the shortest number of edges in an unweighted directed graph. The graph has `n` nodes numbered from `0` to `n-1`, and each node `i` has a list of outgoing neighbors in `adjacencyList[i]`. The function should return the shortest path length from `start` to `end` using Breadth-First Search (BFS). If no path exists, return `-1`. The graph may contain cycles, and `start` and `end` can be any valid node indices (including the same node, in which case return `0`). The adjacency list may contain duplicate edges, which should be ignored (no need to optimize, just handle them naturally). Provide a self-contained implementation with proper headers and avoid using a `main` function in the solution.
// The problem is a classic shortest path in an unweighted graph, which is optimally solved using BFS. BFS explores nodes level by level: starting from `start`, we visit all nodes at distance 1 first, then distance 2, and so on. We maintain a queue of nodes to process and a distance array initialized to a large sentinel (e.g., `INT_MAX`). Set `distance[start] = 0` and push `start`. While the queue is non-empty, pop the front node `u`, and for each neighbor `v` in `adjacencyList[u]`, if `distance[v]` is still `INT_MAX`, set it to `distance[u] + 1` and push `v`. The first time we pop or set the `end` node, that distance is the shortest. If after BFS `distance[end]` remains `INT_MAX`, return `-1`. Special cases: if `start == end`, the distance is `0` (handled automatically by initialization). The graph may have cycles, but BFS visits each node at most once, so no infinite loop. Duplicate edges do not cause issues because once a node is visited, we ignore further visits. Complexity is `O(V + E)` time, where `V` is number of nodes and `E` is total number of edges (including duplicates), and `O(V)` auxiliary space for the distance array and queue.
#include <vector>
#include <queue>
#include <limits>

// Returns the shortest number of edges from 'start' to 'end' in a directed graph.
// The graph is given as an adjacency list. Returns -1 if no path exists.
int shortestDistance(int start, int end, const std::vector<std::vector<int>>& adjacencyList) {
    const int n = static_cast<int>(adjacencyList.size());
    if (start < 0 || start >= n || end < 0 || end >= n) {
        return -1; // Invalid indices
    }
    if (start == end) {
        return 0;
    }

    std::vector<int> distance(n, std::numeric_limits<int>::max());
    std::queue<int> q;

    distance[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adjacencyList[u]) {
            if (distance[v] == std::numeric_limits<int>::max()) {
                distance[v] = distance[u] + 1;
                if (v == end) {
                    return distance[v];
                }
                q.push(v);
            }
        }
    }

    return -1;
}
#include <cassert>
#include <vector>

// The function signature is from the solution; include it here or assume it's included.
int main() {
    // Simple linear graph: 0 -> 1 -> 2 -> 3
    std::vector<std::vector<int>> graph1 = {{1}, {2}, {3}, {}};
    assert(shortestDistance(0, 3, graph1) == 3);
    assert(shortestDistance(0, 1, graph1) == 1);
    assert(shortestDistance(3, 0, graph1) == -1); // no reverse edge

    // Same node
    assert(shortestDistance(2, 2, graph1) == 0);

    // Graph with cycle and multiple paths
    std::vector<std::vector<int>> graph2 = {{1, 2}, {2, 3}, {1, 3}, {0}};
    // 0 -> 1 -> 3 (length 2) and 0 -> 2 -> 3 (length 2 also)
    assert(shortestDistance(0, 3, graph2) == 2);

    // Disconnected node
    std::vector<std::vector<int>> graph3 = {{1}, {}, {3}, {}};
    assert(shortestDistance(0, 3, graph3) == -1);
    assert(shortestDistance(1, 0, graph3) == -1);

    // Duplicate edges: 0 -> 1 twice, 1 -> 2
    std::vector<std::vector<int>> graph4 = {{1, 1}, {2}, {}};
    assert(shortestDistance(0, 2, graph4) == 2);

    // Single node graph
    std::vector<std::vector<int>> graph5 = {{}};
    assert(shortestDistance(0, 0, graph5) == 0);
    assert(shortestDistance(0, 1, graph5) == -1); // invalid end

    // Larger graph with direct edge
    std::vector<std::vector<int>> graph6 = {{3}, {}, {}, {}};
    assert(shortestDistance(0, 3, graph6) == 1);

    return 0;
}
