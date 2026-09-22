/*
Write a C++ function `bool isEulerian(const vector<vector<int>>& graph)` that takes an adjacency matrix of an undirected simple graph (with no self-loops, meaning diagonal entries are always 0) and returns `true` if the graph is Eulerian (i.e., all vertices have even degree), and `false` otherwise. The graph is represented as a square matrix of 0s and 1s, where `graph[i][j] == 1` indicates an edge between vertex `i` and vertex `j`. The function must handle the case of a graph with zero vertices (return `true` vacuously) and must not modify the input matrix. The function should compute the degree of each vertex by counting the number of 1s in its corresponding row and check that every degree is even. Time complexity should be O(V^2), where V is the number of vertices, and auxiliary space O(1).
*/
#include <vector>

// Returns true if every vertex in the undirected graph has even degree.
bool isEulerian(const std::vector<std::vector<int>>& graph) {
    if (graph.empty()) return true;
    const int v = static_cast<int>(graph.size());
    for (int i = 0; i < v; ++i) {
        int degree = 0;
        // Count edges incident to vertex i by summing row i.
        for (int j = 0; j < v; ++j) {
            degree += graph[i][j];
        }
        if (degree % 2 != 0) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>

// Declare the solution function (or include the header)
bool isEulerian(const std::vector<std::vector<int>>& graph);

int main() {
    // Empty graph: trivially Eulerian
    std::vector<std::vector<int>> g0;
    assert(isEulerian(g0) == true);

    // 1 vertex, no edges (degree 0 is even)
    std::vector<std::vector<int>> g1 = {{0}};
    assert(isEulerian(g1) == true);

    // 2 vertices connected by one edge: degrees 1 and 1 (odd) -> not Eulerian
    std::vector<std::vector<int>> g2 = {{0, 1}, {1, 0}};
    assert(isEulerian(g2) == false);

    // 3 vertices forming a triangle: each degree 2 (even) -> Eulerian
    std::vector<std::vector<int>> g3 = {{0, 1, 1}, {1, 0, 1}, {1, 1, 0}};
    assert(isEulerian(g3) == true);

    // 4 vertices path: degrees 1,2,2,1 -> not Eulerian
    std::vector<std::vector<int>> g4 = {{0,1,0,0},{1,0,1,0},{0,1,0,1},{0,0,1,0}};
    assert(isEulerian(g4) == false);

    // 4 vertices cycle: all degrees 2 -> Eulerian
    std::vector<std::vector<int>> g5 = {{0,1,0,1},{1,0,1,0},{0,1,0,1},{1,0,1,0}};
    assert(isEulerian(g5) == true);

    // 5 vertices with isolated vertex and a triangle: degrees 2,2,2,0 -> all even
    std::vector<std::vector<int>> g6 = {{0,1,1,0,0},{1,0,1,0,0},{1,1,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
    assert(isEulerian(g6) == true);

    // 4 vertices with a single vertex having degree 3 -> not Eulerian
    std::vector<std::vector<int>> g7 = {{0,1,0,0},{1,0,1,1},{0,1,0,0},{0,1,0,0}};
    assert(isEulerian(g7) == false);

    return 0;
}
// The solution iterates over each vertex (row) of the adjacency matrix and counts the number of edges incident to it by summing the row entries. Since the graph is undirected and given as a symmetric matrix, counting either the row or column gives the degree. For each vertex, if the count is odd, the graph is not Eulerian, so return `false` immediately. If the loop completes without finding an odd degree, return `true`. Edge cases include an empty graph (0 vertices) which trivially satisfies the condition, and a graph with isolated vertices (all zeros in a row) where degree 0 is even. Self-loops are not considered because the problem statement specifies the diagonal is 0. The main algorithm runs in O(V^2) time because each of the V rows is scanned fully; space usage is O(1) aside from the input itself. The function correctly uses `const` reference to avoid copying and to guarantee no mutation.
