Write a C++ function `std::vector<int> shortestDistancesFromSource(int source, const std::vector<std::vector<int>>& adjacencyList)` that takes a source node index (0-based) and an adjacency list of an unweighted, undirected graph with at most 100,000 nodes. The function must return a vector `dist` of the same size as the number of nodes, where `dist[i]` is the shortest number of edges from `source` to node `i`. If a node is unreachable, set its distance to `-1`. The graph may contain self-loops and parallel edges, but no negative weights (as it is unweighted). Ensure the solution uses breadth-first search (BFS) and handles large graphs efficiently.
// The problem is a classic single-source shortest path on an unweighted graph, which is solved optimally with BFS. BFS explores nodes level by level: starting from the source, it visits all neighbors at distance 1, then all nodes at distance 2, and so on. We maintain a distance array initialized to `-1` for unreachable nodes, set `dist[source] = 0`, and use a queue. For each node popped, we iterate its adjacency list; if a neighbor hasn't been visited yet (`dist[v] == -1`), we set its distance to `dist[u] + 1` and push it. Edge cases include: an empty graph (just the source node, returns all `-1` except source), self-loops (ignore because they don't improve distance), and parallel edges (no special handling needed because BFS checks visited status). Time complexity is O(V + E) where V is the number of nodes and E is total edges across all adjacency lists, since each edge is examined once. Space complexity is O(V) for the distance array and queue, plus input storage.
#include <vector>
#include <queue>
#include <algorithm>

// Compute shortest distances from a source node in an unweighted undirected graph.
// Returns a vector where dist[i] is the minimum edge count from source to i, or -1 if unreachable.
std::vector<int> shortestDistancesFromSource(int source, const std::vector<std::vector<int>>& adjacencyList) {
    const int n = static_cast<int>(adjacencyList.size());
    std::vector<int> dist(n, -1);
    std::queue<int> q;

    dist[source] = 0;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adjacencyList[u]) {
            if (dist[v] == -1) { // not visited
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist;
}
#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.

int main() {
    // Test 1: Simple chain 0-1-2
    std::vector<std::vector<int>> graph1 = {{1}, {0,2}, {1}};
    std::vector<int> d1 = shortestDistancesFromSource(0, graph1);
    assert((d1 == std::vector<int>{0, 1, 2}));

    // Test 2: Disconnected nodes
    std::vector<std::vector<int>> graph2 = {{1}, {0}, {}};
    std::vector<int> d2 = shortestDistancesFromSource(0, graph2);
    assert((d2 == std::vector<int>{0, 1, -1}));

    // Test 3: Self-loop and parallel edges (node 0 has self-loop and two parallel edges to 1)
    std::vector<std::vector<int>> graph3 = {{0,1,1}, {0}};
    std::vector<int> d3 = shortestDistancesFromSource(0, graph3);
    assert((d3 == std::vector<int>{0, 1}));

    // Test 4: Larger graph with multiple paths (0-3, 0-1-2-3)
    std::vector<std::vector<int>> graph4 = {{1,3}, {0,2}, {1,3}, {0,2}};
    std::vector<int> d4 = shortestDistancesFromSource(0, graph4);
    assert((d4 == std::vector<int>{0, 1, 2, 1}));

    // Test 5: Source with no edges
    std::vector<std::vector<int>> graph5 = {{}, {0}, {0}};
    std::vector<int> d5 = shortestDistancesFromSource(0, graph5);
    assert((d5 == std::vector<int>{0, -1, -1}));

    // Test 6: Complete graph K4
    std::vector<std::vector<int>> graph6 = {{1,2,3}, {0,2,3}, {0,1,3}, {0,1,2}};
    std::vector<int> d6 = shortestDistancesFromSource(0, graph6);
    assert((d6 == std::vector<int>{0, 1, 1, 1}));

    // Test 7: Single node
    std::vector<std::vector<int>> graph7 = {{}};
    std::vector<int> d7 = shortestDistancesFromSource(0, graph7);
    assert((d7 == std::vector<int>{0}));

    // Test 8: Source not the first node
    std::vector<std::vector<int>> graph8 = {{1}, {0,2}, {1}};
    std::vector<int> d8 = shortestDistancesFromSource(2, graph8);
    assert((d8 == std::vector<int>{2, 1, 0}));
}
