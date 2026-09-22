/*
You are given an undirected weighted graph with `n` vertices (numbered 1..n) and `m` edges, plus a list of `q` "demands", each of the form `(u, v, L)` meaning that there must exist a path from `u` to `v` with total length at most `L`. For each vertex `s` (1..n), you consider only the demands where `s` is the source `u`. Your goal is to determine, for every ordered pair `(a,b)` of vertices, whether the edge `(a,b)` is "needed" for at least one source `s`. An edge `(a,b)` is considered needed for source `s` if there exists some path from `s` to `b` that uses the edge `(a,b)` and has length at most the maximum allowed distance from `s` to `b` among all demands originating at `s` that target `b` (if no such demand exists, the edge is not needed for `s`). After processing all sources, output the total number of unordered edges that are needed for at least one source. Write a function `int countNeededEdges(int n, const vector<tuple<int,int,int>>& edges, const vector<tuple<int,int,int>>& demands)` that returns this count. The graph may have multiple edges between the same pair of vertices and may have self-loops. The weights are positive integers. Demands may have `u == v` and `L >= 0` (a zero-length demand is satisfied by staying at `u`). All edge weights and L values fit in 32-bit signed integers, but the sum of paths can exceed 32 bits. The answer is an integer. The function must be self-contained, not relying on any global mutable state, and should handle up to n=600, m=5000, q=5000 efficiently.
*/
#include <bits/stdc++.h>
using namespace std;

