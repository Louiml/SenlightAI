/*
Write a C++ function `int countConnectedComponents(int n, const std::vector<std::pair<int, int>>& edges)` that, given a set of vertices labeled from 1 to `n` and a list of undirected edges (each pair is 1-indexed), returns the number of connected components in the graph. The function must internally use a disjoint-set (union-find) data structure with path compression and union by rank (or by smaller/larger root label). You may assume the graph has at least one vertex (`n >= 1`) and the edge list may be empty or contain duplicate edges, self-loops, and edges that do not form new connections. The function should not modify the input. This is an independent exercise; you must implement the entire union-find logic inside the function.
*/
#include <vector>
#include <numeric>

// Count connected components in an undirected graph with vertices 1..n.
int countConnectedComponents(int n, const std::vector<std::pair<int, int>>& edges) {
    // parent[i] stores the root or a node in the same set.
    std::vector<int> parent(n + 1);
    // Initialize each vertex as its own set.
    for (int i = 1; i <= n; ++i) {
        parent[i] = i;
    }

    // Find the root with path compression.
    auto find = [&](int a) -> int {
        while (parent[a] != a) {
            parent[a] = parent[parent[a]];  // halving
            a = parent[a];
        }
        return a;
    };

    // Union by attaching smaller root to larger root (by label).
    auto unionSets = [&](int a, int b) {
        int rootA = find(a);
        int rootB = find(b);
        if (rootA != rootB) {
            if (rootA < rootB) {
                parent[rootB] = rootA;
            } else {
                parent[rootA] = rootB;
            }
        }
    };

    // Process all edges.
    for (const auto& edge : edges) {
        unionSets(edge.first, edge.second);
    }

    // Count distinct roots.
    int components = 0;
    for (int i = 1; i <= n; ++i) {
        if (find(i) == i) {
            ++components;
        }
    }
    return components;
}
#include <cassert>
#include <vector>

// Include the solution function here (or paste above)

int main() {
    // Empty graph: all isolated.
    std::vector<std::pair<int, int>> edges1;
    assert(countConnectedComponents(5, edges1) == 5);

    // Simple connected chain 1-2-3.
    std::vector<std::pair<int, int>> edges2 = {{1,2}, {2,3}};
    assert(countConnectedComponents(3, edges2) == 1);

    // Two separate components: 1-2 and 3-4, plus isolated 5.
    std::vector<std::pair<int, int>> edges3 = {{1,2}, {3,4}};
    assert(countConnectedComponents(5, edges3) == 3);

    // Duplicate edges and self-loop should not change count.
    std::vector<std::pair<int, int>> edges4 = {{1,1}, {1,2}, {2,1}, {1,2}};
    assert(countConnectedComponents(2, edges4) == 1);

    // Star graph: all connected to 1.
    std::vector<std::pair<int, int>> edges5 = {{1,2}, {1,3}, {1,4}, {1,5}};
    assert(countConnectedComponents(5, edges5) == 1);

    // Complete graph on 4 vertices.
    std::vector<std::pair<int, int>> edges6 = {{1,2}, {1,3}, {1,4}, {2,3}, {2,4}, {3,4}};
    assert(countConnectedComponents(4, edges6) == 1);

    // Two cliques: {1,2} and {3,4,5}.
    std::vector<std::pair<int, int>> edges7 = {{1,2}, {3,4}, {4,5}, {3,5}};
    assert(countConnectedComponents(5, edges7) == 2);

    // Single vertex with no edges.
    std::vector<std::pair<int, int>> edges8;
    assert(countConnectedComponents(1, edges8) == 1);

    // All vertices connected through a cycle.
    std::vector<std::pair<int, int>> edges9 = {{1,2}, {2,3}, {3,4}, {4,1}};
    assert(countConnectedComponents(4, edges9) == 1);

    // Multiple components with many duplicate edges.
    std::vector<std::pair<int, int>> edges10 = {{1,2}, {1,2}, {2,1}, {5,6}, {6,5}, {7,8}};
    assert(countConnectedComponents(8, edges10) == 5); // components: {1,2}, {3}, {4}, {5,6}, {7,8}

    return 0;
}
// The solution uses a union-find data structure to group vertices that are reachable from each other. Initialize each vertex as its own parent (`parent[i] = i`) and optionally track rank/size. For each edge `(a, b)`, find the roots of both endpoints using path compression (recursively set parent to root) and then union them by attaching the root with higher rank under the other root (or by smaller index if you prefer, but rank better balances the tree). After processing all edges, the number of connected components equals the number of distinct roots. You can count these by iterating over all vertices, calling `find` on each, and counting unique roots (e.g., using a set or by checking if `i == find(i)` after path compression). Edge cases: no edges (components = n), duplicate edges (union is idempotent), self-loops (union with itself is no-op), and multiple components. Time complexity: O((n + m) α(n)) where α is the inverse Ackermann function (nearly constant) due to path compression and union by rank. Space complexity: O(n) for the parent array.
