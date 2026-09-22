/*
Write a C++ function `long long treeDiameterSum(const std::vector<std::vector<std::pair<int,int>>>& adj)` that takes an adjacency list representation of a weighted, undirected tree with `n` nodes (nodes numbered from 1 to n, but the function should work with 0-indexed as well; assume zero‑based indexing for simplicity) and returns the sum of edge weights along the longest path (diameter) of the tree. The graph is guaranteed to be a connected tree, so there are exactly `n-1` edges. Edge weights are positive integers. The function must compute the diameter by performing two breadth‑first searches (BFS): first from an arbitrary node (e.g., node 0) to find the farthest node, then from that farthest node to find the maximum distance. Return that maximum distance as a `long long` (distances can exceed 32-bit int). The input adjacency list should be passed by `const` reference, and the function should not modify it. Assume the graph is non-empty (n ≥ 1). For a single node, diameter is 0 (since there are no edges).
*/

#include <vector>
#include <queue>
#include <utility>
#include <algorithm>

/**
 * Computes the sum of edge weights along the longest path (diameter) of a tree.
 * The tree is represented by an adjacency list with (neighbor, weight) pairs.
 * Nodes are zero-indexed.
 */
long long treeDiameterSum(const std::vector<std::vector<std::pair<int,int>>>& adj) {
    int n = (int)adj.size();
    if (n <= 1) return 0LL;

    // BFS helper: given a start node, returns (farthestNode, farthestDistance)
    auto bfs = [&](int start) -> std::pair<int,long long> {
        std::vector<long long> dist(n, -1);
        std::vector<bool> visited(n, false);
        std::queue<int> q;
        q.push(start);
        dist[start] = 0;
        visited[start] = true;

        int farthestNode = start;
        long long maxDist = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto& edge : adj[u]) {
                int v = edge.first;
                int w = edge.second;
                if (!visited[v]) {
                    visited[v] = true;
                    dist[v] = dist[u] + w;
                    q.push(v);
                    if (dist[v] > maxDist) {
                        maxDist = dist[v];
                        farthestNode = v;
                    }
                }
            }
        }
        return {farthestNode, maxDist};
    };

    // First BFS from node 0 to find an endpoint of the diameter
    auto first = bfs(0);
    int endpoint = first.first;

    // Second BFS from that endpoint to get the actual diameter distance
    auto second = bfs(endpoint);
    return second.second;
}

#include <cassert>
#include <vector>
#include <utility>

// Solution function declaration (assume defined above)
long long treeDiameterSum(const std::vector<std::vector<std::pair<int,int>>>& adj);

