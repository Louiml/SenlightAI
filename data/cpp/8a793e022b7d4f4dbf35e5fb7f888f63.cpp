// Write a C++ function `convertAdjacencyToIncidence(int n, const std::vector<std::vector<int>>& adjacency)` that takes the number of vertices `n` (vertices numbered 1–n) and an `n+1` by `n+1` adjacency matrix (indices 0..n, with 0 unused, symmetric, containing only 0s and 1s, no self-loops) and returns a vector of vectors representing the incidence matrix (size `(n+1) × (number_of_edges)`), where each column corresponds to an edge that connects exactly two distinct vertices, and there are no duplicate edges. In the returned incidence matrix, each column must have exactly two entries equal to 1 (one for each endpoint), and every other entry in that column must be 0. The columns should be generated in the order the edges are encountered when scanning the adjacency matrix row by row from row 1 to n, and within a row from column 1 to n, only considering edges where the row index is less than the column index to avoid duplicates. Ensure that the returned matrix has rows indexed from 0 to n (row 0 unused), and the number of columns equals the total number of edges in the graph. Do not modify the input matrix.

The task requires converting an adjacency matrix representation of an undirected, simple graph (no self-loops, no multiple edges) into an incidence matrix. The incidence matrix has one row per vertex (plus an unused row 0) and one column per edge. For each edge between vertices `u` and `v` (with `u < v`), we set the entries in that column at rows `u` and `v` to 1 and all others to 0. The main algorithm is straightforward: iterate over all pairs `(i, j)` with `1 ≤ i < j ≤ n` (we can scan the full matrix, but only act when `i < j` to avoid counting each edge twice). If `adjacency[i][j] == 1`, we know there is an edge, so we append a new column vector of size `n+1` initialized to 0, then set positions `i` and `j` to 1. The order of columns is determined by the iteration order: row by row, and within a row, column by column, but only for `j > i`. Edge cases: the graph may have zero edges, in which case the returned incidence matrix has `n+1` rows and 0 columns (so a vector of size `n+1` each an empty vector). The graph may be disconnected; that is fine. We must ensure that we do not accidentally treat `i == j` entries (which would be self-loops) because the problem states no self-loops, but even if present we ignore them since we require exactly two 1s per column. Time complexity is O(n^2) to scan the matrix plus O(n) to create each column, but since each column has size n+1, total time is O(n^2 + E·n) where E is the number of edges. In the worst case, E is O(n^2), so overall O(n^3) if we naively create each column as a full vector, but in practice for small n it's fine. For typical competitive programming constraints with n ≤ 100, this is acceptable. Space complexity is O(n·E) for the output matrix, which in the worst case is O(n^3) but again manageable for n ≤ 100.

#include <vector>
#include <algorithm>

// Convert an undirected graph's adjacency matrix (vertices 1..n, no self-loops, no duplicates)
// into an incidence matrix (rows 1..n, one column per edge, exactly two 1s per column).
// The returned matrix has n+1 rows (row 0 unused), and columns added in row-major order
// considering only pairs (i, j) with i < j.
std::vector<std::vector<int>> adjacencyToIncidence(int n, const std::vector<std::vector<int>>& adjacency) {
    std::vector<std::vector<int>> incidence(n + 1); // rows 1..n, initially empty columns

    // First, count edges to know the number of columns.
    int edgeCount = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            if (adjacency[i][j] == 1) {
                ++edgeCount;
            }
        }
    }

    // Reserve space for each row's columns (each row has edgeCount entries).
    for (int i = 1; i <= n; ++i) {
        incidence[i].assign(edgeCount, 0);
    }

    // Fill columns in the same order we counted edges.
    int col = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            if (adjacency[i][j] == 1) {
                incidence[i][col] = 1;
                incidence[j][col] = 1;
                ++col;
            }
        }
    }

    return incidence;
}

#include <cassert>
#include <vector>

// Forward declaration for the solution (or include the header in a real project)
std::vector<std::vector<int>> adjacencyToIncidence(int n, const std::vector<std::vector<int>>& adjacency);

int main() {
    // Test 1: Simple triangle (3 vertices, 3 edges)
    {
        int n = 3;
        std::vector<std::vector<int>> adj(n+1, std::vector<int>(n+1, 0));
        adj[1][2] = adj[2][1] = 1;
        adj[2][3] = adj[3][2] = 1;
        adj[1][3] = adj[3][1] = 1;
        auto inc = adjacencyToIncidence(n, adj);
        assert(inc.size() == 4); // rows 0..3
        assert(inc[1].size() == 3);
        // Expected columns (in order of pairs (1,2), (1,3), (2,3)):
        // col0: [0,1,1,0]
        // col1: [0,1,0,1]
        // col2: [0,0,1,1]
        assert(inc[1][0] == 1 && inc[2][0] == 1 && inc[3][0] == 0);
        assert(inc[1][1] == 1 && inc[2][1] == 0 && inc[3][1] == 1);
        assert(inc[1][2] == 0 && inc[2][2] == 1 && inc[3][2] == 1);
        assert(inc[0].empty()); // row 0 unused
    }

    // Test 2: Single edge (2 vertices)
    {
        int n = 2;
        std::vector<std::vector<int>> adj(n+1, std::vector<int>(n+1, 0));
        adj[1][2] = adj[2][1] = 1;
        auto inc = adjacencyToIncidence(n, adj);
        assert(inc.size() == 3);
        assert(inc[1].size() == 1);
        assert(inc[1][0] == 1 && inc[2][0] == 1);
        assert(inc[0].empty());
    }

    // Test 3: Empty graph (no edges)
    {
        int n = 4;
        std::vector<std::vector<int>> adj(n+1, std::vector<int>(n+1, 0));
        auto inc = adjacencyToIncidence(n, adj);
        assert(inc.size() == 5);
        for (int i = 1; i <= n; ++i) {
            assert(inc[i].empty());
        }
    }

    // Test 4: Star graph centered at vertex 1 (4 leaves)
    {
        int n = 5;
        std::vector<std::vector<int>> adj(n+1, std::vector<int>(n+1, 0));
        for (int v = 2; v <= n; ++v) {
            adj[1][v] = adj[v][1] = 1;
        }
        auto inc = adjacencyToIncidence(n, adj);
        assert(inc[1].size() == 4); // 4 edges
        // Each column has exactly two 1s
        for (int col = 0; col < 4; ++col) {
            int ones = 0;
            for (int row = 1; row <= n; ++row) {
                ones += inc[row][col];
            }
            assert(ones == 2);
        }
        // Vertex 1 appears in every column
        for (int col = 0; col < 4; ++col) {
            assert(inc[1][col] == 1);
        }
    }

    // Test 5: Path of 4 vertices (1-2-3-4)
    {
        int n = 4;
        std::vector<std::vector<int>> adj(n+1, std::vector<int>(n+1, 0));
        adj[1][2] = adj[2][1] = 1;
        adj[2][3] = adj[3][2] = 1;
        adj[3][4] = adj[4][3] = 1;
        auto inc = adjacencyToIncidence(n, adj);
        assert(inc[1].size() == 3);
        // Expected columns order: (1,2), (2,3), (3,4)
        assert(inc[1][0] == 1 && inc[2][0] == 1);
        assert(inc[2][1] == 1 && inc[3][1] == 1);
        assert(inc[3][2] == 1 && inc[4][2] == 1);
    }

    return 0;
}
