// Write a C++ function `shortestDistanceAfterQueries` that takes an integer `n` representing the number of cities (labeled from 0 to n-1) and a vector of queries, where each query is a pair `[a, b]` representing the addition of a new directed road from city `a` to city `b`. Initially, there are directed roads from each city `i` to `i+1` for all `0 <= i < n-1` (so the initial shortest path from 0 to n-1 is exactly n-1). After each query is applied (i.e., after adding the new road), the function must return an integer vector containing the current shortest path distance from city 0 to city n-1, **after each query**, in the same order as the queries. The graph is directed, and all edges have unit weight. The function should handle up to `n = 1000` and up to `10^5` queries efficiently, and must return a vector of the same length as the number of queries. If a query adds a redundant edge that does not change the shortest distance, the corresponding result should reflect the unchanged distance.
// The core idea is to maintain a dynamic shortest-path from source 0 to target n-1 as edges are added incrementally. Since all edges have unit weight and the graph is a DAG (because edges always go from lower index to higher index: the initial forward edges and the added edges also satisfy `a < b` since they represent shortcuts), we can use a specialized relaxation that only updates distances that could be affected by the newly added edge. The initial distance from 0 to any city `i` is simply `i` (via the forward chain). We store a distance array `dist[0..n-1]` and an adjacency list `G`. After adding a new edge `(a, b)`, the only distances that can decrease are those of nodes `u` such that `u >= a` (since the new edge starts at `a`; nodes before `a` cannot use it to reach anywhere better). We relax from `a` upward: for each node `u` from `a` to `n-1`, consider all outgoing edges and update `dist[v] = min(dist[v], dist[u] + 1)`. To optimize, we can stop early: if after processing node `a`, the distance to `b` has not changed, then the new edge provides no improvement and the rest of the relaxation is unnecessary — however, note that the new edge may still improve distances beyond `b` even if `dist[b]` doesn't change? Actually, if `dist[b]` doesn't change, then no path using the new edge can improve anything beyond `b` because any improvement would require reaching `b` earlier; since `dist[b]` is unchanged, no new path is provided. So we can break. For correctness, we must process all nodes from `a` to `n-1` in increasing order (which is a valid topological order for this DAG). The time complexity is `O(n + E)` per query in the worst case if we do full relaxation, but with the early break it is often much faster; worst-case overall is `O(Q * n + total_edges)` but with `n` up to 1000 and Q up to 1e5, that could be 1e8, which is acceptable in C++ with optimization. Space is `O(n + total_edges)` for adjacency and distances. Edge cases: duplicate edges, queries that don't reduce distance, and queries where `a` and `b` are adjacent (no effect) or `b < a` (should not happen per problem constraints, but we can assume valid input). Also note that the initial distance for node `i` is `i` (since 0 to i is i steps). The final answer after the last query is the last element.
#include <vector>
#include <algorithm>

// Given n cities and a list of queries adding directed edges (a -> b),
// return the shortest distance from city 0 to city n-1 after each query.
// The graph starts with edges i -> i+1 for all i in [0, n-2].
// All edges have unit weight.
std::vector<int> shortestDistanceAfterQueries(int n, const std::vector<std::vector<int>>& queries) {
    std::vector<std::vector<int>> graph(n);          // adjacency list
    std::vector<int> dist(n);                        // current shortest distances from 0
    for (int i = 1; i < n; ++i) {
        graph[i - 1].push_back(i);                   // initial forward edges
        dist[i] = i;                                 // initial distance via forward chain
    }
    dist[0] = 0;

    std::vector<int> result;
    result.reserve(queries.size());

    for (const auto& q : queries) {
        int a = q[0];
        int b = q[1];
        graph[a].push_back(b);                       // add the new directed edge

        int oldDistB = dist[b];                      // distance to b before relaxation
        // Relax from a upward. The graph is a DAG with topological order 0..n-1.
        // Only nodes >= a can benefit from the new edge.
        for (int u = a; u < n; ++u) {
            for (int v : graph[u]) {
                if (dist[u] + 1 < dist[v]) {
                    dist[v] = dist[u] + 1;
                }
            }
            // Early exit: if after processing 'a' the distance to b didn't change,
            // the new edge offers no improvement, so further relaxation is unnecessary.
            if (u == a && dist[b] == oldDistB) {
                break;
            }
        }
        result.push_back(dist[n - 1]);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// (Include the solution function here or above.)

int main() {
    // Example 1: Basic case
    {
        int n = 5;
        std::vector<std::vector<int>> queries = {{1, 3}, {0, 4}};
        std::vector<int> result = shortestDistanceAfterQueries(n, queries);
        // Initial distance 0->4 is 4.
        // After adding 1->3: path 0->1->3->4 is 0-1-3-4 = 3? Actually 0->1 (1), 1->3 (1), 3->4 (1) = 3. So min(4,3)=3.
        // After adding 0->4: direct edge gives 1.
        assert(result == std::vector<int>({3, 1}));
    }

    // Example 2: No improvement
    {
        int n = 3;
        std::vector<std::vector<int>> queries = {{0, 2}, {1, 2}, {0, 2}};
        std::vector<int> result = shortestDistanceAfterQueries(n, queries);
        // Initial 0->2 is 2.
        // After 0->2: dist becomes 1.
        // After 1->2: no improvement (still 1).
        // After duplicate 0->2: still 1.
        assert(result == std::vector<int>({1, 1, 1}));
    }

    // Example 3: Chain of improvements
    {
        int n = 6;
        std::vector<std::vector<int>> queries = {{1, 4}, {0, 3}, {2, 5}};
        std::vector<int> result = shortestDistanceAfterQueries(n, queries);
        // Initial 0->5 = 5.
        // After 1->4: 0->1->4->5 = 3? 0-1 (1), 1-4 (1), 4-5 (1) = 3. So min(5,3)=3.
        // After 0->3: path 0-3-4-5 = 3? 0-3 (1), 3-4 (1), 4-5 (1) = 3. But also 0-1-4-5 = 3, so still 3.
        // After 2->5: path 0-1-2-5? 0-1 (1), 1-2 (1), 2-5 (1) = 3. Or 0-3-4-5 = 3. Still 3.
        assert(result == std::vector<int>({3, 3, 3}));
    }

    // Example 4: Single node
    {
        int n = 1;
        std::vector<std::vector<int>> queries = {}; // no queries
        std::vector<int> result = shortestDistanceAfterQueries(n, queries);
        assert(result.empty());
    }

    // Example 5: Query with a and b equal? assume valid input, but test a=0,b=1
    {
        int n = 4;
        std::vector<std::vector<int>> queries = {{0, 1}}; // duplicate of existing edge
        std::vector<int> result = shortestDistanceAfterQueries(n, queries);
        // Initial 0->3 = 3. Adding duplicate edge does nothing.
        assert(result == std::vector<int>({3}));
    }

    std::cout << "All tests passed.\n";
    return 0;
}
