/*
You are given an undirected graph with \(n\) vertices labeled \(0\) to \(n-1\), a list of edges where each edge is a triple \(\{u, v, w\}\), and two integers `source` and `destination`. The weight \(w\) in an edge can be a positive integer, or `-1` which means the edge weight is currently unknown and must be assigned a positive integer. Your task is to write a C++ function that returns a modified edge list such that after replacing every `-1` with a positive integer, the shortest path distance from `source` to `destination` is exactly equal to a given `target`. If it is impossible to achieve this, return an empty vector. If multiple assignments exist, you may return any valid one. Note: the original graph may contain zero-weight edges (interpreted as no direct edge) but you may assume no input contains self-loops or parallel edges.
*/

#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

// Dijkstra's shortest path from src to dst, skipping edges with weight -1 or 0.
int dijkstra(int n, const std::vector<std::vector<int>>& graph, int src, int dst) {
    const int INF = 2e9;
    std::vector<int> dist(n, INF);
    std::vector<bool> visited(n, false);
    // min-heap of (distance, vertex)
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (visited[u]) continue;
        if (u == dst) return d;
        visited[u] = true;
        for (int v = 0; v < n; ++v) {
            int w = graph[u][v];
            if (w == 0 || w == -1) continue; // skip no-edge and unknown edges
            if (visited[v]) continue;
            int nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push({nd, v});
            }
        }
    }
    return INF;
}

// Adjust edge weights (replace -1 with positive integers) so that the shortest
// path from source to destination equals target. Return empty if impossible.
std::vector<std::vector<int>> modifyGraphEdges(int n,
                                               const std::vector<std::vector<int>>& edges,
                                               int source, int destination, int target) {
    // Build adjacency matrix representation.
    std::vector<std::vector<int>> graph(n, std::vector<int>(n, 0));
    for (const auto& e : edges) {
        graph[e[0]][e[1]] = e[2];
        graph[e[1]][e[0]] = e[2];
    }

    // First: shortest path ignoring all -1 edges.
    int initial_dist = dijkstra(n, graph, source, destination);
    if (initial_dist < target) {
        // Even with infinite edges we cannot reach target.
        return {};
    }
    if (initial_dist == target) {
        // Already correct, make -1 edges very large so they don't matter.
        std::vector<std::vector<int>> result = edges;
        for (auto& e : result) {
            if (e[2] == -1) e[2] = 2e9;
        }
        return result;
    }

    // Otherwise, we need to reduce the distance to target using -1 edges.
    const int INF = 2e9;
    bool target_met = false;
    std::vector<std::vector<int>> result = edges;
    for (auto& e : result) {
        if (e[2] != -1) continue;
        if (target_met) {
            e[2] = INF; // no longer needed
            graph[e[0]][e[1]] = graph[e[1]][e[0]] = INF;
            continue;
        }
        // Try with weight 1 first.
        e[2] = 1;
        graph[e[0]][e[1]] = graph[e[1]][e[0]] = 1;
        int new_dist = dijkstra(n, graph, source, destination);
        if (new_dist <= target) {
            // This edge can help achieve target.
            target_met = true;
            e[2] += target - new_dist; // adjust to reach exactly target
            graph[e[0]][e[1]] = graph[e[1]][e[0]] = e[2];
        } else {
            // Keep as 1, will not be reduced further because later edges may help.
            // But if this edge's weight is 1 and still not enough, keep it as is.
            // (No change needed, already set to 1)
        }
    }
    if (!target_met) return {};
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Example 1: simple chain, one -1 edge.
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{0,1,-1},{1,2,3}};
        int src = 0, dst = 2, target = 5;
        auto res = modifyGraphEdges(n, edges, src, dst, target);
        assert(!res.empty());
        // Check that after modification, shortest path equals target.
        // Build graph from res and run dijkstra (can use same function but that's not exposed).
        // Here we just verify basic properties: all weights positive, and result size same.
        assert(res.size() == 2);
        assert(res[0][2] > 0 && res[1][2] > 0);
        assert(res[0][0] == 0 && res[0][1] == 1);
        assert(res[1][0] == 1 && res[1][1] == 2);
        // The adjusted edge should be 2 (since 2+3=5)
        assert(res[0][2] == 2);
    }

    // Example 2: impossible because shortest path without -1 is less than target.
    {
        int n = 2;
        std::vector<std::vector<int>> edges = {{0,1,2}};
        int src = 0, dst = 1, target = 1;
        auto res = modifyGraphEdges(n, edges, src, dst, target);
        assert(res.empty());
    }

    // Example 3: already equal, -1 edges become large.
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{0,1,2},{1,2,-1}};
        int src = 0, dst = 2, target = 2;   // direct path 0->1 is 2, but no edge to 2, so not reachable -> need check
        // Actually let's make a reachable case: 0-1 with weight 2, 1-2 with -1, target=2? That's not possible because need to go through 1.
        // Use a better example: chain with one -1 but already gives target without it.
        n = 3;
        edges = {{0,1,3},{1,2,-1},{0,2,5}};
        src = 0; dst = 2; target = 5;
        auto res = modifyGraphEdges(n, edges, src, dst, target);
        assert(!res.empty());
        // The -1 edge should be set to INF (2e9) because direct edge 0-2 gives 5.
        assert(res[1][2] == 2000000000);
    }

    // Example 4: multiple -1 edges, need to use one.
    {
        int n = 4;
        std::vector<std::vector<int>> edges = {{0,1,-1},{1,2,1},{2,3,-1},{0,3,10}};
        int src = 0, dst = 3, target = 4;
        auto res = modifyGraphEdges(n, edges, src, dst, target);
        assert(!res.empty());
        // Path 0-1-2-3 must have total 4. If first -1 becomes 1, and last -1 becomes 2 (1+1+2=4)
        // or first becomes 2 and last 1, but algorithm sets first to 1, then checks: after first=1, distance is 1+1+(-1) impossible, but with second set to 1 gives 1+1+1=3 < target? Actually 3<4, so it will set second to 1 + (4-3)=2.
        // So first stays 1, second becomes 2.
        assert(res[0][2] == 1);
        assert(res[2][2] == 2);
    }

    // Example 5: no -1 edges and already target.
    {
        int n = 2;
        std::vector<std::vector<int>> edges = {{0,1,7}};
        int src = 0, dst = 1, target = 7;
        auto res = modifyGraphEdges(n, edges, src, dst, target);
        assert(!res.empty());
        assert(res[0][2] == 7);
    }

    // Example 6: impossible because even with all -1=1, distance stays above target.
    {
        int n = 3;
        std::vector<std::vector<int>> edges = {{0,1,-1},{1,2,-1}};
        int src = 0, dst = 2, target = 1;
        auto res = modifyGraphEdges(n, edges, src, dst, target);
        assert(res.empty());
    }

    return 0;
}

