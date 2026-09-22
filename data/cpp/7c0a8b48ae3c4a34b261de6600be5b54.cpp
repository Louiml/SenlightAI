/*
Write a C++ function `int shortestPathWeight(int n, const std::vector<std::vector<std::pair<int,int>>>& graph)` that takes the number of vertices `n` (vertices numbered 0 to n-1) and an undirected weighted graph represented as an adjacency list, and returns the weight of the shortest path from vertex 0 to vertex `n-1`. The graph has non-negative edge weights. If there is no path, return `-1`. The function must use Dijkstra's algorithm with a `std::set` (or priority queue, but a set is acceptable) and must handle cases where `n == 1` (distance 0) and where the target is unreachable.
*/
#include <vector>
#include <set>
#include <limits>
#include <utility>

// Returns the shortest path weight from vertex 0 to vertex n-1, or -1 if unreachable.
// graph is an adjacency list: graph[u] contains pairs (v, weight) for each undirected edge.
int shortestPathWeight(int n, const std::vector<std::vector<std::pair<int,int>>>& graph) {
    const long long INF = std::numeric_limits<long long>::max();
    if (n == 0) return -1;
    if (n == 1) return 0;

    std::vector<long long> dist(n, INF);
    dist[0] = 0;

    std::set<std::pair<long long, int>> pq; // (distance, vertex)
    pq.emplace(dist[0], 0);

    while (!pq.empty()) {
        auto it = pq.begin();
        long long currentDist = it->first;
        int u = it->second;
        pq.erase(it);

        // If we reach the target, we can return early (optional optimization)
        if (u == n - 1) return static_cast<int>(currentDist);

        // Skip stale entries (if a better distance already exists, this is stale)
        if (currentDist > dist[u]) continue;

        for (const auto& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            long long newDist = currentDist + weight;
            if (newDist < dist[v]) {
                // Erase old entry if exists
                auto old = pq.find({dist[v], v});
                if (old != pq.end()) pq.erase(old);
                dist[v] = newDist;
                pq.emplace(newDist, v);
            }
        }
    }

    return (dist[n - 1] == INF) ? -1 : static_cast<int>(dist[n - 1]);
}
#include <cassert>
#include <vector>
#include <utility>

// Function declaration (provided in solution)
int shortestPathWeight(int n, const std::vector<std::vector<std::pair<int,int>>>& graph);

int main() {
    // Test 1: Simple chain 0-1-2, weights 10 and 20 -> total 30
    std::vector<std::vector<std::pair<int,int>>> g1 = {{{1,10}}, {{0,10},{2,20}}, {{1,20}}};
    assert(shortestPathWeight(3, g1) == 30);

    // Test 2: Direct edge 0-2 weight 5 vs via 1 weight 100 -> direct wins
    std::vector<std::vector<std::pair<int,int>>> g2 = {{{1,100},{2,5}}, {{0,100},{2,1}}, {{0,5},{1,1}}};
    assert(shortestPathWeight(3, g2) == 5);

    // Test 3: Single vertex
    std::vector<std::vector<std::pair<int,int>>> g3 = {};
    assert(shortestPathWeight(1, g3) == 0);

    // Test 4: Unreachable target (0 and 2 not connected)
    std::vector<std::vector<std::pair<int,int>>> g4 = {{{1,3}}, {{0,3}}, {}};
    assert(shortestPathWeight(3, g4) == -1);

    // Test 5: Disconnected but target reachable via no edge? Already covered. 
    // Test: Two vertices disconnected
    std::vector<std::vector<std::pair<int,int>>> g5 = {{{1,2}}, {{0,2}}}; // actually connected
    assert(shortestPathWeight(2, g5) == 2);

    // Test 6: Larger graph with equal paths (0-1-3 weight 10+10=20, 0-2-3 weight 15+5=20)
    std::vector<std::vector<std::pair<int,int>>> g6 = {
        {{1,10},{2,15}}, 
        {{0,10},{3,10}}, 
        {{0,15},{3,5}}, 
        {{1,10},{2,5}}
    };
    assert(shortestPathWeight(4, g6) == 20);

    // Test 7: Zero-weight edges
    std::vector<std::vector<std::pair<int,int>>> g7 = {{{1,0}}, {{0,0},{2,0}}, {{1,0}}};
    assert(shortestPathWeight(3, g7) == 0);

    // Test 8: Negative edge? Not expected, but if given, Dijkstra may fail. Skipping.

    // Test 9: n=0 (invalid, but function should return -1)
    std::vector<std::vector<std::pair<int,int>>> g9 = {};
    assert(shortestPathWeight(0, g9) == -1);

    // Test 10: Large weight (within int range)
    std::vector<std::vector<std::pair<int,int>>> g10 = {{{1, 1000000}}, {{0,1000000},{2, 2000000}}, {{1,2000000}}};
    assert(shortestPathWeight(3, g10) == 3000000);

    return 0;
}
// The solution uses Dijkstra's algorithm since all edge weights are non-negative (implicitly assumed; the snippet uses `LLONG_MAX` and no negative edges). Initialize a distance vector `d` of size `n` with `INF` (use `std::numeric_limits<long long>::max()`), set `d[0] = 0`. Use a `std::set` of pairs `(distance, vertex)` for automatic ordering by distance, which allows extraction of the minimum-distance vertex and lazy deletion when distances update (erase old pair, insert new). When relaxing an edge `(to, weight)` from current `temp`, if `d[temp] + weight < d[to]`, erase the old `(d[to], to)` pair from the set (if it exists), update `d[to]`, and insert the new pair. After the loop, if `d[n-1]` is still `INF`, return `-1`; else return `d[n-1]`. Edge cases: `n == 1` returns 0 immediately; unreachable target returns -1; empty graph (n=0) is not expected since task implies n>=1. Time complexity: `O((n + m) log n)` where `m` is number of edges, due to set operations. Space complexity: `O(n + m)` for graph and distance vector.
