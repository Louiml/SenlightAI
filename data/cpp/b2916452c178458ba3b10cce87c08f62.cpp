Write a C++ function `countPaintings` that takes the number of barns `n`, a vector of undirected edges `edges` (each edge as a pair of 0-indexed barn indices), and a vector of pre-determined color assignments `fixed` (each pair is `(barnIndex, color)` with color in `{0,1,2}`). The function must return the number of ways to color each barn with one of three colors such that no two adjacent barns share the same color, and the fixed assignments are respected. The result should be returned modulo `1e9+7`. The tree is guaranteed to be connected with `n >= 1`.

This is a classic tree DP coloring problem. Root the tree at node 0. For each node `c` and each color `x` in `{0,1,2}`, define `dp[c][x]` as the number of valid colorings of the subtree rooted at `c` given that `c` is colored `x`. For a leaf, `dp[c][x] = 1` if `x` is allowed (not fixed to another color), else `0`. For an internal node, after computing all children's DP values, for each allowed `x`, multiply the sums `(dp[child][a] + dp[child][b])` where `a` and `b` are the two colors different from `x`, across all children. This works because children subtrees are independent once the parent's color is fixed. Finally, the total number of valid colorings for the whole tree is `dp[root][0] + dp[root][1] + dp[root][2]` modulo `MOD`. Edge cases: when a node has a fixed color, we must set DP for the other two colors to 0; also, if `n=1`, the answer is the number of allowed colors (1 if fixed, 3 otherwise). Time complexity is `O(n)` since each edge is visited once and each node processes at most 3 colors. Space complexity is `O(n)` for adjacency list and DP table (using `long long` for multiplication).

#include <vector>
#include <cstdint>

// Count valid tree colorings modulo 1e9+7.
// n: number of nodes (0..n-1), edges: undirected edges, fixed: (node, color) assignments.
// Colors are {0,1,2}, return total valid colorings of the entire tree.
int64_t countPaintings(int n, const std::vector<std::pair<int,int>>& edges,
                       const std::vector<std::pair<int,int>>& fixed) {
    const int64_t MOD = 1000000007LL;
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // allowed[c][x] = true if node c can be colored x.
    std::vector<std::vector<bool>> allowed(n, std::vector<bool>(3, true));
    for (const auto& f : fixed) {
        int node = f.first;
        int color = f.second;
        // disallow other colors
        for (int x = 0; x < 3; ++x) {
            if (x != color) allowed[node][x] = false;
        }
    }

    // dp[c][x] = number of valid colorings of subtree of c with c colored x.
    std::vector<std::vector<int64_t>> dp(n, std::vector<int64_t>(3, 0));

    // iterative DFS to avoid recursion depth issues; but recursion is fine for n<=1e5 in this exercise
    // Using a post-order traversal with a stack.
    std::vector<int> parent(n, -1);
    std::vector<int> order;
    order.reserve(n);
    std::vector<int> stack = {0};
    parent[0] = -2; // mark visited
    while (!stack.empty()) {
        int c = stack.back();
        stack.pop_back();
        order.push_back(c);
        for (int v : adj[c]) {
            if (v == parent[c]) continue;
            parent[v] = c;
            stack.push_back(v);
        }
    }

    // Process nodes in reverse order (post-order)
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        int c = *it;
        for (int x = 0; x < 3; ++x) {
            if (!allowed[c][x]) {
                dp[c][x] = 0;
                continue;
            }
            int a = (x + 1) % 3;
            int b = (x + 2) % 3;
            int64_t ways = 1;
            bool has_children = false;
            for (int v : adj[c]) {
                if (v == parent[c]) continue;
                has_children = true;
                int64_t child_sum = (dp[v][a] + dp[v][b]) % MOD;
                ways = (ways * child_sum) % MOD;
            }
            // For a leaf, child_sum product is 1 (since has_children false)
            dp[c][x] = has_children ? ways : (allowed[c][x] ? 1 : 0);
        }
    }

    int64_t ans = (dp[0][0] + dp[0][1]) % MOD;
    ans = (ans + dp[0][2]) % MOD;
    return ans;
}

#include <cassert>
#include <vector>
#include <cstdint>

// Declaration of the function to test (from the solution above)
int64_t countPaintings(int n, const std::vector<std::pair<int,int>>& edges,
                       const std::vector<std::pair<int,int>>& fixed);

int main() {
    // Test 1: single node, no fixed -> 3 ways
    assert(countPaintings(1, {}, {}) == 3);
    // Test 2: single node, fixed to color 0 -> 1 way
    assert(countPaintings(1, {}, {{0,0}}) == 1);
    // Test 3: two nodes connected, no fixed -> 3*2 = 6 ways
    assert(countPaintings(2, {{0,1}}, {}) == 6);
    // Test 4: two nodes, fixed node0=0, node1 free -> node1 can be 1 or 2 -> 2 ways
    assert(countPaintings(2, {{0,1}}, {{0,0}}) == 2);
    // Test 5: line of 3 nodes (0-1-2), no fixed -> each internal has 2 choices -> 3*2*2? Actually root 0 has 3, child 1 has 2 (not equal to 0), child 2 has 2 (not equal to 1) total = 3*2*2 = 12? Let's verify: color 0,1,2 for root, then child1 2 choices, child2 2 choices = 12.
    assert(countPaintings(3, {{0,1},{1,2}}, {}) == 12);
    // Test 6: star with 3 leaves (center 0, leaves 1,2,3), no fixed -> center 3 colors, each leaf can pick one of the other 2 -> 3*2*2*2 = 24
    assert(countPaintings(4, {{0,1},{0,2},{0,3}}, {}) == 24);
    // Test 7: star with fixed center=0, leaves free -> center fixed, each leaf 2 choices -> 2*2*2 = 8
    assert(countPaintings(4, {{0,1},{0,2},{0,3}}, {{0,0}}) == 8);
    // Test 8: line of 3 with fixed leaf0=1 and leaf2=1, then node1 can be 0 or 2 -> 2 ways
    assert(countPaintings(3, {{0,1},{1,2}}, {{0,1},{2,1}}) == 2);
    // Test 9: two nodes fixed to same color (inconsistent) -> 0 ways
    assert(countPaintings(2, {{0,1}}, {{0,0},{1,0}}) == 0);
    // Test 10: larger tree with many nodes, no fixed: chain of 4 nodes (0-1-2-3) -> root 3, child 2, next 2, next 2 = 3*2*2*2 = 24
    assert(countPaintings(4, {{0,1},{1,2},{2,3}}, {}) == 24);
}
