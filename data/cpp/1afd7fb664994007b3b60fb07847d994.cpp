/*
You are given a connected undirected graph with `n` vertices and `m` weighted edges, plus two special parameters `k` (maximum number of special edge upgrades) and a starting vertex `s` and target vertex `t`. For each regular edge of weight `w`, you have the option to use at most once (per traversed edge) a “magic” variant of that edge that costs `cost(w)` instead of `w`, where `cost(w)` is computed as follows: let `p` be the smallest prime factor of `w`; then `cost(w) = min(w, 2*p + w/p)`. However, using a magic variant of any edge consumes one of your `k` total upgrades, and you may use at most `k` such upgraded edges in your entire path from `s` to `t`. You may traverse any edge (normal or magic) any number of times, but each traversal of a magic edge costs one upgrade. Write a C++ function `long long minCostPath(int n, int m, int k, vector<tuple<int,int,int>> edges, int s, int t)` that returns the minimum total cost to go from `s` (1-indexed in input) to `t` (1-indexed) using at most `k` upgraded edges, or `-1` if `t` is unreachable (the graph is connected, but if the function is general, handle it). The graph is undirected, edge weights are positive integers, and `k` may be zero (in which case you cannot use any upgraded edge). The number of vertices `n` and edges `m` are positive, and `k ≤ n` (but you might have more edges than vertices). The result may be large but fits in a 64‑bit signed integer.
*/
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Compute the cost of the "magic" variant of an edge of weight w.
// Returns min(w, 2*smallest_prime_factor + w / smallest_prime_factor).
ll magicCost(ll w) {
    ll original = w;
    ll best = w;
    ll x = w;
    // Find smallest prime factor
    for (ll p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            best = min(best, 2LL * p + w / p);
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) {
        best = min(best, 2LL * x + w / x);
    }
    return best; // This may be < w, equal, or even > w; we only use if < w.
}

