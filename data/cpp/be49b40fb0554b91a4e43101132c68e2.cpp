You are given an undirected weighted graph with `V` nodes (numbered 1 to `V`) and `E` edges. There are two special nodes, `a` and `b`. Write a C++ function that computes the minimum cost of a path that starts at node 1, visits both `a` and `b` in either order (not necessarily consecutively), and ends at node `V`. The path may revisit nodes and edges. If no such path exists (i.e., any required segment is unreachable), the function should return `-1`. Your function must be self-contained, take all necessary parameters (the number of vertices, an adjacency list of edges with weights, and the two special nodes), and implement Dijkstra’s algorithm correctly.

#include <cassert>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (omitted for brevity in this test snippet)

int main() {
    // Test 1: Simple triangle 1-2-3 with 2 as a, 3 as b? Actually a=2,b=3, V=3
    {
        int V = 3;
        vector<vector<pair<int, long long>>> g(V + 1);
        g[1].push_back({2, 5});
        g[2].push_back({1, 5});
        g[2].push_back({3, 7});
        g[3].push_back({2, 7});
        assert(minCostThroughTwoNodes(V, g, 2, 3) == 12); // 1->2 (5) + 2->3 (7) + 3->3 (0)
    }

    // Test 2: Same as above but a=3,b=2; order 1->3->2->3 requires 1->3 direct? Not present, so must go 1->2->3->2->3 = 5+7+7+7=26; order 1->2->3->3 = 12
    {
        int V = 3;
        vector<vector<pair<int, long long>>> g(V + 1);
        g[1].push_back({2, 5});
        g[2].push_back({1, 5});
        g[2].push_back({3, 7});
        g[3].push_back({2, 7});
        assert(minCostThroughTwoNodes(V, g, 3, 2) == 12); // order 1->2->3 is shortest
    }

    // Test 3: Disconnected graph – no path from 1 to some node
    {
        int V = 4;
        vector<vector<pair<int, long long>>> g(V + 1);
        g[1].push_back({2, 1});
        g[2].push_back({1, 1});
        g[3].push_back({4, 1});
        g[4].push_back({3, 1});
        assert(minCostThroughTwoNodes(V, g, 2, 3) == -1); // 3 is unreachable from 1
    }

    // Test 4: a and b are the same node
    {
        int V = 3;
        vector<vector<pair<int, long long>>> g(V + 1);
        g[1].push_back({2, 1});
        g[2].push_back({1, 1});
        g[2].push_back({3, 1});
        g[3].push_back({2, 1});
        assert(minCostThroughTwoNodes(V, g, 2, 2) == 2); // 1->2->3
    }

    // Test 5: a = 1, b = V
    {
        int V = 4;
        vector<vector<pair<int, long long>>> g(V + 1);
        g[1].push_back({2, 2});
        g[2].push_back({1, 2});
        g[2].push_back({3, 3});
        g[3].push_back({2, 3});
        g[3].push_back({4, 4});
        g[4].push_back({3, 4});
        assert(minCostThroughTwoNodes(V, g, 1, 4) == 9); // 1->2->3->4 = 2+3+4 = 9
    }

    // Test 6: Larger graph, multiple paths
    {
        int V = 5;
        vector<vector<pair<int, long long>>> g(V + 1);
        g[1].push_back({2, 10});
        g[2].push_back({1, 10});
        g[2].push_back({3, 1});
        g[3].push_back({2, 1});
        g[3].push_back({4, 1});
        g[4].push_back({3, 1});
        g[4].push_back({5, 10});
        g[5].push_back({4, 10});
        g[1].push_back({3, 100});
        g[3].push_back({1, 100});
        // a=2, b=4, V=5: best 1->2 (10) + 2->3 (1) + 3->4 (1) + 4->5 (10) = 22
        assert(minCostThroughTwoNodes(V, g, 2, 4) == 22);
    }

    // Test 7: Single node (V=1) with a=b=1
    {
        int V = 1;
        vector<vector<pair<int, long long>>> g(V + 1);
        assert(minCostThroughTwoNodes(V, g, 1, 1) == 0); // no edges needed
    }

    // Test 8: a and b unreachable from each other
    {
        int V = 4;
        vector<vector<pair<int, long long>>> g(V + 1);
        g[1].push_back({2, 1});
        g[2].push_back({1, 1});
        g[3].push_back({4, 1});
        g[4].push_back({3, 1});
        // 1 connects to 2 only; 3-4 are separate. a=2, b=3 → unreachable, return -1
        assert(minCostThroughTwoNodes(V, g, 2, 3) == -1);
    }

    return 0;
}

#include <bits/stdc++.h>
using namespace std;

// Returns the minimum cost to go from 'start' to 'end' visiting both 'a' and 'b' (in any order).
// Uses Dijkstra four times: from start, from a, from b, from end.
// Returns -1 if no such path exists.
long long minCostThroughTwoNodes(int V,
                                 const vector<vector<pair<int, long long>>>& graph,
                                 int a, int b) {
    const long long INF = 4e18;

    auto dijkstra = [&](int src) {
        vector<long long> dist(V + 1, INF);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[u]) continue;

            for (const auto& [v, w] : graph[u]) {
                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    };

    auto dist_from_start = dijkstra(1);
    auto dist_from_a = dijkstra(a);
    auto dist_from_b = dijkstra(b);
    auto dist_from_end = dijkstra(V);

    // Order 1: 1 -> a -> b -> V
    long long cost1 = dist_from_start[a] + dist_from_a[b] + dist_from_b[V];
    // Order 2: 1 -> b -> a -> V
    long long cost2 = dist_from_start[b] + dist_from_b[a] + dist_from_a[V];

    long long ans = min(cost1, cost2);
    if (ans >= INF / 2) return -1;  // any segment is unreachable
    return ans;
}

// The problem requires finding the shortest path from 1 to V that necessarily passes through both `a` and `b`. Since the order of visiting `a` and `b` is not fixed, there are exactly two possible orders:  
// 1. 1 → a → b → V  
// 2. 1 → b → a → V  
//
// For each order, the total cost is the sum of three independent shortest paths:  
// - From the start to the first special node,  
// - From the first special node to the second,  
// - From the second special node to the destination.  
//
// We can precompute shortest distances from a single source to all nodes using Dijkstra’s algorithm². Because the graph is undirected, the distance from `x` to `y` equals the distance from `y` to `x`, but for clarity we will run Dijkstra four times: from node 1, from `a`, from `b`, and from `V`. Then:  
// - cost_order1 = dist_from_1[a] + dist_from_a[b] + dist_from_b[V]  
// - cost_order2 = dist_from_1[b] + dist_from_b[a] + dist_from_a[V]  
//
// Take the minimum of the two (if both are computable). Dijkstra returns infinity if a node is unreachable; if any segment in either order is infinite, that order is invalid. If both orders are invalid, return `-1`.  
//
// Edge cases:  
// - `a` or `b` may equal 1 or `V`; the algorithm still works because distances to self are 0.  
// - `a == b`: then both orders reduce to 1 → a → V, and the formula still works because dist_from_a[a] = 0.  
// - Large weights (up to 10^9) and up to 800 nodes, so use `long long` for distances and edge weights to avoid overflow.  
// - The graph may be disconnected; Dijkstra must handle unreachable nodes gracefully.  
//
// Time complexity: Each Dijkstra runs in O((V+E) log V) using a priority queue. We run exactly four of them, so total O((V+E) log V) — effectively O(E log V) since E ≥ V-1 in most cases. Space complexity: O(V+E) for the adjacency list and O(V) for distance arrays.
