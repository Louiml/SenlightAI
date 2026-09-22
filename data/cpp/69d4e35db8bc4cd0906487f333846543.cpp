Write a standalone C++ function `long long minimumCostToVisitAll(const std::vector<int>& vertexCosts, const std::vector<std::tuple<int,int,long long>>& directedEdges)` (indices 0-based) that solves the following problem. You are given `n` cities (indices `0` to `n-1`) and `m` directed roads. Each city has a "production cost" `c[i]`. Starting from any city, you can produce one unit at city `i` by paying `c[i]`, then transport it along directed roads; using a road `(u,v,w)` costs `w` per unit and allows movement from `u` to `v` (you may traverse multiple roads). You need to deliver exactly one unit **to every city** (including the starting city itself), but each city can act as a production source at most once (so you produce at most one unit per city, and all units must be produced somewhere, possibly the same city that receives it). The goal is to minimize the total production plus transportation cost. You may reuse roads arbitrarily (unlimited capacity) but must respect flow conservation: each unit starts at its production city and ends at exactly one distinct destination city, forming a bijection between production cities and destination cities (a permutation). The function should return the minimum total cost, or `LLONG_MAX` if impossible (i.e., some city cannot be reached from any other city via directed roads, considering cycles of zero cost are allowed for staying). Guarantee: input sizes satisfy `2 ≤ n ≤ 200`, `0 ≤ m ≤ n*(n-1)`, all costs are non‑negative (long long, fit in 64‑bit). Edge cases: if `n=0` (not in input) return 0; if a city cannot be reached from any other city (including itself with zero‑cost self‑loop? but no self‑loops given), then impossible.
// The problem is a classic minimum‑cost flow (MCF) with supplies/demands. Model it as a bipartite flow network: left side represents "production" (source side), right side represents "consumption" (destination side). We create a source node `S` and sink node `T`. For each city i (0-indexed), create a left node `L_i` and a right node `R_i`. Add edge `S -> L_i` with capacity 1 and cost 0 (each city can produce at most one unit). Add edge `R_i -> T` with capacity 1 and cost 0 (each city must receive exactly one unit). For every city i, add edge `L_i -> R_i` with capacity `n` (unlimited for practical purposes) and cost `c[i]` (produce and deliver locally). For every directed road `(u,v,w)`, add edge `L_u -> R_v` with capacity `n` and cost `w` (transport from production at u to destination v). Then find the min‑cost max‑flow from S to T. Because all supplies (n units) must be sent, the max flow equals n. If the max flow is less than n, return `LLONG_MAX`. Implement a standard successive shortest‑path algorithm using Bellman‑Ford (or SPFA) because there are negative edges? Actually all costs are non‑negative, so Dijkstra with potentials works, but to keep it simple we can use SPFA (Bellman‑Ford) which handles negative cycles? Here all costs are non‑negative, so no negative cycles. However, the residual graph has reverse edges with negative costs, but we only push flow on augmenting paths; SPFA works with negative edges. Complexity: at most n augmentations (each sends 1 unit if capacities are 1 on source/sink edges), each SPFA runs O(V*E) worst‑case, with V = 2n+2 ≤ 402, E = O(n^2) (edges: n source edges, n sink edges, n self edges, m road edges). So total O(n * n * n^2) = O(n^4) in worst case, acceptable for n=200? 200^4 = 1.6e9 too high. Better use Dijkstra with potentials (Johnson's) or since costs are non‑negative, we can use Dijkstra directly. But residual edges have negative costs after reversing, so we need potentials. Since all original costs are non‑negative, we can initialize potentials to 0, and after each Dijkstra, update potentials: potential[v] += dist[v]. Then reduced cost = original cost + potential[u] - potential[v]. This is standard. Complexity O(n * (E log V)) ≈ 200*40000*log 400 ≈ 200*40000*9 ≈ 72M, fine. We'll implement Dijkstra with potentials using `std::priority_queue`. Edge case: if a city cannot be reached from any other city (including itself? but no self‑loops given; but you can produce locally with cost c[i] even if no roads exist because L_i -> R_i edge exists, so that city is reachable from itself via that edge). So it's always feasible if n>0? Actually the production network has L_i->R_i edge for every city, so every city can be served by itself with cost c[i]. Therefore max flow is always n. So the "impossible" case never happens. Wait, but the problem says "starting from any city, you can produce ... then transport along directed roads to any city." But you must deliver exactly one unit to every city, and each city produces at most once. So you need a perfect matching between producers and consumers, where producer i can serve consumer j if there's a path from i to j with finite cost (including zero length for i=j with cost c[i]). Since L_i->R_i exists for all i, every city can serve itself. Thus always feasible. However, the problem statement might want to allow that you cannot produce from a city that doesn't have enough incoming? Actually no, you can produce at any city. So always feasible. But for safety, we still check if max flow < n and return INF. We'll implement MCF using edges with capacity and cost. The function signature: `long long minimumCostToVisitAll(const std::vector<int>& vertexCosts, const std::vector<std::tuple<int,int,long long>>& directedEdges)`. We'll build network with size = 2*n + 2. Indices: S=0, T=2*n+1, L_i = 1+i, R_i = 1+n+i. For each i, add edge S->L_i cap=1 cost=0; R_i->T cap=1 cost=0; L_i->R_i cap=n cost=vertexCosts[i]; for each (u,v,w) add L_u->R_v cap=n cost=w. Then run MCF. Return total cost.
#include <vector>
#include <tuple>
#include <limits>
#include <queue>
#include <algorithm>
#include <cstdint>

