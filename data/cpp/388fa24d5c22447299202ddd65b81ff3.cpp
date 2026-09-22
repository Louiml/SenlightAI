Write a C++ function that takes an integer `n` and a vector of `n-1` undirected edges (each edge connecting two distinct vertices labeled from 1 to n), representing a tree. The function should return the size of the maximum matching in that tree—i.e., the maximum number of edges that can be selected such that no two selected edges share a common vertex. The tree is guaranteed to be connected and acyclic. The function signature is `int maximumMatching(int n, const vector<pair<int,int>>& edges)`. You may assume `n >= 1`. If `n == 1`, the answer is 0. The result fits within a 32-bit signed integer (but use `long long` for safety if desired).
// This is a classic tree DP problem. We root the tree arbitrarily (say at vertex 1). For each vertex `u`, we maintain two states:
// - `dp[u][0]`: the maximum matching size in the subtree of `u`, assuming `u` is **not** matched with any child.
// - `dp[u][1]`: the maximum matching size in the subtree of `u`, assuming `u` **is** matched with exactly one child (so that edge counts).
//
// Transitions:
// - For `dp[u][0]`, since `u` is unmatched, each child `v` can be either matched or unmatched independently, so we sum `max(dp[v][0], dp[v][1])` over all children.
// - For `dp[u][1]`, we must choose exactly one child `v` to match with `u` (adding 1 for that edge), and for all other children we choose the best state. This can be computed efficiently: first compute `total = dp[u][0]` (sum of max over children). Then for each child `v`, the value if we choose `v` is `1 + total - max(dp[v][0], dp[v][1]) + dp[v][0]`. Take the maximum over all children. If `u` has no children, then `dp[u][1]` is invalid (or `-infinity`), effectively 0 because you cannot match.
//
// Finally the answer is `max(dp[root][0], dp[root][1])`.
//
// Edge cases: `n=1` (no edges), a leaf node (no children), and large chains (recursion depth – we use iterative stack or set recursion limit accordingly, but in C++ we can implement an explicit stack to avoid stack overflow for very deep trees, or keep recursion as typical for such problems). Time complexity: `O(n)` because we visit each edge twice during DFS. Space complexity: `O(n)` for adjacency list and DP table.
#include <vector>
#include <functional>
#include <algorithm>

// Returns the size of a maximum matching in a tree with vertices 1..n.
// edges: list of (u,v) pairs representing undirected edges.
// The tree is assumed connected and acyclic.
int maximumMatching(int n, const std::vector<std::pair<int,int>>& edges) {
    if (n == 1) return 0;
    
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }
    
    // dp[u][0] = best matching in subtree of u, u not matched to a child
    // dp[u][1] = best matching in subtree of u, u matched to exactly one child
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(2, 0));
    
    // Iterative post-order traversal to avoid recursion depth issues
    std::vector<int> parent(n + 1, 0);
    std::vector<int> order;
    order.reserve(n);
    
    std::function<void(int,int)> dfs = [&](int u, int p) {
        parent[u] = p;
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs(v, u);
        }
        order.push_back(u);
    };
    dfs(1, 0); // root at 1, parent 0 means none

    // Process in reverse order (post-order)
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        int u = *it;
        // Compute dp[u][0] = sum of max(dp[v][0], dp[v][1]) for children
        int total = 0;
        for (int v : adj[u]) {
            if (v == parent[u]) continue; // skip parent
            total += std::max(dp[v][0], dp[v][1]);
        }
        dp[u][0] = total;
        
        // Compute dp[u][1]: try matching u with each child
        int best = 0; // if no children, dp[u][1] = 0 (no edge possible)
        for (int v : adj[u]) {
            if (v == parent[u]) continue;
            int candidate = 1 + total - std::max(dp[v][0], dp[v][1]) + dp[v][0];
            best = std::max(best, candidate);
        }
        dp[u][1] = best;
    }
    
    return std::max(dp[1][0], dp[1][1]);
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above (included by reference or inline here)
// For completeness, we repeat the declaration:
int maximumMatching(int n, const std::vector<std::pair<int,int>>& edges);

