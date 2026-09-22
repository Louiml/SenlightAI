Write a C++ function that takes an integer `n` representing the number of vertices (numbered 1 to n) and a vector of undirected edges, and determines whether the graph is bipartite. If it is not bipartite, return an empty vector of vectors. If it is bipartite, return a vector containing two vectors: the first with vertices assigned to color 1 and the second with vertices assigned to color 2, in ascending order of vertex number. The function should handle disconnected graphs by processing each connected component separately, and must correctly handle self-loops and multi-edges (both should cause a non-bipartite result if they create an odd cycle, but a self-loop always makes it non-bipartite). The output format should be: if non-bipartite, return `{}` (empty outer vector); if bipartite, return `{{vertices color 1}, {vertices color 2}}` where each inner vector is sorted ascending.

// The solution uses a breadth-first search (BFS) or depth-first search (DFS) to attempt a two-coloring of the graph. Start by initializing a color array where 0 means unvisited, 1 means color 1, and 2 means color 2. For each unvisited vertex, start a BFS from it, assigning it color 1, then traverse all neighbors: if a neighbor is unvisited, assign it the opposite color and push it to the queue; if it is already visited, check that its color is opposite to the current vertex's color. If any conflict is found (same color as current), the graph is not bipartite and we return `{}`. After processing all components, collect vertices with color 1 and color 2, sort them, and return `{{color1}, {color2}}`. Self-loops and parallel edges are naturally handled: a self-loop will cause a conflict because the vertex's neighbor is itself with the same color, and parallel edges are just redundant checks. Time complexity is O(n + m) where n is vertices and m is edges, and space complexity is O(n + m) for the adjacency list and color array.

#include <vector>
#include <queue>
#include <algorithm>

// Determines if a graph is bipartite and returns a coloring if possible.
// Parameters:
//   n - number of vertices (labels 1..n)
//   edges - vector of {u, v} pairs representing undirected edges
// Returns:
//   {} if graph is not bipartite,
//   {{color1 vertices}, {color2 vertices}} if bipartite, with each vector sorted ascending.
std::vector<std::vector<int>> bipartiteColoring(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        // Self-loop immediately makes it non-bipartite
        if (u == v) {
            return {};
        }
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 0 = unvisited, 1 = color 1, 2 = color 2
    std::vector<int> color(n + 1, 0);

    // Process each connected component
    for (int start = 1; start <= n; ++start) {
        if (color[start] != 0) continue;

        // BFS from start
        std::queue<int> q;
        q.push(start);
        color[start] = 1;

        while (!q.empty()) {
            int curr = q.front();
            q.pop();

            for (int neighbor : adj[curr]) {
                if (color[neighbor] == 0) {
                    color[neighbor] = (color[curr] == 1) ? 2 : 1;
                    q.push(neighbor);
                } else if (color[neighbor] == color[curr]) {
                    // Conflict — same color as current
                    return {};
                }
            }
        }
    }

    // Collect vertices by color
    std::vector<int> color1, color2;
    for (int v = 1; v <= n; ++v) {
        if (color[v] == 1) color1.push_back(v);
        else if (color[v] == 2) color2.push_back(v);
    }

    // They are already collected in ascending order due to loop order
    return {color1, color2};
}

#include <cassert>
#include <vector>

// The solution function is declared above (not shown here for brevity in test)

int main() {
    // Test 1: Simple bipartite chain 1-2-3
    assert(bipartiteColoring(3, {{1,2},{2,3}}) == std::vector<std::vector<int>>{{1,3},{2}});

    // Test 2: Simple non-bipartite triangle
    assert(bipartiteColoring(3, {{1,2},{2,3},{1,3}}) == std::vector<std::vector<int>>{});

    // Test 3: Disconnected graph with one bipartite component and one isolated vertex
    assert(bipartiteColoring(3, {{1,2}}) == std::vector<std::vector<int>>{{1,3},{2}});

    // Test 4: Self-loop causes non-bipartite
    assert(bipartiteColoring(2, {{1,1}}) == std::vector<std::vector<int>>{});

    // Test 5: Two vertices connected (bipartite)
    assert(bipartiteColoring(2, {{1,2}}) == std::vector<std::vector<int>>{{1},{2}});

    // Test 6: Empty graph with 3 vertices (all isolated)
    assert(bipartiteColoring(3, {}) == std::vector<std::vector<int>>{{1,2,3},{}});

    // Test 7: Parallel edges between same two vertices (still bipartite)
    assert(bipartiteColoring(2, {{1,2},{1,2}}) == std::vector<std::vector<int>>{{1},{2}});

    // Test 8: Larger bipartite even cycle 1-2-3-4-1
    assert(bipartiteColoring(4, {{1,2},{2,3},{3,4},{4,1}}) == std::vector<std::vector<int>>{{1,3},{2,4}});

    // Test 9: Odd cycle with extra attached vertex, non-bipartite
    assert(bipartiteColoring(5, {{1,2},{2,3},{3,1},{3,4},{4,5}}) == std::vector<std::vector<int>>{});

    // Test 10: Complete bipartite graph K_{2,2}
    assert(bipartiteColoring(4, {{1,3},{1,4},{2,3},{2,4}}) == std::vector<std::vector<int>>{{1,2},{3,4}});

    return 0;
}
