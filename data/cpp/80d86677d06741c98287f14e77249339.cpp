Write a C++ function `std::string coloringResult(int N, const std::vector<std::pair<int,int>>& edges, const std::vector<int>& colors)` that determines whether a given vertex coloring of an undirected graph is a proper coloring (i.e., no two adjacent vertices share the same color). The graph has `N` vertices labeled `0` to `N-1`, and `edges` is a list of undirected edges (pairs of vertex labels). `colors` is a vector of size `N` where `colors[i]` is the color assigned to vertex `i`. The function must return the string `"K-coloring"` (where `K` is the number of distinct colors used) if the coloring is proper, or the string `"No"` if it is not. Assume the graph may contain self-loops (an edge from a vertex to itself) which automatically make the coloring improper, multiple edges between the same pair of vertices (which should be handled correctly), and that `N >= 1`. The input is guaranteed to be valid such that all edge endpoints are within `[0, N-1]`.
// The core algorithm is to group vertices by their assigned color, then for each color group, check whether any pair of vertices within that group are connected by an edge. If any such edge exists, the coloring is invalid. To do this efficiently, we store the adjacency information as a boolean matrix or unordered_set of pairs; since the graph size and edge count are not given constraints, using an adjacency matrix of size `N*N` (or an `unordered_set` of encoded edge keys) is acceptable for moderate `N`. We then iterate over all color groups: for each group, iterate over all pairs of vertices in that group (using nested loops over indices within the group), and check if `adj[u][v]` is true. If found, return `"No"` immediately. If no conflicts occur after processing all groups, the number of distinct colors is determined from the color values (e.g., using an `unordered_set<int>`), and return `to_string(K) + "-coloring"`. Edge cases: self-loop (u == v) is handled because `adj[u][u]` would be set to true, and any vertex with itself is in the same color group, so a pair (u, u) will be checked and found as conflict. Multiple edges are fine because the adjacency matrix just marks existence. Time complexity: O(N^2 + M + total pairs in color groups) which in worst case is O(N^2) due to nested pair checks; space complexity O(N^2) for the matrix.
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

// Returns "K-coloring" if the given vertex coloring is proper, otherwise "No".
// N: number of vertices (0..N-1), edges: list of undirected edges (vertex pairs),
// colors: vector of length N with the color for each vertex.
std::string coloringResult(int N, const std::vector<std::pair<int,int>>& edges, const std::vector<int>& colors) {
    // Build adjacency matrix
    std::vector<std::vector<bool>> adj(N, std::vector<bool>(N, false));
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        adj[u][v] = true;
        adj[v][u] = true;
    }

    // Group vertices by color
    std::unordered_map<int, std::vector<int>> colorGroups;
    for (int i = 0; i < N; ++i) {
        colorGroups[colors[i]].push_back(i);
    }

    // Check each color group for internal edges
    for (const auto& group : colorGroups) {
        const std::vector<int>& vertices = group.second;
        for (size_t i = 0; i < vertices.size(); ++i) {
            for (size_t j = i; j < vertices.size(); ++j) { // j starts at i to catch self-loops
                int u = vertices[i];
                int v = vertices[j];
                if (adj[u][v]) {
                    return "No";
                }
            }
        }
    }

    // Count distinct colors
    std::unordered_set<int> distinctColors(colors.begin(), colors.end());
    return std::to_string(distinctColors.size()) + "-coloring";
}
#include <cassert>
#include <string>
#include <vector>

// The solution function is declared above. This main function runs tests.

int main() {
    // Test 1: Simple proper coloring with 2 colors
    std::vector<std::pair<int,int>> edges1 = {{0,1}};
    std::vector<int> colors1 = {0, 1};
    assert(coloringResult(2, edges1, colors1) == "2-coloring");

    // Test 2: Proper coloring with 3 colors on a triangle
    std::vector<std::pair<int,int>> edges2 = {{0,1},{1,2},{0,2}};
    std::vector<int> colors2 = {0, 1, 2};
    assert(coloringResult(3, edges2, colors2) == "3-coloring");

    // Test 3: Improper coloring (adjacent vertices share color)
    std::vector<std::pair<int,int>> edges3 = {{0,1}};
    std::vector<int> colors3 = {0, 0};
    assert(coloringResult(2, edges3, colors3) == "No");

    // Test 4: Self-loop makes coloring improper even with one vertex
    std::vector<std::pair<int,int>> edges4 = {{0,0}};
    std::vector<int> colors4 = {5};
    assert(coloringResult(1, edges4, colors4) == "No");

    // Test 5: Multiple edges between same pair, proper coloring
    std::vector<std::pair<int,int>> edges5 = {{0,1},{1,0}};
    std::vector<int> colors5 = {10, 20};
    assert(coloringResult(2, edges5, colors5) == "2-coloring");

    // Test 6: Graph with no edges, single color used
    std::vector<std::pair<int,int>> edges6 = {};
    std::vector<int> colors6 = {7, 7, 7};
    assert(coloringResult(3, edges6, colors6) == "1-coloring");

    // Test 7: Larger graph with 4-color proper coloring
    std::vector<std::pair<int,int>> edges7 = {{0,1},{1,2},{2,3},{3,0}};
    std::vector<int> colors7 = {0, 1, 2, 3};
    assert(coloringResult(4, edges7, colors7) == "4-coloring");

    // Test 8: Improper with two conflicting pairs
    std::vector<std::pair<int,int>> edges8 = {{0,1},{2,3}};
    std::vector<int> colors8 = {0, 0, 1, 1};
    assert(coloringResult(4, edges8, colors8) == "No");

    // Test 9: Single isolated vertex with any color
    std::vector<std::pair<int,int>> edges9 = {};
    std::vector<int> colors9 = {42};
    assert(coloringResult(1, edges9, colors9) == "1-coloring");

    // Test 10: Mixed graph, proper with 2 colors
    std::vector<std::pair<int,int>> edges10 = {{0,1},{1,2},{2,0},{3,4}};
    std::vector<int> colors10 = {0, 1, 0, 1, 0};
    assert(coloringResult(5, edges10, colors10) == "2-coloring");

    return 0;
}
