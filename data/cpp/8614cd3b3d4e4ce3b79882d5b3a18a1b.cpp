// Write a C++ function `longestTreePath(const vector<vector<pair<int,int>>>& adj, int startNode, int nodeCount)` that, given an undirected weighted tree stored as an adjacency list where each entry is `(neighbor, weight)` and weights are positive integers, returns the length of the longest path (the tree diameter) in the tree. The tree has nodes labeled from 1 to `nodeCount`. All edge weights are positive, so the diameter can be found by two breadth-first searches (BFS) or depth-first searches (DFS). The function must be `const`-correct and should not modify the input. It must return the total weight of the longest path between any two nodes.

#include <cassert>
#include <iostream>
#include <vector>
#include <utility>

// Assume the solution function is defined above or included.

int main() {
    // Test 1: simple chain 1-2 (weight 3) and 2-3 (weight 4) => diameter = 7
    {
        std::vector<std::vector<std::pair<int,int>>> adj(4);
        adj[1].push_back({2, 3});
        adj[2].push_back({1, 3});
        adj[2].push_back({3, 4});
        adj[3].push_back({2, 4});
        assert(longestTreePath(adj, 1, 3) == 7);
    }

    // Test 2: single node => diameter = 0
    {
        std::vector<std::vector<std::pair<int,int>>> adj(2);
        // no edges
        assert(longestTreePath(adj, 1, 1) == 0);
    }

    // Test 3: star with center 1 and leaves 2,3,4 with weights 5,6,2 => diameter = 5+6=11
    {
        std::vector<std::vector<std::pair<int,int>>> adj(5);
        adj[1].push_back({2, 5});
        adj[2].push_back({1, 5});
        adj[1].push_back({3, 6});
        adj[3].push_back({1, 6});
        adj[1].push_back({4, 2});
        adj[4].push_back({1, 2});
        assert(longestTreePath(adj, 1, 4) == 11);
    }

    // Test 4: tree with multiple branches: 1-2 (10), 2-3 (20), 2-4 (30) => diameter = 20+10+30=50
    {
        std::vector<std::vector<std::pair<int,int>>> adj(5);
        adj[1].push_back({2, 10});
        adj[2].push_back({1, 10});
        adj[2].push_back({3, 20});
        adj[3].push_back({2, 20});
        adj[2].push_back({4, 30});
        adj[4].push_back({2, 30});
        assert(longestTreePath(adj, 1, 4) == 50);
    }

    // Test 5: asymmetric tree: 1-2 (7), 1-3 (1), 3-4 (2), 3-5 (3) => diameter is 7+1+3=11 (from 2 to 5)
    {
        std::vector<std::vector<std::pair<int,int>>> adj(6);
        adj[1].push_back({2, 7}); adj[2].push_back({1, 7});
        adj[1].push_back({3, 1}); adj[3].push_back({1, 1});
        adj[3].push_back({4, 2}); adj[4].push_back({3, 2});
        adj[3].push_back({5, 3}); adj[5].push_back({3, 3});
        assert(longestTreePath(adj, 1, 5) == 11);
    }

    // Test 6: larger chain: 1-2 (1), 2-3 (1), 3-4 (1), 4-5 (1) => diameter = 4
    {
        std::vector<std::vector<std::pair<int,int>>> adj(6);
        for (int i = 1; i <= 4; ++i) {
            adj[i].push_back({i+1, 1});
            adj[i+1].push_back({i, 1});
        }
        assert(longestTreePath(adj, 1, 5) == 4);
    }

    // Test 7: ensure that starting from a non-1 node works: same tree as test 5 but start at node 4
    {
        std::vector<std::vector<std::pair<int,int>>> adj(6);
        adj[1].push_back({2, 7}); adj[2].push_back({1, 7});
        adj[1].push_back({3, 1}); adj[3].push_back({1, 1});
        adj[3].push_back({4, 2}); adj[4].push_back({3, 2});
        adj[3].push_back({5, 3}); adj[5].push_back({3, 3});
        assert(longestTreePath(adj, 4, 5) == 11);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <queue>
#include <algorithm>
#include <utility>

// Returns the length (sum of edge weights) of the longest path in a weighted tree.
// adj is an adjacency list: adj[vertex] contains pairs (neighbor, weight).
// nodes are labeled 1..nodeCount. startNode is any node (e.g., 1).
int longestTreePath(const std::vector<std::vector<std::pair<int,int>>>& adj, int startNode, int nodeCount) {
    // Helper lambda to perform BFS and return (farthestNode, maxDistance).
    auto bfs = [&](int source) {
        std::vector<int> dist(nodeCount + 1, -1);
        std::queue<int> q;
        dist[source] = 0;
        q.push(source);
        int farthestNode = source;
        int maxDist = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto& edge : adj[u]) {
                int v = edge.first;
                int w = edge.second;
                if (dist[v] == -1) {
                    dist[v] = dist[u] + w;
                    if (dist[v] > maxDist) {
                        maxDist = dist[v];
                        farthestNode = v;
                    }
                    q.push(v);
                }
            }
        }
        return std::make_pair(farthestNode, maxDist);
    };

    // First BFS from startNode to find one endpoint of the diameter.
    auto firstResult = bfs(startNode);
    int endpointA = firstResult.first;

    // Second BFS from that endpoint to get the diameter length.
    auto secondResult = bfs(endpointA);
    return secondResult.second;
}

// The diameter of a weighted tree (the longest path between any two nodes) can be found using two BFS traversals. First, run BFS from an arbitrary node (for example, node 1 or the provided `startNode`) to find the farthest node from it. This farthest node will be one endpoint of the diameter. Then run a second BFS from that farthest node to find the node farthest from it, and the distance accumulated in that second BFS (specifically the maximum distance) is the length of the diameter. This works because in a tree, the farthest node from any node lies on one of the diameter endpoints. Edge cases: the tree must have at least one node; if there is only one node, the diameter length is 0. The graph is undirected, so during traversal we must avoid going back to the parent using a `visited` array. Use a queue for BFS and a distance array. Time complexity is O(V + E) = O(V) for a tree (E = V-1), and space complexity is O(V) for the distance and visited arrays plus the queue. Since the input adjacency list is already provided, no extra graph building is needed.
