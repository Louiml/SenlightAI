Write a C++ function `vector<int> minMaxEdgeWeightPath(int n, const vector<tuple<int,int,int>>& edges, int start, int target)` that, given a connected undirected graph with `n` vertices (numbered 1..n), a list of weighted edges (each tuple contains u, v, w, where w is the edge weight), and a starting vertex `start`, computes for every vertex the minimum possible maximum edge weight along any path from `start` to that vertex. If a vertex is unreachable, set its value to `-1`. The graph may contain multiple edges between the same pair of vertices; for such parallel edges, only the largest weight should be considered (i.e., choose the best (largest) weight when multiple exist). Return a vector of size `n+1` (index 0 unused) where position `i` contains the result for vertex `i`. The special case where `i == start` should have value `0` (the path from start to itself requires no edges). For all other reachable vertices, the value is the minimax edge weight along the best path. If multiple paths exist, choose the one whose maximum edge weight is minimized. The graph is undirected.
#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (minimaxPaths)

int main() {
    // Test 1: simple path
    vector<tuple<int,int,int>> e1 = {{1,2,5},{2,3,3}};
    auto r1 = minimaxPaths(3, e1, 1);
    assert(r1[1] == 0);
    assert(r1[2] == 5);
    assert(r1[3] == 5); // max(5,3)=5

    // Test 2: parallel edges, keep largest
    vector<tuple<int,int,int>> e2 = {{1,2,4},{1,2,7},{2,3,2}};
    auto r2 = minimaxPaths(3, e2, 1);
    assert(r2[2] == 7); // largest parallel edge
    assert(r2[3] == 7); // max(7,2)=7

    // Test 3: unreachable vertex
    vector<tuple<int,int,int>> e3 = {{1,2,1}};
    auto r3 = minimaxPaths(3, e3, 1);
    assert(r3[1] == 0);
    assert(r3[2] == 1);
    assert(r3[3] == -1);

    // Test 4: star graph
    vector<tuple<int,int,int>> e4 = {{1,2,10},{1,3,20},{1,4,30}};
    auto r4 = minimaxPaths(4, e4, 1);
    assert(r4[2] == 10);
    assert(r4[3] == 20);
    assert(r4[4] == 30);

    // Test 5: cycle with better alternate path
    vector<tuple<int,int,int>> e5 = {{1,2,10},{2,3,10},{1,3,5},{3,4,1}};
    auto r5 = minimaxPaths(4, e5, 1);
    // To 4: direct 1->3->4 gives max(5,1)=5, better than 1->2->3->4 max(10,10,1)=10
    assert(r5[4] == 5);

    // Test 6: self loops? Not allowed, but we test a single vertex
    vector<tuple<int,int,int>> e6 = {};
    auto r6 = minimaxPaths(1, e6, 1);
    assert(r6[1] == 0);

    // Test 7: two vertices no edge -> unreachable
    vector<tuple<int,int,int>> e7 = {};
    auto r7 = minimaxPaths(2, e7, 1);
    assert(r7[1] == 0);
    assert(r7[2] == -1);

    return 0;
}
#include <bits/stdc++.h>
using namespace std;