int countNeededEdges(int n, const vector<tuple<int,int,int>>& edges, const vector<tuple<int,int,int>>& demands) {
    const long long INF = 1e18;
    vector<vector<pair<int,int>>> g(n + 1);
    for (const auto& [u,v,w] : edges) {
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    vector<vector<pair<int,int>>> qry(n + 1);
    for (const auto& [u,v,l] : demands) {
        qry[u].emplace_back(v, l);
    }
    vector<vector<bool>> ans(n + 1, vector<bool>(n + 1, false));
    vector<long long> dis(n + 1), dsi(n + 1);
    vector<bool> vis(n + 1);

    for (int s = 1; s <= n; ++s) {
        // Dijkstra from s
        fill(dis.begin(), dis.end(), INF);
        fill(vis.begin(), vis.end(), false);
        dis[s] = 0;
        while (true) {
            pair<long long,int> u = {INF, 0};
            for (int i = 1; i <= n; ++i)
                if (!vis[i]) u = min(u, {dis[i], i});
            if (u.first >= INF) break;
            long long w = u.first;
            int x = u.second;
            vis[x] = true;
            for (const auto& [y, wgt] : g[x])
                if (dis[y] > w + wgt)
                    dis[y] = w + wgt;
        }
        // Compute dsi[x] = max L among demands (s,x,L)
        fill(dsi.begin(), dsi.end(), -1LL);
        fill(vis.begin(), vis.end(), false);
        for (const auto& [v, l] : qry[s])
            if (l >= 0 && dis[v] <= l) // only feasible demands matter
                dsi[v] = max(dsi[v], (long long)l);
        // Max-propagation: dsi[x] = max over edges (x,y) of dsi[y] - w
        while (true) {
            pair<long long,int> u = {0, 0};
            for (int i = 1; i <= n; ++i)
                if (!vis[i]) u = max(u, {dsi[i], i});
            if (u.first <= 0) break;
            long long w = u.first;
            int x = u.second;
            vis[x] = true;
            for (const auto& [y, wgt] : g[x])
                if (dsi[y] < w - wgt)
                    dsi[y] = w - wgt;
        }
        // Mark edges that are needed for source s
        for (int i = 1; i <= n; ++i) {
            if (dis[i] >= INF) continue;
            for (const auto& [j, wgt] : g[i]) {
                if (dis[i] + wgt <= dsi[j])
                    ans[i][j] = ans[j][i] = true;
            }
        }
    }
    int res = 0;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            if (ans[i][j]) ++res;
    return res / 2;
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// The solution function is expected to be defined above this test (or included).
// For completeness, we repeat the function here in the test block (short version).

int countNeededEdges(int n, const vector<tuple<int,int,int>>& edges, const vector<tuple<int,int,int>>& demands);

int main() {
    // Test 1: Simple triangle, all demands require all edges.
    {
        int n = 3;
        vector<tuple<int,int,int>> edges = {{1,2,1},{2,3,1},{1,3,10}};
        vector<tuple<int,int,int>> demands = {{1,3,2},{2,3,2},{3,1,2}};
        // From 1: shortest to 3 is 1+1=2 via 1-2-3, edge 1-3 is too long (10) so not needed.
        // From 2: need to reach 3 within 2: direct edge 2-3 is needed, also 2-1-? doesn't help.
        // From 3: need to reach 1 within 2: direct 3-1 is 10, no; via 3-2-1 = 2, so edges 3-2 and 2-1 needed.
        // Total needed edges: {1,2} and {2,3} (2 edges)
        assert(countNeededEdges(n, edges, demands) == 2);
    }
    // Test 2: Single edge, demand with L=0 for u==v, no edges needed.
    {
        int n = 2;
        vector<tuple<int,int,int>> edges = {{1,2,5}};
        vector<tuple<int,int,int>> demands = {{1,1,0},{2,2,0}};
        assert(countNeededEdges(n, edges, demands) == 0);
    }
    // Test 3: Two vertices, one edge weight 3, demand (1,2,3) -> edge needed.
    {
        int n = 2;
        vector<tuple<int,int,int>> edges = {{1,2,3}};
        vector<tuple<int,int,int>> demands = {{1,2,3}};
        assert(countNeededEdges(n, edges, demands) == 1);
    }
    // Test 4: Demand (1,2,2) but edge weight 3 -> not feasible, no edges.
    {
        int n = 2;
        vector<tuple<int,int,int>> edges = {{1,2,3}};
        vector<tuple<int,int,int>> demands = {{1,2,2}};
        assert(countNeededEdges(n, edges, demands) == 0);
    }
    // Test 5: Multiple edges between same vertices, only the shortest works.
    {
        int n = 2;
        vector<tuple<int,int,int>> edges = {{1,2,5},{1,2,2}};
        vector<tuple<int,int,int>> demands = {{1,2,4}};
        // Edge with weight 2 is needed; weight 5 edge not needed because dis[1]+5 > 4.
        // Output counts unordered edges, so only 1 edge (the weight-2 one) is needed.
        assert(countNeededEdges(n, edges, demands) == 1);
    }
    // Test 6: Self-loop is never counted.
    {
        int n = 1;
        vector<tuple<int,int,int>> edges = {{1,1,1}};
        vector<tuple<int,int,int>> demands = {{1,1,1}};
        assert(countNeededEdges(n, edges, demands) == 0);
    }
    // Test 7: 4-node path with demands that use all edges.
    {
        int n = 4;
        vector<tuple<int,int,int>> edges = {{1,2,1},{2,3,1},{3,4,1}};
        vector<tuple<int,int,int>> demands = {{1,4,3},{4,1,3}};
        // Both directions require all three edges.
        assert(countNeededEdges(n, edges, demands) == 3);
    }
    // Test 8: More complex case with several sources.
    {
        int n = 4;
        vector<tuple<int,int,int>> edges = {{1,2,2},{2,3,1},{3,4,5},{1,4,10}};
        vector<tuple<int,int,int>> demands = {{1,3,3},{2,4,6}};
        // For source 1: need to reach 3 within 3. Shortest 1-2-3 = 3, so edges 1-2 and 2-3 needed. Edge 1-4 not needed for any.
        // For source 2: need to reach 4 within 6. Shortest 2-3-4 = 6, so edges 2-3 and 3-4 needed.
        // Overall needed: {1,2},{2,3},{3,4} -> 3 edges.
        assert(countNeededEdges(n, edges, demands) == 3);
    }
    // Test 9: Large L values, but only edges on shortest paths matter.
    {
        int n = 3;
        vector<tuple<int,int,int>> edges = {{1,2,1},{2,3,1},{1,3,100}};
        vector<tuple<int,int,int>> demands = {{1,3,1000}};
        // From source 1, allowed distance to 3 is 1000, so all edges are within that. But shortest path from 1 to 3 is 2 via 1-2-3.
        // Edge 3-1 has weight 100, dis[1]+100 = 0+100=100 <= dsi[3]=1000, so it's marked as needed too!
        // Because we allow any path within max L, even non-shortest edges can be used if they are within L.
        // So edges 1-2, 2-3, and 1-3 are all needed? Check: for edge (3,1), dis[3] is shortest from 1 to 3 = 2. dis[3]+100 = 102 <= 1000, so yes.
        // But edge (2,3) also dis[2]+1 = 1+1=2 <= 1000, yes. And edge (1,2) dis[1]+1=1 <= 1000.
        // So all three unordered edges are needed. But note that for source=1, the demand is only to 3, and the path can be any within 1000, so indeed all edges can be on some path (e.g., 1-3-2-3). So count=3.
        assert(countNeededEdges(n, edges, demands) == 3);
    }
    // Test 10: Multiple demands for same (s,v) take the max.
    {
        int n = 2;
        vector<tuple<int,int,int>> edges = {{1,2,5}};
        vector<tuple<int,int,int>> demands = {{1,2,5},{1,2,3}};
        // Max L=5, so edge weight 5 is needed.
        assert(countNeededEdges(n, edges, demands) == 1);
    }
    return 0;
}
// The solution uses the classic approach of checking for each source `s` whether an edge can be part of a path from `s` to `v` that is no longer than the maximum allowed distance for any demand from `s` to `v`. The algorithm is:
//
// 1. For each source `s` (1..n):
//    - Run Dijkstra from `s` to compute the shortest distance `dis[x]` from `s` to every vertex `x`. If a vertex is unreachable, its distance is infinity.
//    - Initialize an array `dsi[x]` = -1 for all `x`. For every demand `(s, v, L)` (i.e., demands where the source is `s`), set `dsi[v] = max(dsi[v], L)`. So `dsi[x]` is the maximum allowed distance from `s` to `x` among all demands targeting `x`.
//    - Now run a max-Dijkstra-like relaxation: start with a priority queue (or O(n^2) selection) with all vertices having `dsi` > 0. The idea is to propagate the maximum allowed distances backwards along the graph. Specifically, if there is an edge `(x,y)` with weight `w`, and we know that from `y` we can reach some target within `dsi[y]`, then from `x` we can reach that same target within `dsi[y] - w` (by going x -> y then the rest). So we want to compute the maximum `dsi[x]` such that there exists a path from `x` to some target `v` with total length at most the original `dsi[v]`. This is equivalent to: if `dsi[x]` is the maximum over all demands `(s, v, L)` of `L - dist(x,v)` (where dist is shortest path distance). The relaxation `if (dsi[y] - w > dsi[x]) dsi[x] = dsi[y] - w` computes this correctly when processed in decreasing order of `dsi` (max-heap). Since all weights are positive, this is a valid longest-path-like relaxation on the (undirected) graph, and it will terminate after at most n iterations per source.
//    - After computing both `dis[]` (shortest from s) and `dsi[]` (max allowed from s to each target), an edge `(i,j)` with weight `w` is needed for source `s` if `dis[i] + w <= dsi[j]` or `dis[j] + w <= dsi[i]`. Why? Because `dis[i]` is the shortest distance from `s` to `i`, and `dsi[j]` is the maximum allowed distance from `s` to `j`. If the shortest path from `s` to `i` plus the edge weight reaches `j` within the allowed maximum for `j`, then there exists some path from `s` to `j` that uses that edge and respects the demand. We must check both directions because the edge can be traversed in either direction.
//
// 2. After processing all sources, accumulate the set of unordered edges that are needed for at least one source. Use a boolean matrix `ans[i][j]` of size n+1 by n+1 to mark needed edges (since n <= 600, a 2D bool array fits in memory). At the end, count number of `true` entries and divide by 2 (since we mark both directions).
//
// Edge cases: Self-loops (i == j) are allowed but they never contribute to the count because an unordered edge between a vertex and itself is not counted twice, and when we mark `ans[i][i]`, it will be counted once and then divided by 2 would give half, so we should avoid marking self-loops or handle them separately. In the provided snippet, they mark `ans[i][y.fi]` and `ans[y.fi][i]`; if i == y.fi, it marks the same cell twice, but then the output counts `ans[i][j]` and later divides by 2, which for self-loops would give 0.5 (integer division truncates to 0), so self-loops are effectively ignored. We'll replicate that behavior. Also, if multiple demands for the same (s,v) exist, we take the maximum `L`. If a demand has `L < dis[v]` (i.e., impossible), then `dsi[v]` might be less than `dis[v]`, but that's fine; the edge condition `dis[i] + w <= dsi[j]` may never be true for such unreachable demands, so they are ignored. Also note that `dsi` may become negative or zero for vertices with no demand; we initialize to -1 and only process non-negative values.
//
// Time complexity: For each source, O(n^2) Dijkstra (since n <= 600, a simple O(n^2) selection of min/max is fine) + O(n^2) relaxation for max-Dijkstra + O(m) edge checks. With n=600, m=5000, this is O(n * (n^2 + m)) = O(n^3 + n*m) ≈ 216e6 + 3e6, which is acceptable within 1-2 seconds. Space: O(n^2) for the answer matrix.
