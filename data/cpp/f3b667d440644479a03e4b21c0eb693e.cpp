// Given an undirected graph represented in CSR (compressed sparse row) format with `n` vertices, an edge array `adj`, and an offset array `xadj`, write a C++ function that computes the Reverse Cuthill-McKee (RCM) ordering of the graph. The function should accept the CSR arrays, the number of vertices `n`, and output arrays for the ordering (`order`) and for scratch space (a queue buffer). The returned ordering must be a permutation of vertices `0` through `n-1` that reduces the bandwidth of the adjacency matrix. Your implementation must be deterministic, handle disconnected graphs by processing each component separately, and for each connected component, start from a pseudo-peripheral vertex (found using BFS from an arbitrary start). The final ordering is the reverse of the BFS traversal order within each component, and components are processed in increasing vertex-index order. The function signature is: `void rcm_ordering(const std::vector<int>& xadj, const std::vector<int>& adj, int n, std::vector<int>& order)`.
// The Reverse Cuthill-McKee algorithm reduces profile and bandwidth by renumbering vertices based on a breadth-first search (BFS) from a pseudo-peripheral vertex. The core steps are: (1) For each unvisited vertex (in increasing index order), find a pseudo-peripheral vertex in its connected component using a two-phase BFS approach: start from the arbitrary vertex, run BFS to find a vertex at maximum distance, then run BFS again from that vertex and repeat until the eccentricity stabilizes. (2) From this pseudo-peripheral root, perform a BFS where vertices are visited in non-decreasing degree order; this is achieved by using a priority queue that always pops the unvisited neighbor with the smallest degree (or if using a heap with ties broken by vertex index). (3) Append the BFS traversal sequence to the global order. (4) After all components are processed, reverse the entire ordering—this is the RCM heuristic, since reversing BFS order reduces bandwidth better. Edge cases include isolated vertices (degree 0), components with a single vertex, and graphs with multiple disconnected components. Time complexity: Each BFS is O(V+E) per component, so overall O(n+m). The pseudo-peripheral search loops at most a constant number of times (usually few) per component, so still O(n+m). Space complexity O(n) for auxiliary arrays (distance, visited, queue/priority queue). The function must not use global mutable state, must be const-correct on inputs, and must work deterministically for any valid CSR representation.
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

// Compute Reverse Cuthill-McKee ordering for an undirected graph in CSR format.
// Input: xadj (size n+1), adj (size m), n = number of vertices.
// Output: order (size n) is a permutation of [0, n-1].
void rcm_ordering(const std::vector<int>& xadj, const std::vector<int>& adj, int n, std::vector<int>& order) {
    order.resize(n);
    if (n == 0) return;

    std::vector<int> visited(n, 0);
    std::vector<int> dist(n, -1);
    std::vector<int> temp_order;
    temp_order.reserve(n);

    // Helper lambda for BFS from a given source, returns the farthest vertex and its distance.
    auto bfs_farthest = [&](int source) -> std::pair<int, int> {
        std::fill(dist.begin(), dist.end(), -1);
        std::queue<int> q;
        q.push(source);
        dist[source] = 0;
        int farthest = source;
        int max_dist = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int e = xadj[u]; e < xadj[u + 1]; ++e) {
                int v = adj[e];
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                    if (dist[v] > max_dist) {
                        max_dist = dist[v];
                        farthest = v;
                    }
                }
            }
        }
        return {farthest, max_dist};
    };

    // Helper lambda to find a pseudo-peripheral vertex in the component of 'start'.
    auto find_pseudo_peripheral = [&](int start) -> int {
        int r = start;
        int r_dist = 0;
        while (true) {
            auto [candidate, candidate_dist] = bfs_farthest(r);
            if (candidate_dist <= r_dist) break;
            r = candidate;
            r_dist = candidate_dist;
        }
        return r;
    };

    // Process each unvisited vertex in increasing order.
    for (int start = 0; start < n; ++start) {
        if (visited[start]) continue;

        // Handle isolated vertex.
        if (xadj[start] == xadj[start + 1]) {
            visited[start] = 1;
            temp_order.push_back(start);
            continue;
        }

        // Find a pseudo-peripheral root in this component.
        int root = find_pseudo_peripheral(start);

        // BFS from root with vertex degree ordering using a priority queue.
        // Use a priority queue that returns the smallest (degree, vertex) via a max-heap with negation.
        std::priority_queue<std::pair<int, int>> pq; // stores (-degree, -vertex) to get min degree then min vertex
        visited[root] = 1;
        pq.push({0, -root}); // degree is not needed for root; use 0 as placeholder, but we'll use negative vertex for tie-break.

        std::vector<int> component_bfs;
        component_bfs.reserve(n);

        while (!pq.empty()) {
            auto [neg_deg, neg_u] = pq.top();
            pq.pop();
            int u = -neg_u;
            component_bfs.push_back(u);

            // Collect unvisited neighbors with their degrees.
            std::vector<std::pair<int, int>> neighbors; // (degree, vertex)
            for (int e = xadj[u]; e < xadj[u + 1]; ++e) {
                int v = adj[e];
                if (!visited[v]) {
                    int deg = xadj[v + 1] - xadj[v];
                    neighbors.emplace_back(deg, v);
                }
            }

            // Sort neighbors by (degree, vertex) for deterministic ordering.
            std::sort(neighbors.begin(), neighbors.end());
            for (auto& p : neighbors) {
                int v = p.second;
                if (!visited[v]) {
                    visited[v] = 1;
                    int deg = xadj[v + 1] - xadj[v];
                    pq.push({-deg, -v});
                }
            }
        }

        // Append this component's BFS order (not reversed yet) to temp_order.
        for (int v : component_bfs) {
            temp_order.push_back(v);
        }
    }

    // Reverse the entire ordering to get RCM.
    std::reverse(temp_order.begin(), temp_order.end());
    order = temp_order;
}
#include <cassert>
#include <vector>
#include <iostream>

