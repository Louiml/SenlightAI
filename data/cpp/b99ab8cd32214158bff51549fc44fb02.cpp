// Write a C++ function `bool hasNegativeCycle(int n, const std::vector<std::tuple<int,int,int>>& edges)` that takes the number of vertices `n` (vertices are labeled 1 through `n`) and a list of directed edges as `(u, v, w)` tuples, where `w` is the edge weight (may be negative, zero, or positive). The function should return `true` if there exists a negative cycle reachable from vertex 1, and `false` otherwise. An undirected edge with weight `w >= 0` is represented as two directed edges `(u,v,w)` and `(v,u,w)`; for negative `w`, only the directed edge `(u,v,w)` is given (this mirrors the snippet's logic where negative undirected edges are not reversed). If a negative cycle is reachable from vertex 1, the function must return `true`; otherwise `false`. Assume the graph may be disconnected and vertex 1 is always present. Use the Bellman-Ford algorithm to detect negative cycles; treat unreachable vertices as having infinite distance. Use a sentinel value representing infinity (e.g., `0x3f3f3f3f`) and avoid overflow when adding weights to infinity.
// The problem is a classic negative cycle detection in a directed graph, specifically for cycles reachable from a source (vertex 1). The underlying algorithm is Bellman-Ford. Initialize `dis[1] = 0` and all other distances to a large sentinel (say `INF = 0x3f3f3f3f`). Then relax all edges `n` times. After `n-1` relaxations, if an additional relaxation (the `n`-th pass) improves any distance, then a negative cycle is reachable from the source. However, the Bellman-Ford property guarantees that if a negative cycle exists somewhere in the graph, then distances along vertices reachable from that cycle will keep decreasing indefinitely. But if the cycle is not reachable from vertex 1, it won't affect the distances of reachable vertices, so the function should not return `true` in that case. The snippet's approach effectively runs `n` relaxation passes, and after that checks if any edge can still relax — but this can incorrectly detect negative cycles not reachable from the source because unreachable vertices have `dis[u] = INF` and `add(INF, w)` returns `INF`, so they never relax. So the standard approach works: run `n-1` passes, then one more pass to see if any edge relaxes; if yes, return `true`. Edge case: an undirected edge with non-negative weight is converted to two directed edges, but that doesn't change the logic. For negative-weight undirected edges, the snippet only adds the directed edge `u->v` (not reversed), which we must respect in the input representation; the problem statement clarifies this. Time complexity is O(n * E) where E is the number of directed edges (after expansion), and space complexity is O(n) for distances plus storage for edges.
#include <vector>
#include <tuple>
#include <cstring>
#include <climits>

// Returns true if there is a negative cycle reachable from vertex 1.
bool hasNegativeCycle(int n, const std::vector<std::tuple<int,int,int>>& edges) {
    const int INF = 0x3f3f3f3f;
    std::vector<int> dis(n + 1, INF);
    dis[1] = 0;

    // Relax edges n-1 times.
    for (int i = 0; i < n - 1; ++i) {
        bool changed = false;
        for (const auto& [u, v, w] : edges) {
            if (dis[u] != INF && dis[u] + w < dis[v]) {
                dis[v] = dis[u] + w;
                changed = true;
            }
        }
        if (!changed) break;
    }

    // Check for a negative cycle reachable from source.
    for (const auto& [u, v, w] : edges) {
        if (dis[u] != INF && dis[u] + w < dis[v]) {
            return true;
        }
    }
    return false;
}
#include <cassert>
#include <vector>
#include <tuple>

// The solution function is declared above.

int main() {
    // Test 1: Simple negative cycle reachable from 1.
    std::vector<std::tuple<int,int,int>> e1 = {{1,2,-1},{2,1,-1}};
    assert(hasNegativeCycle(2, e1) == true);

    // Test 2: No negative cycle.
    std::vector<std::tuple<int,int,int>> e2 = {{1,2,1},{2,3,2}};
    assert(hasNegativeCycle(3, e2) == false);

    // Test 3: Negative edge but no cycle.
    std::vector<std::tuple<int,int,int>> e3 = {{1,2,-5},{2,3,2}};
    assert(hasNegativeCycle(3, e3) == false);

    // Test 4: Negative cycle reachable from 1 with extra disconnected graph.
    std::vector<std::tuple<int,int,int>> e4 = {{1,2,-2},{2,1,-2},{3,4,-1},{4,3,-1}};
    // The cycle on 3-4 is not reachable from 1, so should be false.
    assert(hasNegativeCycle(4, e4) == false);

    // Test 5: Undirected positive edge (converted to two directed edges) plus negative cycle.
    std::vector<std::tuple<int,int,int>> e5 = {{1,2,1},{2,1,1},{2,3,-1},{3,2,-1}};
    assert(hasNegativeCycle(3, e5) == true);

    // Test 6: Single vertex no edges.
    std::vector<std::tuple<int,int,int>> e6 = {};
    assert(hasNegativeCycle(1, e6) == false);

    // Test 7: Negative cycle only in a disconnected component, but source isolated.
    std::vector<std::tuple<int,int,int>> e7 = {{2,3,-1},{3,2,-1}};
    assert(hasNegativeCycle(3, e7) == false);

    // Test 8: Zero-weight cycle (not negative).
    std::vector<std::tuple<int,int,int>> e8 = {{1,2,0},{2,1,0}};
    assert(hasNegativeCycle(2, e8) == false);

    // Test 9: Multiple negative cycles one reachable.
    std::vector<std::tuple<int,int,int>> e9 = {{1,2,-1},{2,1,-1},{2,3,-2},{3,2,-2}};
    assert(hasNegativeCycle(3, e9) == true);

    // Test 10: Large positive weight does not overflow (sentinel handling).
    std::vector<std::tuple<int,int,int>> e10 = {{1,2,0x3f3f3f3f - 1},{2,3,0x3f3f3f3f - 2}};
    assert(hasNegativeCycle(3, e10) == false);
}
