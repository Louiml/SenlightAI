// You are given a directed graph where each vertex has an associated "busyness" value, and the weight of an edge from vertex `u` to vertex `v` is defined as the cube of the difference between their busyness values: `(busyness[v] - busyness[u])^3`. In addition, some edges may produce negative weights, and the graph may contain negative cycles. Write a standalone C++ function `vector<int> shortestDistancesFromSource(int n, const vector<int>& busyness, const vector<tuple<int,int,int>>& edges, int source, int queryCount, const vector<int>& queries)` that, given the number of vertices `n` (1-indexed), the busyness values (index 0 for vertex 1), the edges as tuples `(u, v, weight)` with `u` and `v` 1-indexed, a source vertex (1-indexed), and a list of query vertices, returns for each query either the shortest distance from the source if it is well-defined (i.e., reachable and not affected by negative cycles) and is at least 3, or the sentinel value `-1` to indicate that the distance is undefined (meaning it is less than 3, unreachable, or infinite due to a negative cycle). The Bellman-Ford algorithm should be used, but you must first detect if negative cycles exist, and if so, mark all vertices that can be reached from any negative-cycle vertex as having undefined distances, because their shortest distances are not meaningful. Additionally, the raw shortest distance could be a negative value or very large, so you must cap it: if the computed distance is less than 3, greater than 999999000, or is INF (use 1,000,000,007), return -1. Otherwise return the distance as-is. The function must be implemented without any global variables and must be self-contained.

#include <cassert>
#include <vector>
#include <tuple>

// The solution function is declared above. Include its definition here for testing.

int main() {
    // Test 1: Simple graph without negative cycles.
    // busyness: [1, 2, 3], edges: 1->2 weight=(2-1)^3=1, 2->3 weight=(3-2)^3=1.
    {
        int n = 3;
        std::vector<int> busy = {1,2,3};
        std::vector<std::tuple<int,int,int>> edges = {{1,2,1},{2,3,1}};
        int source = 1;
        std::vector<int> queries = {1,2,3};
        auto res = shortestDistancesFromSource(n, busy, edges, source, queries.size(), queries);
        assert(res == (std::vector<int>{0,1,2}));
    }

    // Test 2: Edge weight negative but no cycle: busyness [3,1] edge 1->2 = (1-3)^3 = -8.
    {
        int n = 2;
        std::vector<int> busy = {3,1};
        std::vector<std::tuple<int,int,int>> edges = {{1,2,-8}};
        int source = 1;
        std::vector<int> queries = {2};
        auto res = shortestDistancesFromSource(n, busy, edges, source, queries.size(), queries);
        // distance = -8, which is <3 -> undefined -> -1
        assert(res == (std::vector<int>{-1}));
    }

    // Test 3: Negative cycle reachable.
    // busyness [0,0,0], edges 1->2 weight 5, 2->3 weight -10, 3->2 weight 5 => cycle 2-3 has negative sum? total = -5? actually 2->3 -10, 3->2 5 => neg -5.
    {
        int n = 3;
        std::vector<int> busy = {0,0,0};
        std::vector<std::tuple<int,int,int>> edges = {{1,2,5},{2,3,-10},{3,2,5}};
        int source = 1;
        std::vector<int> queries = {1,2,3};
        auto res = shortestDistancesFromSource(n, busy, edges, source, queries.size(), queries);
        // Vertex 1 distance 0 (>=3? 0 <3 -> -1). Vertex 2 and 3 reachable from negative cycle -> -1.
        assert(res == (std::vector<int>{-1,-1,-1}));
    }

    // Test 4: Distance exactly 3 should be allowed.
    // busyness [0,3] edge 1->2 weight=27? Actually (3-0)^3=27, but we want exactly 3. Use busyness [0, sqrt(3)]? Not integer. Use direct edge weight 3.
    {
        int n = 2;
        std::vector<int> busy = {0,3};
        // override edge weight to 3 manually (as per problem, weight depends on busyness, but for test we pass explicit weight)
        std::vector<std::tuple<int,int,int>> edges = {{1,2,3}};
        int source = 1;
        std::vector<int> queries = {2};
        auto res = shortestDistancesFromSource(n, busy, edges, source, queries.size(), queries);
        assert(res == (std::vector<int>{3}));
    }

    // Test 5: Unreachable vertex.
    {
        int n = 3;
        std::vector<int> busy = {1,2,3};
        std::vector<std::tuple<int,int,int>> edges = {{1,2,1}};
        int source = 1;
        std::vector<int> queries = {3};
        auto res = shortestDistancesFromSource(n, busy, edges, source, queries.size(), queries);
        assert(res == (std::vector<int>{-1})); // INF -> -1
    }

    // Test 6: Large distance cap.
    {
        int n = 2;
        std::vector<int> busy = {1000, -1000}; // edge weight = (-2000)^3 huge negative? Actually (busy[1]-busy[0])^3 = (-2000)^3 = -8e9, but cap > 999999000? -8e9 <3? yes <3 -> -1.
        // Directly use edge weight 1000000000 to test cap.
        std::vector<std::tuple<int,int,int>> edges = {{1,2,1000000000}};
        int source = 1;
        std::vector<int> queries = {2};
        auto res = shortestDistancesFromSource(n, busy, edges, source, queries.size(), queries);
        assert(res == (std::vector<int>{-1})); // >999999000
    }

    return 0;
}

