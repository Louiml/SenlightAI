/*
Given an unweighted tree with \(n\) nodes (numbered \(1\) to \(n\)), write a C++ function that returns the expected value of the depth of a uniformly random node, where the depth of a node is defined as its distance (number of edges) from node \(1\). The tree is represented by a list of undirected edges, and the function should take the number of nodes and a vector of edge pairs as input, returning a `double` precision result. The tree is connected and has no cycles.
*/
#include <vector>
#include <functional>

// Compute the expected depth of a uniformly random node in a tree rooted at node 0 (node 1 in 1-based).
// edges: vector of pairs (u, v) with 1-based node numbers. Returns the expectation as a double.
double expectedDepth(int n, const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        int u = e.first - 1;
        int v = e.second - 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    long long sumDepths = 0;
    std::function<void(int,int,int)> dfs = [&](int u, int parent, int depth) {
        sumDepths += depth;
        for (int v : adj[u]) {
            if (v != parent) {
                dfs(v, u, depth + 1);
            }
        }
    };

    dfs(0, -1, 0);
    return static_cast<double>(sumDepths) / n;
}
#include <cassert>
#include <vector>
#include <utility>
#include <cmath>

int main() {
    // n=1 tree with no edges: expected depth = 0
    assert(std::fabs(expectedDepth(1, {}) - 0.0) < 1e-12);

    // n=2 tree: edge (1,2). depths: root 0, other 1 => expectation 0.5
    assert(std::fabs(expectedDepth(2, {{1,2}}) - 0.5) < 1e-12);

    // n=3 chain: edges (1-2, 2-3). depths: 0,1,2 => sum=3, avg=1.0
    assert(std::fabs(expectedDepth(3, {{1,2},{2,3}}) - 1.0) < 1e-12);

    // n=3 star: edges (1-2, 1-3). depths: 0,1,1 => sum=2, avg=2/3
    assert(std::fabs(expectedDepth(3, {{1,2},{1,3}}) - (2.0/3.0)) < 1e-12);

    // n=4 chain: depths 0,1,2,3 => sum=6, avg=1.5
    assert(std::fabs(expectedDepth(4, {{1,2},{2,3},{3,4}}) - 1.5) < 1e-12);

    // n=4 star with root 1: depths 0,1,1,1 => sum=3, avg=0.75
    assert(std::fabs(expectedDepth(4, {{1,2},{1,3},{1,4}}) - 0.75) < 1e-12);

    return 0;
}
// The solution performs a single depth-first search (DFS) from the root node (node 1) to compute the depth of every node. Since the tree has \(n-1\) edges, the DFS visits each node exactly once. For each node at depth \(d\) (distance from root), the contribution to the expected depth is \(d/n\), but instead of computing depths as integers and then dividing, we can directly accumulate \(1.0 / n\) per unit depth. A simpler equivalent is to sum all depths and divide by \(n\) at the end, but the original snippet accumulates \(1.0/(\text{depth}+1)\) which is unrelated. For the task as stated (expected depth of a random node), we compute the sum of depths and then divide by \(n\). Edge cases: the tree has at least 1 node, so \(n \geq 1\). If \(n=1\), the only node has depth 0, so expected depth is 0.0. The DFS must avoid revisiting the parent to prevent infinite recursion. Time complexity: \(O(n)\) because each node and edge is processed once. Space complexity: \(O(n)\) for the adjacency list and DFS recursion stack (depth up to \(n\) in a chain).