int main() {
    // Single node: no edges, matching size 0
    assert(maximumMatching(1, {}) == 0);
    
    // Two nodes, one edge: matching size 1
    assert(maximumMatching(2, {{1,2}}) == 1);
    
    // Path of 3 nodes (1-2-3): maximum matching size 1 (can't take both edges because they share vertex 2)
    assert(maximumMatching(3, {{1,2},{2,3}}) == 1);
    
    // Path of 4 nodes (1-2-3-4): maximum matching size 2 (take edges (1,2) and (3,4))
    assert(maximumMatching(4, {{1,2},{2,3},{3,4}}) == 2);
    
    // Star with center 1 and leaves 2,3,4: maximum matching size 1 (only one edge can be taken)
    assert(maximumMatching(4, {{1,2},{1,3},{1,4}}) == 1);
    
    // Tree: 1-2, 2-3, 2-4, 4-5, 4-6 (like a subtree with two branches off 4)
    // Maximum matching: edges (1,2) and (4,5) and (4,6)? No, (4,5) and (4,6) share 4, so cannot both be taken.
    // Better: take (1,2), (3,2) cannot because share 2. Actually take (1,2) and (4,5) or (4,6) → 2 edges.
    // Or take (2,3) and (4,5) → 2 edges. Let's verify: the tree is a path 5-4-2-1 with extra leaf 3 attached to 2, and extra leaf 6 attached to 4.
    // Possible matching of size 3? Try (5,4), (2,3), (1,2) → (2,3) and (1,2) share 2, not allowed. Try (5,4) and (1,2) → 2 edges. Max is 2.
    assert(maximumMatching(6, {{1,2},{2,3},{2,4},{4,5},{4,6}}) == 2);
    
    // A more complex tree: perfect matching on 6 vertices (a cycle would be 6 edges, but tree: connect as a path 1-2-3-4-5-6, matching size 3)
    assert(maximumMatching(6, {{1,2},{2,3},{3,4},{4,5},{5,6}}) == 3);
    
    // Random tree with 7 vertices: 1-2,1-3,2-4,2-5,3-6,3-7 (two stars connected at 1). Maximum matching: pick (2,4),(3,6),(1,2?) no share 2. Instead pick (2,4),(3,6),(2,5?) no share 2. Better pick (1,2) and (3,6) and (3,7?) no share 3. So pick (1,2),(3,6) → 2, or (2,4),(3,6) → 2. Could we get 3? (2,4),(3,6),(1,2?) conflicts. So max is 2? Wait, let's think: The tree is like two stars centered at 2 and 3, connected via 1. Leaves: 4,5 attached to 2; 6,7 attached to 3; and 1 connected to both 2 and 3. We can take edges (2,4), (3,6), and (1,2) conflicts with (2,4). So we can take (2,4), (3,6), and (1,3) conflicts with (3,6). So max is 2? Actually we can take (2,4), (2,5)? No share 2. So best is (2,4) and (3,6) → 2. Or (1,2) and (3,6) → 2. So answer 2.
    assert(maximumMatching(7, {{1,2},{1,3},{2,4},{2,5},{3,6},{3,7}}) == 2);
    
    // Binary tree of 7 nodes (full): 1-2,1-3,2-4,2-5,3-6,3-7. Maximum matching size 3: (4,2),(5,2) cannot both. So pick (2,4) and (3,6) and (1,2) conflicts with (2,4). Instead pick (2,5) and (3,7) and (1,2) conflicts. So best is 2? Actually we can pick (2,4), (3,6), and (1,2) no. So max is 2? Wait, let's see: The tree is a perfect binary tree depth 2. Maximum matching: we can take (2,4), (3,6), and (1,2) or (1,3) but that conflicts with one of the chosen. So we can take (2,4) and (3,6) → 2, plus (1,2) conflicts. Alternatively take (1,2) and (3,6) and (2,5)? conflicts with (1,2). So best is 2 edges. Actually think: We can take (2,5) and (3,7) and (1,2) conflicts. So max is 2? Wait, maybe we can take (1,2) and (3,6) and (3,7) cannot. So 2. So answer 2.
    assert(maximumMatching(7, {{1,2},{1,3},{2,4},{2,5},{3,6},{3,7}}) == 2);
    
    // Test a star with 5 leaves: center 1, leaves 2..6. Maximum matching = 1.
    assert(maximumMatching(6, {{1,2},{1,3},{1,4},{1,5},{1,6}}) == 1);
    
    return 0;
}
