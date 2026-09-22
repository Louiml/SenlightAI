// Given a weighted tree with `n` vertices (2 ≤ n ≤ 2·10^5) and `n-1` undirected edges, where each edge has a positive integer weight, write a C++ function that computes the maximum possible sum of weights of a matching in the tree. A matching is a set of edges no two of which share a common vertex. Output the maximum total weight of such a set. The vertices are numbered 1 to n, and the function should take the number of vertices and a vector of edges (each edge as a tuple `(u, v, weight)` with 1-indexed vertices) and return the maximum total weight as a `long long`.

This is the classic maximum weight matching problem on a tree, solvable via tree DP. Root the tree at vertex 0 (or 1). For each node `v`, define two values:
- `not_taken[v]`: maximum total weight of a matching in the subtree rooted at `v` when `v` is not matched to any of its children.
- `taken[v]`: maximum total weight of a matching in the subtree rooted at `v` when `v` may or may not be matched, but we consider the best option.

A standard approach uses two DP arrays: `dp0[v]` = maximum weight in subtree when `v` is NOT incident to any selected edge; `dp1[v]` = maximum weight in subtree when `v` may be incident to one selected edge (to a child). The recurrence: first compute `sum0[v] = sum over children u of max(dp0[u], dp1[u])` (children not forced to be matched to v). Then `dp0[v] = sum0[v]`. For `dp1[v]`, we may choose exactly one child `u` to pair with `v` via edge weight `w`, but then `u` must be in state `dp0[u]` (since it's used), and for all other children we take `max(dp0, dp1)`. So `dp1[v] = max over children u of (sum0[v] - max(dp0[u], dp1[u]) + w + dp0[u])`. The answer is `max(dp0[root], dp1[root])`. This matches the given snippet logic, where `childrenSum` acts as `dp0` and `d` acts as `dp1`. Edge cases: leaf nodes have both values 0. For n=2, only one edge, answer is its weight. Complexity: O(n) time and O(n) space for adjacency lists, recursion depth up to n (so an iterative or DFS with explicit stack might be needed for very deep trees, but typical recursion with `-O2` may handle up to 2e5 if the tree is not degenerate; otherwise use iterative DFS). The solution uses long long because weights are positive integers up to 10^9 and sums can exceed 32-bit.

#include <vector>
#include <algorithm>

// Compute maximum total weight of a matching in a weighted tree.
// vertices are 1-indexed; edges are tuples (u, v, weight).
long long maximumMatchingWeight(int n, const std::vector<std::tuple<int,int,int>>& edges) {
    std::vector<std::vector<std::pair<int,int>>> g(n);
    for (const auto& e : edges) {
        int u, v, w;
        std::tie(u, v, w) = e;
        --u; --v;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    std::vector<long long> notTaken(n, 0);
    std::vector<long long> maybeTaken(n, 0);

    // Depth-first search using explicit stack to avoid recursion depth issues.
    std::vector<int> parent(n, -1);
    std::vector<int> order;
    order.reserve(n);
    std::vector<int> stack = {0};
    parent[0] = -2; // mark root

    while (!stack.empty()) {
        int v = stack.back();
        stack.pop_back();
        order.push_back(v);
        for (const auto& [u, w] : g[v]) {
            if (u == parent[v]) continue;
            parent[u] = v;
            stack.push_back(u);
        }
    }

    // Process in reverse order (post-order)
    for (int idx = n-1; idx >= 0; --idx) {
        int v = order[idx];
        long long sum = 0;
        long long best = 0;
        // First pass: compute sum of max(notTaken, maybeTaken) over children
        for (const auto& [u, w] : g[v]) {
            if (u == parent[v]) continue;
            sum += std::max(notTaken[u], maybeTaken[u]);
        }
        notTaken[v] = sum;
        // Second pass: choose at most one child to pair with v
        maybeTaken[v] = sum;
        for (const auto& [u, w] : g[v]) {
            if (u == parent[v]) continue;
            long long candidate = sum - std::max(notTaken[u], maybeTaken[u]) + w + notTaken[u];
            maybeTaken[v] = std::max(maybeTaken[v], candidate);
        }
    }

    return std::max(notTaken[0], maybeTaken[0]);
}

#include <cassert>
#include <vector>
#include <tuple>

// solution function declaration (assumed from above)
long long maximumMatchingWeight(int n, const std::vector<std::tuple<int,int,int>>& edges);

int main() {
    // Test 1: single edge
    assert(maximumMatchingWeight(2, {{1,2,5}}) == 5);

    // Test 2: path of 3 nodes: 1-2 (3), 2-3 (4) => can pick only one edge, max is 4
    assert(maximumMatchingWeight(3, {{1,2,3},{2,3,4}}) == 4);

    // Test 3: star with center 1 and leaves 2,3,4 weights 10,20,30 => pick all leaves, sum=60
    assert(maximumMatchingWeight(4, {{1,2,10},{1,3,20},{1,4,30}}) == 60);

    // Test 4: two disjoint edges in a tree of 5 nodes: edges (1-2,7) and (3-4,8), plus (2-5,1) connecting
    // Optimal: pick (1-2,7) and (3-4,8) = 15
    assert(maximumMatchingWeight(5, {{1,2,7},{2,5,1},{3,4,8}}) == 15);

    // Test 5: triangle impossible in tree; instead a path with 4 nodes: 1-2(5),2-3(6),3-4(7)
    // Pick either (1-2,5) & (3-4,7) = 12, or (2-3,6) = 6 => max 12
    assert(maximumMatchingWeight(4, {{1,2,5},{2,3,6},{3,4,7}}) == 12);

    // Test 6: weighted tree with all edges weight 1 and perfect matching
    // A path of 2 nodes: just one edge
    assert(maximumMatchingWeight(2, {{1,2,1}}) == 1);

    // Test 7: larger tree: star with 3 leaves plus an extra edge between two leaves
    // Vertices: 1 center, 2,3,4 leaves; edges: 1-2(1),1-3(2),1-4(3),3-4(10) -> but that would be a cycle? No, 3-4 makes it not a tree, invalid. So use a valid tree: 
    // Path: 1-2(100), 2-3(1), 2-4(1) => pick 1-2(100) because leaf 3 and 4 can't both pair with 2. So max=100
    assert(maximumMatchingWeight(4, {{1,2,100},{2,3,1},{2,4,1}}) == 100);

    // Test 8: both branches have multiple edges, test combination
    // Tree: 1-2(9),2-3(8),1-4(7),4-5(6) => pick (2-3,8) and (4-5,6) = 14, or (1-2,9)+(4-5,6)=15, or (1-4,7)+(2-3,8)=15
    assert(maximumMatchingWeight(5, {{1,2,9},{2,3,8},{1,4,7},{4,5,6}}) == 15);

    // Test 9: single vertex? Not allowed per constraints but handle gracefully
    // n=1, no edges => 0
    assert(maximumMatchingWeight(1, {}) == 0);

    // Test 10: heavy weights and large sum
    // Path of 3: 1-2(10^9),2-3(10^9) => pick one, max=10^9
    assert(maximumMatchingWeight(3, {{1,2,1000000000},{2,3,1000000000}}) == 1000000000LL);
}
