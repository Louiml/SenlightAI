// Write a C++ function that simulates the spread of a computer virus through a network. The input describes a directed graph with `n` nodes (computers), numbered 1 through `n`, and `d` directed edges, where each edge represents a dependency: if computer `b` gets infected, then computer `a` becomes infected after a delay of `s` seconds. Initially, computer `c` is infected at time 0. The function should take the number of computers `n`, the list of edges as a vector of triples `(a, b, s)` meaning "from `b` to `a` with delay `s`", and the starting infected computer `c`. It should return a pair `(total_infected, time_to_last_infection)` where `total_infected` is the total number of computers that eventually become infected (including the starting one) and `time_to_last_infection` is the maximum time at which any computer becomes infected. If some computers never get infected, they are ignored in the count and time. All edge weights are non-negative integers, and delays can be up to 10,000. The function must handle up to `n ≤ 10,000` nodes and `d ≤ 100,000` edges efficiently.

#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Test case 1: Simple chain 1 -> 2 -> 3, start at 1
    // Edges: (2,1,5) means 1->2 with delay 5; (3,2,3) means 2->3 with delay 3
    std::vector<std::tuple<int,int,int>> edges1 = {{2,1,5}, {3,2,3}};
    auto res1 = virusSpread(3, edges1, 1);
    assert(res1 == std::make_pair(3, 8)); // all infected, last at 8

    // Test case 2: Disconnected graph, start at 1, only 1 infected
    std::vector<std::tuple<int,int,int>> edges2 = {{2,3,10}}; // 3->2, but 1 not connected
    auto res2 = virusSpread(3, edges2, 1);
    assert(res2 == std::make_pair(1, 0)); // only source

    // Test case 3: Single node
    std::vector<std::tuple<int,int,int>> edges3 = {};
    auto res3 = virusSpread(1, edges3, 1);
    assert(res3 == std::make_pair(1, 0));

    // Test case 4: Multiple paths, shortest wins
    // Nodes 1,2,3. Edges: (2,1,10) and (2,1,5) both from 1->2 with different delays
    std::vector<std::tuple<int,int,int>> edges4 = {{2,1,10}, {2,1,5}};
    auto res4 = virusSpread(2, edges4, 1);
    assert(res4 == std::make_pair(2, 5)); // both infected, last at 5

    // Test case 5: Cycle with different delays, start at 1
    // 1->2 delay 2, 2->1 delay 1, 2->3 delay 4
    std::vector<std::tuple<int,int,int>> edges5 = {{2,1,2}, {1,2,1}, {3,2,4}};
    auto res5 = virusSpread(3, edges5, 1);
    assert(res5 == std::make_pair(3, 7)); // 1 at 0, 2 at 2, 3 via 2 at 2+4=6? Actually check: 1->2 at 2, 2->3 at 2+4=6, so max 6
    // Corrected: expected maxTime = 6
    assert(res5 == std::make_pair(3, 6));

    // Test case 6: Large delays, start at middle
    std::vector<std::tuple<int,int,int>> edges6 = {{2,1,100}, {3,1,200}, {4,2,50}};
    auto res6 = virusSpread(4, edges6, 1);
    // 1 at 0, 2 at 100, 3 at 200, 4 via 2 at 150
    assert(res6 == std::make_pair(4, 200));

    // Test case 7: Starting node not reachable from itself via back edges? Always infected
    std::vector<std::tuple<int,int,int>> edges7 = {{1,2,3}}; // 2->1 but start at 2
    auto res7 = virusSpread(2, edges7, 2);
    // 2 infected at 0, 1 infected at 3
    assert(res7 == std::make_pair(2, 3));

    // Test case 8: Empty edges with many nodes
    std::vector<std::tuple<int,int,int>> edges8 = {};
    auto res8 = virusSpread(5, edges8, 3);
    assert(res8 == std::make_pair(1, 0));

    return 0;
}

#include <vector>
#include <queue>
#include <utility>
#include <limits>

// Returns {total_infected_count, time_to_last_infection}
std::pair<int, int> virusSpread(int n, const std::vector<std::tuple<int,int,int>>& edges, int c) {
    const int INF = 1e9;
    // Build adjacency: adj[from] = list of (to, weight)
    std::vector<std::vector<std::pair<int,int>>> adj(n + 1);
    for (const auto& e : edges) {
        int a, b, s;
        std::tie(a, b, s) = e;
        adj[b].push_back({a, s}); // infection goes b -> a with delay s
    }

    std::vector<int> dist(n + 1, INF);
    dist[c] = 0;
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<>> pq;
    pq.push({0, c});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue; // stale entry
        for (const auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    int count = 0;
    int maxTime = 0;
    for (int i = 1; i <= n; ++i) {
        if (dist[i] != INF) {
            ++count;
            maxTime = std::max(maxTime, dist[i]);
        }
    }
    return {count, maxTime};
}

// The problem is essentially computing the shortest paths from a single source `c` in a directed graph where edge weights represent infection delays. However, the edges are reversed relative to the natural direction: the input gives `(a, b, s)` meaning that infection spreads from `b` to `a`, so we need to build an adjacency list where for each directed edge `b → a` with weight `s`, we store it in `adj[b]` as `(a, s)`. Then we run Dijkstra's algorithm with a min-heap (or priority queue) starting from node `c` with distance 0. Since all weights are non-negative, Dijkstra is correct. After computing the shortest distances, we count all nodes whose distance is finite (not infinity) and determine the maximum finite distance. The total infected count includes the source itself, so if `c` is reachable only from itself (trivially), we count it. Edge cases: if the graph is disconnected, some nodes remain with infinite distance and are ignored. Also, the source `c` is always counted. Time complexity is `O((n + d) log n)` due to Dijkstra with a binary heap. Space complexity is `O(n + d)` for adjacency list and distance array.
//
// One subtle point: the original code snippet uses `memset` incorrectly on `vector<vector<int>>` and `dis` with a large value, but we will implement a clean solution with `vector` of `vector<pair<int,int>>` and initialize distances to a large constant like `1e9` (or `INT_MAX/2` to avoid overflow). We must ensure that we do not double-count the source: the final count is `1 + number of nodes with distance < INF and not equal to c`. Also, the maximum time is the maximum finite distance, which will be 0 if only the source is infected.
