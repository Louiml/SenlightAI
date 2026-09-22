/*
You are given an undirected weighted graph with `n` vertices (numbered 1 to `n`) and `m` edges. Write a C++ function `int countEssentialVertices(int n, const vector<vector<pair<int,int>>>& adj)` that returns the number of vertices that lie on **at least one shortest path** from vertex 1 to vertex `n`. A vertex `k` is considered essential if there exists a shortest path from 1 to `n` that passes through `k`. The graph is connected, edge weights are positive integers, and there may be multiple edges between the same pair of vertices. You must use Dijkstra's algorithm twice: once from vertex 1 and once from vertex `n`, and then count all vertices `k` such that `distFromStart[k] + distFromEnd[k] == distFromStart[n-1]` (with 0-indexed vertices). Ensure your solution handles large `n` and `m` efficiently (up to 10^5 vertices and 2*10^5 edges). Return the count as an integer.
*/

#include <vector>
#include <queue>
#include <limits>
using namespace std;

// Dijkstra's algorithm: fills dist with shortest distances from start node (0-indexed)
void dijkstra(const vector<vector<pair<int,int>>>& adj, vector<long long>& dist, int start) {
    const long long INF = numeric_limits<long long>::max();
    dist.assign(adj.size(), INF);
    dist[start] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, start});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue; // skip outdated entries
        for (const auto& [v, w] : adj[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
}

// Count vertices that lie on at least one shortest path from vertex 0 to vertex n-1
int countEssentialVertices(int n, const vector<vector<pair<int,int>>>& adj) {
    vector<long long> distS, distE;
    dijkstra(adj, distS, 0);
    dijkstra(adj, distE, n - 1);
    long long total = distS[n - 1]; // must be finite because graph is connected
    int count = 0;
    for (int k = 0; k < n; ++k) {
        if (distS[k] + distE[k] == total) {
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <vector>
#include <utility>
using namespace std;

// test function from solution (copy above)
int countEssentialVertices(int n, const vector<vector<pair<int,int>>>& adj);

int main() {
    // Test 1: Simple line 1-2-3, all vertices essential
    {
        int n = 3;
        vector<vector<pair<int,int>>> adj(n);
        adj[0].emplace_back(1, 1);
        adj[1].emplace_back(0, 1);
        adj[1].emplace_back(2, 1);
        adj[2].emplace_back(1, 1);
        assert(countEssentialVertices(n, adj) == 3);
    }
    // Test 2: Triangle with equal weights, all three vertices on some shortest path
    {
        int n = 3;
        vector<vector<pair<int,int>>> adj(n);
        adj[0].emplace_back(1, 1);
        adj[1].emplace_back(0, 1);
        adj[1].emplace_back(2, 1);
        adj[2].emplace_back(1, 1);
        adj[2].emplace_back(0, 1);
        adj[0].emplace_back(2, 1);
        assert(countEssentialVertices(n, adj) == 3);
    }
    // Test 2b: Triangle with heavier direct edge, middle vertex still essential
    {
        int n = 3;
        vector<vector<pair<int,int>>> adj(n);
        adj[0].emplace_back(1, 1);
        adj[1].emplace_back(0, 1);
        adj[1].emplace_back(2, 1);
        adj[2].emplace_back(1, 1);
        adj[2].emplace_back(0, 10);
        adj[0].emplace_back(2, 10);
        assert(countEssentialVertices(n, adj) == 3);
    }
    // Test 3: Graph where only start and end are essential (direct edge, plus longer detour)
    {
        int n = 4;
        vector<vector<pair<int,int>>> adj(n);
        adj[0].emplace_back(3, 5); // direct edge
        adj[3].emplace_back(0, 5);
        adj[0].emplace_back(1, 10);
        adj[1].emplace_back(0, 10);
        adj[1].emplace_back(2, 1);
        adj[2].emplace_back(1, 1);
        adj[2].emplace_back(3, 1);
        adj[3].emplace_back(2, 1);
        // shortest path 0->3 directly length 5; detour 0->1->2->3 length 12, so only 0 and 3 are on shortest paths
        assert(countEssentialVertices(n, adj) == 2);
    }
    // Test 4: Multiple shortest paths share a common intermediate vertex
    {
        int n = 5;
        vector<vector<pair<int,int>>> adj(n);
        // 0 - 1 - 4 with weight 1 each, and 0 - 2 - 3 - 4 with weights 2,1,1 => total 4? Let's make equal:
        // 0-1 (1), 1-4 (3) = 4; 0-2 (2), 2-3 (1), 3-4 (1) = 4; vertex 2 and 3 are on one shortest path only
        adj[0].emplace_back(1, 1); adj[1].emplace_back(0, 1);
        adj[1].emplace_back(4, 3); adj[4].emplace_back(1, 3);
        adj[0].emplace_back(2, 2); adj[2].emplace_back(0, 2);
        adj[2].emplace_back(3, 1); adj[3].emplace_back(2, 1);
        adj[3].emplace_back(4, 1); adj[4].emplace_back(3, 1);
        // Shortest distance = 4. Vertices on at least one shortest path: 0,1,4 (first path) and 0,2,3,4 (second path) => all 5 vertices
        assert(countEssentialVertices(n, adj) == 5);
    }
    // Test 5: Single vertex graph (n=1)
    {
        int n = 1;
        vector<vector<pair<int,int>>> adj(n);
        assert(countEssentialVertices(n, adj) == 1);
    }
    // Test 6: Two vertices connected by one edge
    {
        int n = 2;
        vector<vector<pair<int,int>>> adj(n);
        adj[0].emplace_back(1, 7);
        adj[1].emplace_back(0, 7);
        assert(countEssentialVertices(n, adj) == 2);
    }
    return 0;
}

// The problem is a classic "vertices on all shortest paths" problem. The key insight is that if we compute the shortest distance from the start (vertex 0) to all vertices (`distS`) and from the end (vertex n-1) to all vertices (`distE`), then a vertex `k` lies on some shortest path from start to end if and only if `distS[k] + distE[k] == distS[n-1]`. This works because the shortest path from start to end must pass through any vertex `k` that satisfies this equality; otherwise, if there were a shorter path that goes through `k`, it would contradict the definition of `distS` or `distE`. The algorithm runs Dijkstra twice, each `O((n+m) log n)` time, and then a linear scan over all vertices. Space complexity is `O(n+m)` for the adjacency list and `O(n)` for distance arrays. Edge cases include: the graph may have equal-cost multiple paths, but the condition still counts vertices correctly; the start and end vertices themselves are always counted (since `distS[0] + distE[0] == distS[n-1]` because `distE[0]` is the shortest distance from end to start, which equals `distS[n-1]`). The graph is guaranteed connected, so no unreachable vertices appear.
