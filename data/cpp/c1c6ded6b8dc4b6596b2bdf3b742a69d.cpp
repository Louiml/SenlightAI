/*
Write a C++ function `std::vector<int> findTreeCentroids(const std::vector<std::vector<int>>& adjacencyList)` that takes an undirected tree represented as an adjacency list (nodes labeled 0 to n-1, where n is the number of vertices) and returns a sorted vector of all centroid node labels (1-indexed as the original problem outputs, or 0-indexed? — here we must decide: the task will return 0-indexed labels to be idiomatic, but the test can transform as needed. Let's specify clearly: the function returns the 0-based indices of all centroids). A centroid of a tree is a node whose removal splits the tree into components, each with at most n/2 nodes. If no centroid exists (which is impossible for a tree with one or more nodes — every tree has at least one centroid), return an empty vector. The input tree is connected and acyclic, with n >= 1. The function must handle large trees efficiently (up to 100,000 nodes). For example, given a tree with edges (0-1, 1-2), the centroids are {1} (0-indexed), and for a path of 4 nodes (0-1-2-3), the centroids are {1, 2}.
*/
#include <vector>
#include <algorithm>

// Given an undirected tree as an adjacency list (0-indexed nodes),
// return a sorted vector of all centroid node labels (0-indexed).
std::vector<int> findTreeCentroids(const std::vector<std::vector<int>>& adjacencyList) {
    const int n = static_cast<int>(adjacencyList.size());
    std::vector<int> subtreeSize(n, 0);
    std::vector<bool> visited(n, false);
    std::vector<int> centroids;

    // Depth-first search that computes subtree sizes and identifies centroids.
    // Returns the size of the subtree rooted at 'node'.
    // The lambda captures by reference to mutate state.
    std::function<int(int)> dfs = [&](int node) -> int {
        visited[node] = true;
        int size = 1;                 // Count the node itself
        int maxPart = 0;              // Largest component after removal

        for (int neighbor : adjacencyList[node]) {
            if (!visited[neighbor]) {
                int childSize = dfs(neighbor);
                size += childSize;
                maxPart = std::max(maxPart, childSize);
            }
        }

        // The component containing the parent is the rest of the tree.
        int restSize = n - size;
        maxPart = std::max(maxPart, restSize);

        if (maxPart <= n / 2) {
            centroids.push_back(node);
        }

        return size;
    };

    dfs(0);  // Tree is connected, so starting at node 0 visits all nodes.

    // The DFS naturally visits nodes in some order; ensure sorted output.
    std::sort(centroids.begin(), centroids.end());
    return centroids;
}
#include <cassert>
#include <vector>

// The solution function is declared above; here we provide a main with assertions.

int main() {
    // Single node tree
    {
        std::vector<std::vector<int>> adj = {{}};
        auto result = findTreeCentroids(adj);
        assert(result == std::vector<int>{0});
    }

    // Two nodes connected
    {
        std::vector<std::vector<int>> adj = {{1}, {0}};
        auto result = findTreeCentroids(adj);
        assert(result == std::vector<int>{0, 1});
    }

    // Path of 3 nodes: 0-1-2 (centroid is 1)
    {
        std::vector<std::vector<int>> adj = {{1}, {0, 2}, {1}};
        auto result = findTreeCentroids(adj);
        assert(result == std::vector<int>{1});
    }

    // Path of 4 nodes: 0-1-2-3 (centroids are 1 and 2)
    {
        std::vector<std::vector<int>> adj = {{1}, {0, 2}, {1, 3}, {2}};
        auto result = findTreeCentroids(adj);
        assert(result == std::vector<int>{1, 2});
    }

    // Star with center 0 and leaves 1,2,3 (only 0 is centroid)
    {
        std::vector<std::vector<int>> adj = {{1, 2, 3}, {0}, {0}, {0}};
        auto result = findTreeCentroids(adj);
        assert(result == std::vector<int>{0});
    }

    // Balanced tree: root 0 with two children (1 and 2), and child 1 has two leaves (3,4)
    // n=5, centroid is 0 (removal leaves components sizes 3 and 1, max=3 <= 2? No, 3 > 2, so not centroid.
    // Actually let's construct a real case: n=7, root 0 with children 1 and 2; child 1 has children 3,4; child 2 has children 5,6.
    // Removing 0 gives components sizes 3 (from 1's subtree) and 3 (from 2's), max=3, n/2=3 -> centroid. Also nodes 1 and 2? Removing 1 gives components: subtree of 1 (size 3) becomes three components of 1, plus rest of tree size 4 -> max=4 > 3, so not. So only {0}.
    {
        std::vector<std::vector<int>> adj = {
            {1, 2},          // 0
            {0, 3, 4},       // 1
            {0, 5, 6},       // 2
            {1},             // 3
            {1},             // 4
            {2},             // 5
            {2}              // 6
        };
        auto result = findTreeCentroids(adj);
        assert(result == std::vector<int>{0});
    }

    // Asymmetric tree: n=6, edges: 0-1, 0-2, 1-3, 1-4, 2-5
    // Compute: subtree sizes from root 0: node 1 size=3, node 2 size=2, node 0 size=6.
    // For node 0: maxPart = max(3,2,6-6=0)=3, n/2=3 -> centroid.
    // Node 1: subtree size 3, children sizes: 1,1 -> maxPart = max(1,1,6-3=3)=3 -> centroid.
    // Node 2: subtree size 2, child size 1 -> maxPart = max(1,6-2=4)=4 > 3 -> not.
    // On smaller nodes 3,4,5: maxPart will be large (e.g., node3: rest=5) -> not.
    {
        std::vector<std::vector<int>> adj = {
            {1, 2},       // 0
            {0, 3, 4},    // 1
            {0, 5},       // 2
            {1},          // 3
            {1},          // 4
            {2}           // 5
        };
        auto result = findTreeCentroids(adj);
        assert(result == std::vector<int>{0, 1});
    }

    return 0;
}
// The classic approach is a depth-first search (DFS) from an arbitrary root (e.g., node 0). For each node, compute the size of its subtree (including itself) using recursion. For a given node `u`, after computing the subtree sizes of all its children, the largest component formed by removing `u` is the maximum among two values: (1) the largest subtree size among `u`'s children, and (2) the size of the rest of the tree, which is `n - subtreeSize(u)`. If this maximum is at most `n/2`, then `u` is a centroid. We perform one DFS that returns subtree size and records centroids as a side effect. Since the graph is a tree, no visited array is needed if we pass the parent node to avoid revisiting, but we can also use a `visited` vector for clarity. Edge cases: when n == 1, the only node is a centroid; when n == 2, both nodes are centroids (removing either leaves a component of size 1, which is ≤ 1). The algorithm runs in O(n) time and O(n) space for the recursion stack and adjacency storage.
