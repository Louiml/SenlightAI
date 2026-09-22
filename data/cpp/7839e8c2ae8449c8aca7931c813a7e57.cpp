/*
You are given a bipartite graph with vertices numbered 1..n on the left side and 1..n on the right side. The graph is represented as a set of directed edges from a left vertex to a right vertex. Write a function `int maximumMatching(const std::vector<std::pair<int,int>>& edges, int n)` that returns the size of the maximum matching in this bipartite graph. The matching is a set of edges such that no two edges share a left vertex and no two share a right vertex. The input graph may have multiple edges between the same pair (treat them as one), and may have isolated vertices. The size `n` can be up to 500, and the number of edges up to `n^2`.
*/
#include <vector>
#include <unordered_set>

// Returns the size of the maximum bipartite matching.
// edges: list of directed edges (left, right) with 1-based indices.
// n: number of vertices on each side (total vertices = 2n).
int maximumMatching(const std::vector<std::pair<int,int>>& edges, int n) {
    // Build adjacency list, deduplicate edges.
    std::vector<std::unordered_set<int>> adj(n + 1);
    for (const auto& e : edges) {
        int u = e.first, v = e.second;
        if (u >= 1 && u <= n && v >= 1 && v <= n) {
            adj[u].insert(v);
        }
    }

    std::vector<int> matchR(n + 1, 0); // matchR[v] = left vertex matched to right v
    std::vector<int> visited(n + 1, 0);
    int timestamp = 0;

    // DFS recursive function.
    // Uses lambda capturing by reference; returns true if augmenting path found.
    std::function<bool(int)> dfs = [&](int u) -> bool {
        if (visited[u] == timestamp) return false;
        visited[u] = timestamp;
        for (int v : adj[u]) {
            if (matchR[v] == 0 || dfs(matchR[v])) {
                matchR[v] = u;
                return true;
            }
        }
        return false;
    };

    int matching_size = 0;
    for (int u = 1; u <= n; ++u) {
        ++timestamp;
        if (dfs(u)) {
            ++matching_size;
        }
    }
    return matching_size;
}
#include <cassert>
#include <vector>
#include <utility>
#include <iostream>

// Include the solution function here (or above).

int main() {
    // Empty graph
    assert(maximumMatching({}, 3) == 0);

    // Single edge
    assert(maximumMatching({{1,1}}, 1) == 1);

    // Two disjoint edges
    assert(maximumMatching({{1,1},{2,2}}, 2) == 2);

    // Three vertices, all edges from left to same right -> only one match
    assert(maximumMatching({{1,1},{2,1},{3,1}}, 3) == 1);

    // Complete bipartite K2,2
    assert(maximumMatching({{1,1},{1,2},{2,1},{2,2}}, 2) == 2);

    // Path case: left1->right1, left2->right1, left2->right2 (maximum is 2)
    assert(maximumMatching({{1,1},{2,1},{2,2}}, 2) == 2);

    // Duplicate edges
    assert(maximumMatching({{1,1},{1,1},{2,2}}, 2) == 2);

    // Larger dangling: n=4, edges form a "star" plus one extra
    assert(maximumMatching({{1,1},{2,1},{3,1},{4,2}}, 4) == 2);

    // All isolated vertices
    assert(maximumMatching({{1,2},{3,4}}, 5) == 2);

    // Chain of length 4 (needs augmenting paths)
    // left1-right1, left2-right1, left2-right2, left3-right2, left3-right3
    assert(maximumMatching({{1,1},{2,1},{2,2},{3,2},{3,3}}, 3) == 3);

    std::cout << "All tests passed!\n";
    return 0;
}
// The problem is a classic maximum bipartite matching problem. The graph is unweighted and we need the size of a maximum matching. The standard approach uses the Hungarian algorithm (Kuhn's algorithm) for bipartite matching. We maintain an array `matchR` where `matchR[v]` stores which left vertex is matched to right vertex `v` (0 if unmatched). For each left vertex `u`, we attempt to find an augmenting path using DFS. To avoid revisiting the same right vertex in a single DFS run, we use a `visited` array (can be an integer timestamp to avoid resetting each time). The DFS tries every right neighbor `v` of `u`; if `v` is unmatched, we match it to `u` and return true; otherwise, we try to re-match the previously matched left vertex `matchR[v]` recursively. If successful, we update `matchR[v] = u`. If no such path exists, return false. Sum of successful tries is the matching size. Edge case: duplicates in edges do not affect the result because the adjacency list can be deduplicated (or DFS will simply try them repeatedly but that's harmless). Time complexity is O(n * E) where E is number of distinct edges (worst O(n^3) for dense graph). Space complexity O(n^2) if we store adjacency matrix, or O(n + E) with adjacency lists. We use adjacency list for efficiency.
