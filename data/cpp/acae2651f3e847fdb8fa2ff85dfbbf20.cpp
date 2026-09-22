Write a C++ function `vector<int> bfsLevels(const vector<int>& adjacency, const vector<int>& startIndex, const vector<int>& edgeCount, int source)` that performs a level-order Breadth-First Search on an unweighted directed graph stored in a compressed adjacency list format. The graph has `N` nodes. The `adjacency` array contains all edge destinations concatenated in order, and for each node `i`, its outgoing edges occupy positions `startIndex[i]` through `startIndex[i] + edgeCount[i] - 1` in the `adjacency` array. The function must return a vector of length `N` where each element is the shortest distance (in edges) from `source` to that node, or `-1` if unreachable. The BFS must be implemented iteratively using a queue, not recursion, and must not modify the input arrays.

The graph is stored compactly: node `i`'s neighbors are `adjacency[startIndex[i]]` through `adjacency[startIndex[i] + edgeCount[i] - 1]`. Initialize a `dist` vector of size `N` with `-1`, set `dist[source] = 0`, and use a queue initialized with `source`. While the queue is not empty, pop the front node `u`, iterate over all its neighbors by looping from `startIndex[u]` to `startIndex[u] + edgeCount[u] - 1`, and for each neighbor `v`, if `dist[v] == -1`, set `dist[v] = dist[u] + 1` and push `v` onto the queue. This guarantees each node is visited exactly once, and because BFS processes nodes in order of increasing distance, the first time a node is discovered its distance is minimal. Edge cases: an unreachable node remains `-1`; a graph with no edges still returns correct distances (source is `0`, others `-1`); duplicate edges in the adjacency list are harmless because we only process unvisited nodes. Time complexity is `O(N + E)` where `E` is the total number of edges (size of `adjacency`), and space complexity is `O(N)` for the distance vector and queue.

#include <vector>
#include <queue>
#include <cstddef>

// Perform level-order BFS on a compressed adjacency list graph.
// adjacency: concatenated edge destinations; startIndex[i] and edgeCount[i] define node i's edges.
// source: starting node.
// Returns a vector where dist[v] = shortest path length from source to v, or -1 if unreachable.
std::vector<int> bfsLevels(const std::vector<int>& adjacency,
                           const std::vector<int>& startIndex,
                           const std::vector<int>& edgeCount,
                           int source) {
    const int n = static_cast<int>(startIndex.size());
    std::vector<int> dist(n, -1);
    std::queue<int> q;

    dist[source] = 0;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        int begin = startIndex[u];
        int end = startIndex[u] + edgeCount[u];
        for (int i = begin; i < end; ++i) {
            int v = adjacency[i];
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist;
}

#include <cassert>
#include <vector>

int main() {
    // Graph: 0->1, 0->2, 1->3, 2->3, 3->4
    std::vector<int> adj = {1, 2, 3, 3, 4};
    std::vector<int> start = {0, 2, 3, 4, 5};
    std::vector<int> edges = {2, 1, 1, 1, 0};
    auto d = bfsLevels(adj, start, edges, 0);
    assert(d[0] == 0);
    assert(d[1] == 1);
    assert(d[2] == 1);
    assert(d[3] == 2);
    assert(d[4] == 3);

    // Unreachable node: 5 is not connected to anything else
    std::vector<int> adj2 = {1, 2};
    std::vector<int> start2 = {0, 2, 2}; // node 2 has no edges
    std::vector<int> edges2 = {2, 0, 0};
    auto d2 = bfsLevels(adj2, start2, edges2, 0);
    assert(d2[0] == 0);
    assert(d2[1] == 1);
    assert(d2[2] == -1);

    // Single node, no edges
    std::vector<int> adj3 = {};
    std::vector<int> start3 = {0};
    std::vector<int> edges3 = {0};
    auto d3 = bfsLevels(adj3, start3, edges3, 0);
    assert(d3[0] == 0);

    // Graph with duplicate edges: 0->1 appears twice
    std::vector<int> adj4 = {1, 1, 2};
    std::vector<int> start4 = {0, 2, 3};
    std::vector<int> edges4 = {2, 1, 0};
    auto d4 = bfsLevels(adj4, start4, edges4, 0);
    assert(d4[0] == 0);
    assert(d4[1] == 1);
    assert(d4[2] == 1);

    // BFS from a non-source node
    std::vector<int> adj5 = {2, 0, 2};
    std::vector<int> start5 = {0, 1, 2}; // node0->2, node1->0, node2->2
    std::vector<int> edges5 = {1, 1, 1};
    auto d5 = bfsLevels(adj5, start5, edges5, 1);
    assert(d5[0] == 1);
    assert(d5[1] == 0);
    assert(d5[2] == 2);

    return 0;
}
