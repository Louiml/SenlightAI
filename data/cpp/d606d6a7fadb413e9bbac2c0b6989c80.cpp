Write a C++ function `long long shortestPath(int n, const std::vector<std::vector<std::pair<int,int>>>& graph)` that computes the shortest path distance from node 0 to node n-1 in an undirected weighted graph with non‑negative edge weights. The graph is represented as an adjacency list where `graph[i]` contains pairs `(neighbor, weight)`. Nodes are numbered from 0 to n-1. Return -1 if node n-1 is unreachable from node 0 (i.e., the graph is disconnected). Your function must use Dijkstra’s algorithm and must handle large edge weights (up to 10^9) and node counts up to 10^5, so use a priority queue (heap) to achieve `O((n+m) log n)` time, where `m` is the number of edges. The input graph is guaranteed to have no self‑loops or negative weights, but there may be multiple edges between the same pair of nodes and the graph may be disconnected.

// We use Dijkstra’s algorithm because all edge weights are non‑negative. The main idea is to maintain a min‑priority queue of `(distance, node)` pairs. Initialize `distance[0] = 0` and all other distances to infinity. While the queue is not empty, pop the node with the smallest tentative distance; if it has already been finalized (visited), skip it. For each neighbor, if the current tentative distance plus the edge weight is less than the neighbor’s stored distance, update the neighbor’s distance and push the new pair into the queue. This ensures that when a node is popped, its distance is the true shortest distance (due to the greedy property of Dijkstra with non‑negative weights). We use a min‑heap via `std::priority_queue` with `std::greater` to get the smallest element in `O(log n)`. After the algorithm completes, if `distance[n-1]` is still infinity, return -1; otherwise return that value. Edge cases: a graph with only one node (distance 0), no edges (unreachable if n>1), and multiple edges – the algorithm naturally handles them because we only relax when a better path is found. Time complexity is `O((n + m) log n)` and space complexity is `O(n + m)` for the adjacency list and distance array.

#include <vector>
#include <queue>
#include <limits>

using ll = long long;
const ll INF = std::numeric_limits<ll>::max();

// Returns the shortest distance from node 0 to node n-1 in an undirected weighted graph.
// Returns -1 if node n-1 is unreachable.
ll shortestPath(int n, const std::vector<std::vector<std::pair<int,int>>>& graph) {
    std::vector<ll> dist(n, INF);
    dist[0] = 0;
    
    // Min-heap of (distance, node)
    std::priority_queue<std::pair<ll,int>, std::vector<std::pair<ll,int>>, std::greater<std::pair<ll,int>>> pq;
    pq.push({0, 0});
    
    std::vector<bool> visited(n, false);
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        if (u == n - 1) return d; // early exit if target reached
        
        for (const auto& [v, w] : graph[u]) {
            if (d + w < dist[v]) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    
    return (dist[n-1] == INF) ? -1 : dist[n-1];
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Simple chain 0-1-2, weights 5 and 7
    std::vector<std::vector<std::pair<int,int>>> g1 = {{{1,5}}, {{0,5},{2,7}}, {{1,7}}};
    assert(shortestPath(3, g1) == 12);
    
    // Test 2: Direct edge 0->2 weight 10, unreachable node 3
    std::vector<std::vector<std::pair<int,int>>> g2 = {{{1,2},{2,10}}, {{0,2}}, {{0,10}}, {}};
    assert(shortestPath(4, g2) == -1);
    
    // Test 3: Single node
    std::vector<std::vector<std::pair<int,int>>> g3 = {};
    assert(shortestPath(1, g3) == 0);
    
    // Test 4: Two nodes connected by multiple edges, choose the smallest
    std::vector<std::vector<std::pair<int,int>>> g4 = {{{1,3},{1,1}}, {{0,3},{0,1}}};
    assert(shortestPath(2, g4) == 1);
    
    // Test 5: Triangle with longer alternative
    std::vector<std::vector<std::pair<int,int>>> g5 = {{{1,1},{2,10}}, {{0,1},{2,1}}, {{0,10},{1,1}}};
    assert(shortestPath(3, g5) == 2);
    
    // Test 6: Disconnected but start has no edges
    std::vector<std::vector<std::pair<int,int>>> g6 = {{{1,1}}, {{0,1}}, {}};
    assert(shortestPath(3, g6) == -1);
    
    // Test 7: Large weights, check no overflow
    std::vector<std::vector<std::pair<int,int>>> g7 = {{{1,1000000000}}, {{0,1000000000},{2,1000000000}}, {{1,1000000000}}};
    assert(shortestPath(3, g7) == 2000000000LL);
    
    return 0;
}
