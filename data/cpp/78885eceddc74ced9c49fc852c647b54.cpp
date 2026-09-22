/*
Write a C++ function `int countConnectedComponents(int n, const std::vector<std::pair<int,int>>& edges)` that takes the number of vertices `n` (labeled from 1 to `n`) and a vector of undirected edges, and returns the number of connected components in the graph. Use a disjoint-set (union-find) data structure with union by rank and path compression. The graph may be disconnected, may contain self-loops, parallel edges, and edges that connect already-connected vertices — these should not affect the count. For `n = 0` return `0`. The function must be self-contained, efficient, and robust for up to 50,000 vertices and any number of edges.
*/
#include <vector>

// Count connected components in an undirected graph with vertices 1..n.
int countConnectedComponents(int n, const std::vector<std::pair<int,int>>& edges) {
    if (n <= 0) return 0;

    std::vector<int> parent(n + 1);
    std::vector<int> rank(n + 1, 0);

    // Initialize each vertex as its own set.
    for (int i = 1; i <= n; ++i) {
        parent[i] = i;
    }

    // Find with path compression (iterative to avoid recursion overhead).
    auto find = [&](int x) -> int {
        int root = x;
        while (root != parent[root]) {
            root = parent[root];
        }
        // Path compression: point all nodes on path directly to root.
        while (x != root) {
            int next = parent[x];
            parent[x] = root;
            x = next;
        }
        return root;
    };

    // Union by rank.
    auto unite = [&](int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return;  // Already in same set.
        if (rank[ra] < rank[rb]) {
            parent[ra] = rb;
        } else if (rank[ra] > rank[rb]) {
            parent[rb] = ra;
        } else {
            parent[rb] = ra;
            ++rank[ra];
        }
    };

    for (const auto& edge : edges) {
        unite(edge.first, edge.second);
    }

    // Count distinct roots.
    int count = 0;
    for (int i = 1; i <= n; ++i) {
        if (find(i) == i) {
            ++count;
        }
    }
    return count;
}
#include <cassert>
#include <vector>

int main() {
    // Empty graph: all vertices isolated.
    assert(countConnectedComponents(5, {}) == 5);

    // Fully connected graph of 3 vertices.
    assert(countConnectedComponents(3, {{1,2},{2,3}}) == 1);

    // Disconnected: two components.
    assert(countConnectedComponents(4, {{1,2},{3,4}}) == 2);

    // Self-loops and parallel edges do not change components.
    assert(countConnectedComponents(3, {{1,1},{2,2},{1,1},{2,3},{3,2}}) == 2);

    // Already-connected vertices: union redundant.
    assert(countConnectedComponents(2, {{1,2},{1,2}}) == 1);

    // Single vertex with no edges.
    assert(countConnectedComponents(1, {}) == 1);

    // n = 0.
    assert(countConnectedComponents(0, {{1,2}}) == 0);

    // Larger chain with a cycle.
    assert(countConnectedComponents(5, {{1,2},{2,3},{3,1},{4,5}}) == 2);

    // Isolated vertex among connected ones.
    assert(countConnectedComponents(4, {{1,2},{2,3}}) == 2);

    // All vertices connected via a path.
    assert(countConnectedComponents(6, {{1,2},{2,3},{3,4},{4,5},{5,6}}) == 1);

    return 0;
}
// The task is a classic application of the disjoint-set data structure. Initialize each vertex as its own set (parent = itself, rank = 0). For each edge `(a, b)`, perform `union(a, b)` using the `find` operation with path compression to flatten the tree and `union by rank` to keep trees shallow. After processing all edges, call `find` on every vertex to ensure each parent points directly to its root, then count how many distinct roots exist — that equals the number of connected components. Edge cases: vertices with no edges are isolated components; self-loops do not change components; parallel edges are redundant; if `n` is 0, the answer is 0 (loop from 1 to `n` handles it naturally). Time complexity is nearly `O((n + m) * α(n))`, where `α` is the inverse Ackermann function (effectively constant), and space is `O(n)` for the parent and rank arrays. The solution uses iterative or recursive find; recursive is simpler but must handle depth — with path compression, recursion depth stays small in practice.