// Edge structure for min-cost max-flow
struct MCFEdge {
    int to, rev;
    long long cap, cost;
    MCFEdge(int t, int r, long long c, long long co) : to(t), rev(r), cap(c), cost(co) {}
};

// Min-cost max-flow implementation using Dijkstra with potentials
long long minCostMaxFlow(const std::vector<std::vector<MCFEdge>>& graph, int S, int T) {
    const int n = graph.size();
    const long long INF = std::numeric_limits<long long>::max() / 4;
    long long totalCost = 0;
    long long maxFlow = 0;
    std::vector<long long> potential(n, 0);
    std::vector<int> prevv(n), preve(n);

    while (true) {
        std::vector<long long> dist(n, INF);
        std::vector<bool> used(n, false);
        dist[S] = 0;
        using P = std::pair<long long, int>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
        pq.push({0, S});

        while (!pq.empty()) {
            auto [d, v] = pq.top(); pq.pop();
            if (dist[v] < d) continue;
            for (int i = 0; i < (int)graph[v].size(); ++i) {
                const auto &e = graph[v][i];
                if (e.cap > 0) {
                    long long nd = dist[v] + e.cost + potential[v] - potential[e.to];
                    if (nd < dist[e.to]) {
                        dist[e.to] = nd;
                        prevv[e.to] = v;
                        preve[e.to] = i;
                        pq.push({nd, e.to});
                    }
                }
            }
        }

        if (dist[T] == INF) break;

        for (int v = 0; v < n; ++v) {
            if (dist[v] < INF) potential[v] += dist[v];
        }

        long long add = INF;
        for (int v = T; v != S; v = prevv[v]) {
            add = std::min(add, graph[prevv[v]][preve[v]].cap);
        }
        for (int v = T; v != S; v = prevv[v]) {
            auto &e = graph[prevv[v]][preve[v]];
            e.cap -= add;
            graph[v][e.rev].cap += add;
            totalCost += add * e.cost;
        }
        maxFlow += add;
    }

    // If max flow is less than n, we'd return INF, but here we just return totalCost assuming all reachable.
    // The caller will check if maxFlow < required. We'll return totalCost and also set maxFlow via reference? Simpler: return totalCost and rely on caller to know flow is always n.
    return totalCost;
}

