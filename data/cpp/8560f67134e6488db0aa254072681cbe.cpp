/*
Write a C++ function that, given the number of nodes `n` (2 ≤ n ≤ 2000) and a list of undirected edges forming a tree, returns the **diameter** of the tree — the length (in edges) of the longest path between any two nodes. The input is represented as an adjacency list where nodes are labeled from 1 to n, and the edges are provided as pairs. The function should compute the diameter efficiently without recursion depth issues for n up to 2000. Implement the classic two-DFS approach: first, find the farthest node from an arbitrary starting node (e.g., node 1), then find the farthest distance from that node — that distance is the diameter. Ensure the function is robust for both balanced and skewed trees.
*/

#include <vector>
#include <utility>
#include <functional>

// Return the diameter (longest path length in edges) of an undirected tree.
// adj is a 1-indexed adjacency list, n is the number of nodes (>=2).
int treeDiameter(const std::vector<std::vector<int>>& adj, int n) {
    // DFS that returns {max_distance_from_start, node_achieving_that_distance}
    std::function<std::pair<int,int>(int,int,int)> dfs = [&](int node, int par, int dist) -> std::pair<int,int> {
        std::pair<int,int> best = {dist, node};
        for (int nb : adj[node]) {
            if (nb == par) continue;
            auto sub = dfs(nb, node, dist + 1);
            if (sub.first > best.first) best = sub;
        }
        return best;
    };

    // First DFS from node 1 to find one endpoint of a diameter
    std::pair<int,int> first = dfs(1, -1, 0);
    // Second DFS from that endpoint to find the actual diameter distance
    std::pair<int,int> second = dfs(first.second, -1, 0);
    return second.first;
}

#include <cassert>
#include <vector>

// Include the solution function here (for brevity in the test, assume it's defined above)

int main() {
    // Test 1: simple tree 1-2, 2-3, 3-4 (diameter 3)
    {
        std::vector<std::vector<int>> adj(5);
        adj[1].push_back(2); adj[2].push_back(1);
        adj[2].push_back(3); adj[3].push_back(2);
        adj[3].push_back(4); adj[4].push_back(3);
        assert(treeDiameter(adj, 4) == 3);
    }
    // Test 2: star with center 1 and leaves 2,3,4 (diameter 2)
    {
        std::vector<std::vector<int>> adj(5);
        adj[1] = {2,3,4};
        adj[2] = {1};
        adj[3] = {1};
        adj[4] = {1};
        assert(treeDiameter(adj, 4) == 2);
    }
    // Test 3: just two nodes (diameter 1)
    {
        std::vector<std::vector<int>> adj(3);
        adj[1].push_back(2); adj[2].push_back(1);
        assert(treeDiameter(adj, 2) == 1);
    }
    // Test 4: balanced binary tree of 7 nodes (diameter 4)
    {
        std::vector<std::vector<int>> adj(8);
        adj[1] = {2,3};
        adj[2] = {1,4,5};
        adj[3] = {1,6,7};
        adj[4] = {2}; adj[5] = {2}; adj[6] = {3}; adj[7] = {3};
        assert(treeDiameter(adj, 7) == 4);
    }
    // Test 5: skewed tree (chain of 5 nodes) diameter 4
    {
        std::vector<std::vector<int>> adj(6);
        adj[1].push_back(2); adj[2].push_back(1);
        adj[2].push_back(3); adj[3].push_back(2);
        adj[3].push_back(4); adj[4].push_back(3);
        adj[4].push_back(5); adj[5].push_back(4);
        assert(treeDiameter(adj, 5) == 4);
    }
    // Test 6: tree with a longer branch, diameter 5
    {
        std::vector<std::vector<int>> adj(7);
        adj[1] = {2,3};
        adj[2] = {1,4};
        adj[3] = {1,5};
        adj[4] = {2,6};
        adj[5] = {3};
        adj[6] = {4};
        // Path from 5 to 6: 5-3-1-2-4-6 → length 5
        assert(treeDiameter(adj, 6) == 5);
    }
    return 0;
}

// The diameter of a tree is the maximum distance between any two nodes. A well-known property: starting from any node, find the farthest node (call it `u`) via a DFS/BFS; then starting from `u`, find the farthest distance (which is the diameter). This works because the endpoint of any diameter is always a farthest node from any starting point. The provided code uses DFS on an undirected tree, tracking the maximum distance and the node that achieves it. The first DFS from node 1 returns the farthest node `u`; the second DFS from `u` returns the maximum distance, which is the diameter.  
// Edge cases: The tree has at least two nodes (n≥2). For n=2, the diameter is 1, and the algorithm correctly returns 1 (first DFS from 1 returns node 2 with distance 1, second DFS from node 2 returns distance 1). For skewed trees (e.g., a chain), recursion depth equals n, but with n≤2000, recursion is safe within default stack limits. The time complexity is O(n) for each DFS, so O(n) total, and space complexity is O(n) for the adjacency list and recursion stack. The solution uses `pair<int,int>` (distance, node) to track results.
