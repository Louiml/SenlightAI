// Write a C++ function `vector<long long> findAllDistances(int n, const vector<array<int,3>>& positiveEdges, const vector<array<int,3>>& negativeEdges, const vector<int>& querySources)` that, for each source vertex `s` given in `querySources`, computes the shortest distance from `s` to every other vertex in a directed graph. The graph has `n` vertices (numbered 1 to n) and two types of directed edges: `positiveEdges` (all weights are non-negative) and `negativeEdges` (each edge is given as `(u, v, w)` where the actual edge weight is `-w`, i.e., always negative or zero). For each query, return a vector of size `n` (index 0 corresponds to vertex 1, index `n-1` to vertex n) where the value is the shortest path distance if it exists and is finite, otherwise `-1`. If a negative cycle is reachable from the source (detected via Bellman-Ford relaxation after `n-1` passes followed by one more pass), then for vertices affected by the cycle and all vertices reachable from them, the shortest distance is undefined and must be reported as `-1`; all other reachable vertices keep their finite distances. Vertices not reachable at all also get `-1`. The graph may have multiple edges, self-loops, and no guarantee of no parallel edges. The function should be efficient for multiple queries (up to 100 queries) and moderate graph sizes (n up to 1000, total edges up to 2000).

#include <cassert>
#include <vector>
#include <array>
#include <iostream>

// Include the solution function here (or link it)
// For completeness, we copy the function definition above.

