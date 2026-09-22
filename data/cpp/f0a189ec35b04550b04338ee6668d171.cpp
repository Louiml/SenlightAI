Write a C++ function `longestHappyPath` that takes a directed graph with `n` vertices (numbered 1 to n) and `m` edges, where each edge has an integer weight (can be negative, zero, or positive), and returns the path from vertex 1 to vertex n that maximizes the sum of edge weights, as a vector of vertex IDs in order. If no path exists, return an empty vector. If there is a positive-weight cycle that is reachable from vertex 1 and can reach vertex n, then the maximum sum is unbounded; in that case, return a vector containing a single `-1` to indicate "infinite profit". If multiple paths tie, any is acceptable. The graph may have self-loops and parallel edges. The function signature: `std::vector<int> longestHappyPath(int n, const std::vector<std::tuple<int,int,int>>& edges)`, where each tuple is `(from, to, weight)`.
This is a longest-path problem on a directed graph with possibly negative weights, which is NP-hard in general if there are cycles. However, the problem becomes solvable in polynomial time if we allow unbounded detection via positive cycles. We use Bellman-Ford algorithm adapted for maximum distance: initialize `dist[1] = 0` and all others to very negative, then relax edges for `n-1` iterations. After that, run one more iteration to detect if any edge can still improve `dist[to]`; if yes, and that node can reach target n (and also is reachable from start), then there is a positive cycle on a path from 1 to n, so return `-1`. To decide reachability to n, we can first compute reverse adjacency and BFS/DFS from n to see which nodes can reach n; also mark nodes reachable from 1 (forward BFS). Only if a node in a positive cycle is both reachable from 1 and can reach n, we output `-1`. For path reconstruction, keep a `prev` array during relaxation, but if a node is part of a positive cycle that affects the answer, we handle it earlier. If no infinite case, reconstruct path from n back to 1 using `prev` (but we need to ensure we only follow prev nodes that were updated in the first n-1 relaxations; since we stop after that, prev may point to a cycle? Actually, in longest path with no positive cycle on the path, after n-1 relaxations the dist is correct and prev forms a simple path to 1; but we must be careful to set prev only when we actually update. In the detection iteration, we do not update prev to avoid corruption. Complexity: O(n*m) time, O(n + m) space for adjacency and reverse adjacency.
#include <bits/stdc++.h>
using namespace std;

// Returns longest path from 1 to n as vector of vertices.
// If no path, returns empty vector.
// If maximum is unbounded (positive cycle on a valid path), returns {-1}.
std::vector<int> longestHappyPath(int n, const std::vector<std::tuple<int,int,int>>& edges) {
    const long long NEG_INF = -(1LL << 60);
    vector<vector<pair<int,int>>> adj(n+1);
    vector<vector<int>> radj(n+1);
    for (auto [u,v,w] : edges) {
        adj[u].push_back({v,w});
        radj[v].push_back(u);
    }

    // Reachability from 1
    vector<bool> reachFromStart(n+1, false);
    queue<int> q;
    q.push(1); reachFromStart[1] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto [v,w] : adj[u]) if (!reachFromStart[v]) {
            reachFromStart[v] = true;
            q.push(v);
        }
    }

    // Can reach n
    vector<bool> canReachEnd(n+1, false);
    q.push(n); canReachEnd[n] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : radj[u]) if (!canReachEnd[v]) {
            canReachEnd[v] = true;
            q.push(v);
        }
    }

    // If 1 cannot reach n, no path
    if (!reachFromStart[n]) return {};

    vector<long long> dist(n+1, NEG_INF);
    vector<int> prev(n+1, -1);
    dist[1] = 0;

    // Relax edges n-1 times
    for (int iter = 0; iter < n-1; ++iter) {
        bool changed = false;
        for (auto [u,v,w] : edges) {
            if (dist[u] == NEG_INF) continue;
            if (dist[v] < dist[u] + w) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                changed = true;
            }
        }
        if (!changed) break;
    }

    // Check for positive cycle on a path from 1 to n
    for (auto [u,v,w] : edges) {
        if (dist[u] == NEG_INF) continue;
        if (dist[v] < dist[u] + w) {
            // This edge can be improved => v (or u) is part of a positive cycle
            // If that cycle is on a path from 1 to n, return -1
            if (reachFromStart[u] && canReachEnd[u]) return {-1};
            if (reachFromStart[v] && canReachEnd[v]) return {-1};
        }
    }

    // Reconstruct path
    vector<int> path;
    int cur = n;
    while (cur != -1) {
        path.push_back(cur);
        cur = prev[cur];
    }
    reverse(path.begin(), path.end());
    if (path.front() != 1) return {}; // safety
    return path;
}
#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

// Assume function is included above

int main() {
    // Simple path: 1->2 (5), 2->3 (3), 1->3 (1). Best = 1-2-3 = 8
    {
        vector<tuple<int,int,int>> edges = {{1,2,5},{2,3,3},{1,3,1}};
        auto res = longestHappyPath(3, edges);
        assert(res.size() == 3);
        assert(res[0]==1 && res[1]==2 && res[2]==3);
    }

    // No path
    {
        vector<tuple<int,int,int>> edges = {{1,2,1}};
        auto res = longestHappyPath(3, edges);
        assert(res.empty());
    }

    // Positive cycle on path 1->2->3->2->3 (cycle 2-3 with weight 1 each side)
    {
        vector<tuple<int,int,int>> edges = {{1,2,10},{2,3,10},{3,2,1}};
        auto res = longestHappyPath(3, edges);
        assert(res.size()==1 && res[0]==-1);
    }

    // Negative weights but no positive cycle
    {
        vector<tuple<int,int,int>> edges = {{1,2,-5},{2,3,-3},{1,3,-2}};
        auto res = longestHappyPath(3, edges);
        // Best is directly 1->3 (-2) or 1->2->3 (-8). So path 1 3
        assert(res.size()==2);
        assert(res[0]==1 && res[1]==3);
    }

    // Parallel edges: choose the better one
    {
        vector<tuple<int,int,int>> edges = {{1,2,1},{1,2,5},{2,3,1}};
        auto res = longestHappyPath(3, edges);
        assert(res.size()==3);
        assert(res[0]==1 && res[1]==2 && res[2]==3);
    }

    // Self-loop positive on a path to n
    {
        vector<tuple<int,int,int>> edges = {{1,2,1},{2,2,100},{2,3,1}};
        auto res = longestHappyPath(3, edges);
        assert(res.size()==1 && res[0]==-1);
    }

    // Cycle not on path to n (only connected from 1 but cannot reach n) => finite
    {
        vector<tuple<int,int,int>> edges = {{1,2,1},{2,2,100},{1,3,1}};
        auto res = longestHappyPath(3, edges);
        // best is 1->3 directly (1), or 1->2->... but cannot reach 3, so path 1 3
        assert(res.size()==2);
        assert(res[0]==1 && res[1]==3);
    }

    return 0;
}