int main() {
    // Test 1: Single node, diameter = 0
    {
        std::vector<std::vector<std::pair<int,int>>> adj(1);
        assert(treeDiameterSum(adj) == 0LL);
    }

    // Test 2: Two nodes with edge weight 5
    {
        std::vector<std::vector<std::pair<int,int>>> adj(2);
        adj[0].push_back({1, 5});
        adj[1].push_back({0, 5});
        assert(treeDiameterSum(adj) == 5LL);
    }

    // Test 3: Simple line of 3 nodes: 0-1 (weight 2), 1-2 (weight 3) => diameter 5
    {
        std::vector<std::vector<std::pair<int,int>>> adj(3);
        adj[0].push_back({1, 2});
        adj[1].push_back({0, 2});
        adj[1].push_back({2, 3});
        adj[2].push_back({1, 3});
        assert(treeDiameterSum(adj) == 5LL);
    }

    // Test 4: Star: center 0 connected to leaves 1,2,3 with weights 4,7,1 => longest path between leaves 1 and 2 is 4+7=11
    {
        std::vector<std::vector<std::pair<int,int>>> adj(4);
        adj[0].push_back({1, 4});
        adj[1].push_back({0, 4});
        adj[0].push_back({2, 7});
        adj[2].push_back({0, 7});
        adj[0].push_back({3, 1});
        adj[3].push_back({0, 1});
        assert(treeDiameterSum(adj) == 11LL);
    }

    // Test 5: Larger tree with branch: 0-1 (10), 1-2 (20), 1-3 (30) => diameter between 2 and 3 is 20+30=50
    {
        std::vector<std::vector<std::pair<int,int>>> adj(4);
        adj[0].push_back({1, 10});
        adj[1].push_back({0, 10});
        adj[1].push_back({2, 20});
        adj[2].push_back({1, 20});
        adj[1].push_back({3, 30});
        adj[3].push_back({1, 30});
        assert(treeDiameterSum(adj) == 50LL);
    }

    // Test 6: Tree with 5 nodes forming a "Y": 0 connected to 1 (1), 1 connected to 2 (2) and 3 (3), 3 connected to 4 (4)
    // Longest path from 2 to 4: 2 + 3 + 4 = 9
    {
        std::vector<std::vector<std::pair<int,int>>> adj(5);
        adj[0].push_back({1, 1});
        adj[1].push_back({0, 1});
        adj[1].push_back({2, 2});
        adj[2].push_back({1, 2});
        adj[1].push_back({3, 3});
        adj[3].push_back({1, 3});
        adj[3].push_back({4, 4});
        adj[4].push_back({3, 4});
        assert(treeDiameterSum(adj) == 9LL);
    }

    // Test 7: Large weights to ensure long long is used. Path 0-1 weight 100000, 1-2 weight 200000 => diameter 300000
    {
        std::vector<std::vector<std::pair<int,int>>> adj(3);
        adj[0].push_back({1, 100000});
        adj[1].push_back({0, 100000});
        adj[1].push_back({2, 200000});
        adj[2].push_back({1, 200000});
        assert(treeDiameterSum(adj) == 300000LL);
    }

    // Test 8: Unbalanced tree with multiple branches: 0-1 (2), 0-2 (3), 1-3 (4), 1-4 (5), 2-5 (6), 2-6 (7)
    // Longest path: between 4 and 6 → 5 + 2 + 3 + 7 = 17
    {
        std::vector<std::vector<std::pair<int,int>>> adj(7);
        adj[0].push_back({1, 2});
        adj[1].push_back({0, 2});
        adj[0].push_back({2, 3});
        adj[2].push_back({0, 3});
        adj[1].push_back({3, 4});
        adj[3].push_back({1, 4});
        adj[1].push_back({4, 5});
        adj[4].push_back({1, 5});
        adj[2].push_back({5, 6});
        adj[5].push_back({2, 6});
        adj[2].push_back({6, 7});
        adj[6].push_back({2, 7});
        assert(treeDiameterSum(adj) == 17LL);
    }

    return 0;
}

// The solution uses the well‑known tree diameter algorithm: run BFS (or DFS) from any node to find the node `u` that is farthest from it (in terms of cumulative edge weights). Then run BFS from `u` to find the farthest distance from `u`; this distance is exactly the diameter. Why it works: In a tree, the farthest node from any arbitrary node is always one endpoint of the diameter. Proof relies on tree properties: no cycles, and paths are unique. BFS works because edge weights are positive and we are just accumulating distances; BFS visits nodes in non‑decreasing distance from the starting node (since all edges have same "hop" cost of 1, but we sum weights, BFS still works because we only need to compute distances from source, and the tree structure ensures correct propagation). Implementation details: use two arrays/vectors for distances, a queue, and a visited boolean vector. For each BFS, initialize distances to a large sentinel (e.g., `-1`) and mark visited. When popping a node, iterate over its neighbors; if not visited, set distance as `dist[current] + edgeWeight`, push to queue. After BFS, find the node with maximum distance. For the first BFS, start from node 0, get `u`. Then run second BFS from `u`, get max distance. Since weights can be large, use `long long` for distances. Edge cases: single node (diameter 0), two nodes with a single edge. Time complexity: O(n) because each edge is traversed twice (once per BFS). Space complexity: O(n) for adjacency list, distance arrays, visited array, and queue.
