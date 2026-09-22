// Write a C++ function that, given a list of `n` adjacency lists where each node i (1-indexed) is followed by a count of its neighbors and then those neighbor IDs, determines whether the underlying multigraph has an Eulerian cycle or Eulerian path. If it has an Eulerian cycle (all vertices have even degree), return a vector containing the vertices of a valid Eulerian cycle starting and ending at node 1. If it has an Eulerian path (exactly two vertices have odd degree), return a vector containing the vertices of a valid Eulerian path starting at one odd-degree vertex and ending at the other. If neither exists, return an empty vector. The input is provided as a vector of pairs: for each node `i`, the pair `(neighbor, edge_id_placeholder)` is irrelevant — instead, the function should receive the vertex degrees and edge connectivity implicitly through a list of edges. Specifically, the function signature should be: `std::vector<int> findEulerianTrail(int n, const std::vector<std::pair<int,int>>& edges)` where `edges` contains undirected edges as pairs (u,v) with 1 ≤ u,v ≤ n. Note that multiple edges between the same pair of vertices are allowed, and self-loops are not present. The function must handle up to 100,000 vertices and 300,000 edges efficiently.
// We must compute the degree of each vertex from the edge list. If all degrees are even, an Eulerian cycle exists starting at vertex 1. If exactly two vertices have odd degree, an Eulerian path exists starting at one odd vertex and ending at the other. Otherwise, return empty. To find the actual trail, we use Hierholzer's algorithm with an iterative or recursive DFS that removes edges as it uses them. We need a data structure to efficiently find unused edges from each vertex. Since there may be multiple edges between the same pair, we maintain for each vertex a list of adjacency entries (neighbor, edge index). We also maintain a global edge used flag. The recursive function visits an edge only if not used, marks it used, then recurses, and after recursion appends the destination vertex. At the end, we reverse the collected vertices to get the trail. For the cycle case, we must ensure we start from vertex 1 and that the trail ends at vertex 1. For the path case, we start from one odd vertex and the algorithm naturally ends at the other. We must also handle the edge case where n=1 and no edges: that is a valid Eulerian cycle (just vertex 1) so return {1}. Time complexity is O(n + m) where m is number of edges, due to each edge being visited once. Space complexity is O(n + m) for adjacency list and edge flags.
#include <vector>
#include <algorithm>

std::vector<int> findEulerianTrail(int n, const std::vector<std::pair<int,int>>& edges) {
    int m = edges.size();
    std::vector<int> deg(n + 1, 0);
    std::vector<std::vector<std::pair<int,int>>> adj(n + 1); // (neighbor, edge_id)
    for (int i = 0; i < m; ++i) {
        int u = edges[i].first;
        int v = edges[i].second;
        deg[u]++;
        deg[v]++;
        adj[u].emplace_back(v, i);
        adj[v].emplace_back(u, i);
    }

    std::vector<int> odd;
    for (int i = 1; i <= n; ++i) {
        if (deg[i] % 2) odd.push_back(i);
    }

    int start = -1;
    if (odd.empty()) {
        // Eulerian cycle exists, start at 1 if n>0
        start = (n >= 1) ? 1 : 0;
        if (n == 0) return {}; // no vertices? handle gracefully
    } else if (odd.size() == 2) {
        start = odd[0];
    } else {
        return {}; // no Eulerian trail
    }

    std::vector<char> used(m, false);
    std::vector<int> trail;
    // iterative recursion to avoid stack overflow
    std::vector<int> stack;
    stack.push_back(start);
    std::vector<int> it(n + 1, 0); // iterator index for each vertex's adjacency

    while (!stack.empty()) {
        int v = stack.back();
        bool moved = false;
        while (it[v] < (int)adj[v].size()) {
            auto [nei, eid] = adj[v][it[v]++];
            if (!used[eid]) {
                used[eid] = true;
                stack.push_back(nei);
                moved = true;
                break;
            }
        }
        if (!moved) {
            trail.push_back(v);
            stack.pop_back();
        }
    }

    // trail currently has vertices in reverse order of traversal.
    // For cycle, it should start and end at start; for path, start and end at odd vertices.
    std::reverse(trail.begin(), trail.end());

    // Verify we used all edges (in case of disconnected graph with same degree pattern)
    for (bool b : used) if (!b) return {};
    if (trail.front() != start) return {}; // safety

    return trail;
}
#include <cassert>
#include <vector>
#include <algorithm>

// Declaration of the function being tested
std::vector<int> findEulerianTrail(int n, const std::vector<std::pair<int,int>>& edges);

int main() {
    // Test 1: Simple cycle 1-2-3-1
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
        auto trail = findEulerianTrail(3, edges);
        assert(!trail.empty() && trail.front() == 1 && trail.back() == 1);
        // Check each edge appears exactly once in trail (consecutive pairs)
        std::vector<std::pair<int,int>> used_edges;
        for (size_t i = 0; i+1 < trail.size(); ++i) {
            int a = trail[i], b = trail[i+1];
            used_edges.emplace_back(std::min(a,b), std::max(a,b));
        }
        std::vector<std::pair<int,int>> expected = {{1,2},{2,3},{1,3}};
        std::sort(used_edges.begin(), used_edges.end());
        std::sort(expected.begin(), expected.end());
        assert(used_edges == expected);
    }

    // Test 2: Path with two odd vertices 1-2-3 (degrees: 1:1,2:2,3:1)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        auto trail = findEulerianTrail(3, edges);
        assert(!trail.empty() && trail.front() == 1 && trail.back() == 3);
        assert(trail.size() == 3);
    }

    // Test 3: Multiple edges (parallel) 1-2 twice and 2-3 once -> degrees: 1:2,2:3,3:1 -> odd:2,3 -> path from 2 to 3
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,2},{2,3}};
        auto trail = findEulerianTrail(3, edges);
        assert(!trail.empty() && trail.front() == 2 && trail.back() == 3);
        // trail must have exactly 4 edges used => size 5 (vertices)
        assert(trail.size() == 5);
    }

    // Test 4: No Eulerian trail (4 odd vertices)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{3,4}};
        assert(findEulerianTrail(4, edges).empty());
    }

    // Test 5: Single vertex no edges -> cycle of length 1
    {
        std::vector<std::pair<int,int>> edges;
        auto trail = findEulerianTrail(1, edges);
        assert(!trail.empty() && trail.front() == 1 && trail.back() == 1);
    }

    // Test 6: Disconnected graph but each component has Eulerian cycle -> must return empty (all degrees even but not connected)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{3,4}}; // both even degree but disconnected
        assert(findEulerianTrail(4, edges).empty());
    }

    // Test 7: Larger cycle with multiple parallel edges
    {
        std::vector<std::pair<int,int>> edges;
        edges.emplace_back(1,2);
        edges.emplace_back(2,3);
        edges.emplace_back(3,1);
        edges.emplace_back(1,2); // duplicate
        edges.emplace_back(2,3); // duplicate
        edges.emplace_back(3,1); // duplicate
        auto trail = findEulerianTrail(3, edges);
        assert(!trail.empty() && trail.front() == 1 && trail.back() == 1);
        assert(trail.size() == 7); // 6 edges +1
    }

    // Test 8: Path with self-loop? Not allowed per spec, but test edges with same endpoints? not needed.

    return 0;
}
