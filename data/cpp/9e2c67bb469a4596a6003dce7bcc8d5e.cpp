// Write a C++ function `findHamiltonianPaths` that takes an undirected graph represented as an adjacency matrix (a square `std::vector<std::vector<int>>` where `1` means an edge exists and `0` means no edge, and diagonal entries are always `0`) and a starting vertex index (1-based, within the valid range of vertex labels). The function must return a `std::vector<std::string>` containing all Hamiltonian paths (simple paths that visit every vertex exactly once) that start at the given vertex. Each path should be represented as a string of vertex labels concatenated without separators (e.g., for vertices 1,2,3, a path would be "123"). The vertices are labeled from 1 to `n`, where `n = adjacencyMatrix.size() - 1` (since the matrix includes an extra row/column at index 0 that is unused by any vertex, but the rows/columns from 1 to n correspond to real vertices). The function must explore all possible orders, and the returned vector can be in any order (but for testing, we'll sort the results or compare sets). The graph may be disconnected, and if no Hamiltonian path exists starting from the given vertex, return an empty vector. Implement the function recursively using backtracking, marking vertices as visited in the current path.

// The problem is a classic Hamiltonian path enumeration on a small graph. We use depth-first search with backtracking: start at the given vertex, mark it visited, and at each step try all unvisited neighbors (based on adjacency matrix). When the path length equals the number of vertices `n`, we have found a complete path and record it as a string. On returning, we unmark the vertex and remove it from the current path. The adjacency matrix is passed by const reference to avoid copying. The key edge cases: (1) the graph might have only one vertex (n=1), then the path is just the starting vertex; (2) the graph may be disconnected so some vertices unreachable — the recursion naturally handles it; (3) duplicate edges are not present since matrix has 0/1. Time complexity: in the worst case, we explore every permutation of vertices, which is O(n!) for n vertices, but for n ≤ 5 (like in the snippet) it's trivial. Space complexity: O(n) for recursion stack and current path, plus O(#paths × n) for the output vector of strings.

#include <vector>
#include <string>

// Returns all Hamiltonian paths starting at 'start' in the undirected graph
// described by adjacency matrix 'adj' (1-based vertex labels, index 0 unused).
std::vector<std::string> findHamiltonianPaths(
    const std::vector<std::vector<int>>& adj,
    int start)
{
    const int n = static_cast<int>(adj.size()) - 1; // number of real vertices
    std::vector<std::string> result;
    std::vector<int> current;
    std::vector<bool> visited(n + 1, false);

    // Depth-first backtracking helper.
    // current.size() == number of vertices in the current path.
    // v is the last vertex in the current path.
    auto dfs = [&](auto&& self, int v) -> void {
        current.push_back(v);
        visited[v] = true;

        if (static_cast<int>(current.size()) == n) {
            // Complete path found: convert to string without separators.
            std::string path;
            for (int vertex : current) {
                path += std::to_string(vertex);
            }
            result.push_back(path);
        } else {
            // Try all unvisited neighbors.
            for (int u = 1; u <= n; ++u) {
                if (adj[v][u] == 1 && !visited[u]) {
                    self(self, u);
                }
            }
        }

        // Backtrack.
        current.pop_back();
        visited[v] = false;
    };

    dfs(dfs, start);
    return result;
}

#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

// The solution function from above is assumed to be declared here.
// (In a full test, include the header or paste the function above.)

int main() {
    // Example from the snippet: 5 vertices with edges as given.
    std::vector<std::vector<int>> adj(6, std::vector<int>(6, 0));
    adj[1][2] = adj[1][3] = adj[1][5] = 1;
    adj[2][3] = adj[2][5] = 1;
    adj[3][4] = adj[3][5] = 1;
    adj[4][5] = 1;
    // Ensure symmetry (undirected).
    for (int i = 1; i <= 5; ++i)
        for (int j = i+1; j <= 5; ++j)
            adj[j][i] = adj[i][j];

    auto paths1 = findHamiltonianPaths(adj, 1);
    assert(!paths1.empty());
    // Every path should have length 5 (5 vertices).
    for (const auto& p : paths1) assert(p.size() == 5);
    // Count expected? For this graph, there are exactly 4 Hamiltonian paths starting at 1.
    assert(paths1.size() == 4);
    // Check that all paths are distinct and sorted to test deterministic content.
    std::sort(paths1.begin(), paths1.end());
    assert(paths1[0] == "12354");
    assert(paths1[1] == "12534");
    assert(paths1[2] == "13254");
    assert(paths1[3] == "13524");

    // Test single vertex.
    std::vector<std::vector<int>> adj1(2, std::vector<int>(2, 0));
    auto paths2 = findHamiltonianPaths(adj1, 1);
    assert(paths2.size() == 1);
    assert(paths2[0] == "1");

    // Test disconnected graph: no Hamiltonian path from 1 to reach all vertices.
    std::vector<std::vector<int>> adj3(4, std::vector<int>(4, 0));
    adj3[1][2] = adj3[2][1] = 1; // component {1,2} and {3} isolated
    auto paths3 = findHamiltonianPaths(adj3, 1);
    assert(paths3.empty());

    // Test a complete graph on 4 vertices.
    std::vector<std::vector<int>> adj4(5, std::vector<int>(5, 0));
    for (int i = 1; i <= 4; ++i)
        for (int j = 1; j <= 4; ++j)
            if (i != j) adj4[i][j] = 1;
    auto paths4 = findHamiltonianPaths(adj4, 2);
    assert(paths4.size() == 6); // (4-1)! = 6 paths starting at 2
    for (const auto& p : paths4) {
        assert(p.size() == 4);
        assert(p.find('2') == 0); // starting vertex 2 first
    }

    return 0;
}
