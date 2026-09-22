/*
Write a C++ function `analyzeDirectedGraph(int n, const std::vector<std::pair<int,int>>& edges)` that, given a directed graph with vertices numbered `0` to `n-1` and a list of directed edges, returns a `std::vector<int>` representing either a directed cycle (if one exists) or a topological ordering (if the graph is acyclic). The function must detect a cycle using DFS state coloring (`0` = unvisited, `1` = in current recursion stack, `2` = fully processed). If a cycle is found, return the sequence of vertices forming that cycle, starting and ending at the same vertex (e.g., `[1,2,3,1]`). If no cycle exists, return a valid topological ordering of all vertices (where every edge `u -> v` appears with `u` before `v` in the output). The input graph may be disconnected, may have parallel edges, and may contain self-loops (which are cycles of length 1). The solution should not rely on any global variables; all state must be local to the function.
*/

#include <vector>
#include <algorithm>
#include <functional>

// Analyze a directed graph given by number of vertices n and a list of directed edges.
// Returns a vector representing a cycle if one exists (starting and ending at same vertex),
// otherwise returns a topological ordering of all vertices.
std::vector<int> analyzeDirectedGraph(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
    }

    // Colors: 0 = unvisited, 1 = currently on recursion stack, 2 = fully processed
    std::vector<int> color(n, 0);
    std::vector<int> parent(n, -1);
    int cycle_start = -1;
    int cycle_end = -1;
    bool has_cycle = false;

    // DFS for cycle detection
    std::function<bool(int)> dfs_cycle = [&](int v) -> bool {
        color[v] = 1;
        for (int to : adj[v]) {
            if (color[to] == 0) {
                parent[to] = v;
                if (dfs_cycle(to)) return true;
            } else if (color[to] == 1) {
                // Found a back edge forming a cycle
                cycle_start = to;
                cycle_end = v;
                return true;
            }
        }
        color[v] = 2;
        return false;
    };

    for (int i = 0; i < n; ++i) {
        if (color[i] == 0) {
            if (dfs_cycle(i)) {
                has_cycle = true;
                break;
            }
        }
    }

    if (has_cycle) {
        // Reconstruct cycle from cycle_start to cycle_end
        std::vector<int> cycle;
        cycle.push_back(cycle_start);
        for (int v = cycle_end; v != cycle_start; v = parent[v]) {
            cycle.push_back(v);
        }
        cycle.push_back(cycle_start);
        std::reverse(cycle.begin(), cycle.end());
        return cycle;
    }

    // No cycle: build topological order using iterative DFS
    std::vector<int> order;
    std::vector<bool> visited(n, false);
    for (int start = 0; start < n; ++start) {
        if (visited[start]) continue;
        std::vector<int> stack;
        stack.push_back(start);
        while (!stack.empty()) {
            int v = stack.back();
            if (!visited[v]) {
                visited[v] = true;
                // Push all unvisited neighbors onto stack (order doesn't matter for correctness)
                for (int to : adj[v]) {
                    if (!visited[to]) {
                        stack.push_back(to);
                    }
                }
            } else {
                // All neighbors processed, add v to order
                order.push_back(v);
                stack.pop_back();
            }
        }
    }
    std::reverse(order.begin(), order.end());
    return order;
}

#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here (or link)

int main() {
    // Test 1: Simple acyclic graph
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{0,1},{0,2},{1,2}};
        auto res = analyzeDirectedGraph(n, edges);
        // Topological order must be [0,1,2] (only valid order)
        assert(res == std::vector<int>({0,1,2}));
    }

    // Test 2: Simple cycle
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,0}};
        auto res = analyzeDirectedGraph(n, edges);
        // Cycle could be [0,1,2,0] or any rotation
        assert(res.size() == 4);
        assert(res[0] == res.back());
        assert((res[0]==0 && res[1]==1 && res[2]==2) ||
               (res[0]==1 && res[1]==2 && res[2]==0) ||
               (res[0]==2 && res[1]==0 && res[2]==1));
    }

    // Test 3: Self-loop
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{0,0},{0,1}};
        auto res = analyzeDirectedGraph(n, edges);
        assert(res.size() == 2);
        assert(res[0] == 0 && res[1] == 0);
    }

    // Test 4: Disconnected acyclic graph
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges = {{0,1},{2,3},{3,4}};
        auto res = analyzeDirectedGraph(n, edges);
        // Topological order must have 0 before 1, 2 before 3 before 4
        auto pos = [&](int x) {
            for (size_t i = 0; i < res.size(); ++i) if (res[i] == x) return (int)i;
            return -1;
        };
        assert(pos(0) < pos(1));
        assert(pos(2) < pos(3));
        assert(pos(3) < pos(4));
        assert(res.size() == 5);
    }

    // Test 5: Graph with parallel edges but acyclic
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{0,1},{0,1},{1,2}};
        auto res = analyzeDirectedGraph(n, edges);
        assert(res == std::vector<int>({0,1,2}));
    }

    // Test 6: Graph with cycle of length 2 (parallel opposite edges)
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,0}};
        auto res = analyzeDirectedGraph(n, edges);
        assert(res.size() == 3);
        assert(res[0] == res.back());
        assert((res[0]==0 && res[1]==1) || (res[0]==1 && res[1]==0));
    }

    // Test 7: Cycle deeper in graph (5 vertices)
    {
        int n = 5;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3},{3,4},{4,2}};
        auto res = analyzeDirectedGraph(n, edges);
        assert(res.size() == 4); // cycle 2-3-4-2
        assert(res[0] == res.back());
        // Verify cycle vertices are exactly {2,3,4}
        std::vector<bool> present(5,false);
        for (int v : res) present[v] = true;
        assert(present[2] && present[3] && present[4] && !present[0] && !present[1]);
    }

    // Test 8: Empty graph (n=0)
    {
        int n = 0;
        std::vector<std::pair<int,int>> edges;
        auto res = analyzeDirectedGraph(n, edges);
        assert(res.empty());
    }

    // Test 9: Single vertex with no edges
    {
        int n = 1;
        std::vector<std::pair<int,int>> edges;
        auto res = analyzeDirectedGraph(n, edges);
        assert(res == std::vector<int>({0}));
    }

    // Test 10: Long chain acyclic
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3}};
        auto res = analyzeDirectedGraph(n, edges);
        assert(res == std::vector<int>({0,1,2,3}));
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The algorithm uses depth-first search (DFS) with three-color marking (`0` unvisited, `1` gray/on current path, `2` black/fully processed). For each vertex, we perform DFS. If we encounter a vertex that is currently gray (color `1`), we have found a back edge, meaning a cycle exists. We record the start and end of the cycle by tracking parent vertices during DFS. Then we reconstruct the cycle by walking backward from the end vertex to the start vertex using the parent array, then prepend the start vertex to close the loop. If no back edge is found across all DFS trees, the graph is acyclic. In that case, we perform a standard topological sort: after processing all outgoing edges of a vertex, we push that vertex onto a list, and at the end reverse the list to get a valid topological order. Important edge cases: self-loops are cycles of length one (the vertex appears twice in the result). Parallel edges do not affect correctness because any duplicate edge that goes to a gray vertex still triggers cycle detection. Disconnected graphs: the outer loop iterates over all vertices to ensure every component is visited. Time complexity is O(V + E) because each vertex and edge is processed a constant number of times; space complexity is O(V) for arrays and stack.
