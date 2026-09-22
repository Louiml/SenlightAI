Given an undirected graph with `n` vertices (numbered 1 to `n`) and `m` edges, write a C++ function that computes all connected components of the graph. The function should take the number of vertices `n`, the number of edges `m`, and a vector of edge pairs (each pair `(a, b)` representing an edge between vertices `a` and `b`, 1-indexed) as input. It should return a vector of vectors of integers, where each inner vector contains the vertices (in ascending order) of one connected component, and the outer vector contains all components in any order. The function must handle graphs with zero edges, isolated vertices, and disconnected graphs. The solution must use a depth-first search (DFS) approach with an adjacency list representation.

#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above; here we test it.
std::vector<std::vector<int>> findConnectedComponents(int n, int m, const std::vector<std::pair<int, int>>& edges);

int main() {
    // Test 1: Empty graph with 3 isolated vertices
    {
        std::vector<std::pair<int, int>> edges;
        auto comps = findConnectedComponents(3, 0, edges);
        assert(comps.size() == 3);
        assert(comps[0] == std::vector<int>{1});
        assert(comps[1] == std::vector<int>{2});
        assert(comps[2] == std::vector<int>{3});
    }
    // Test 2: Simple connected graph
    {
        std::vector<std::pair<int, int>> edges = {{1,2}, {2,3}, {3,1}};
        auto comps = findConnectedComponents(3, 3, edges);
        assert(comps.size() == 1);
        assert(comps[0] == std::vector<int>({1,2,3}));
    }
    // Test 3: Two components
    {
        std::vector<std::pair<int, int>> edges = {{1,2}, {3,4}, {4,5}};
        auto comps = findConnectedComponents(5, 3, edges);
        assert(comps.size() == 2);
        // Components can be in any order; check sorted order manually
        std::vector<std::vector<int>> expected = {{1,2}, {3,4,5}};
        std::sort(comps.begin(), comps.end());
        std::sort(expected.begin(), expected.end());
        assert(comps == expected);
    }
    // Test 4: Graph with self-loop and parallel edges
    {
        std::vector<std::pair<int, int>> edges = {{1,1}, {1,2}, {2,2}, {1,2}};
        auto comps = findConnectedComponents(3, 4, edges);
        assert(comps.size() == 2);
        std::vector<std::vector<int>> expected = {{1,2}, {3}};
        std::sort(comps.begin(), comps.end());
        std::sort(expected.begin(), expected.end());
        assert(comps == expected);
    }
    // Test 5: Single vertex, no edges
    {
        std::vector<std::pair<int, int>> edges;
        auto comps = findConnectedComponents(1, 0, edges);
        assert(comps.size() == 1);
        assert(comps[0] == std::vector<int>{1});
    }
    // Test 6: Large path graph, ensure no crash and correct count
    {
        int n = 1000;
        std::vector<std::pair<int, int>> edges;
        for (int i = 1; i < n; ++i) edges.emplace_back(i, i+1);
        auto comps = findConnectedComponents(n, n-1, edges);
        assert(comps.size() == 1);
        assert(comps[0].size() == (size_t)n);
    }
    return 0;
}

#include <vector>
#include <algorithm>

// Compute connected components of an undirected graph.
// edges: vector of pairs (a, b) where 1 <= a, b <= n.
// Returns a vector of components, each component is a sorted vector<int> of vertices (1-indexed).
std::vector<std::vector<int>> findConnectedComponents(int n, int m, const std::vector<std::pair<int, int>>& edges) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1); // 1-indexed
    for (const auto& e : edges) {
        int a = e.first;
        int b = e.second;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    // Remove duplicate edges to avoid redundant work (optional, but safe)
    for (int i = 1; i <= n; ++i) {
        std::sort(adj[i].begin(), adj[i].end());
        adj[i].erase(std::unique(adj[i].begin(), adj[i].end()), adj[i].end());
    }

    std::vector<int> visited(n + 1, 0);
    std::vector<std::vector<int>> components;

    // Iterative DFS to avoid recursion depth issues (optional, but robust)
    for (int v = 1; v <= n; ++v) {
        if (visited[v]) continue;
        std::vector<int> comp;
        std::vector<int> stack = {v};
        visited[v] = 1;
        while (!stack.empty()) {
            int cur = stack.back();
            stack.pop_back();
            comp.push_back(cur);
            for (int nb : adj[cur]) {
                if (!visited[nb]) {
                    visited[nb] = 1;
                    stack.push_back(nb);
                }
            }
        }
        std::sort(comp.begin(), comp.end());
        components.push_back(std::move(comp));
    }
    return components;
}

// The algorithm performs a DFS from each unvisited vertex. Starting at an unvisited vertex `v`, the DFS traverses all reachable vertices and collects them into a set that is later converted to a sorted list. Since the graph may contain self-loops or parallel edges, using a `set` for the adjacency list naturally handles duplicates, but for efficiency we can use `vector<int>` and manually ensure each vertex is collected exactly once via the `visited` array. For each unvisited starting vertex, we run DFS, collect all visited vertices into a vector, sort it, and push it to the result. Important edge cases: isolated vertices (m=0) each form their own component, and components with a single vertex. Time complexity is O(n + m) because each vertex and each edge is visited once, with an additional O(n log n) sorting per component (total O(n log n) in worst case if one component contains all vertices). Space complexity is O(n + m) for the adjacency list and visited array, plus O(n) for the recursion stack in the worst case (e.g., a path graph).
