Write a C++ function `int findShortestPathLength(int nodes, const std::vector<std::pair<int,int>>& edges, int source, int destination)` that, given an undirected, unweighted graph with node labels from 0 to nodes-1, returns the length (number of edges) of the shortest path between source and destination. The graph is defined by a list of undirected edges. If source and destination are the same, return 0. If no path exists, return -1. The function should handle graphs with up to 1005 nodes and avoid using global arrays (use local data structures inside the function).

// The problem reduces to computing the unweighted shortest path in an undirected graph. Since all edges have equal weight (1), Breadth-First Search (BFS) from the source gives the shortest distances to all reachable nodes in O(V+E) time. We initialize a distance array with -1 (representing unvisited) and set distance[source]=0. We use a queue, push the source, and while the queue is not empty, pop a node and for each unvisited neighbor, set its distance to current distance+1 and push it. BFS naturally finds shortest paths because it explores nodes level by level. After BFS, we check if distance[destination] is -1 (unreachable) and return -1; otherwise return that distance. Edge cases: (1) source == destination returns 0 (handled as reachable with distance 0), (2) disconnected graph yields -1. Note that the original snippet printed parents and levels, but here we only need the distance. We must not use global arrays; instead, we allocate a local distance vector of size `nodes` and an adjacency list as a vector of vectors. Time complexity is O(V+E) because each edge is visited once and each vertex is enqueued once. Space complexity is O(V+E) for the adjacency list plus O(V) for the distance array and queue.

#include <vector>
#include <queue>
#include <algorithm>

// Returns the length of the shortest path between source and destination
// in an undirected unweighted graph with 'nodes' vertices (0..nodes-1).
// Returns -1 if no path exists.
int findShortestPathLength(int nodes,
                           const std::vector<std::pair<int,int>>& edges,
                           int source,
                           int destination) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(nodes);
    for (const auto& [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Initialize distances with -1 (meaning unvisited)
    std::vector<int> dist(nodes, -1);
    dist[source] = 0;

    std::queue<int> q;
    q.push(source);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // Early exit if we reached the destination
        if (current == destination) {
            return dist[current];
        }

        for (int neighbor : adj[current]) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[current] + 1;
                q.push(neighbor);
            }
        }
    }

    return (dist[destination] == -1) ? -1 : dist[destination];
}

#include <cassert>
#include <vector>
#include <utility>

// Declare the function (in actual test, include the solution code above)
int findShortestPathLength(int nodes,
                           const std::vector<std::pair<int,int>>& edges,
                           int source,
                           int destination);

int main() {
    // Test 1: Simple connected graph
    std::vector<std::pair<int,int>> edges1 = {{0,1},{1,2},{2,3}};
    assert(findShortestPathLength(4, edges1, 0, 3) == 3);

    // Test 2: Same source and destination
    assert(findShortestPathLength(4, edges1, 1, 1) == 0);

    // Test 3: Disconnected graph - no path
    std::vector<std::pair<int,int>> edges2 = {{0,1},{2,3}};
    assert(findShortestPathLength(4, edges2, 0, 3) == -1);

    // Test 4: Graph with multiple paths - shortest is 2
    std::vector<std::pair<int,int>> edges3 = {{0,1},{0,2},{1,3},{2,3},{1,2}};
    assert(findShortestPathLength(4, edges3, 0, 3) == 2);

    // Test 5: Single node with no edges
    assert(findShortestPathLength(1, {}, 0, 0) == 0);

    // Test 6: Unreachable from source but destination is reachable from elsewhere
    std::vector<std::pair<int,int>> edges4 = {{1,2}};
    assert(findShortestPathLength(3, edges4, 0, 2) == -1);

    // Test 7: Larger graph with direct edge
    std::vector<std::pair<int,int>> edges5 = {{0,5},{5,4},{4,3},{3,2},{2,1},{1,0}};
    assert(findShortestPathLength(6, edges5, 0, 3) == 3);

    // Test 8: Isolated node as destination
    std::vector<std::pair<int,int>> edges6 = {{0,1}};
    assert(findShortestPathLength(3, edges6, 0, 2) == -1);

    // Test 9: Graph with 1 edge, source and destination adjacent
    std::vector<std::pair<int,int>> edges7 = {{0,1}};
    assert(findShortestPathLength(2, edges7, 0, 1) == 1);

    // Test 10: Graph with 1005 nodes, chain from 0 to 1004
    std::vector<std::pair<int,int>> edges8;
    for (int i = 0; i < 1004; ++i) {
        edges8.push_back({i, i+1});
    }
    assert(findShortestPathLength(1005, edges8, 0, 1004) == 1004);

    return 0;
}
