// Write a C++ function `countValidColorings(int n, const std::vector<std::pair<int,int>>& edges)` that takes the number of vertices in an undirected graph and a list of undirected edges, and returns the number of ways to assign each vertex one of two colors (0 or 1) such that no two adjacent vertices share the same color. However, if a connected component of the graph has more than one vertex, then the component is not allowed to be monochromatic (i.e., there must be at least one edge whose endpoints differ). For a connected component with exactly one vertex, there are 3 colorings (both colors individually, plus a special "uncolored" state that counts as a valid configuration), while for a connected component that is bipartite and has at least two vertices, the number of valid colorings is 2 (the two possible proper 2-colorings). If any connected component of size ≥2 is not bipartite, the entire graph has 0 valid colorings. The final answer must be returned modulo 998244353. The graph may be disconnected; the total answer is the product over all connected components. Note that the input may contain self-loops or parallel edges, and the graph can have up to 300,000 vertices and arbitrary edges (bounded by memory constraints). The vertices are numbered from 1 to n, and the edge list may be empty.
#include <cassert>
#include <vector>
#include <utility>

// Assume the function is provided above.

int main() {
    // Test 1: single vertex, no edges -> 3
    {
        int n = 1;
        std::vector<std::pair<int,int>> edges;
        assert(countValidColorings(n, edges) == 3);
    }

    // Test 2: two vertices connected by edge -> 2 (proper 2-colorings)
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{1,2}};
        assert(countValidColorings(n, edges) == 2);
    }

    // Test 3: triangle (cycle of 3) -> not bipartite -> 0
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
        assert(countValidColorings(n, edges) == 0);
    }

    // Test 4: two isolated vertices -> 3*3 = 9
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges;
        assert(countValidColorings(n, edges) == 9);
    }

    // Test 5: isolated vertex + an edge between two others -> 3*2 = 6
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{2,3}};
        assert(countValidColorings(n, edges) == 6);
    }

    // Test 6: path of 4 vertices -> bipartite -> 2 (only one component of size 4)
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        assert(countValidColorings(n, edges) == 2);
    }

    // Test 7: self-loop on a vertex -> component not bipartite -> 0
    {
        int n = 1;
        std::vector<std::pair<int,int>> edges = {{1,1}};
        assert(countValidColorings(n, edges) == 0);
    }

    // Test 8: two components: one isolated, one bipartite with 3 vertices in a star -> 3*2 = 6
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3}};
        assert(countValidColorings(n, edges) == 6);
    }

    // Test 9: larger bipartite component with parallel edges -> still 2
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{1,2},{2,3}};
        assert(countValidColorings(n, edges) == 2);
    }

    // Test 10: graph with two separate edges (each a component of size 2) -> 2*2 = 4
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{3,4}};
        assert(countValidColorings(n, edges) == 4);
    }

    return 0;
}
#include <vector>
#include <queue>
#include <utility>

// Counts the number of valid 2-colorings with special handling for isolated vertices.
// Returns the count modulo 998244353.
long long countValidColorings(int n, const std::vector<std::pair<int,int>>& edges) {
    const long long MOD = 998244353;
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    std::vector<int> color(n + 1, -1); // -1 = uncolored, 0/1 = colors
    std::vector<bool> visited(n + 1, false);

    long long answer = 1;

    for (int start = 1; start <= n; ++start) {
        if (visited[start]) continue;

        // BFS to explore component and attempt bipartite coloring
        std::queue<int> q;
        visited[start] = true;
        color[start] = 0;
        q.push(start);
        int component_size = 1;
        bool is_bipartite = true;

        while (!q.empty() && is_bipartite) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    color[v] = 1 - color[u];
                    ++component_size;
                    q.push(v);
                } else if (color[v] == color[u]) {
                    is_bipartite = false;
                    break;
                }
            }
        }

        if (!is_bipartite) {
            // Component with size >=2 is not bipartite, whole graph invalid
            return 0;
        }

        if (component_size == 1) {
            // Isolated vertex: 3 valid configurations
            answer = (answer * 3) % MOD;
        } else {
            // Bipartite component with >=2 vertices: exactly 2 valid colorings
            answer = (answer * 2) % MOD;
        }
    }

    return answer;
}
// We need to count the number of global assignments of colors (0/1) to vertices such that every edge connects opposite colors, but with an additional twist: a connected component of size 1 has 3 valid configurations (we treat that single vertex as either color 0, color 1, or an "uncolored" state that does not violate any edge because there are none). For a connected component of size ≥2, any proper 2-coloring must be accounted for. Since the graph may be disconnected, we process each connected component independently. For each component, we run a BFS/DFS to try to 2-color it. If we encounter a conflict (an edge connecting same-colored vertices), the whole component contributes 0. If the component has size 1, it contributes 3. Otherwise, if it's bipartite, there are exactly 2 valid 2-colorings (swap the colors). The final answer is the product of contributions from each component, modulo 998244353. Important edge cases: self-loops automatically make the component non-bipartite; parallel edges don't change bipartiteness. The graph may have isolated vertices (size 1 components) which contribute 3 each. The algorithm runs BFS from each unvisited vertex, maintaining a color array initialized to -1, and a visited array. Time complexity is O(n + m) where m is the number of edges, and space O(n + m) for the adjacency list and arrays. Since n can be up to 300,000, we must be careful with recursion depth, so we use an iterative BFS with a queue. The modulo is prime, but we only need multiplication and addition of precomputed powers of 2 (though we don't actually need powers because contributions are small: 3 or 2). We simply compute the product of 3s and 2s as needed. However, the original snippet computes pow2s for something else, but here we only need the simple product. We'll just multiply ans by 3 or 2 accordingly. For n=0? The problem defines vertices numbered 1..n, so n≥1. If n=1 and no edges, the single component contributes 3.
