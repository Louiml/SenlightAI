/*
Design a C++ function named `isConnectedComponents` that takes a non-negative integer `n` (the number of elements numbered `0` to `n-1`) and an `std::vector<std::pair<int, int>>` representing undirected edges between elements. Use a disjoint-set (union-find) data structure with path compression and union-by-size (as inspired by the provided snippet) to determine whether the graph is fully connected (i.e., all `n` vertices belong to exactly one connected component). The function should return `true` if there is exactly one component, and `false` otherwise (including the case where `n == 0` is treated as not connected, or handle it as you see fit but document your choice). The edges may contain duplicates, self-loops, or out-of-range vertices; such invalid edges should be ignored safely. Provide the implementation of the disjoint-set operations inline within your function (you may use a local struct or class) without using external libraries beyond `<vector>` and `<utility>`. The function must be `const`-correct where applicable and must not modify the input vector.
*/

#include <vector>
#include <utility>

// Determine if a graph with n vertices (0..n-1) and given edges is fully connected.
bool isConnectedComponents(int n, const std::vector<std::pair<int, int>>& edges) {
    if (n == 0) return false; // Convention: empty graph not connected.
    
    // Disjoint-set data structure using local struct for clarity.
    struct DisjointSets {
        std::vector<int> parent;
        std::vector<int> size;

        DisjointSets(int count) : parent(count), size(count, 0) {
            for (int i = 0; i < count; ++i) {
                parent[i] = i; // Each vertex starts as its own representative.
                size[i] = 1;
            }
        }

        // Find with path compression (recursive).
        int find(int x) {
            if (parent[x] != x) {
                parent[x] = find(parent[x]);
            }
            return parent[x];
        }

        // Union by size, assuming roots as arguments.
        void unionSets(int rootX, int rootY) {
            if (rootX == rootY) return;
            if (size[rootX] < size[rootY]) {
                parent[rootX] = rootY;
                size[rootY] += size[rootX];
            } else {
                parent[rootY] = rootX;
                size[rootX] += size[rootY];
            }
        }
    };

    DisjointSets ds(n);

    // Process each edge; ignore invalid or self-loops.
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        if (u < 0 || u >= n || v < 0 || v >= n) continue;
        if (u == v) continue; // Self-loop does nothing.
        int ru = ds.find(u);
        int rv = ds.find(v);
        if (ru != rv) {
            ds.unionSets(ru, rv);
        }
    }

    // Count distinct roots.
    int rootCount = 0;
    for (int i = 0; i < n; ++i) {
        if (ds.find(i) == i) {
            ++rootCount;
        }
    }
    return rootCount == 1;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Single vertex, no edges: connected (one component).
    assert(isConnectedComponents(1, {}) == true);

    // Two vertices with no edges: not connected.
    assert(isConnectedComponents(2, {}) == false);

    // Two vertices connected by one edge.
    assert(isConnectedComponents(2, {{0,1}}) == true);

    // Triangle: 3 vertices with 3 edges.
    assert(isConnectedComponents(3, {{0,1},{1,2},{2,0}}) == true);

    // Chain of 4 vertices: connected.
    assert(isConnectedComponents(4, {{0,1},{1,2},{2,3}}) == true);

    // Disconnected: two components.
    assert(isConnectedComponents(4, {{0,1},{2,3}}) == false);

    // Duplicate edges and self-loops should not affect.
    assert(isConnectedComponents(3, {{0,0},{1,1},{0,1},{0,1},{1,2}}) == true);

    // Out-of-range edges ignored.
    assert(isConnectedComponents(2, {{0,5},{5,1}}) == false);

    // Zero vertices: conventionally not connected.
    assert(isConnectedComponents(0, {}) == false);

    // Larger graph: star connected.
    assert(isConnectedComponents(5, {{0,1},{0,2},{0,3},{0,4}}) == true);

    // Larger graph with one orphan.
    assert(isConnectedComponents(5, {{0,1},{1,2},{2,3}}) == false);

    return 0;
}

// The core solution is to simulate union-find with the operations `MakeSet`, `Find`, and `Union` as in the given snippet, but adapted to handle a dynamically sized array. Since we only care about connectivity (not actual component membership), we can initialize each vertex as its own parent with size 1 (or size 0 before making a set), then process each edge: skip if either endpoint is out of range `[0, n)`, or if both endpoints are the same (self-loop, no effect). For valid edges, first find the representatives of both endpoints using recursive `Find` with path compression (if a vertex’s parent is itself, it’s a root; otherwise, recursively find the root and set the parent directly). Then apply union-by-size: if the sizes are equal or `repy` is larger, attach `repx` under `repy` and update size; otherwise attach `repy` under `repx`. After processing all edges, count the number of roots (vertices where `parent[i] == i`). If this count equals 1, the graph is connected. Edge cases: `n == 0` — there are no vertices; I choose to return `false` because “all vertices connected” is vacuously true but conventionally a graph with zero vertices is not considered connected (or you can document returning `true`; here we return `false` to be explicit). Duplicate edges and self-loops do not change the component count; out-of-range edges are ignored. Time complexity is nearly \(O(n + m \cdot \alpha(n))\) where \(m\) is the number of edges and \(\alpha\) is the inverse Ackermann function (practically constant), due to path compression and union-by-size. Space complexity is \(O(n)\) for the parent and size arrays.