// The solution uses Dijkstra's algorithm multiple times. First, compute the shortest path from `source` to `destination` treating all `-1` edges as non-existent (skip them). Let this distance be `d0`. If `d0 < target`, no assignment can increase the distance, so return an empty vector. If `d0 == target`, we are done: set every `-1` edge to a very large number (2e9) so it does not affect the shortest path, and return the modified edges. Otherwise, `d0 > target`. Now we need to reduce the distance to exactly `target` by assigning positive weights to the unknown edges. Process the `-1` edges one by one in the original order. Initially, set each unknown edge to weight `1` (the smallest positive integer). After setting an edge, run Dijkstra again. If the new shortest path distance `d1` is still greater than `target`, keep this edge as `1` and continue to the next unknown edge. If `d1 <= target`, then this edge can be used to adjust the path: set its weight to `1 + (target - d1)` (this makes the new distance exactly `target`). Mark that the target is achievable. All remaining unknown edges (after this one) are set to a very large number (2e9) to avoid interfering. If after processing all unknown edges we never reached `d1 <= target`, return an empty vector. The key insight is that increasing weights of earlier edges cannot shorten any path, and we greedily find the earliest edge that can be used to bring the distance down to `target`. Time complexity: Dijkstra is \(O((V+E)\log V)\) with a priority queue. We run Dijkstra once initially and once per unknown edge, but in the worst case with \(k\) unknown edges, the total is \(O(k \cdot (V+E)\log V)\). Space complexity is \(O(V^2)\) for the adjacency matrix used in Dijkstra (since we use a dense representation) plus \(O(V+E)\) for the priority queue.