#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <limits>

// Return the shortest distances from source for each query, or -1 if undefined.
// undefined means: unreachable, affected by negative cycle, distance < 3, or distance > 999999000.
std::vector<int> shortestDistancesFromSource(
    int n,
    const std::vector<int>& busyness,
    const std::vector<std::tuple<int,int,int>>& edges,
    int source,
    int queryCount,
    const std::vector<int>& queries)
{
    const int INF = 1000000007;
    // Convert to 0-indexed internally.
    int s = source - 1;

    std::vector<int> dist(n, INF);
    dist[s] = 0;

    // Build adjacency lists for later negative-cycle reachability.
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        int u, v, w;
        std::tie(u, v, w) = e;
        adj[u-1].push_back(v-1);
    }

    // Bellman-Ford relax n-1 times.
    for (int i = 0; i < n-1; ++i) {
        bool updated = false;
        for (const auto& e : edges) {
            int u, v, w;
            std::tie(u, v, w) = e;
            u--; v--;
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                updated = true;
            }
        }
        if (!updated) break;
    }

    // Detect vertices that are part or affected by negative cycles.
    std::vector<bool> affected(n, false);
    // Run one more relaxation to find vertices that can still be relaxed.
    for (const auto& e : edges) {
        int u, v, w;
        std::tie(u, v, w) = e;
        u--; v--;
        if (dist[u] != INF && dist[u] + w < dist[v]) {
            // v is reachable from a negative cycle (may be the cycle itself).
            affected[v] = true;
        }
    }

    // Build reverse graph.
    std::vector<std::vector<int>> rev_adj(n);
    for (const auto& e : edges) {
        int u, v, w;
        std::tie(u, v, w) = e;
        rev_adj[v-1].push_back(u-1);
    }

    // BFS/DFS from all affected vertices on reverse graph to mark all that can reach them.
    std::queue<int> q;
    std::vector<bool> visited(n, false);
    for (int i = 0; i < n; ++i) {
        if (affected[i]) {
            q.push(i);
            visited[i] = true;
        }
    }
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        for (int prev : rev_adj[cur]) {
            if (!visited[prev]) {
                visited[prev] = true;
                q.push(prev);
            }
        }
    }

    // Prepare answer for each query.
    std::vector<int> result;
    for (int i = 0; i < queryCount; ++i) {
        int node = queries[i] - 1;
        if (visited[node] || dist[node] == INF || dist[node] < 3 || dist[node] > 999999000) {
            result.push_back(-1);
        } else {
            result.push_back(dist[node]);
        }
    }
    return result;
}

// The core problem is a variant of single-source shortest paths with possible negative edge weights, solved via Bellman-Ford. First, initialize all distances to a large INF (1,000,000,007) and set distance[source] = 0. Then relax all edges n-1 times, where n is the number of vertices, updating `dist[v] = min(dist[v], dist[u] + weight)` for each edge (u,v). After these relaxations, we have the shortest distances for vertices not reachable from any negative cycle. To detect negative cycles, run one extra relaxation pass over all edges: if any edge (u,v) can still be relaxed (i.e., `dist[u] + weight < dist[v]`), then vertex v (and potentially u) is part of a negative cycle or reachable from one. To handle the "reachable from negative cycle" case, perform a reverse reachability analysis: build a reverse adjacency list (for each edge (u,v), add v to u's reverse list), then mark all vertices that can reach any vertex that was updated in the extra pass by traversing the reverse graph from all such "updated" vertices using BFS/DFS. Those marked vertices have undefined distances. Finally, for each query vertex, if it is marked as affected by negative cycles, or its distance is INF, or its distance is less than 3, or its distance is greater than 999999000, return -1; otherwise return the distance. Time complexity: O(n * E + (n + E)) for Bellman-Ford and the reverse reachability BFS, which is O(n*E). Space complexity: O(n + E) for distance array, adjacency list, and reverse adjacency list. Input vertices are 1-indexed, so we convert to 0-indexed internally.
