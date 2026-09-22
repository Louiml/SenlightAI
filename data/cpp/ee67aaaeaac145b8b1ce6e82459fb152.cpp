Write a C++ function `std::vector<std::vector<int>> buildGraph(int b, long long m)` that, given the number of vertices `b` (between 2 and 50) and a desired number of distinct directed paths `m` (between 1 and 10^18), constructs an adjacency matrix `g` of size `b x b` (using 0s and 1s, where `g[i][j] = 1` means a directed edge from vertex `i` to vertex `j`) such that the number of distinct directed paths from vertex 0 to vertex `b-1` is exactly `m`. If it is impossible to achieve exactly `m` paths with `b` vertices, return an empty matrix. The graph must be acyclic (no cycles allowed), and all edges must go from a lower index to a higher index (i.e., only allowed if `i < j`), which automatically guarantees acyclicity. The function should handle all possible `b` and `m` values correctly. The output should be the adjacency matrix in row-major order. If impossible, return `{}` (empty vector). The function should be self-contained and not rely on global variables.
#include <cassert>
#include <vector>

// Function declaration (must match the solution)
std::vector<std::vector<int>> buildGraph(int b, long long m);

// Helper to count paths from 0 to last vertex in a given graph (assumes acyclic).
long long countPaths(const std::vector<std::vector<int>>& g) {
    int b = (int)g.size();
    if (b == 0) return -1; // empty graph
    std::vector<long long> paths(b, 0);
    paths[0] = 1;
    for (int u = 0; u < b; ++u) {
        for (int v = u + 1; v < b; ++v) {
            if (g[u][v]) {
                paths[v] += paths[u];
            }
        }
    }
    return paths[b - 1];
}

int main() {
    // Impossible cases
    assert(buildGraph(2, 2) == std::vector<std::vector<int>>{});
    assert(buildGraph(3, 8) == std::vector<std::vector<int>>{});

    // Simple cases
    auto g1 = buildGraph(2, 1);
    assert(g1.size() == 2);
    assert(countPaths(g1) == 1);
    assert(g1[0][1] == 1);

    auto g2 = buildGraph(3, 1);
    assert(countPaths(g2) == 1);
    auto g3 = buildGraph(3, 2);
    assert(countPaths(g3) == 2);
    auto g4 = buildGraph(3, 3);
    assert(countPaths(g4) == 3);
    auto g5 = buildGraph(3, 4);
    assert(countPaths(g5) == 4);

    // Larger values
    auto g6 = buildGraph(5, 10);
    assert(countPaths(g6) == 10);
    auto g7 = buildGraph(10, 1000000);
    assert(countPaths(g7) == 1000000LL);
    auto g8 = buildGraph(50, 1LL << 48);
    assert(countPaths(g8) == (1LL << 48));
    auto g9 = buildGraph(50, (1LL << 48) + 12345);
    assert(countPaths(g9) == (1LL << 48) + 12345);

    // Ensure acyclicity and proper structure (all edges i<j)
    for (auto& g : {g1, g2, g3, g4, g5, g6, g7, g8, g9}) {
        for (int i = 0; i < (int)g.size(); ++i) {
            for (int j = 0; j < (int)g.size(); ++j) {
                if (g[i][j]) assert(i < j);
            }
        }
    }

    return 0;
}
#include <vector>

// Build a directed acyclic graph with exactly m paths from vertex 0 to vertex b-1.
// Returns an empty vector if impossible.
std::vector<std::vector<int>> buildGraph(int b, long long m) {
    // Find the smallest n such that 2^n >= m
    int n = 0;
    long long v = 1;  // v = 2^n
    while (v < m) {
        ++n;
        v <<= 1;
    }
    // Need at least n+2 vertices: n intermediate plus source and sink.
    if (b < n + 2) {
        return {};
    }

    // Initialize adjacency matrix (b x b) with zeros.
    std::vector<std::vector<int>> g(b, std::vector<int>(b, 0));

    // Special case: if m is exactly a power of two, add a direct edge from source to sink
    // and reduce m by 1 so we only need to cover the remaining lower bits.
    if (v == m) {
        g[0][b - 1] = 1;
        --m;
    }

    // Build intermediate vertices from near sink (index b-2) downwards.
    // The vertex at index i (for i from b-2 down to b-n-1) will have path count = 2^(dist from sink).
    // Connect each such vertex to all higher-indexed vertices (including sink).
    for (int i = b - 2; i >= b - n - 1; --i) {
        // Connect i to all j > i
        for (int j = i + 1; j < b; ++j) {
            g[i][j] = 1;
        }
        // Based on the current low bit of m, optionally add an edge from source (0) to i.
        if ((m & 1) != 0) {
            g[0][i] = 1;
        }
        m >>= 1;
    }

    return g;
}
// The problem is a classic construction of a graph with a specific number of paths from source (vertex 0) to sink (vertex b-1). The key insight is that by building a "chain" of intermediate vertices (from vertex b-2 down to some lower index), each intermediate vertex forms a complete subgraph to all higher-indexed vertices, creating powers of two in path counts. Specifically, let `n` be the smallest integer such that `2^n >= m`. If `b < n + 2`, then even the maximal graph (complete DAG) has only `2^(b-2)` paths, which is less than `m`, so return empty. Otherwise, we construct the graph as follows: First, create a "base" chain of `n` intermediate vertices (indices `b-2, b-3, ..., b-n-1`). For each such vertex `i`, add all edges from `i` to every higher-indexed vertex (including the sink `b-1` and other intermediates). This gives each intermediate vertex a path count equal to the sum of paths of all vertices it points to. The vertex at index `b-2` (the one just before the sink) has exactly 1 path (direct edge to sink). The vertex `b-3` has 2 paths (direct to sink, and through b-2). So the k-th intermediate from the sink has `2^(k-1)` paths if we only connect to the next intermediate and sink. But we connect to all higher intermediates, which doubles the count each time, so the vertex at position `i = b-2` has 1 path, `b-3` has 2, `b-4` has 4, etc. Now, we need exactly `m` paths from vertex 0. Write `m` in binary. If `m` is a power of two (i.e., `v == m` after finding `v` as smallest power of two >= m), then we add a direct edge from 0 to sink and reduce `m` by 1 (since that direct edge contributes one path). Then, for each intermediate vertex from highest index (near sink) to lowest, we check the current low bit of `m`. If it is 1, we add an edge from 0 to that intermediate vertex (which contributes exactly that intermediate's path count to the total). Then shift `m` right by 1. This constructs a graph where total paths from 0 to sink equals the original `m`. The construction uses at most `n+1` intermediate vertices (including the direct edge case), so we need `b >= n+2`. Edge cases: if `m == 1`, then `n=0` (since `2^0=1`), and we simply add a direct edge from 0 to sink; the loop for intermediates is empty. If `m` is a power of two, the special case of adding direct edge handles it. Complexity: The number of vertices is at most 50, so building the `b x b` matrix is O(b^2) time and O(b^2) space.