// Main solution: compute minimum total cost to deliver one unit to each city.
long long minimumCostToVisitAll(const std::vector<int>& vertexCosts,
                                const std::vector<std::tuple<int,int,long long>>& directedEdges) {
    const int n = (int)vertexCosts.size();
    if (n == 0) return 0;

    const int S = 0;
    const int T = 2 * n + 1;
    const int totalNodes = 2 * n + 2;

    std::vector<std::vector<MCFEdge>> graph(totalNodes);
    auto add_edge = [&](int u, int v, long long cap, long long cost) {
        graph[u].emplace_back(v, (int)graph[v].size(), cap, cost);
        graph[v].emplace_back(u, (int)graph[u].size() - 1, 0, -cost);
    };

    // S -> L_i (capacity 1, cost 0)
    // R_i -> T (capacity 1, cost 0)
    for (int i = 0; i < n; ++i) {
        int L = 1 + i;
        int R = 1 + n + i;
        add_edge(S, L, 1, 0);
        add_edge(R, T, 1, 0);
        // L_i -> R_i with cost vertexCosts[i] (produce locally)
        add_edge(L, R, n, vertexCosts[i]);
    }

    // Directed roads: L_u -> R_v with cost w
    for (const auto& [u, v, w] : directedEdges) {
        int L_u = 1 + u;
        int R_v = 1 + n + v;
        add_edge(L_u, R_v, n, w);
    }

    // Find min cost flow of n units
    long long totalCost = 0;
    long long maxFlow = 0;
    const long long INF = std::numeric_limits<long long>::max() / 4;
    std::vector<long long> potential(totalNodes, 0);
    std::vector<int> prevv(totalNodes), preve(totalNodes);

    while (maxFlow < n) {
        std::vector<long long> dist(totalNodes, INF);
        dist[S] = 0;
        using P = std::pair<long long, int>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
        pq.push({0, S});

        while (!pq.empty()) {
            auto [d, v] = pq.top(); pq.pop();
            if (dist[v] < d) continue;
            for (int i = 0; i < (int)graph[v].size(); ++i) {
                const auto& e = graph[v][i];
                if (e.cap > 0) {
                    long long nd = dist[v] + e.cost + potential[v] - potential[e.to];
                    if (nd < dist[e.to]) {
                        dist[e.to] = nd;
                        prevv[e.to] = v;
                        preve[e.to] = i;
                        pq.push({nd, e.to});
                    }
                }
            }
        }

        if (dist[T] == INF) break; // cannot push more flow

        for (int v = 0; v < totalNodes; ++v) {
            if (dist[v] < INF) potential[v] += dist[v];
        }

        long long add = INF;
        for (int v = T; v != S; v = prevv[v]) {
            add = std::min(add, graph[prevv[v]][preve[v]].cap);
        }
        for (int v = T; v != S; v = prevv[v]) {
            auto& e = graph[prevv[v]][preve[v]];
            e.cap -= add;
            graph[v][e.rev].cap += add;
            totalCost += add * e.cost;
        }
        maxFlow += add;
    }

    if (maxFlow < n) return std::numeric_limits<long long>::max();
    return totalCost;
}
#include <cassert>
#include <vector>
#include <tuple>
#include <limits>

// Function declaration
long long minimumCostToVisitAll(const std::vector<int>& vertexCosts,
                                const std::vector<std::tuple<int,int,long long>>& directedEdges);

