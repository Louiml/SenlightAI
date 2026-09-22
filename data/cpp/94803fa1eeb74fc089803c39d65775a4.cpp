Given an undirected weighted graph with `n` vertices (numbered 1 to `n`), `m` edges, a source vertex `s`, and two special vertices `g` and `h` that are connected by a direct edge, write a C++ function `vector<int> findValidDestinations(int n, int m, int s, int g, int h, const vector<tuple<int,int,int>>& edges, const vector<int>& candidates)` that returns a sorted list (ascending order) of all candidate destination vertices `t` such that there exists a shortest path from `s` to `t` that passes through the edge between `g` and `h` (in either direction). A path is considered valid if its total distance equals the overall shortest distance from `s` to `t` AND that path traverses the specific edge `(g,h)` or `(h,g)`. The graph is connected, edge weights are positive integers, and the candidates list contains distinct vertices. If no candidate satisfies the condition, return an empty vector.
// The key insight is that a shortest path from `s` to `t` that goes through edge `(g,h)` must have one of two forms:
// 1. `s -> ... -> g -> h -> ... -> t`
// 2. `s -> ... -> h -> g -> ... -> t`
//
// So the distance of such a path is either `dist_s[g] + w(g,h) + dist_h[t]` or `dist_s[h] + w(h,g) + dist_g[t]`. Here `dist_s[x]` is the shortest distance from `s` to `x`, `dist_g[x]` from `g` to `x`, and `dist_h[x]` from `h` to `x`. Note that because the graph is undirected, `w(g,h) = w(h,g)`. We compute these three distance arrays using Dijkstra's algorithm from `s`, `g`, and `h`. Then for each candidate `t`, we check if the actual shortest distance `dist_s[t]` equals either of the two path distances above; if so, `t` is valid. This handles the subtle case where there may be multiple shortest paths, some of which do not go through `g` and `h`; we only accept if at least one shortest path goes through that edge. Edge cases include when `t` is `g` or `h` themselves (then the path through the edge might still be valid if it's a shortest path) and when the graph has parallel edges (we treat the minimum weight edge between any two vertices). The algorithm runs Dijkstra three times, each with O((V+E) log V) time, and then O(T) for checking candidates, so total time complexity O((V+E) log V + T) and space O(V+E).
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <limits>

using namespace std;

