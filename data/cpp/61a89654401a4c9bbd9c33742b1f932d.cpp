Write a C++ function `int treeDiameter(int n, const std::vector<std::pair<int, int>>& edges)` that accepts the number of nodes `n` (numbered 1 through `n`) and an undirected tree given by `edges`, and returns the length (in edges) of the longest path between any two nodes in the tree, also known as the tree diameter. The tree is connected and acyclic, and `n >= 1`. If `n == 1`, the diameter is 0. The function must handle large trees efficiently (up to 10,000 nodes) and avoid recursive stack overflow by using an iterative approach or by carefully bounding recursion depth.
The diameter of a tree can be found using the classic two-DFS/BFS approach: pick any node (e.g., node 1), find the farthest node from it via a breadth-first search or depth-first traversal, then from that farthest node, perform another traversal to find the maximum distance, which is the diameter. This works because in a tree, the farthest node from any arbitrary node is always an endpoint of a diameter. Since the graph is a tree, there are no cycles, so simple traversal avoiding the parent is sufficient. The adjacency list can be built as a vector of vectors. For `n == 1`, return 0 immediately. Edge cases include chains (where diameter is `n-1`) and stars (where diameter is 2). Time complexity is O(n) because each edge is visited twice per traversal, and space complexity is O(n) for the adjacency list and visited/distance arrays. To avoid recursion depth issues, use an explicit stack (iterative DFS) or a queue (BFS) for each traversal; BFS is straightforward and yields distances directly.
#include <vector>
#include <queue>
#include <algorithm>

// Returns the diameter (longest path length in edges) of an undirected tree.
// n: number of nodes (1-indexed), edges: list of undirected edges.
int treeDiameter(int n, const std::vector<std::pair<int, int>>& edges) {
    if (n <= 1) return 0;

    // Build adjacency list.
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // BFS helper: returns the farthest node and its distance from 'start'.
    auto bfs = [&](int start) -> std::pair<int, int> {
        std::vector<int> dist(n + 1, -1);
        std::queue<int> q;
        dist[start] = 0;
        q.push(start);
        int farthest = start;
        int maxDist = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    if (dist[v] > maxDist) {
                        maxDist = dist[v];
                        farthest = v;
                    }
                    q.push(v);
                }
            }
        }
        return {farthest, maxDist};
    };

    // First BFS from node 1 to find one endpoint of a diameter.
    auto [endpoint, _] = bfs(1);
    // Second BFS from that endpoint to find the actual diameter.
    auto [_, diameter] = bfs(endpoint);
    return diameter;
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Single node
    assert(treeDiameter(1, {}) == 0);

    // Two nodes
    assert(treeDiameter(2, {{1,2}}) == 1);

    // Star with 5 nodes: center 1, leaves 2-5
    assert(treeDiameter(5, {{1,2},{1,3},{1,4},{1,5}}) == 2);

    // Simple chain of 4 nodes
    assert(treeDiameter(4, {{1,2},{2,3},{3,4}}) == 3);

    // A more complex tree: path 2-1-3 with extra branch 3-4-5
    // Diameter is between 2 and 5: length 4 (2-1-3-4-5)
    assert(treeDiameter(5, {{1,2},{1,3},{3,4},{4,5}}) == 4);

    // Balanced binary tree with 7 nodes (root 1, children 2,3; leaves 4-7)
    // Diameter between a leaf in left and a leaf in right: 4
    assert(treeDiameter(7, {{1,2},{1,3},{2,4},{2,5},{3,6},{3,7}}) == 4);

    // Larger chain: 10 nodes
    std::vector<std::pair<int,int>> chain;
    for (int i = 1; i < 10; ++i) chain.push_back({i, i+1});
    assert(treeDiameter(10, chain) == 9);

    // Tree with 10000 nodes as a single chain (stress test for recursion safety)
    int bigN = 10000;
    std::vector<std::pair<int,int>> bigChain;
    for (int i = 1; i < bigN; ++i) bigChain.push_back({i, i+1});
    assert(treeDiameter(bigN, bigChain) == bigN - 1);

    return 0;
}