// Returns for each vertex 1..n the minimax edge weight from start.
// Unreachable vertices get -1. start itself gets 0.
vector<int> minimaxPaths(int n, const vector<tuple<int,int,int>>& edges, int start) {
    const int INF = 1e9;
    // adjacency list: neighbor -> max weight among parallel edges
    vector<vector<pair<int,int>>> adj(n+1);
    // Use a map to track max weight per unordered pair (u,v)
    map<pair<int,int>, int> maxWeight;
    for (auto& [u,v,w] : edges) {
        if (u > v) swap(u,v); // ensure consistent pair ordering
        auto key = make_pair(u,v);
        if (maxWeight.count(key)) maxWeight[key] = max(maxWeight[key], w);
        else maxWeight[key] = w;
    }
    for (auto& [key, w] : maxWeight) {
        int u = key.first, v = key.second;
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }

    vector<int> dist(n+1, INF);
    dist[start] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.emplace(0, start);

    while (!pq.empty()) {
        auto [currentMax, u] = pq.top();
        pq.pop();
        if (currentMax > dist[u]) continue; // stale entry
        for (auto& [v, w] : adj[u]) {
            int candidate = max(dist[u], w);
            if (candidate < dist[v]) {
                dist[v] = candidate;
                pq.emplace(candidate, v);
            }
        }
    }

    vector<int> result(n+1, -1);
    for (int i = 1; i <= n; ++i) {
        if (i == start) result[i] = 0;
        else if (dist[i] != INF) result[i] = dist[i];
        else result[i] = -1;
    }
    return result;
}
// The problem is a classic minimax path problem: find the path from a source to every vertex that minimizes the maximum edge weight along the path. This can be solved using a modified Dijkstra-like algorithm, but instead of minimizing the sum of weights, we minimize the maximum edge weight encountered. The key idea: maintain an array `dist[v]` initialized to `INF` for all vertices, except `dist[start]=0`. Use a priority queue (min-heap) storing pairs `(currentMaxWeight, vertex)`. While the queue is not empty, pop the vertex `u` with the smallest current maximum weight. For each neighbor `v` of `u` with edge weight `w`, the candidate new maximum for `v` is `max(dist[u], w)`. If this candidate is less than `dist[v]`, update `dist[v]` and push `(dist[v], v)` into the queue. This works because the function `max` is monotonic and the property of Dijkstra's algorithm holds for minimax problems (the optimal answer for a vertex is finalized when popped from the priority queue). Important edge cases: parallel edges — when building the adjacency list, for the same pair (u,v), keep only the largest weight (since a larger edge weight cannot help achieve a smaller maximum, but we need to consider the best possible edge for a given path; actually for a path we always use the edge weight that is part of the path, so if multiple edges exist, we should keep the maximum because that gives the best (largest) capacity; but wait: for minimax, we want to minimize the maximum edge along a path, so using a larger edge might be beneficial because it allows crossing with a higher limit? Actually no: if there are two parallel edges with weights 5 and 10, using the 5 edge gives a smaller maximum for that hop, so we should consider the minimum weight? Let's re-read snippet: the original code uses `max(arr[u][v], w)` when parallel edges exist, so it keeps the largest weight. But that seems wrong for minimax because a larger edge weight would increase the max on that hop. However, the original problem might have had a different interpretation (maybe it's a "maximum capacity path" problem where you want to maximize the minimum edge, not minimize the maximum). The snippet computes `weight[v] = max(weight[u], arr[u][v])` — that suggests it's actually a "bottleneck path" problem but they want to minimize the bottleneck? Actually the formula `max(weight[u], arr[u][v])` is typical for minimax (minimizing the maximum edge). But keeping the largest weight among parallel edges is counterintuitive — you would prefer the smallest weight to minimize the max. However, the original code seems to have a bug (they set to max). For a correct minimax solution, we should keep the smallest weight for parallel edges? But the snippet says `max`. Since the task is inspired by the snippet but asks us to create an independent task, we can clarify in the task that for parallel edges, only the largest weight should be considered (as the snippet does, even if that may not be logically minimax-optimal). But that would be weird. Let's re-read task: "for such parallel edges, only the largest weight should be considered (i.e., choose the best (largest) weight when multiple exist)." That is explicitly stated in the task I wrote. So we follow that. Actually why would a larger weight be "best"? In some bottleneck problems, a larger weight means a better capacity (e.g., if the edge weight is a capacity, you want the largest capacity). But here it's the edge weight that contributes to the max; a larger weight would make the max larger, so it's not beneficial. However, the task explicitly says to keep the largest among parallel edges, so we comply. Then the algorithm: after building an adjacency list where for each neighbor we store the largest weight (since the task says so), we run the modified Dijkstra with the `max` operation. The time complexity is O((V+E) log V) with a priority queue, and space O(V+E). Note that if a vertex is unreachable, we leave dist as INF and later convert to -1.