// Declaration of the function to test.
void rcm_ordering(const std::vector<int>& xadj, const std::vector<int>& adj, int n, std::vector<int>& order);

// Helper to check if order is a valid permutation of 0..n-1.
bool isPermutation(const std::vector<int>& order, int n) {
    if ((int)order.size() != n) return false;
    std::vector<int> seen(n, 0);
    for (int v : order) {
        if (v < 0 || v >= n || seen[v]) return false;
        seen[v] = 1;
    }
    return true;
}

// Helper to build CSR from an edge list (0-based vertices).
void buildCSR(int n, const std::vector<std::pair<int,int>>& edges,
              std::vector<int>& xadj, std::vector<int>& adj) {
    std::vector<int> deg(n, 0);
    for (auto& e : edges) {
        deg[e.first]++;
        deg[e.second]++;
    }
    xadj.assign(n + 1, 0);
    for (int i = 0; i < n; ++i) xadj[i + 1] = xadj[i] + deg[i];
    adj.assign(xadj[n], 0);
    std::vector<int> cur = xadj;
    for (auto& e : edges) {
        adj[cur[e.first]++] = e.second;
        adj[cur[e.second]++] = e.first;
    }
}

int main() {
    // Test 1: Simple path 0-1-2-3 (chain)
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3}};
        std::vector<int> xadj, adj;
        buildCSR(n, edges, xadj, adj);
        std::vector<int> order;
        rcm_ordering(xadj, adj, n, order);
        assert(isPermutation(order, n));
        // RCM on a path gives reversed path: {3,2,1,0} or {0,1,2,3}? Actually RCM reverses BFS, so from 0, BFS gives 0,1,2,3, reversed -> 3,2,1,0.
        std::vector<int> expected = {3,2,1,0};
        assert(order == expected);
    }

    // Test 2: Disconnected graph: one edge 0-1, isolated 2, and a triangle 3-4-5-3
    {
        int n = 6;
        std::vector<std::pair<int,int>> edges = {{0,1},{3,4},{4,5},{5,3}};
        std::vector<int> xadj, adj;
        buildCSR(n, edges, xadj, adj);
        std::vector<int> order;
        rcm_ordering(xadj, adj, n, order);
        assert(isPermutation(order, n));
        // Component 0-1: processed first, reverse gives {1,0} in final reverse? Actually overall reverse: temp_order = [0,1, 2, 3,4,5] after components, then reverse -> [5,4,3,2,1,0]. But check that 2 is placed correctly (3rd from end).
        // Just verify that the ordering is valid and that 2 appears somewhere.
        bool found2 = false;
        for (int v : order) if (v == 2) found2 = true;
        assert(found2);
    }

    // Test 3: Star graph: center 0 connected to 1,2,3
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{0,1},{0,2},{0,3}};
        std::vector<int> xadj, adj;
        buildCSR(n, edges, xadj, adj);
        std::vector<int> order;
        rcm_ordering(xadj, adj, n, order);
        assert(isPermutation(order, n));
        // BFS from 0 gives 0,1,2,3 (in order of degree, all leaves deg1, so tie by vertex). Reverse gives 3,2,1,0.
        std::vector<int> expected = {3,2,1,0};
        assert(order == expected);
    }

    // Test 4: Single vertex
    {
        int n = 1;
        std::vector<int> xadj = {0,0};
        std::vector<int> adj = {};
        std::vector<int> order;
        rcm_ordering(xadj, adj, n, order);
        assert(order.size() == 1 && order[0] == 0);
    }

    // Test 5: Complete graph K4 (all pairs)
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
        std::vector<int> xadj, adj;
        buildCSR(n, edges, xadj, adj);
        std::vector<int> order;
        rcm_ordering(xadj, adj, n, order);
        assert(isPermutation(order, n));
        // Any ordering is valid because all vertices have same degree; BFS from 0 yields 0,1,2,3, reversed -> 3,2,1,0.
        std::vector<int> expected = {3,2,1,0};
        assert(order == expected);
    }

    // Test 6: Empty graph with 3 isolated vertices
    {
        int n = 3;
        std::vector<int> xadj = {0,0,0,0};
        std::vector<int> adj = {};
        std::vector<int> order;
        rcm_ordering(xadj, adj, n, order);
        std::vector<int> expected = {2,1,0}; // process 0,1,2, then reverse
        assert(order == expected);
    }

    // Test 7: Two separate edges: 0-1 and 2-3, both isolated from each other
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{0,1},{2,3}};
        std::vector<int> xadj, adj;
        buildCSR(n, edges, xadj, adj);
        std::vector<int> order;
        rcm_ordering(xadj, adj, n, order);
        assert(isPermutation(order, n));
        // After processing components, temp = [0,1,2,3], reverse -> [3,2,1,0].
        std::vector<int> expected = {3,2,1,0};
        assert(order == expected);
    }

    // Test 8: Cycle of 5 vertices (0-1-2-3-4-0)
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3},{3,4},{4,0}};
        std::vector<int> xadj, adj;
        buildCSR(n, edges, xadj, adj);
        std::vector<int> order;
        rcm_ordering(xadj, adj, n, order);
        assert(isPermutation(order, n));
        // BFS from 0 gives 0,1,4,2,3 (depending on degree and tie-breaking), reversed yields something but must be a valid permutation.
        // Just check validity.
    }

    // Test 9: Larger graph with 10 vertices, random edges (deterministic construction)
    {
        int n = 10;
        std::vector<std::pair<int,int>> edges;
        // Build a small grid-like structure (3x3 plus extra node) to ensure non-trivial
        edges = {{0,1},{1,2},{3,4},{4,5},{6,7},{7,8},{0,3},{3,6},{1,4},{4,7},{2,5},{5,8},{8,9}};
        std::vector<int> xadj, adj;
        buildCSR(n, edges, xadj, adj);
        std::vector<int> order;
        rcm_ordering(xadj, adj, n, order);
        assert(isPermutation(order, n));
        // Check that the ordering is reversed BFS from pseudo-peripheral; we just check it's a permutation.
    }

    // Test 10: Graph with a single edge and many isolated vertices
    {
        int n = 100;
        std::vector<std::pair<int,int>> edges = {{10,20}};
        std::vector<int> xadj, adj;
        buildCSR(n, edges, xadj, adj);
        std::vector<int> order;
        rcm_ordering(xadj, adj, n, order);
        assert(isPermutation(order, n));
        // The order should have the edge's two vertices adjacent at the start (before reversal) and all isolated after, then reversed.
        // Just verify permutation.
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
