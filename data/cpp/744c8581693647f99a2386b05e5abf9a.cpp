// You are given a bipartite graph with `N` vertices on the left side (numbered 1..N) and `M` vertices on the right side (numbered 1..M). Each left vertex can be matched to one or more right vertices (a list of its neighbors). Write a C++ function `int maxAssignments(int N, int M, const std::vector<std::vector<int>>& adj)` that returns the maximum number of left vertices that can be matched to distinct right vertices, but with a twist: each left vertex may be used **twice** (i.e., can be assigned to two different right vertices). The function must compute the size of a maximum matching in this "double‑matching" problem, where a left vertex can be matched to at most two right vertices, and each right vertex to at most one left vertex. The input graph is simple (no self‑loops, no duplicate edges). The function should handle cases where `N` or `M` is zero, and where some left vertices have no neighbors (they can still be counted only if they can be matched—but with zero neighbors they cannot be assigned at all). Use an augmenting‑path algorithm (like DFS‑based Kuhn) extended to allow two matches per left vertex. The function must be self‑contained, not rely on any global variables, and be reusable.

The problem is an extension of the classic maximum bipartite matching. Normally each left vertex can be used once. Here each left vertex can be used twice, so we effectively clone each left vertex into two copies (or equivalently, run the standard matching algorithm but allow the DFS to start from the same left vertex twice). The standard Kuhn algorithm finds an augmenting path from a left vertex; if we run it twice for every left vertex, we allow that vertex to be matched up to two times. This is correct because the algorithm never reassigns a left vertex to a right vertex that is already matched to that same left vertex (the `check` array prevents revisiting right vertices in a single DFS, and the matching array `d` stores which left vertex is matched to a given right vertex). Running `dfs(i)` twice with a fresh `check` array each time effectively gives each left vertex a chance to acquire two distinct right vertices. Edge cases: (1) If a left vertex has fewer than two neighbors, it can be matched at most as many times as its degree. (2) If `M` is less than `N` (or less than `2*N`), the answer is bounded by `M`. (3) For `N=0` or `M=0`, the answer is 0. The time complexity is O(2*N*E) where E is the total number of edges (because we run DFS for each copy of each vertex, and each DFS traverses adjacency lists). Space complexity is O(N+M) for the matching and visited arrays.

#include <vector>
#include <algorithm>

// Compute the maximum number of assignments where each left vertex can be used twice,
// each right vertex at most once.
// N: number of left vertices (1-indexed)
// M: number of right vertices (1-indexed)
// adj: adjacency list, adj[i] contains right vertices connected to left vertex i.
int maxAssignments(int N, int M, const std::vector<std::vector<int>>& adj) {
    // Match array: for each right vertex (1..M), which left vertex it is matched to (0 = unmatched).
    std::vector<int> matchRight(M + 1, 0);
    int result = 0;

    // DFS to find an augmenting path for left vertex 'cur'.
    // visited[] marks right vertices already tried in this DFS.
    std::vector<bool> visited(M + 1, false);
    auto dfs = [&](auto&& self, int cur) -> bool {
        for (int nxt : adj[cur - 1]) { // input is 1-indexed for vertices, but adj is 0-indexed
            if (visited[nxt]) continue;
            visited[nxt] = true;
            if (matchRight[nxt] == 0 || self(self, matchRight[nxt])) {
                matchRight[nxt] = cur;
                return true;
            }
        }
        return false;
    };

    // For each left vertex, attempt to match it twice.
    for (int i = 1; i <= N; ++i) {
        for (int copy = 0; copy < 2; ++copy) {
            std::fill(visited.begin(), visited.end(), false);
            if (dfs(dfs, i)) {
                ++result;
            } else {
                // If we cannot match this copy, break to avoid redundant attempts.
                // (A left vertex can never get a second match if the first fails.)
                break;
            }
        }
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple case, 2 left, 2 right, each left connected to both rights.
    // Maximum assignment: left1->right1, left1->right2? No, each right only once.
    // With double matching, left1 can take right1, left2 can take right2, but left1 cannot take both
    // because that would block left2. However left1 could take both, leaving left2 unmatched -> 2 assignments.
    std::vector<std::vector<int>> adj1 = {{1, 2}, {1, 2}};
    assert(maxAssignments(2, 2, adj1) == 2);

    // Test 2: Three left, two right. Each left connected to all rights.
    // With double matching per left, maximum cannot exceed M=2.
    std::vector<std::vector<int>> adj2 = {{1, 2}, {1, 2}, {1, 2}};
    assert(maxAssignments(3, 2, adj2) == 2);

    // Test 3: One left connects to two rights. Can match both.
    std::vector<std::vector<int>> adj3 = {{1, 2}};
    assert(maxAssignments(1, 2, adj3) == 2);

    // Test 4: One left connects to one right. Can match at most one.
    std::vector<std::vector<int>> adj4 = {{1}};
    assert(maxAssignments(1, 1, adj4) == 1);

    // Test 5: Left with no edges.
    std::vector<std::vector<int>> adj5 = {{}};
    assert(maxAssignments(1, 1, adj5) == 0);

    // Test 6: Empty graph.
    std::vector<std::vector<int>> adj6 = {};
    assert(maxAssignments(0, 3, adj6) == 0);

    // Test 7: More rights than needed, but limited edges.
    // Left1 -> {1}, Left2 -> {1,2}
    // Max: left1 takes right1, left2 takes right2 (or left2 takes both but then left1 none) -> 2
    std::vector<std::vector<int>> adj7 = {{1}, {1, 2}};
    assert(maxAssignments(2, 3, adj7) == 2);

    // Test 8: Complete bipartite K(2,3) but limit is M=3.
    // Left1 and left2 both connect to all three rights.
    // Two lefts can each take two rights? No, each right once. Total matches = min(2*2, 3) = 3.
    std::vector<std::vector<int>> adj8 = {{1,2,3},{1,2,3}};
    assert(maxAssignments(2, 3, adj8) == 3);

    // Test 9: Chain that requires reassignment.
    // Left1->{1}, Left2->{1,2}, Left3->{2}
    // Optimal: left1->1, left3->2, left2 can't get second because both rights taken? Actually left2 could take 1, and left1 fails, or left2 takes 2 and left3 fails. Max is 2? Let's compute: left1->1, left3->2 gives 2, left2 unmatched. But left2 could take 1, left1 fails, left3 takes 2 -> 2. So answer 2.
    std::vector<std::vector<int>> adj9 = {{1}, {1,2}, {2}};
    assert(maxAssignments(3, 2, adj9) == 2);

    // Test 10: Larger N, M where each left has degree 1 and rights are unique.
    // Each left can only match one, so total = N.
    std::vector<std::vector<int>> adj10 = {{1},{2},{3},{4}};
    assert(maxAssignments(4, 4, adj10) == 4);

    return 0;
}