// helper to run Dijkstra from 'start' on graph represented by adjacency list
static vector<long long> dijkstra(int start, int n, const vector<vector<pair<int,int>>>& adj) {
    const long long INF = numeric_limits<long long>::max() / 4;
    vector<long long> dist(n+1, INF);
    dist[start] = 0;
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;
    pq.push({0, start});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

// Function: returns sorted list of candidate destinations that have a shortest path from s passing through edge (g,h) or (h,g)
vector<int> findValidDestinations(int n, int m, int s, int g, int h, 
                                  const vector<tuple<int,int,int>>& edges, 
                                  const vector<int>& candidates) {
    // build adjacency list, keep min weight for parallel edges if any (but given typical input, we just store all)
    vector<vector<pair<int,int>>> adj(n+1);
    long long edge_gh = -1;
    for (auto [a, b, w] : edges) {
        adj[a].push_back({b, w});
        adj[b].push_back({a, w});
        if ((a == g && b == h) || (a == h && b == g)) {
            // if multiple edges, we take the minimum, but for simplicity we just capture the first; 
            // in a correct problem, there's only one edge between g-h. We'll keep the min if multiple.
            if (edge_gh == -1 || w < edge_gh) edge_gh = w;
        }
    }
    
    vector<long long> dist_s = dijkstra(s, n, adj);
    vector<long long> dist_g = dijkstra(g, n, adj);
    vector<long long> dist_h = dijkstra(h, n, adj);
    
    long long w_gh = edge_gh; // weight of the direct edge between g and h
    
    vector<int> result;
    for (int t : candidates) {
        // path s -> g -> h -> t
        if (dist_s[t] == dist_s[g] + w_gh + dist_h[t])
            result.push_back(t);
        // path s -> h -> g -> t
        else if (dist_s[t] == dist_s[h] + w_gh + dist_g[t])
            result.push_back(t);
    }
    sort(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>
#include <tuple>
#include <iostream>

// Include the solution code here (or copy the function above)

int main() {
    // Test 1: Simple triangle, s=1, g=2, h=3, destinations {3} must be valid
    {
        vector<tuple<int,int,int>> edges = {{1,2,1}, {2,3,1}, {1,3,2}};
        vector<int> candidates = {3};
        auto res = findValidDestinations(3, 3, 1, 2, 3, edges, candidates);
        assert(res == vector<int>({3}));
    }
    // Test 2: Graph where shortest path to candidate does NOT go through g-h
    {
        // s=1, g=2, h=3, edges: 1-2 (10), 2-3 (1), 1-4 (1), 4-3 (1)
        // Shortest path 1->4->3 = 2, but going through g-h is 1->2->3 = 11, so not valid
        vector<tuple<int,int,int>> edges = {{1,2,10}, {2,3,1}, {1,4,1}, {4,3,1}};
        vector<int> candidates = {3};
        auto res = findValidDestinations(4, 4, 1, 2, 3, edges, candidates);
        assert(res.empty());
    }
    // Test 3: Multiple candidates, sorted output
    {
        // s=1, g=2, h=3, edges: 1-2 (1), 2-3 (1), 3-4 (1), 1-4 (3), 2-5 (1), 5-6 (1)
        // Shortest 1->4 is either 1-2-3-4 (3) or 1-4 (3) both equal, but only path through g-h exists → valid
        // Destination 5: shortest 1->2->5 (2) goes through g? no, that's 1->2->5 but does not use edge (2,3) so invalid
        // Destination 6: shortest 1->2->5->6 (3) does not use (2,3), invalid
        vector<tuple<int,int,int>> edges = {{1,2,1}, {2,3,1}, {3,4,1}, {1,4,3}, {2,5,1}, {5,6,1}};
        vector<int> candidates = {3,4,5,6};
        auto res = findValidDestinations(6, 6, 1, 2, 3, edges, candidates);
        assert(res == vector<int>({3,4})); // 3 is directly h, valid; 4 has equal path through g-h
    }
    // Test 4: Candidate equals g itself
    {
        vector<tuple<int,int,int>> edges = {{1,2,2}, {2,3,1}};
        vector<int> candidates = {2};
        auto res = findValidDestinations(3, 2, 1, 2, 3, edges, candidates);
        // Shortest 1->2 is 2 via direct edge, but does that path go through (2,3)? No, it doesn't. So invalid.
        assert(res.empty());
    }
    // Test 5: Single edge graph, s=g, h is other end
    {
        vector<tuple<int,int,int>> edges = {{1,2,5}};
        vector<int> candidates = {2};
        auto res = findValidDestinations(2, 1, 1, 1, 2, edges, candidates);
        // shortest 1->2 = 5, path goes through (1,2) directly, valid
        assert(res == vector<int>({2}));
    }
    // Test 6: Larger graph, ensure correct sorting
    {
        vector<tuple<int,int,int>> edges = {{1,3,1}, {3,2,1}, {2,4,1}, {1,4,3}, {2,5,2}};
        vector<int> candidates = {4,5,2,3};
        auto res = findValidDestinations(5, 5, 1, 3, 2, edges, candidates);
        // Distances: s=1, g=3, h=2. 
        // dist_s: to 3=1, to 2=2 (via 3), to 4=3 (via 3-2-4) or 1-4=3 (both), to 5=4 (via 3-2-5)
        // Path through g-h: for 4: 1->3->2->4 = 3 equals dist_s[4], valid.
        // for 5: 1->3->2->5 = 4 equals dist_s[5], valid.
        // for 3 (g): dist_s[3]=1, but path through g-h must start from s and go 1->...->3->2->...->3? no, that's not a simple path. 
        // Actually for t=3, path through g-h would need to be s->...->h->g->...->3, that is 1->2->3 = 1+1=2, not equal to 1, so invalid.
        // for 2: path through g-h would be s->...->g->h->...->2? that's 1->3->2 = 2, equals dist_s[2], so valid.
        // So valid: 2,4,5 sorted → {2,4,5}
        assert(res == vector<int>({2,4,5}));
    }
    std::cout << "All tests passed!\n";
    return 0;
}
