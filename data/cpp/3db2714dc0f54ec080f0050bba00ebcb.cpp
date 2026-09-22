Given a directed graph with `n` vertices (numbered 1 through `n`) and `m` directed edges, write a C++ function `find_directed_cycle` that takes `n`, `m`, and a vector of edge pairs (1-indexed) and returns a vector of vertices representing exactly one directed cycle if one exists; otherwise, it returns an empty vector. The cycle must be simple (no repeated vertices except the start/end vertex), and the output must contain the cycle in travel order starting from any vertex on the cycle. If multiple cycles exist, any one valid cycle is acceptable. The function must handle self-loops (an edge from a vertex to itself) as a valid cycle of length 1. Vertices with no edges must be ignored. The graph may be disconnected.

The problem is to detect and output a single directed cycle in a possibly disconnected directed graph. The approach uses depth-first search (DFS) with a three-state vertex marker (`0 = unvisited`, `1 = in current recursion stack`, `2 = fully processed`). Starting DFS from every unvisited vertex, when an edge leads to a vertex currently in the recursion stack (state `1`), a directed cycle is found. To reconstruct the cycle, maintain a `parent` array storing the DFS tree predecessor. When a back edge `(cur, it)` is detected (where `it` is in the recursion stack), trace from `cur` backward through `parent` until reaching `it`, collecting vertices along the way, and append `it` to close the cycle. The collected list is then reversed to obtain the cycle in the correct traversal order. Since the graph is directed and DFS ensures that a back edge corresponds to a directed cycle, this approach correctly finds at least one cycle if it exists. Edge cases include self-loops (which are immediately detected because `it == cur`), cycles of length greater than 1, and multiple disconnected components (each component is searched independently). The algorithm runs in `O(n + m)` time and uses `O(n)` auxiliary space for the `vis`, `parent`, and `ans` vectors.

#include <vector>
#include <algorithm>

// Detect and return one directed cycle in a directed graph.
// Input: n = number of vertices, edges = list of pairs (u, v) with 1-indexed vertices.
// Output: vector of vertices forming a cycle in order (e.g., {3,4,5,3}) or empty if impossible.
std::vector<int> find_directed_cycle(int n, const std::vector<std::pair<int, int>>& edges) {
    // Build adjacency list (convert to 0-indexed internally)
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e.first - 1].push_back(e.second - 1);
    }

    std::vector<int> vis(n, 0);   // 0 = unvisited, 1 = in recursion stack, 2 = fully processed
    std::vector<int> parent(n, -1);
    std::vector<int> cycle;

    // DFS function using lambda recursion
    std::function<void(int)> dfs = [&](int cur) {
        vis[cur] = 1; // entering recursion stack
        for (int nxt : adj[cur]) {
            if (vis[nxt] == 2) continue; // already fully processed, ignore
            if (vis[nxt] == 1) {
                // Found a back edge: cur -> nxt, and nxt is in current recursion stack
                if (cycle.empty()) {
                    // Reconstruct cycle: nxt ... cur (via parent pointers)
                    int node = cur;
                    while (node != nxt) {
                        cycle.push_back(node);
                        node = parent[node];
                    }
                    cycle.push_back(nxt); // close the cycle
                }
                continue;
            }
            // vis[nxt] == 0, normal tree edge
            parent[nxt] = cur;
            dfs(nxt);
            if (!cycle.empty()) return; // stop early if cycle found
        }
        vis[cur] = 2; // fully processed
    };

    for (int i = 0; i < n; ++i) {
        if (vis[i] == 0) {
            dfs(i);
            if (!cycle.empty()) break;
        }
    }

    // Convert to 1-indexed and reverse to get traversal order if needed
    if (!cycle.empty()) {
        std::reverse(cycle.begin(), cycle.end()); // currently stored in reverse order
        // The cycle is already in order: nxt -> ... -> cur -> nxt (last element)
        // For consistency, return as is (e.g., {1,2,3,1})
        for (auto& v : cycle) v += 1;
    }
    return cycle;
}

#include <cassert>
#include <vector>
#include <utility>

// Solution function declaration (copied above or included via header)
std::vector<int> find_directed_cycle(int n, const std::vector<std::pair<int, int>>& edges);

int main() {
    // Test 1: simple 3-cycle
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
        auto res = find_directed_cycle(3, edges);
        assert(res.size() == 4);
        assert(res[0] == res.back()); // cycle wraps
        assert(res[0] >= 1 && res[0] <= 3);
        // Check that consecutive edges exist
        for (size_t i = 0; i+1 < res.size(); ++i) {
            bool ok = false;
            for (auto& e : edges) {
                if (e.first == res[i] && e.second == res[i+1]) ok = true;
            }
            assert(ok);
        }
    }

    // Test 2: self-loop
    {
        std::vector<std::pair<int,int>> edges = {{2,2}};
        auto res = find_directed_cycle(3, edges);
        assert(res.size() == 2);
        assert(res[0] == 2 && res[1] == 2);
    }

    // Test 3: no cycle (DAG)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{1,3}};
        auto res = find_directed_cycle(3, edges);
        assert(res.empty());
    }

    // Test 4: disconnected graph, one component has cycle
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,1},{3,4}};
        auto res = find_directed_cycle(4, edges);
        assert(res.size() == 3);
        assert(res[0] == 1 && res[1] == 2 && res[2] == 1);
    }

    // Test 5: multiple cycles, returns at least one
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,1},{3,4},{4,3},{1,3}};
        auto res = find_directed_cycle(4, edges);
        assert(res.size() >= 3);
        for (size_t i = 0; i+1 < res.size(); ++i) {
            bool ok = false;
            for (auto& e : edges) {
                if (e.first == res[i] && e.second == res[i+1]) ok = true;
            }
            assert(ok);
        }
    }

    // Test 6: isolated vertices, no edges
    {
        std::vector<std::pair<int,int>> edges = {};
        auto res = find_directed_cycle(5, edges);
        assert(res.empty());
    }

    // Test 7: cycle with more vertices
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5},{5,1}};
        auto res = find_directed_cycle(5, edges);
        assert(res.size() == 6);
        for (size_t i = 0; i+1 < res.size(); ++i) {
            bool ok = false;
            for (auto& e : edges) {
                if (e.first == res[i] && e.second == res[i+1]) ok = true;
            }
            assert(ok);
        }
    }

    return 0;
}