int main() {
    // Test 1: Simple graph with positive edges only
    {
        int n = 3;
        std::vector<std::array<int,3>> pos = {{1,2,5}, {2,3,2}};
        std::vector<std::array<int,3>> neg;
        std::vector<int> queries = {1, 2, 3};
        auto res = findAllDistances(n, pos, neg, queries);
        assert(res[0] == std::vector<long long>({0,5,7})); // from 1
        assert(res[1] == std::vector<long long>({-1,0,2})); // from 2
        assert(res[2] == std::vector<long long>({-1,-1,0})); // from 3
    }

    // Test 2: Negative edges but no negative cycle
    {
        int n = 3;
        std::vector<std::array<int,3>> pos = {{1,2,1}};
        std::vector<std::array<int,3>> neg = {{3,1,4}}; // edge 3->1 weight -4
        std::vector<int> queries = {1, 3};
        auto res = findAllDistances(n, pos, neg, queries);
        // From 1: 1->2=1, 1->? cannot reach 3 without positive? Actually 1->3 none, so -1
        assert(res[0] == std::vector<long long>({0,1,-1}));
        // From 3: 3->1=-4, 3->2=-3, 3->? 
        assert(res[1] == std::vector<long long>({-4,-3,0}));
    }

    // Test 3: Negative cycle reachable from source
    {
        int n = 2;
        std::vector<std::array<int,3>> pos;
        std::vector<std::array<int,3>> neg = {{1,2,1}, {2,1,1}}; // cycle 1->-1, 2->-1 total -2
        std::vector<int> queries = {1, 2};
        auto res = findAllDistances(n, pos, neg, queries);
        // Both vertices affected by cycle
        assert(res[0] == std::vector<long long>({-1,-1}));
        assert(res[1] == std::vector<long long>({-1,-1}));
    }

    // Test 4: Negative self-loop
    {
        int n = 1;
        std::vector<std::array<int,3>> pos;
        std::vector<std::array<int,3>> neg = {{1,1,1}}; // self-loop weight -1
        std::vector<int> queries = {1};
        auto res = findAllDistances(n, pos, neg, queries);
        assert(res[0] == std::vector<long long>({-1}));
    }

    // Test 5: Disconnected component
    {
        int n = 4;
        std::vector<std::array<int,3>> pos = {{1,2,3}};
        std::vector<std::array<int,3>> neg;
        std::vector<int> queries = {3};
        auto res = findAllDistances(n, pos, neg, queries);
        assert(res[0] == std::vector<long long>({-1,-1,0,-1}));
    }

    // Test 6: Negative cycle not reachable from source
    {
        int n = 4;
        std::vector<std::array<int,3>> pos = {{1,2,1}};
        std::vector<std::array<int,3>> neg = {{3,4,1}, {4,3,1}}; // cycle on 3-4
        std::vector<int> queries = {1};
        auto res = findAllDistances(n, pos, neg, queries);
        // From 1: reach 2 with 1, others unreachable
        assert(res[0] == std::vector<long long>({0,1,-1,-1}));
    }

    // Test 7: Multiple edges and zero-weight edges
    {
        int n = 2;
        std::vector<std::array<int,3>> pos = {{1,2,0}, {1,2,5}, {2,1,2}};
        std::vector<std::array<int,3>> neg;
        std::vector<int> queries = {1};
        auto res = findAllDistances(n, pos, neg, queries);
        assert(res[0] == std::vector<long long>({0,0}));
    }

    // Test 8: Large weights but no overflow (safe with long long)
    {
        int n = 2;
        std::vector<std::array<int,3>> pos = {{1,2,1000000000}};
        std::vector<std::array<int,3>> neg;
        std::vector<int> queries = {1};
        auto res = findAllDistances(n, pos, neg, queries);
        assert(res[0] == std::vector<long long>({0,1000000000LL}));
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <vector>
#include <array>
#include <queue>
#include <limits>
#include <algorithm>

// Computes shortest distances from each query source to all vertices.
// positiveEdges: directed edges (u, v, w) with w >= 0.
// negativeEdges: directed edges (u, v, w) where actual weight is -w (<= 0).
// Returns a vector of vectors: for each query, a vector of size n (index 0 = vertex 1).
std::vector<std::vector<long long>> findAllDistances(
    int n,
    const std::vector<std::array<int,3>>& positiveEdges,
    const std::vector<std::array<int,3>>& negativeEdges,
    const std::vector<int>& querySources) {

    const long long INF = std::numeric_limits<long long>::max() / 4;
    std::vector<std::array<int,3>> allEdges = positiveEdges;
    for (const auto& e : negativeEdges) {
        allEdges.push_back({e[0], e[1], -e[2]}); // actual negative weight
    }
    int m = static_cast<int>(allEdges.size());

    std::vector<std::vector<long long>> results;
    results.reserve(querySources.size());

    for (int s : querySources) {
        std::vector<long long> dist(n + 1, INF);
        dist[s] = 0;

        // Bellman-Ford relaxation n-1 times
        for (int iter = 0; iter < n - 1; ++iter) {
            bool updated = false;
            for (const auto& e : allEdges) {
                int u = e[0], v = e[1], w = e[2];
                if (dist[u] != INF && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    updated = true;
                }
            }
            if (!updated) break;
        }

        // Copy distances before checking for negative cycles
        std::vector<long long> distBefore = dist;

        // One extra relaxation to detect negative cycle reachable vertices
        std::vector<bool> affected(n + 1, false);
        for (const auto& e : allEdges) {
            int u = e[0], v = e[1], w = e[2];
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                affected[v] = true; // v is on or reachable from a negative cycle
                dist[v] = dist[u] + w; // relax (optional, we don't need exact values)
            }
        }

        // Propagate affected status: if u is affected, then v is also affected
        // Repeat until stable or n iterations (since paths length <= n-1)
        for (int iter = 0; iter < n - 1; ++iter) {
            bool changed = false;
            for (const auto& e : allEdges) {
                int u = e[0], v = e[1];
                if (affected[u] && !affected[v]) {
                    affected[v] = true;
                    changed = true;
                }
            }
            if (!changed) break;
        }

        // Build result for this source
        std::vector<long long> ans(n);
        for (int i = 1; i <= n; ++i) {
            if (dist[i] == INF || affected[i]) {
                ans[i - 1] = -1;
            } else {
                ans[i - 1] = dist[i];
            }
        }
        results.push_back(ans);
    }
    return results;
}

// The core algorithm is Bellman-Ford for each query. For each source `s`, initialize a distance array `dist` with `INF` (e.g., 1e18) and set `dist[s]=0`. Perform exactly `n-1` relaxation passes over all edges (both positive and negative edges combined). During each pass, for every edge `(u,v,w)`, if `dist[u]` is not `INF` and `dist[u]+w < dist[v]`, update `dist[v]`. After these `n-1` passes, make a copy `dist2` of `dist`. Then run one extra relaxation pass; during this pass, any edge that can be relaxed indicates that the destination vertex `v` is part of or reachable from a negative cycle. Mark such vertices as "affected". Then, to propagate the effect of negative cycles to all vertices reachable from affected vertices, we need to run a final pass (or a simple DFS/BFS) that marks all vertices reachable from the affected set as also affected. However, a simpler standard approach is: after the extra pass, run additional `n-1` passes (or equivalently, one more Bellman-Ford iteration) that propagates the "affected" status. In practice, since the graph is directed, we can iterate again: for any edge `(u,v,w)`, if `u` is affected, then `v` becomes affected. Repeat this up to `n-1` times or until no new affected vertices appear (to avoid infinite loops). Finally, for each vertex `i`, if `dist[i] == INF` or `i` is affected, output `-1`, else output `dist[i]`. Complexity: For each query, Bellman-Ford takes `O((n-1)*(m_total))` where `m_total = positive + negative edges` and the affected propagation takes at most `O(n * m_total)` in the worst case, but typically O(m_total * n) which is fine. Overall for `q` queries: `O(q * n * m_total)`. With n=1000 and m=2000, that's about 2e6 per query, and 100 queries gives 2e8, acceptable in C++ with optimizations. Space: O(n) per query.
//
// Important edge cases:  
// - Self-loops with negative weight can cause immediate negative cycles.  
// - If no negative edges exist, the extra pass will not relax anything, so all reachable vertices get finite distances.  
// - If a source is disconnected, all distances remain `INF` and output `-1`.  
// - If the graph has a negative cycle but not reachable from source, it doesn't affect the result.  
// - The problem statement's snippet uses `1e9` as INF and prints `-1` for both unreachable and negative-cycle affected. We use `long long` to avoid overflow with large weights.
