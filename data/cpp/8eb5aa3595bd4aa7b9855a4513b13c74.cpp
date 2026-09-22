Write a C++ function `int findMotherVertex(const std::vector<std::vector<int>>& adj, int n)` that takes an adjacency list representation of a directed graph with `n` vertices (labeled `0` to `n-1`) and returns the index of a mother vertex if one exists, or `-1` otherwise. A mother vertex is a vertex from which all other vertices are reachable via directed paths. The graph may be disconnected, may contain cycles, and may have zero edges. Your function must traverse the graph efficiently and correctly handle graphs with no mother vertex. Test for edge cases such as a single vertex, a strongly connected component not covering all vertices, and graphs where only a specific vertex reaches all others.
// The solution uses a two-pass DFS strategy based on the observation that if a mother vertex exists, the vertex with the greatest finish time in a full DFS over all vertices (with an arbitrary starting order) must be one such mother vertex. In the first pass, run DFS on every unvisited vertex in order, updating a candidate `mother` each time a new DFS is initiated. The last vertex that starts a DFS will be the vertex with the largest finish time. In the second pass, reset all visited flags and run DFS from this candidate. If this DFS visits all vertices, the candidate is a mother vertex; otherwise, no mother vertex exists. This works because the last starting DFS in the first pass reaches either all vertices or none beyond its own component, and any true mother vertex would have been the last to finish. Edge cases include an empty graph (n=0, should return -1), a graph with a single vertex (which is its own mother vertex), and a graph with multiple disjoint components (no mother vertex). Time complexity is O(V+E) for both passes, and space complexity is O(V) for the visited array.
#include <vector>
#include <functional>

// Finds a mother vertex in a directed graph or returns -1 if none exists.
// adj is the adjacency list, n is the number of vertices (0 to n-1).
int findMotherVertex(const std::vector<std::vector<int>>& adj, int n) {
    if (n == 0) return -1;

    std::vector<bool> visited(n, false);
    int candidate = -1;

    // First DFS pass: find the last vertex that initiates a DFS
    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                dfs(v);
            }
        }
    };

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs(i);
            candidate = i;
        }
    }

    // Second DFS pass: check if candidate can reach all vertices
    std::fill(visited.begin(), visited.end(), false);
    dfs(candidate);

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) return -1;
    }
    return candidate;
}
#include <cassert>
#include <vector>

int main() {
    // Single vertex: itself is mother
    {
        std::vector<std::vector<int>> adj = {{}};
        assert(findMotherVertex(adj, 1) == 0);
    }

    // Two vertices with edge 0->1, mother is 0
    {
        std::vector<std::vector<int>> adj = {{1}, {}};
        assert(findMotherVertex(adj, 2) == 0);
    }

    // Two vertices with edge 1->0, mother is 1
    {
        std::vector<std::vector<int>> adj = {{}, {0}};
        assert(findMotherVertex(adj, 2) == 1);
    }

    // No edges, multiple vertices: no mother
    {
        std::vector<std::vector<int>> adj = {{}, {}, {}};
        assert(findMotherVertex(adj, 3) == -1);
    }

    // Cycle covering all 3 vertices: any vertex is mother; our algo returns last start
    {
        std::vector<std::vector<int>> adj = {{1}, {2}, {0}};
        int result = findMotherVertex(adj, 3);
        assert(result >= 0 && result < 3); // all are mothers
    }

    // Two components: one is a cycle, one single. No mother
    {
        std::vector<std::vector<int>> adj = {{1}, {0}, {}};
        assert(findMotherVertex(adj, 3) == -1);
    }

    // Star graph: center 0 reaches all, others reach only themselves
    {
        std::vector<std::vector<int>> adj = {{1,2,3}, {}, {}, {}};
        assert(findMotherVertex(adj, 4) == 0);
    }

    // Reverse star: leaves reach center but not each other, no mother
    {
        std::vector<std::vector<int>> adj = {{}, {0}, {0}, {0}};
        assert(findMotherVertex(adj, 4) == -1);
    }

    // Disconnected with self-loops: no mother
    {
        std::vector<std::vector<int>> adj = {{0}, {1}};
        assert(findMotherVertex(adj, 2) == -1);
    }

    // Empty graph
    {
        std::vector<std::vector<int>> adj = {};
        assert(findMotherVertex(adj, 0) == -1);
    }

    return 0;
}