// Returns minimum cost from s to t using at most k upgraded edges, or -1.
long long minCostPath(int n, int m, int k,
                      const vector<tuple<int,int,ll>>& edges,
                      int s, int t) {
    // Convert to 0-based indexing
    int S = s - 1;
    int T = t - 1;

    // Number of nodes in layered graph: n*(k+1)
    const ll INF = 0x3f3f3f3f3f3f3f3fLL;
    vector<vector<pair<ll,int>>> adj(n * (k + 1));
    auto addEdge = [&](int u, int v, ll w) {
        adj[u].push_back({w, v});
    };

    // Build layered graph
    for (auto [a, b, w] : edges) {
        --a; --b; // to 0-based
        // Normal edges: in every layer j, both directions
        for (int j = 0; j <= k; ++j) {
            int base = j * n;
            addEdge(base + a, base + b, w);
            addEdge(base + b, base + a, w);
        }
        // Magic edges: from layer j to j+1 if magic cost is better
        ll mc = magicCost(w);
        if (mc < w) {
            for (int j = 0; j < k; ++j) {
                int cur = j * n;
                int nxt = (j + 1) * n;
                addEdge(cur + a, nxt + b, mc);
                addEdge(cur + b, nxt + a, mc);
            }
        }
    }

    // Dijkstra on layered graph
    vector<ll> dist(n * (k + 1), INF);
    vector<bool> vis(n * (k + 1), false);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    dist[S] = 0;
    pq.push({0, S});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (vis[u]) continue;
        vis[u] = true;
        // If we reached the target vertex in any layer, we are done
        // (since we pop in increasing distance order)
        if (u % n == T) return d;
        for (auto [w, v] : adj[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    return -1;
}
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Declare the function from the solution (put include of solution header here)
ll magicCost(ll w);
long long minCostPath(int n, int m, int k,
                      const vector<tuple<int,int,ll>>& edges,
                      int s, int t);

int main() {
    // Test 1: simple triangle, no upgrades needed
    // n=3, edges: 1-2 (5), 2-3 (7), 1-3 (9), k=0, s=1, t=3
    vector<tuple<int,int,ll>> e1 = {{1,2,5},{2,3,7},{1,3,9}};
    assert(minCostPath(3,3,0,e1,1,3) == 12); // 5+7

    // Test 2: with one upgrade, might be cheaper
    // Edge 1-2 (15): magic cost = min(15, 2*3+5=11) = 11, so 11+7=18? Actually 11+7=18, but normal 5+7=12 is cheaper.
    // Let's design a case where magic helps: edge 1-2 weight 14 (magic cost: smallest prime 2 => 2*2+7=11), and edge 2-3 weight 1.
    // Without upgrade: 14+1=15, with upgrade: 11+1=12.
    vector<tuple<int,int,ll>> e2 = {{1,2,14},{2,3,1}};
    assert(minCostPath(3,2,1,e2,1,3) == 12); // use magic on first edge

    // Test 3: k=0 means no magic allowed, so same as above but with k=0 gives 15
    assert(minCostPath(3,2,0,e2,1,3) == 15);

    // Test 4: multiple edges, k larger than needed, answer stays same
    assert(minCostPath(3,2,5,e2,1,3) == 12);

    // Test 5: disconnected (should return -1)
    vector<tuple<int,int,ll>> e3 = {{1,2,3},{3,4,4}};
    // n=4, s=1, t=4: no path
    assert(minCostPath(4,2,1,e3,1,4) == -1);

    // Test 6: self-loop? Problem says undirected simple graph, but if there is a self-loop, we ignore? We assume no self-loops, but test with a simple line.
    // Test 6: single edge, k=0, direct
    vector<tuple<int,int,ll>> e4 = {{1,2,10}};
    assert(minCostPath(2,1,0,e4,1,2) == 10);
    // With k=1, magic cost of 10 = min(10, 2*2+5=9) = 9, so 9
    assert(minCostPath(2,1,1,e4,1,2) == 9);

    // Test 7: larger weight where magic cost may be > w (e.g., prime weight 13: smallest prime 13, 2*13+1=27 >13 so no improvement)
    vector<tuple<int,int,ll>> e5 = {{1,2,13}};
    assert(minCostPath(2,1,1,e5,1,2) == 13); // no upgrade used

    // Test 8: k=0 and path uses multiple edges
    vector<tuple<int,int,ll>> e6 = {{1,2,2},{2,3,3},{3,4,4}};
    assert(minCostPath(4,3,0,e6,1,4) == 9);

    // Test 9: k=2 and magic on two edges
    // Edge weights: 1-2: 14 (magic 11), 2-3: 10 (magic 9), 3-4: 1
    vector<tuple<int,int,ll>> e7 = {{1,2,14},{2,3,10},{3,4,1}};
    assert(minCostPath(4,3,0,e7,1,4) == 14+10+1); // 25
    assert(minCostPath(4,3,1,e7,1,4) == min(11+10+1, 14+9+1) == 22); // 22
    assert(minCostPath(4,3,2,e7,1,4) == 11+9+1); // 21

    // Test 10: all edges have magic cost >= w, so upgrades don't matter
    vector<tuple<int,int,ll>> e8 = {{1,2,13},{2,3,17}};
    assert(minCostPath(3,2,5,e8,1,3) == 30); // no benefit

    cout << "All tests passed." << endl;
    return 0;
}
// The problem is a classic “shortest path with at most `k` special edges” which can be solved by constructing a layered graph (also called a “state graph” or “expanded graph”). We create `k+1` copies of the original vertex set, where copy `j` (0 ≤ j ≤ k) represents being at a vertex having used exactly `j` upgraded edges so far. For each original undirected edge of weight `w` between `a` and `b`, we add two kinds of transitions: (1) a normal transition in all copies `j` from `a` to `b` and `b` to `a` with cost `w` (this does not increase the upgrade count), and (2) if the magic cost `cost(w) < w`, we add a transition from copy `j` to copy `j+1` (for `j=0..k-1`) from `a` to `b` and `b` to `a` with cost `cost(w)`. The total number of nodes is `n*(k+1)`, and each original edge contributes up to `2*(k+1)` normal directed edges and up to `2*k` magic directed edges. Then we run Dijkstra’s algorithm on this layered graph starting from `(s,0)`. The answer is the minimum over all `j` of the distance to `(t,j)` (since we may use fewer than `k` upgrades). We stop as soon as we pop a node whose vertex index (mod `n`) equals `t`, because Dijkstra processes nodes in increasing distance order and any later node will have distance at least that. If distance remains infinite, return `-1`. Complexity: nodes = `O(n*k)`, edges = `O(m*k)` (each normal edge appears in all layers, each magic edge appears in `k` layers), so time is `O(m*k log(n*k))` and space `O(n*k + m*k)`. Edge cases: `k=0` means no magic edges; the magic cost formula might be larger than `w` (then we do not add the magic edge); weights are positive so Dijkstra works; the graph is undirected so we add both directions; the input uses 1‑based vertices and we convert to 0‑based internally.
