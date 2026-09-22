// Write a C++ function `vector<int> distancesFromSeeds(int n, const vector<pair<int,int>>& edges, const vector<int>& seeds)` that, given an undirected graph with vertices numbered 1 to `n`, a list of undirected edges, and a list of seed vertices, returns a vector of length `n+1` (indexed 1..n) where the value at index `i` is the minimum number of edges needed to travel from vertex `i` to any seed vertex. If a vertex is unreachable from all seeds, its value should be -1. The distance from a seed vertex to itself is 0. The graph may have multiple edges between the same pair and may be disconnected. The seed list is non-empty and contains only valid vertices (1..n).

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or link it)

int main() {
    // Test 1: Simple connected graph, single seed
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5}};
        std::vector<int> seeds = {1};
        std::vector<int> result = distancesFromSeeds(n, edges, seeds);
        assert(result[1] == 0);
        assert(result[2] == 1);
        assert(result[3] == 2);
        assert(result[4] == 3);
        assert(result[5] == 4);
    }

    // Test 2: Multiple seeds, disconnected graph
    {
        int n = 6;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{4,5}}; // 6 is isolated
        std::vector<int> seeds = {1, 5};
        std::vector<int> result = distancesFromSeeds(n, edges, seeds);
        assert(result[1] == 0);
        assert(result[2] == 1);
        assert(result[3] == 2);
        assert(result[4] == 1); // distance to seed 5 via edge 4-5
        assert(result[5] == 0);
        assert(result[6] == -1); // isolated
    }

    // Test 3: Duplicate seeds, multiple edges
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{1,2},{2,3}};
        std::vector<int> seeds = {2, 2, 1};
        std::vector<int> result = distancesFromSeeds(n, edges, seeds);
        assert(result[1] == 0);
        assert(result[2] == 0);
        assert(result[3] == 1);
    }

    // Test 4: All vertices are seeds
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {};
        std::vector<int> seeds = {1,2,3,4};
        std::vector<int> result = distancesFromSeeds(n, edges, seeds);
        for (int i = 1; i <= n; i++) {
            assert(result[i] == 0);
        }
    }

    // Test 5: Empty edges but one seed
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {};
        std::vector<int> seeds = {2};
        std::vector<int> result = distancesFromSeeds(n, edges, seeds);
        assert(result[1] == -1);
        assert(result[2] == 0);
        assert(result[3] == -1);
    }

    // Test 6: Cycle, all reachable
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,1}};
        std::vector<int> seeds = {1};
        std::vector<int> result = distancesFromSeeds(n, edges, seeds);
        assert(result[1] == 0);
        assert(result[2] == 1);
        assert(result[3] == 2);
        assert(result[4] == 1);
    }

    return 0;
}

#include <vector>
#include <queue>
#include <algorithm>

// Returns distances from each vertex (1..n) to the nearest seed.
// Uses multi-source BFS. Unreachable vertices get -1.
std::vector<int> distancesFromSeeds(
    int n,
    const std::vector<std::pair<int,int>>& edges,
    const std::vector<int>& seeds
) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // Distance array, initialized to -1 (unreachable)
    std::vector<int> dist(n + 1, -1);
    std::queue<int> q;

    // Initialize with all seeds
    for (int s : seeds) {
        if (dist[s] == -1) { // avoid duplicate seeds
            dist[s] = 0;
            q.push(s);
        }
    }

    // Multi-source BFS
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist; // index 0 is unused, but kept for size convenience
}

// This is a classic multi-source BFS problem. Initialize a queue with all seed vertices, and set their distances to 0 in a `dist` array. Mark them as visited (or use a separate visited flag). Then perform BFS level by level: pop a vertex, for each neighbor not yet visited, set its distance to `current_distance + 1`, push it onto the queue, and mark as visited. Since BFS processes nodes in increasing distance order, the first time a vertex is reached yields the minimum distance. Any vertex left unvisited after the BFS is unreachable, so set its distance to -1. Edge cases: seeds may contain duplicates—handle by checking visited before pushing; self-loops are ignored naturally because the neighbor check skips visited; multiple edges cause no issue. Time complexity is O(n + m) where m is the number of edges (each edge is examined twice in adjacency list). Space complexity is O(n + m) for adjacency and O(n) for distances and visited.
