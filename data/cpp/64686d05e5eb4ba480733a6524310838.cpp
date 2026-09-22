/*
Write a C++ function named `detectSuspiciousBugs` that takes the number of bugs `n`, the number of interactions `m`, and a vector of interaction pairs (each pair represented as a `std::pair<int,int>` with 1-based bug indices) and returns a boolean indicating whether any suspicious (odd-cycle) situation exists. The function must perform a graph bipartiteness check: the bugs are vertices, and each interaction is an undirected edge. The function should return `true` if the graph is bipartite (no suspicious bugs found) and `false` otherwise. The graph may be disconnected, and you must handle all components. Assume `1 ≤ n ≤ 20000` and `0 ≤ m ≤ 50000`. The input may contain duplicate edges, but they should not affect the result. Your implementation should be efficient for these constraints.
*/

#include <vector>
#include <queue>
#include <utility>

// Check if the graph of bugs and interactions is bipartite.
// Returns true if no suspicious bugs (bipartite), false otherwise.
bool detectSuspiciousBugs(int n, int m, const std::vector<std::pair<int,int>>& interactions) {
    // Build adjacency list with 1-based indexing.
    std::vector<std::vector<int>> graph(n + 1);
    for (const auto& p : interactions) {
        int u = p.first;
        int v = p.second;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    std::vector<int> color(n + 1, -1); // -1 = uncolored, 0 and 1 are colors.
    std::vector<bool> visited(n + 1, false);

    // Process each connected component.
    for (int start = 1; start <= n; ++start) {
        if (visited[start]) continue;

        // BFS from start.
        std::queue<int> q;
        q.push(start);
        color[start] = 0;
        visited[start] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : graph[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    color[v] = color[u] ^ 1; // alternate color.
                    q.push(v);
                } else {
                    // If same color on an edge, not bipartite.
                    if (color[u] == color[v]) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // No interactions: bipartite.
    assert(detectSuspiciousBugs(5, 0, {}) == true);

    // Simple bipartite graph: two edges forming a path of length 2.
    std::vector<std::pair<int,int>> edges1 = {{1,2},{2,3}};
    assert(detectSuspiciousBugs(3, 2, edges1) == true);

    // Triangle (odd cycle): not bipartite.
    std::vector<std::pair<int,int>> edges2 = {{1,2},{2,3},{3,1}};
    assert(detectSuspiciousBugs(3, 3, edges2) == false);

    // Disconnected components: one bipartite, one bipartite → true.
    std::vector<std::pair<int,int>> edges3 = {{1,2},{3,4}};
    assert(detectSuspiciousBugs(4, 2, edges3) == true);

    // Disconnected: one component is a triangle → false.
    std::vector<std::pair<int,int>> edges4 = {{1,2},{2,3},{3,1},{4,5}};
    assert(detectSuspiciousBugs(5, 4, edges4) == false);

    // Duplicate edges should not affect result (single edge duplicated).
    std::vector<std::pair<int,int>> edges5 = {{1,2},{1,2},{2,3}};
    assert(detectSuspiciousBugs(3, 3, edges5) == true);

    // Self-loop: always not bipartite.
    std::vector<std::pair<int,int>> edges6 = {{1,1}};
    assert(detectSuspiciousBugs(2, 1, edges6) == false);

    // Larger bipartite graph (even cycle).
    std::vector<std::pair<int,int>> edges7 = {{1,2},{2,3},{3,4},{4,1}};
    assert(detectSuspiciousBugs(4, 4, edges7) == true);

    // Larger non-bipartite graph (odd cycle with extra vertex).
    std::vector<std::pair<int,int>> edges8 = {{1,2},{2,3},{3,1},{3,4}};
    assert(detectSuspiciousBugs(4, 4, edges8) == false);

    return 0;
}

// The problem reduces to checking whether an undirected graph is bipartite. A graph is bipartite if and only if it contains no odd-length cycle. We use graph coloring via DFS or BFS: assign alternating colors (0 and 1) to vertices as we traverse each connected component. If we ever encounter an edge where both endpoints have the same color, the graph is not bipartite, so we return `false`. We must visit every unvisited vertex to handle disconnected components. Edge cases: empty graph (m=0) is trivially bipartite; self-loops (u==v) would immediately cause a conflict since a vertex cannot have itself as a neighbor with a different color; duplicate edges are harmless because the first edge will set colors and ignore later duplicates. Time complexity is O(n + m) since each vertex and edge is processed once. Space complexity is O(n + m) for adjacency list and O(n) for visited and color arrays.
