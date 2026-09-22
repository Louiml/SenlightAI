Given an unweighted, undirected tree with `n` nodes (numbered 1 to n) and `n-1` edges, write a C++ function that returns a vector of integers where the i-th element (0-indexed for node i+1) is the distance from node i+1 to the farthest node in the tree. The function must take the node count `n` and a vector of undirected edges (each edge as a pair of node indices) as input, and compute the eccentricity (max distance to any other node) for every node using double DFS/BFS.

#include <cassert>
#include <vector>

// Solution function declaration (assume it is defined above in the same translation unit)
std::vector<int> farthestDistances(int n, const std::vector<std::pair<int, int>>& edges);

int main() {
    // Test 1: Single node
    {
        std::vector<int> result = farthestDistances(1, {});
        assert(result.size() == 1 && result[0] == 0);
    }

    // Test 2: Two nodes connected
    {
        std::vector<int> result = farthestDistances(2, {{1, 2}});
        assert((result == std::vector<int>{1, 1}));
    }

    // Test 3: Path of 4 nodes 1-2-3-4
    {
        std::vector<int> result = farthestDistances(4, {{1, 2}, {2, 3}, {3, 4}});
        assert((result == std::vector<int>{3, 2, 2, 3}));
    }

    // Test 4: Star with center 1 and leaves 2,3,4
    {
        std::vector<int> result = farthestDistances(4, {{1, 2}, {1, 3}, {1, 4}});
        assert((result == std::vector<int>{1, 2, 2, 2}));
    }

    // Test 5: Asymmetric tree: 1-2, 2-3, 2-4, 4-5
    {
        std::vector<int> result = farthestDistances(5, {{1, 2}, {2, 3}, {2, 4}, {4, 5}});
        // Distances:
        // 1: to 5 = 3, to 3 = 2 -> max 3
        // 2: to 5 = 2, to 3 = 1 -> max 2
        // 3: to 5 = 3, to 1 = 2 -> max 3
        // 4: to 1 = 2, to 3 = 2 -> max 2
        // 5: to 1 = 3, to 3 = 3 -> max 3
        assert((result == std::vector<int>{3, 2, 3, 2, 3}));
    }

    // Test 6: Larger balanced tree: root 1 with children 2,3; 2 with child 4; 3 with child 5
    {
        std::vector<int> result = farthestDistances(5, {{1, 2}, {1, 3}, {2, 4}, {3, 5}});
        // Diameter between 4 and 5 has length 3 (4-2-1-3-5? actually 4-2-1-3-5 is 4 edges)
        // Let's compute carefully: 4-2-1-3-5 distance 4. So farthest from 4 is 5 (4), from 5 is 4 (4).
        // From 1: max to 4 (2) and to 5 (2) -> 2
        // From 2: max to 5 (3) -> 3
        // From 3: max to 4 (3) -> 3
        assert((result == std::vector<int>{2, 3, 3, 4, 4}));
    }

    return 0;
}

#include <vector>
#include <algorithm>
#include <functional>

std::vector<int> farthestDistances(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    std::function<void(int, int, std::vector<int>&, std::vector<int>&)> dfs = [&](int u, int p, std::vector<int>& dist, std::vector<int>& parent) {
        for (int v : adj[u]) {
            if (v == p) continue;
            dist[v] = dist[u] + 1;
            parent[v] = u;
            dfs(v, u, dist, parent);
        }
    };

    std::vector<int> distA(n + 1, 0), parentA(n + 1, 0);
    dfs(1, 0, distA, parentA);
    int u = std::max_element(distA.begin() + 1, distA.end()) - distA.begin();

    std::vector<int> distFromU(n + 1, 0), parentU(n + 1, 0);
    dfs(u, 0, distFromU, parentU);
    int v = std::max_element(distFromU.begin() + 1, distFromU.end()) - distFromU.begin();

    std::vector<int> distFromV(n + 1, 0), parentV(n + 1, 0);
    dfs(v, 0, distFromV, parentV);

    std::vector<int> result(n);
    for (int i = 1; i <= n; ++i) {
        result[i - 1] = std::max(distFromU[i], distFromV[i]);
    }
    return result;
}

// The key observation is that in a tree, the farthest node from any node is one of the two endpoints of the tree's diameter. The diameter can be found by first running a DFS/BFS from an arbitrary node (say node 1) to find the farthest node `u`, then running a second DFS/BFS from `u` to find the farthest node `v`; the distance from `u` to `v` is the diameter. Once both endpoints are known, the farthest distance from any node `x` is the maximum of the distance from `x` to `u` and the distance from `x` to `v`. This can be computed with two more DFS traversals: one rooted at `u` and one rooted at `v`, storing distances in two arrays. The answer for each node is the element-wise maximum of these two distance arrays. Edge cases: a single-node tree (n=1) has all distances zero; the tree is connected so no isolated nodes exist. Time complexity is O(n) for the four DFS traversals, and space complexity is O(n) for adjacency lists and distance arrays.
