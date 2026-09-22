// You are given a directed graph with `n` vertices (numbered 1 to `n`) and `m` edges, where each edge has an integer weight. Additionally, you are given a positive integer parameter `p`. Your task is to implement a C++ function `long long maxScorePath(int n, const std::vector<std::tuple<int,int,long long>>& edges, long long p)` that computes the maximum possible "score" of a path from vertex 1 to vertex `n`, where the score of a path is the sum of `(edge_weight - p)` for all edges on the path. If the maximum score is negative, the function should return 0. However, if there exists a positive-weight cycle (after subtracting `p` from each edge) that is reachable from vertex 1 and from which vertex `n` is reachable, then no finite upper bound exists; in that case, the function should return a special sentinel value `LLONG_MIN` to indicate "infinite" positive score. You may assume `1 ≤ n ≤ 3000`, `0 ≤ m ≤ 100000`, edge weights are between `-10^9` and `10^9`, and `p` is positive. The graph may have self-loops and multiple edges, but no parallel edges between the same ordered pair in opposite directions are guaranteed. Your function must be efficient enough for the given constraints.
This problem is a variant of the "longest path in a graph with possible positive cycles" problem, transformable into a "shortest path with negative cycles" problem by negating all effective edge weights (`p - edge_weight`). The intended algorithm is Bellman-Ford to detect positive cycles (which become negative cycles after negation). Key steps:

1. Build the adjacency list for the original graph (using effective weight `edge_weight - p`), and also build the reverse graph (only needed for reachability to `n`).
2. Perform a BFS from vertex `n` on the reverse graph to mark all vertices that can reach `n`. This is necessary because a positive cycle only causes infinite score if it can reach the destination.
3. Run Bellman-Ford relaxation exactly `n` times (one more than `n-1`) starting from vertex 1 with initial distance 0 and all others set to negative infinity (a very small sentinel). In each relaxation, for every edge `(u, v, w_eff)`, if `dist[u]` is not sentinel and `dist[v] < dist[u] + w_eff`, update `dist[v]`.
4. After the `n` passes, do one final pass. If any edge `(u, v, w_eff)` can still be relaxed (i.e., `dist[u] + w_eff > dist[v]`) and `v` is reachable from `n` (using the BFS result), then a positive cycle that affects the destination exists → return `LLONG_MIN` (infinite positive score). Note: We check the relaxation on `v`, not necessarily `u`, because the cycle may be upstream but its effect can propagate.
5. If no infinite cycle, the answer is `max(0, dist[n-1])` (since vertex indices are 0-based). Be careful with negative infinity: use a very small sentinel like `-1e18` (or `LLONG_MIN/4` for safety). Ensure we only relax from vertices with finite distance.
6. Time complexity: BFS is O(n + m), Bellman-Ford is O(n * m). With n=3000, m=100000, worst-case 300 million operations, which is borderline but acceptable in C++ with optimization. Space complexity is O(n + m) for adjacency lists and reverse graph.

Edge cases: No path from 1 to n → dist[n-1] remains sentinel → return max(0, sentinel) which should be 0 (handle by checking sentinel explicitly). Self-loops: if a self-loop has positive effective weight and the vertex is reachable from 1 and can reach n, infinite. Multiple edges: handled naturally. Negative effective weights are fine; they only reduce the score.
#include <bits/stdc++.h>
using namespace std;

const long long NEG_INF = -1000000000000000000LL; // -1e18

/**
 * Computes the maximum score from vertex 0 (1) to vertex n-1 (n)
 * where each edge's contribution is (original_weight - p).
 * Returns LLONG_MIN if a positive cycle reachable from source and able to reach sink exists.
 * Otherwise returns max(0, maximum_finite_score).
 */
long long maxScorePath(int n, const vector<tuple<int, int, long long>>& edges, long long p) {
    // Build adjacency list for original graph (effective weight = w - p)
    vector<vector<pair<int, long long>>> g(n);
    vector<vector<int>> rev(n); // reverse graph for reachability to sink
    for (const auto& [u, v, w] : edges) {
        long long eff = w - p;
        g[u].push_back({v, eff});
        rev[v].push_back(u);
    }

    // BFS from sink (n-1) on reverse graph to find vertices that can reach sink
    vector<bool> canReachSink(n, false);
    queue<int> q;
    q.push(n - 1);
    canReachSink[n - 1] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int prev : rev[u]) {
            if (!canReachSink[prev]) {
                canReachSink[prev] = true;
                q.push(prev);
            }
        }
    }

    // Bellman-Ford for longest path: initialize distances to -inf, source = 0
    vector<long long> dist(n, NEG_INF);
    dist[0] = 0;

    for (int iter = 0; iter < n; ++iter) { // exactly n passes (V iterations)
        bool changed = false;
        for (int u = 0; u < n; ++u) {
            if (dist[u] == NEG_INF) continue;
            for (const auto& [v, w] : g[u]) {
                if (dist[v] < dist[u] + w) {
                    dist[v] = dist[u] + w;
                    changed = true;
                }
            }
        }
        if (!changed) break; // early exit optimization
    }

    // Check for positive cycle that can affect the sink
    for (int u = 0; u < n; ++u) {
        if (dist[u] == NEG_INF) continue;
        for (const auto& [v, w] : g[u]) {
            if (dist[v] == NEG_INF) continue; // not reachable from source
            if (dist[v] < dist[u] + w && canReachSink[v]) {
                return LLONG_MIN; // infinite positive score
            }
        }
    }

    // If sink is unreachable, dist[n-1] is NEG_INF; return 0 as per spec
    if (dist[n-1] == NEG_INF) return 0LL;
    return max(0LL, dist[n-1]);
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Include the solution function here (or link it)
// For brevity, assume the function above is available.

