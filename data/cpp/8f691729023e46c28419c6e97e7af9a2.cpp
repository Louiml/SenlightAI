/*
You are given an undirected graph with `n` vertices numbered from 1 to `n` and `m` edges. Write a C++ function `connectComponents` that takes the number of vertices `n` and a vector of pairs `edges` (where each pair `{a, b}` represents an undirected edge between vertex `a` and vertex `b`, with vertices numbered 1-based) and returns a vector of pairs `{{u1, v1}, {u2, v2}, ...}` such that adding the listed edges makes the graph connected. The function must output the minimum number of edges needed (which is the number of connected components minus 1), and then provide a specific set of edges to add — each edge connects one vertex from one component to a vertex from the next component in discovery order (the first vertex encountered in each component during depth-first traversal from 1 to n). Return the vector of added edges in that order; the function itself does not print anything. Assume the graph is non-empty (`n >= 1`), and if the graph is already connected, return an empty vector. Edge cases include isolated vertices (components of size 1), multiple components, and graphs with self-loops or duplicate edges (ignore duplicates; they don't affect connectivity). The solution must be efficient for up to `n = 10^5` and `m = 2*10^5`.
*/

#include <vector>
#include <functional>

// Find connected components and return a list of edges to add to connect the graph.
// n: number of vertices (1-indexed), edges: vector of undirected edges {a, b}.
// Returns a vector of pairs {u, v} representing edges to add, connecting components in traversal order.
std::vector<std::pair<int, int>> connectComponents(int n, const std::vector<std::pair<int, int>>& edges) {
    // Build adjacency list
    std::vector<std::vector<int>> graph(n + 1);
    for (const auto& edge : edges) {
        int a = edge.first;
        int b = edge.second;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    std::vector<bool> visited(n + 1, false);
    std::vector<int> component_representatives;

    // DFS to mark all vertices reachable from start
    std::function<void(int)> dfs = [&](int node) {
        visited[node] = true;
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                dfs(neighbor);
            }
        }
    };

    // Discover components
    for (int i = 1; i <= n; ++i) {
        if (!visited[i]) {
            component_representatives.push_back(i);
            dfs(i);
        }
    }

    // If only one component, no edges to add
    if (component_representatives.size() <= 1) {
        return {};
    }

    // Connect components in a chain
    std::vector<std::pair<int, int>> added_edges;
    for (size_t i = 1; i < component_representatives.size(); ++i) {
        added_edges.push_back({component_representatives[i - 1], component_representatives[i]});
    }
    return added_edges;
}

#include <cassert>
#include <vector>
#include <utility>

// Function declaration (matching the solution)
std::vector<std::pair<int, int>> connectComponents(int n, const std::vector<std::pair<int, int>>& edges);

int main() {
    // Test 1: Already connected graph (single component)
    std::vector<std::pair<int, int>> edges1 = {{1, 2}, {2, 3}};
    assert(connectComponents(3, edges1).empty());

    // Test 2: Two disconnected components
    std::vector<std::pair<int, int>> edges2 = {{1, 2}, {3, 4}};
    auto result2 = connectComponents(4, edges2);
    assert(result2.size() == 1);
    assert(result2[0].first == 1 && result2[0].second == 3);

    // Test 3: Isolated vertices (all separate)
    std::vector<std::pair<int, int>> edges3;
    auto result3 = connectComponents(3, edges3);
    assert(result3.size() == 2);
    assert(result3[0].first == 1 && result3[0].second == 2);
    assert(result3[1].first == 2 && result3[1].second == 3);

    // Test 4: Self-loop and duplicate edges don't affect result
    std::vector<std::pair<int, int>> edges4 = {{1, 1}, {2, 3}, {2, 3}};
    auto result4 = connectComponents(3, edges4);
    assert(result4.size() == 1);
    assert(result4[0].first == 1 && result4[0].second == 2);

    // Test 5: Large graph with two components (first component starts at 1, second at 4)
    std::vector<std::pair<int, int>> edges5 = {{1, 2}, {2, 3}, {4, 5}};
    auto result5 = connectComponents(5, edges5);
    assert(result5.size() == 1);
    assert(result5[0].first == 1 && result5[0].second == 4);

    // Test 6: Single vertex (already connected, empty result)
    std::vector<std::pair<int, int>> edges6;
    assert(connectComponents(1, edges6).empty());

    // Test 7: Multiple components with a component starting at vertex 2 (vertex 1 isolated)
    std::vector<std::pair<int, int>> edges7 = {{2, 3}, {4, 4}};
    auto result7 = connectComponents(4, edges7);
    assert(result7.size() == 2);
    assert(result7[0].first == 1 && result7[0].second == 2);
    assert(result7[1].first == 2 && result7[1].second == 4);

    // Test 8: Graph with three components, starting at 1, 3, and 5
    std::vector<std::pair<int, int>> edges8 = {{1, 2}, {3, 3}, {5, 6}};
    auto result8 = connectComponents(6, edges8);
    assert(result8.size() == 2);
    assert(result8[0].first == 1 && result8[0].second == 3);
    assert(result8[1].first == 3 && result8[1].second == 5);

    return 0;
}

// The problem reduces to finding all connected components of the undirected graph. Use depth-first search (DFS) or breadth-first search (BFS) to traverse each component. Maintain a visited array. Iterate through vertices 1 to n; whenever an unvisited vertex is found, it is the start of a new component. Record this start vertex as the "representative" of the new component. To connect all components with minimal edges, we only need to connect them in a chain: for each consecutive pair of components (in discovery order), add an edge between the representative of the previous component and the representative of the current component. Each added edge merges two components, and after adding `(components - 1)` edges, the whole graph becomes connected. This is optimal because any connected graph with `c` components requires at least `c-1` edges. The discovery order is naturally determined by the loop from 1 to n. Edge cases: if there is only one component, return an empty vector. Isolated vertices are simply components of size 1 and are handled normally. Duplicate edges and self-loops don't affect the DFS because they don't create new connectivity beyond what already exists; the visited array handles this naturally. Complexity: DFS visits each vertex and each edge once, so time is O(n + m). The storage for the adjacency list is O(n + m), and the visited array is O(n). The returned vector has at most n-1 elements, so O(n) space.
