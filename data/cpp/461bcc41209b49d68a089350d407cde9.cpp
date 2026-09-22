Write a C++ function `minimumCostPath` that takes two integer vectors `start` and `target` (each of size 2 representing x and y coordinates on a 2D grid), and a vector of special roads `specialRoads`, where each special road is a vector of 5 integers: `{x1, y1, x2, y2, cost}`. The function must return the minimum cost to travel from `start` to `target`. You can move between any two points (including start, target, and the endpoints of any special road) at a cost equal to the Manhattan distance (`abs(x1-x2) + abs(y1-y2)`). Additionally, you may use any special road from its start to its end at the given cost (which may be less than Manhattan distance), but you can only use each road in the forward direction (from `x1,y1` to `x2,y2`). You may also traverse special roads in reverse at Manhattan distance cost if desired (i.e., the reverse direction is not cheaper unless it coincides with regular travel). The special costs can be negative? For simplicity, assume all costs are non-negative integers up to 10^6. The coordinates range from -10^9 to 10^9, and there can be up to 100 special roads. Return the minimum cost as an integer.
#include <cassert>
#include <vector>

int minimumCostPath(const std::vector<int>& start, const std::vector<int>& target, const std::vector<std::vector<int>>& specialRoads);

int main() {
    // Example 1: No special roads, simple Manhattan.
    assert(minimumCostPath({0,0}, {3,4}, {}) == 7);

    // Example 2: One special road cheaper than Manhattan.
    assert(minimumCostPath({0,0}, {10,10}, {{0,0,10,10,5}}) == 5);

    // Example 3: Special road not helpful.
    assert(minimumCostPath({0,0}, {2,2}, {{0,0,2,2,10}}) == 4); // Manhattan = 4

    // Example 4: Multiple roads, combining them.
    // Start (0,0) -> (5,0) cost 5 special, then (5,0) -> (5,5) Manhattan = 5, total 10 vs direct Manhattan = 10.
    assert(minimumCostPath({0,0}, {5,5}, {{0,0,5,0,2}, {5,0,5,5,20}}) == 7); // 2 + 5 = 7

    // Example 5: Roads with endpoints not exactly start/target.
    // Start (0,0), target (100,0). Road from (10,0) to (90,0) cost 1.
    // Path: Manhattan to (10,0) = 10, use road cost 1, then Manhattan to (100,0) = 10, total 21 vs direct 100.
    assert(minimumCostPath({0,0}, {100,0}, {{10,0,90,0,1}}) == 21);

    // Example 6: Same coordinates appearing multiple times.
    assert(minimumCostPath({1,1}, {1,1}, {}) == 0);
    assert(minimumCostPath({1,1}, {1,1}, {{1,1,1,1,0}}) == 0);

    // Example 7: Special road in reverse direction is more expensive than Manhattan but forward is cheap.
    // Road: (0,0)->(10,0) cost 3, but we need to go from (10,0) to (0,0).
    // Direct Manhattan = 10, reverse special not allowed, so still 10.
    assert(minimumCostPath({10,0}, {0,0}, {{0,0,10,0,3}}) == 10);

    // Example 8: More complex chain.
    // Start (0,0), target (10,10). Roads: (0,0)->(5,5) cost 1, (5,5)->(10,10) cost 1.
    assert(minimumCostPath({0,0}, {10,10}, {{0,0,5,5,1}, {5,5,10,10,1}}) == 2);

    // Example 9: Negative coordinates.
    assert(minimumCostPath({-5,-5}, {5,5}, {{-5,-5,5,5,7}}) == 7);
    assert(minimumCostPath({-5,-5}, {5,5}, {}) == 20);
}
#include <vector>
#include <map>
#include <queue>
#include <cmath>
#include <algorithm>
#include <utility>

// Computes minimum travel cost from start to target using Manhattan moves and optional cheaper special roads.
int minimumCostPath(const std::vector<int>& start, const std::vector<int>& target, const std::vector<std::vector<int>>& specialRoads) {
    // Map unique coordinates to node indices.
    std::map<std::pair<int,int>, int> idMap;
    std::vector<std::pair<int,int>> nodes;

    auto getId = [&](int x, int y) {
        auto key = std::make_pair(x, y);
        auto it = idMap.find(key);
        if (it != idMap.end()) return it->second;
        int newId = static_cast<int>(nodes.size());
        idMap[key] = newId;
        nodes.push_back(key);
        return newId;
    };

    int startId = getId(start[0], start[1]);
    int targetId = getId(target[0], target[1]);
    for (const auto& road : specialRoads) {
        getId(road[0], road[1]);
        getId(road[2], road[3]);
    }

    int n = static_cast<int>(nodes.size());
    // Complete graph with Manhattan distances.
    std::vector<std::vector<int>> graph(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            int dist = std::abs(nodes[i].first - nodes[j].first) + std::abs(nodes[i].second - nodes[j].second);
            graph[i][j] = dist;
            graph[j][i] = dist;
        }
    }

    // Apply special road costs (only forward direction).
    for (const auto& road : specialRoads) {
        int a = getId(road[0], road[1]);
        int b = getId(road[2], road[3]);
        graph[a][b] = std::min(graph[a][b], road[4]);
    }

    // Dijkstra from startId.
    const int INF = 1e9;
    std::vector<int> dist(n, INF);
    dist[startId] = 0;
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<>> pq;
    pq.push({0, startId});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;
        for (int v = 0; v < n; ++v) {
            int nd = d + graph[u][v];
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }
    return dist[targetId];
}
// The problem reduces to a shortest path on a graph where nodes are all unique coordinate points: the start, the target, and the two endpoints of each special road. There are at most `2 + 2*specialRoads.size()` nodes. Build a complete undirected graph between all nodes with edge weight equal to the Manhattan distance between the coordinates. Then, for each special road, add a directed edge from its start node to its end node with weight equal to the given cost (and take the minimum with the already existing Manhattan weight). Then run Dijkstra's algorithm from the start node to all nodes, because all edge weights are non-negative. The answer is the shortest distance to the target node. Key edge cases: if no special roads, just Manhattan distance; if a special road's cost is higher than Manhattan, it's effectively ignored because we take the minimum; duplicate coordinates in special roads or with start/target need to be merged into a single node to avoid self-loops, which is handled by the id mapping. Time complexity: building the complete graph takes O(n^2) where n ≤ 202, and Dijkstra runs in O(n^2) using a simple binary heap, so overall O(n^2 log n) or O(n^2) with a dense matrix. Space is O(n^2) for adjacency matrix.
