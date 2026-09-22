// Write a C++ function that takes an undirected graph represented by `n` vertices (numbered 1 through n) and an edge list as input, and determines whether the graph is bipartite. If it is bipartite, return a `vector<vector<int>>` containing exactly two partitions: the first inner vector lists all vertices assigned color 1, and the second lists all vertices assigned color 2 (isolated vertices with no edges may be assigned color 1 and included in the first partition, or excluded entirely — but you must include every vertex exactly once across both partitions). If the graph is not bipartite, return an empty `vector<vector<int>>`. Use BFS to color the graph with two colors (1 and 2) such that no two adjacent vertices share the same color.
The problem reduces to checking whether the graph is bipartite, which is equivalent to whether it can be colored with two colors such that adjacent vertices have different colors. The standard approach is BFS (or DFS) from each uncolored vertex that has at least one edge. For each BFS component, we assign color 1 to the starting vertex, then for every neighbor of the current vertex, if the neighbor is uncolored, assign it the opposite color (3 - current color) and enqueue it; if the neighbor has the same color as the current vertex, a conflict exists, and the graph is not bipartite. We must visit all components because the graph may be disconnected. Isolated vertices (degree zero) do not affect bipartiteness and can be assigned color 1 arbitrarily, but to satisfy the requirement of including every vertex, we assign them color 1 and place them in the first partition. The time complexity is O(n + m) where m is the number of edges, since each vertex and edge is processed once. Space complexity is O(n + m) for the adjacency list and queue.
#include <vector>
#include <queue>

// Returns the two partitions of a bipartite graph, or an empty vector if not bipartite.
// Vertices are 1-indexed. n is the number of vertices, edges is a list of pairs {u, v}.
std::vector<std::vector<int>> bipartitePartitions(int n, const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    std::vector<int> color(n + 1, 0); // 0 = uncolored, 1 or 2 = partition label
    std::queue<int> q;

    for (int start = 1; start <= n; ++start) {
        if (color[start] != 0) continue;
        // Assign color 1 to isolated vertices and to the start of each component.
        color[start] = 1;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (color[v] == 0) {
                    color[v] = 3 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return {}; // Not bipartite
                }
            }
        }
    }

    std::vector<int> part1, part2;
    for (int i = 1; i <= n; ++i) {
        if (color[i] == 1) part1.push_back(i);
        else part2.push_back(i);
    }
    return {part1, part2};
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is defined above (include it here, or place in header).
// The code below compiles when the solution is placed before main.

int main() {
    // Simple bipartite graph: 1-2, 2-3 => partitions {1,3} and {2}
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        auto result = bipartitePartitions(3, edges);
        assert(result.size() == 2);
        // Sort or check both partitions combine to all vertices
        std::vector<int> all;
        for (const auto& part : result) all.insert(all.end(), part.begin(), part.end());
        assert(all.size() == 3);
        // Check no edge is within the same partition
        for (auto e : edges) {
            int p1 = -1, p2 = -1;
            for (int i = 0; i < 2; ++i) {
                for (int v : result[i]) {
                    if (v == e.first) p1 = i;
                    if (v == e.second) p2 = i;
                }
            }
            assert(p1 != p2);
        }
    }

    // Odd cycle: triangle => not bipartite
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
        auto result = bipartitePartitions(3, edges);
        assert(result.empty());
    }

    // Disconnected graph: one bipartite component and isolated vertex
    {
        std::vector<std::pair<int,int>> edges = {{1,2}};
        auto result = bipartitePartitions(3, edges);
        assert(result.size() == 2);
        // Both partitions combined include all vertices
        int count = 0;
        for (const auto& part : result) count += part.size();
        assert(count == 3);
    }

    // Empty graph (no edges): all vertices in partition 1
    {
        std::vector<std::pair<int,int>> edges;
        auto result = bipartitePartitions(3, edges);
        assert(result.size() == 2);
        assert(result[0].size() == 3);
        assert(result[1].empty());
    }

    // Larger bipartite graph: 4-cycle
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,1}};
        auto result = bipartitePartitions(4, edges);
        assert(result.size() == 2);
        // Check bipartiteness
        for (auto e : edges) {
            int p1 = -1, p2 = -1;
            for (int i = 0; i < 2; ++i) {
                for (int v : result[i]) {
                    if (v == e.first) p1 = i;
                    if (v == e.second) p2 = i;
                }
            }
            assert(p1 != p2);
        }
    }

    // Graph with multiple components, one not bipartite
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1},{4,5}};
        auto result = bipartitePartitions(5, edges);
        assert(result.empty()); // The triangle makes it non-bipartite
    }

    return 0;
}
