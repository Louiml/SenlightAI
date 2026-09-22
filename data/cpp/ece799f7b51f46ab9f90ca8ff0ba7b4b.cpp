// Given an undirected graph with `n` vertices (numbered 1 to `n`) and `m` edges, where each edge connects `u` and `v` and has two integer parameters `c` and `d`, write a C++ function `long long shortestAvoidanceTime(int n, const vector<Edge>& edges)` that computes the minimum possible travel time from vertex 1 to vertex `n`. For each edge, if you arrive at one endpoint at time `T`, you may traverse the edge, taking exactly `c + d / (T+1)` additional time units, where `/` denotes integer division (truncation). However, you may choose to wait at any vertex before traversing an edge; if you depart at time `t` (where `t >= T`), the total time to reach the other endpoint becomes `t + c + d/(t+1)`. You may wait at any vertex as long as needed, including before the first move (starting time at vertex 1 is 0). If vertex `n` is unreachable, return `-1`. All `c` and `d` are non-negative integers, `n` can be up to 200000, and `m` up to 200000. The graph may contain multiple edges and self-loops.
#include <cassert>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (or ideally in the same translation unit).

int main() {
    // Test 1: Simple two-node graph
    {
        int n = 2;
        vector<vector<Edge>> edges(n + 1);
        edges[1].push_back({2, 1, 0});
        edges[2].push_back({1, 1, 0});
        assert(shortestAvoidanceTime(n, edges) == 1);
    }
    
    // Test 2: Graph with waiting advantage
    {
        int n = 2;
        vector<vector<Edge>> edges(n + 1);
        // d=100, optimal departure ~9, c=0
        edges[1].push_back({2, 0, 100});
        edges[2].push_back({1, 0, 100});
        long long ans = shortestAvoidanceTime(n, edges);
        // If departure at t=9: 9 + 0 + 100/10=19; if t=0: 0+0+100=100; so best is 19.
        assert(ans == 19);
    }
    
    // Test 3: Unreachable graph
    {
        int n = 3;
        vector<vector<Edge>> edges(n + 1);
        edges[1].push_back({2, 1, 1});
        edges[2].push_back({1, 1, 1});
        // vertex 3 isolated
        assert(shortestAvoidanceTime(n, edges) == -1);
    }
    
    // Test 4: Self-loop and multiple edges
    {
        int n = 2;
        vector<vector<Edge>> edges(n + 1);
        edges[1].push_back({1, 5, 100}); // self-loop: doesn't help reach 2
        edges[1].push_back({2, 2, 0});
        edges[1].push_back({2, 10, 1000}); // another edge
        edges[2].push_back({1, 2, 0});
        edges[2].push_back({1, 10, 1000});
        assert(shortestAvoidanceTime(n, edges) == 2);
    }
    
    // Test 5: Large d, check waiting works correctly
    {
        int n = 2;
        vector<vector<Edge>> edges(n + 1);
        long long d = 1000000;
        // optimal departure ~ sqrt(d)-1 ≈ 999, wait yields ~999 + 1000000/1000 = 1999.
        edges[1].push_back({2, 0, d});
        edges[2].push_back({1, 0, d});
        long long ans = shortestAvoidanceTime(n, edges);
        long long best = LLONG_MAX;
        for (long long t = 0; t <= 2000; ++t) {
            best = min(best, t + 0 + d / (t + 1));
        }
        assert(ans == best);
    }
    
    // Test 6: Start at destination
    {
        int n = 1;
        vector<vector<Edge>> edges(n + 1);
        assert(shortestAvoidanceTime(n, edges) == 0);
    }
    
    // Test 7: Path with multiple vertices
    {
        int n = 4;
        vector<vector<Edge>> edges(n + 1);
        edges[1].push_back({2, 3, 0});
        edges[2].push_back({1, 3, 0});
        edges[2].push_back({3, 1, 100});
        edges[3].push_back({2, 1, 100});
        edges[3].push_back({4, 2, 0});
        edges[4].push_back({3, 2, 0});
        // Direct path: 1->2 (0+3=3) then 2->3 (wait? from time 3, depart max(3,9)=9, arrival 9+1+100/10=20) then 3->4 (20+2=22)
        // Alternative: maybe wait at 1? no. So answer 22.
        assert(shortestAvoidanceTime(n, edges) == 22);
    }
    
    // Test 8: Very large n but small graph (stress basic)
    {
        int n = 5;
        vector<vector<Edge>> edges(n + 1);
        edges[1].push_back({5, 100, 0});
        edges[5].push_back({1, 100, 0});
        assert(shortestAvoidanceTime(n, edges) == 100);
    }
    
    printf("All tests passed.\n");
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    long long c;
    long long d;
};

// Compute the optimal departure time for an edge with parameter d, given current arrival time cur.
long long optimalDeparture(long long cur, long long d) {
    // For f(t)=t+c+d/(t+1), the minimizer is near sqrt(d)-1.
    // Use sqrtl for precision.
    long long opt = (long long)(sqrtl((long double)d) + 0.5) - 1;
    if (opt < 0) opt = 0;
    return max(cur, opt);
}

// Compute shortest travel time from vertex 1 to vertex n.
// edges[u] is the adjacency list of edges from u.
long long shortestAvoidanceTime(int n, const vector<vector<Edge>>& edges) {
    const long long INF = LLONG_MAX / 4;
    vector<long long> dist(n + 1, INF);
    vector<char> used(n + 1, false);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    
    dist[1] = 0;
    pq.push({0, 1});
    
    while (!pq.empty()) {
        auto [dcur, u] = pq.top();
        pq.pop();
        if (used[u]) continue;
        used[u] = true;
        if (u == n) break;
        
        for (const Edge& e : edges[u]) {
            long long t = optimalDeparture(dcur, e.d);
            long long arrival = t + e.c + e.d / (t + 1); // integer division
            if (arrival < dist[e.to]) {
                dist[e.to] = arrival;
                pq.push({arrival, e.to});
            }
        }
    }
    
    return (dist[n] == INF) ? -1 : dist[n];
}
// The problem is a variant of shortest path where the time to traverse an edge depends on the departure time. The key observation is that for each edge, the function `f(t) = t + c + d/(t+1)` is convex for non-negative integer `t`. This means the optimal departure time for using that edge from a vertex is either the current arrival time `T` or possibly a later time near the minimizer of `f(t)`, which occurs around `t ≈ sqrt(d) - 1`. Specifically, the optimal departure time is either `T` or the integer `round(sqrt(d)) - 1` (with floor/ceil carefulness), because the function decreases then increases. Thus we can modify Dijkstra's algorithm: when relaxing an edge from a node with current best time `dis[u]`, the candidate time to reach `v` is `bestDeparture(dis[u], d) + c + d / (bestDeparture+1)`, where `bestDeparture = max(dis[u], optimalDeparture(d))` with `optimalDeparture(d) = round(sqrt(d)) - 1`. Since `d` can be large (up to 1e18?), we compute the square root using `sqrtl` and adjust for integer rounding. The algorithm runs a standard priority_queue Dijkstra. Time complexity: O((n+m) log n) for the Dijkstra, with O(1) per edge for the sqrt calculation. Space O(n+m). Edge cases: If no path exists, dis[n] remains infinite, return -1; if `n=1`, answer 0; self-loops and multiple edges are handled naturally.
