Write a C++ function that takes a 2D square matrix representing a directed weighted graph with vertices, where the number of vertices is a compile-time constant `N`, and an entry of `INF` (a large sentinel value) indicates that no direct edge exists between two vertices. The function should compute and return a new matrix (as a `std::array<std::array<int, N>, N>` or equivalent) containing the shortest distances between every pair of vertices using the Floyd‑Warshall algorithm. The input matrix must not be modified; the function should handle graphs where some pairs are unreachable (leaving them as `INF` in the output) and where diagonal entries are zero. Do not assume the graph is acyclic or that all weights are non‑negative — your solution must correctly handle negative edges (but not negative cycles). Also ensure that if a direct edge weight is `INF`, it is not added to any path calculation.

// The solution applies the classic Floyd‑Warshall dynamic programming algorithm. For each intermediate vertex `k` (from `0` to `N-1`), we consider every starting vertex `i` and every ending vertex `j`. If a path from `i` to `k` and from `k` to `j` both exist (i.e., both `dist[i][k]` and `dist[k][j]` are not `INF`), then we update `dist[i][j]` to the minimum of its current value and the sum of those two distances. The algorithm runs in O(N³) time and uses O(N²) additional space for the result matrix (which is returned by value). Key edge cases: (1) Unreachable pairs must remain `INF` — we only update if both intermediate distances are finite; (2) The diagonal must remain zero — the algorithm naturally preserves zeros because adding two distances where one is zero yields a non‑negative or equal value, but if negative cycles existed, diagonals could become negative, but we assume no negative cycles; (3) The input matrix is copied first so that the original is unchanged. The complexity is independent of the number of edges and is suitable for dense graphs or small `N`.

#include <array>
#include <algorithm>
#include <stdexcept>

// Compute all-pairs shortest paths using Floyd-Warshall.
// Returns a new matrix; the input remains unchanged.
// N is the compile-time number of vertices.
// INF must be larger than any possible finite path length.
template<size_t N>
std::array<std::array<int, N>, N> floydWarshall(const std::array<std::array<int, N>, N>& input, int INF) {
    // Work on a copy so the input is not modified.
    std::array<std::array<int, N>, N> dist = input;

    // Floyd-Warshall core algorithm.
    for (size_t k = 0; k < N; ++k) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                // Only consider a path through k if both sub-paths exist.
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    int candidate = dist[i][k] + dist[k][j];
                    if (candidate < dist[i][j]) {
                        dist[i][j] = candidate;
                    }
                }
            }
        }
    }
    return dist;
}

#include <cassert>
#include <array>

// Include the solution function here (or link it).
// For testing, define N=4 and INF=99999.

int main() {
    const size_t N = 4;
    const int INF = 99999;

    // Example from the snippet.
    std::array<std::array<int, N>, N> graph = {{
        {{0, 5, INF, 10}},
        {{INF, 0, 3, INF}},
        {{INF, INF, 0, 1}},
        {{INF, INF, INF, 0}}
    }};

    std::array<std::array<int, N>, N> result = floydWarshall<N>(graph, INF);

    // Expected shortest distances.
    std::array<std::array<int, N>, N> expected = {{
        {{0, 5, 8, 9}},
        {{INF, 0, 3, 4}},
        {{INF, INF, 0, 1}},
        {{INF, INF, INF, 0}}
    }};

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            assert(result[i][j] == expected[i][j]);

    // Verify the input matrix was not modified.
    assert(graph[0][2] == INF);
    assert(graph[2][1] == INF);

    // Test with negative edges (no negative cycles).
    const size_t N2 = 3;
    std::array<std::array<int, N2>, N2> graph2 = {{
        {{0, -1, 4}},
        {{INF, 0, 3}},
        {{INF, INF, 0}}
    }};
    std::array<std::array<int, N2>, N2> result2 = floydWarshall<N2>(graph2, INF);
    assert(result2[0][2] == 2);  // 0->1->2: -1+3
    assert(result2[1][2] == 3);

    // Test unreachable pair remains INF.
    const size_t N3 = 2;
    std::array<std::array<int, N3>, N3> graph3 = {{
        {{0, INF}},
        {{INF, 0}}
    }};
    std::array<std::array<int, N3>, N3> result3 = floydWarshall<N3>(graph3, INF);
    assert(result3[0][1] == INF);

    // Test diagonal stays zero.
    for (size_t i = 0; i < N; ++i)
        assert(result[i][i] == 0);

    return 0;
}