int main() {
    // Test 1: no roads, n=1. Only one city, produce and deliver locally.
    assert(minimumCostToVisitAll({5}, {}) == 5);

    // Test 2: two cities, no roads. Must produce locally each.
    assert(minimumCostToVisitAll({3, 7}, {}) == 10);

    // Test 3: two cities with a road from 0 to 1 cost 1, production costs 10 and 0.
    // Produce at 1 (cost 0) deliver to 1; produce at 0 (cost 10) deliver to 1 via road cost 1? But each city must receive one unit.
    // Better: produce at 1 (cost 0) deliver to 0 via? No road from 1 to 0. So produce at 0 (cost 10) deliver to 0, produce at 1 (cost 0) deliver to 1. Total 10. Or produce at 0 (10) deliver to 1 via road (1) = 11, and produce at 1 (0) deliver to 0? no road. So best 10.
    assert(minimumCostToVisitAll({10, 0}, {{0,1,1}}) == 10);

    // Test 4: two cities, road 0->1 cost 1, production costs 1 and 100.
    // Options: produce at 0 (1) deliver to 0, produce at 1 (100) deliver to 1 => 101.
    // Produce at 0 (1) deliver to 1 via road cost 1 => 2, and produce at 1 (100) deliver to 0? no road.
    // But produce at 1 cannot go to 0. So 101.
    assert(minimumCostToVisitAll({1, 100}, {{0,1,1}}) == 101);

    // Test 5: cycle two cities, both roads exist with costs 1 and 10.
    // n=2, costs {0, 5}. Best: produce at 0 (0) deliver to 1 via cost 1; produce at 1 (5) deliver to 0 via cost 10 => total 0+1+5+10=16.
    // Or produce locally both: 0+5=5 better! Because 0->0 cost 0, 1->1 cost 5 => total 5.
    assert(minimumCostToVisitAll({0, 5}, {{0,1,1},{1,0,10}}) == 5);

    // Test 6: 3 cities with costs {2, 3, 4}, roads 0->1 cost 1, 1->2 cost 1, 2->0 cost 1.
    // Best: produce at 0 (2) deliver to 0 (0) => 2; produce at 1 (3) deliver to 1 (0) =>3; produce at 2 (4) deliver to 2 (0)=>4 total 9.
    // Or produce at 0 (2) deliver to 1 via 1 =>3; produce at 1 (3) deliver to 2 via 1 =>4; produce at 2 (4) deliver to 0 via 1 =>5 total 12. So 9.
    assert(minimumCostToVisitAll({2,3,4}, {{0,1,1},{1,2,1},{2,0,1}}) == 9);

    // Test 7: empty vertexCosts (n=0) -> 0
    assert(minimumCostToVisitAll({}, {}) == 0);

    // Test 8: n=3 with a chain: costs {100,1,100}, roads 0->1 cost 1, 1->2 cost 1.
    // Best: produce at 1 (1) deliver to 0 via? 1->0 no road. But produce at 0 (100) deliver to 0, produce at 1 (1) deliver to 1, produce at 2 (100) deliver to 2 => 201.
    // But produce at 0 (100) ->1 via cost 1 =>101, then produce at 1 (1)->2 via cost 1 =>2 total 103, and produce at 2 (100)->0? no. So still need serve 0. Actually produce at 0 (100) deliver to 0; produce at 1 (1) deliver to 1; produce at 2 (100) deliver to 2 = 201.
    // Could produce at 1 (1) deliver to 1, produce at 2 (100) deliver to 2, produce at 0 (100) deliver to 0. Same.
    // No better. So 201.
    assert(minimumCostToVisitAll({100,1,100}, {{0,1,1},{1,2,1}}) == 201);

    // Test 9: n=3 with all roads zero cost, costs {5,5,5} -> best produce each locally: 15.
    assert(minimumCostToVisitAll({5,5,5}, {{0,1,0},{1,2,0},{2,0,0}}) == 15);

    // Test 10: n=2 with cost {0,10} and road 0->1 cost 5. Best: produce at 0 (0) deliver to 1 via 5, produce at 1 (10) deliver to 0? no road, so must deliver to 1 locally? Actually each city must receive one unit, but producer 1 can only deliver to 1 (no road to 0). So produce at 1 (10) deliver to 1, produce at 0 (0) deliver to 1 via road cost 5 -> total 0+5+10=15. Or produce at 0 deliver to 0 cost 0, produce at 1 deliver to 1 cost 10 =10. So 10.
    assert(minimumCostToVisitAll({0,10}, {{0,1,5}}) == 10);

    return 0;
}
