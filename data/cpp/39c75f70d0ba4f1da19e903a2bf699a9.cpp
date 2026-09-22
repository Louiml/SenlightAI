Write a C++ function `pair<int,int> shortestPathWithMinEdges(const vector<vector<pair<int,int>>>& graph, int source, int target)` that, given an undirected weighted graph represented as an adjacency list (where each entry `graph[u]` contains pairs `{v, weight}` with positive integer weights), returns the total weight of the shortest path from `source` to `target`, and the number of edges on that path that has the minimum total weight. If there are multiple shortest paths with the same total weight, choose the one with the fewest edges. If the target is unreachable, return `{numeric_limits<int>::max(), 0}`. Assume the graph has at least one vertex and vertex indices are 0-based. The function must not modify the input graph and must be const-correct where applicable.

The solution applies Dijkstra's algorithm, but instead of tracking only the shortest distance, each vertex stores a pair `(distance, edgeCount)`. The priority queue (or set) orders vertices first by total distance, then by number of edges, so that when we pop a vertex we have the currently best (shortest, then fewest edges) path to it. When relaxing an edge from `u` to `v`, we compute the new candidate distance `newDist = dist[u] + w` and new edge count `newEdges = edges[u] + 1`. We update `v` only if `newDist` is strictly less than `v`'s current distance, or if equal but `newEdges` is strictly less. This ensures we eventually store the path with minimum distance and, among ties, minimum edges. We terminate early when the target is popped from the priority queue because Dijkstra's property ensures its final value is optimal. Time complexity is O((V+E) log V) and space is O(V+E). Edge cases include unreachable target, self-loops (which are ignored because they do not improve distance), and multiple edges between the same pair (treated normally). The graph is undirected, so each edge appears twice in the adjacency list.

#include <vector>
#include <queue>
#include <limits>
#include <tuple>
#include <utility>

// Returns {totalWeight, numberOfEdges} for the shortest path from source to target
// with minimum total weight, and among ties, minimum number of edges.
// Returns {numeric_limits<int>::max(), 0} if target is unreachable.
std::pair<int, int> shortestPathWithMinEdges(
    const std::vector<std::vector<std::pair<int, int>>>& graph,
    int source, int target) {
    const int n = static_cast<int>(graph.size());
    const int INF = std::numeric_limits<int>::max();

    // distance.first = total weight, distance.second = number of edges
    std::vector<std::pair<int, int>> dist(n, {INF, 0});
    dist[source] = {0, 0};

    // Priority queue stores (distance, edgeCount, vertex)
    // Smaller (distance, edgeCount) has higher priority
    using State = std::tuple<int, int, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    pq.emplace(0, 0, source);

    while (!pq.empty()) {
        auto [d, e, u] = pq.top();
        pq.pop();

        if (u == target) {
            return {d, e};
        }

        // Skip outdated entries
        if (d != dist[u].first || e != dist[u].second) {
            continue;
        }

        for (const auto& [v, weight] : graph[u]) {
            int newDist = d + weight;
            int newEdges = e + 1;

            if (newDist < dist[v].first ||
                (newDist == dist[v].first && newEdges < dist[v].second)) {
                dist[v] = {newDist, newEdges};
                pq.emplace(newDist, newEdges, v);
            }
        }
    }

    return {INF, 0};
}

#include <cassert>
#include <vector>
#include <utility>
#include <limits>

// Assume the solution function is declared above (or included here)

int main() {
    using std::vector;
    using std::pair;

    // Test 1: Simple graph
    vector<vector<pair<int,int>>> g1 = {
        {{1, 10}, {2, 20}},
        {{0, 10}, {2, 5}},
        {{0, 20}, {1, 5}}
    };
    auto r1 = shortestPathWithMinEdges(g1, 0, 2);
    assert(r1 == std::make_pair(15, 2)); // 0->1->2 (10+5)

    // Test 2: Tie in distance, different edges
    vector<vector<pair<int,int>>> g2 = {
        {{1, 5}, {2, 5}},
        {{0, 5}, {2, 10}},
        {{0, 5}, {1, 10}}
    };
    auto r2 = shortestPathWithMinEdges(g2, 0, 2);
    assert(r2 == std::make_pair(5, 1)); // direct edge 0->2

    // Test 3: Unreachable
    vector<vector<pair<int,int>>> g3 = {
        {{1, 1}},
        {{0, 1}},
        {{3, 1}},
        {{2, 1}}
    };
    auto r3 = shortestPathWithMinEdges(g3, 0, 3);
    assert(r3.first == std::numeric_limits<int>::max());
    assert(r3.second == 0);

    // Test 4: Source equals target
    vector<vector<pair<int,int>>> g4 = {{{1, 3}}, {{0, 3}}};
    auto r4 = shortestPathWithMinEdges(g4, 1, 1);
    assert(r4 == std::make_pair(0, 0));

    // Test 5: Multiple edges with same weight, choose fewer edges
    vector<vector<pair<int,int>>> g5 = {
        {{1, 2}, {2, 2}},
        {{0, 2}, {2, 2}},
        {{0, 2}, {1, 2}}
    };
    auto r5 = shortestPathWithMinEdges(g5, 0, 2);
    assert(r5 == std::make_pair(2, 1)); // direct edge

    // Test 6: Graph with cycle where shortest path uses more edges but lower weight
    vector<vector<pair<int,int>>> g6 = {
        {{1, 1}, {2, 10}},
        {{0, 1}, {2, 1}},
        {{0, 10}, {1, 1}}
    };
    auto r6 = shortestPathWithMinEdges(g6, 0, 2);
    assert(r6 == std::make_pair(2, 2)); // 0->1->2 (1+1)

    return 0;
}
