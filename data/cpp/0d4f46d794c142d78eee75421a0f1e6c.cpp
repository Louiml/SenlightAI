Given an undirected graph with \(N\) vertices (numbered \(1\) to \(N\)) and \(M\) edges, write a C++ function `solveGraphColoringWays` that returns the number of valid assignments of one of two colors to every vertex such that no two adjacent vertices share the same color, modulo \(998244353\). If the graph is not bipartite (i.e., no valid 2-coloring exists), return \(0\). The graph may contain isolated vertices, multiple edges between the same pair, and self-loops? (Self-loops make it impossible, so handle them.) The graph may be disconnected. The function should accept the number of vertices and a vector of pairs of edges (1-indexed) and return a long long.

#include <cassert>

int main() {
    // Example 1: Simple bipartite graph (a triangle is not bipartite)
    assert(solveGraphColoringWays(3, {{1,2}, {2,3}, {3,1}}) == 0);

    // Example 2: Single edge
    assert(solveGraphColoringWays(2, {{1,2}}) == 2);

    // Example 3: Two isolated vertices (two components, each with 2 colorings => 4)
    assert(solveGraphColoringWays(2, {}) == 4);

    // Example 4: Self-loop
    assert(solveGraphColoringWays(1, {{1,1}}) == 0);

    // Example 5: Path of length 2 (3 vertices) -> bipartite, one component -> 2
    assert(solveGraphColoringWays(3, {{1,2}, {2,3}}) == 2);

    // Example 6: Disconnected: one edge and one isolated vertex -> 2*2=4
    assert(solveGraphColoringWays(3, {{1,2}}) == 4);

    // Example 7: 4-cycle (bipartite) -> 2
    assert(solveGraphColoringWays(4, {{1,2}, {2,3}, {3,4}, {4,1}}) == 2);

    // Example 8: Empty graph with 1 vertex -> 2
    assert(solveGraphColoringWays(1, {}) == 2);

    // Example 9: Multiple edges between same vertices still bipartite
    assert(solveGraphColoringWays(2, {{1,2}, {1,2}}) == 2);

    // Example 10: Larger component: a star with 5 leaves (bipartite) -> 2
    std::vector<std::pair<int,int>> star;
    for (int i = 2; i <= 6; ++i) star.push_back({1, i});
    assert(solveGraphColoringWays(6, star) == 2);
}

#include <vector>
#include <functional>

const long long MOD = 998244353;

// Count valid 2-colorings of an undirected graph modulo MOD.
// If graph is not bipartite, return 0.
// vertices: total number of vertices (1-indexed)
// edges: vector of pairs (u, v) with 1 <= u, v <= vertices
long long solveGraphColoringWays(int vertices, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> adj(vertices + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    std::vector<int> color(vertices + 1, -1); // -1 = uncolored, 0/1 = color
    bool isBipartite = true;
    int components = 0;

    for (int v = 1; v <= vertices; ++v) {
        if (color[v] != -1) continue;
        ++components;
        color[v] = 0;
        std::function<void(int)> dfs = [&](int u) {
            for (int to : adj[u]) {
                if (color[to] == -1) {
                    color[to] = color[u] ^ 1;
                    dfs(to);
                } else if (color[to] == color[u]) {
                    isBipartite = false;
                }
            }
        };
        dfs(v);
    }

    if (!isBipartite) return 0;

    // Each bipartite component has exactly 2 valid colorings.
    long long answer = 1;
    for (int i = 0; i < components; ++i) {
        answer = (answer * 2) % MOD;
    }
    return answer;
}

// The problem reduces to determining whether each connected component is bipartite and, if so, counting the number of proper 2-colorings for that component. A graph is bipartite iff it can be colored with two colors without conflict. For a connected bipartite component with \(A\) vertices in one partition and \(B\) in the other, there are exactly \(2\) valid colorings: one assigning color 0 to the first partition and color 1 to the second, and the other swapping the colors. Thus the total number of valid colorings for the whole graph is the product over components of \(2\) (since each component can be colored independently). But note: if the component has only one vertex, there are also 2 colorings (that vertex can be color 0 or 1). So for each connected component that is bipartite, multiply the answer by 2. If any component is not bipartite (including self-loops or odd cycles), overall answer is 0. Implementation: use DFS or BFS to assign a "depth parity" to each vertex. Start from an unvisited vertex with depth 0, assign parity as depth mod 2. When visiting a neighbor not yet visited, recurse; when encountering an already visited neighbor, check that its parity differs from the current vertex's parity; if same, return failure. Since depth is just parity, we can store a color array (0 or 1) instead of full depth. Process all components, multiply answer by 2 per component, modulo 998244353. Time complexity: \(O(N+M)\) to traverse graph. Space complexity: \(O(N+M)\) for adjacency list and color array. Edge cases: empty graph (N=0) should return 1? But typical N≥1; self-loop means an edge from a vertex to itself creates an odd cycle of length 1 → not bipartite → answer 0. Multiple edges are harmless. Isolated vertices are their own component → count 2.