int main() {
    // Test 1: Simple path with positive score
    {
        int n = 3;
        vector<tuple<int,int,long long>> edges = {{0,1,10},{1,2,10}};
        long long p = 5;
        // effective weights: 5,5 => total 10
        assert(maxScorePath(n, edges, p) == 10);
    }

    // Test 2: Path with negative total score; must return 0
    {
        int n = 2;
        vector<tuple<int,int,long long>> edges = {{0,1,3}};
        long long p = 5;
        // effective -2 => negative, return 0
        assert(maxScorePath(n, edges, p) == 0);
    }

    // Test 3: Positive cycle reachable and can reach sink -> LLONG_MIN
    {
        int n = 3;
        vector<tuple<int,int,long long>> edges = {
            {0,1,10}, // 0->1 eff=5
            {1,0,10}, // 1->0 eff=5 (cycle total +10)
            {1,2,10}  // 1->2 eff=5
        };
        long long p = 5;
        assert(maxScorePath(n, edges, p) == LLONG_MIN);
    }

    // Test 4: Positive cycle but cannot reach sink -> finite
    {
        int n = 4;
        vector<tuple<int,int,long long>> edges = {
            {0,1,10}, // 0->1 eff=5
            {1,0,10}, // cycle in {0,1} cannot reach 3
            {2,3,10}  // 2->3 eff=5, but 2 not reachable from 0
        };
        long long p = 5;
        // No path 0->3, so return 0
        assert(maxScorePath(n, edges, p) == 0);
    }

    // Test 5: Cycle with zero effective weight (not positive) -> finite
    {
        int n = 3;
        vector<tuple<int,int,long long>> edges = {
            {0,1,5}, // eff=0
            {1,0,5}, // cycle sum 0
            {1,2,5}  // eff=0
        };
        long long p = 5;
        // path 0->1->2 total 0 -> max(0,0)=0
        assert(maxScorePath(n, edges, p) == 0);
    }

    // Test 6: Negative effective weights combined to positive total
    {
        int n = 3;
        vector<tuple<int,int,long long>> edges = {
            {0,1,100}, // eff=90
            {1,2,-80}  // eff=-85
        };
        long long p = 10;
        // total = 90 + (-85) = 5
        assert(maxScorePath(n, edges, p) == 5);
    }

    // Test 7: Self-loop with positive effective weight that can reach sink
    {
        int n = 2;
        vector<tuple<int,int,long long>> edges = {
            {0,0,10}, // self-loop eff=5
            {0,1,0}   // eff=-5 (p=10) still reachable
        };
        long long p = 5;
        // Positive self-loop at 0 that can reach 1 -> infinite
        assert(maxScorePath(n, edges, p) == LLONG_MIN);
    }

    // Test 8: Unreachable sink
    {
        int n = 3;
        vector<tuple<int,int,long long>> edges = {{0,1,100}};
        long long p = 10;
        // No edgeto vertex 2 -> return 0
        assert(maxScorePath(n, edges, p) == 0);
    }

    // Test 9: Large n simple chain
    {
        int n = 100;
        vector<tuple<int,int,long long>> edges;
        for (int i = 0; i < n-1; ++i) {
            edges.emplace_back(i, i+1, 1);
        }
        long long p = 0;
        // Total score = n-1 = 99
        assert(maxScorePath(n, edges, p) == 99);
    }

    // Test 10: Multiple paths, choose maximum
    {
        int n = 3;
        vector<tuple<int,int,long long>> edges = {
            {0,1,10}, // eff=5
            {0,2,20}, // direct eff=15
            {1,2,1}   // eff=-4 (p=5)
        };
        long long p = 5;
        // Direct 0->2 gives 15; via 1 gives 5-4=1 -> max 15
        assert(maxScorePath(n, edges, p) == 15);
    }

    cout << "All tests passed!" << endl;
    return 0;
}
