/*
Given a rooted tree with `n` nodes (1 ≤ n ≤ 100,000), where each node `i` has two possible values `l[i]` and `r[i]` (both integers that fit in a 64-bit signed type), your task is to write a C++ function `long long maxPathSum(int n, const std::vector<int>& L, const std::vector<int>& R, const std::vector<std::pair<int,int>>& edges)` that computes the maximum possible sum of edge weights along a path from the root (node 1) to any node, where the weight of an edge connecting parent `p` to child `c` is `abs(value[p] - value[c])`. For each node, you may independently choose its value to be either `L[i]` or `R[i]`. The root has no parent, so no value is chosen for it from an edge; instead, the total sum is simply the sum of chosen edge weights along the path. The function must return the maximum possible total sum for the entire tree (the maximum over all paths from root to leaf). The tree is undirected but rooted at node 1, and the input edges list is guaranteed to form a tree (no cycles, connected). The function should handle multiple test cases? No—this task is for a single tree per call. The function must be efficient for large `n`; the expected solution runs in O(n) time and O(n) space.
*/
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <functional>

// Compute the maximum root-to-node path sum where each node can choose one of two given values.
// L[i] and R[i] are 0-indexed arrays for node i+1.
// edges is a list of undirected edges (1-indexed node numbers).
long long maxPathSum(int n, const std::vector<int>& L, const std::vector<int>& R, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list (0-indexed internally)
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        int u = e.first - 1;
        int v = e.second - 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // dp[node][0] = best sum when node uses L[node], dp[node][1] when uses R[node]
    std::vector<std::vector<long long>> dp(n, std::vector<long long>(2, 0));

    // DFS from root (node 0). parent is -1 for root.
    std::function<void(int, int)> dfs = [&](int node, int parent) {
        // For root, dp is already 0; for others, it will be set by parent's call.
        // Process all children.
        for (int nxt : adj[node]) {
            if (nxt == parent) continue;
            // Compute dp for child nxt based on parent node's two states.
            long long bestIfChildUsesL = std::max(
                dp[node][0] + std::abs((long long)L[node] - L[nxt]),
                dp[node][1] + std::abs((long long)R[node] - L[nxt])
            );
            long long bestIfChildUsesR = std::max(
                dp[node][0] + std::abs((long long)L[node] - R[nxt]),
                dp[node][1] + std::abs((long long)R[node] - R[nxt])
            );
            dp[nxt][0] = bestIfChildUsesL;
            dp[nxt][1] = bestIfChildUsesR;
            dfs(nxt, node);
        }
    };

    dfs(0, -1);

    // Find the maximum over all nodes of max(dp[node][0], dp[node][1])
    long long answer = 0;
    for (int i = 0; i < n; ++i) {
        answer = std::max(answer, std::max(dp[i][0], dp[i][1]));
    }
    return answer;
}
#include <cassert>
#include <vector>
#include <utility>

// (The solution function is assumed to be defined above.)
long long maxPathSum(int n, const std::vector<int>& L, const std::vector<int>& R, const std::vector<std::pair<int,int>>& edges);

int main() {
    // Test 1: Single node -> no edges, answer 0
    assert(maxPathSum(1, {5}, {7}, {}) == 0);

    // Test 2: Two nodes, root=1, child=2. L1=1,R1=10, L2=5,R2=20.
    // Edge weight options: |1-5|=4, |1-20|=19, |10-5|=5, |10-20|=10 -> max is 19.
    assert(maxPathSum(2, {1,5}, {10,20}, {{1,2}}) == 19);

    // Test 3: Chain 1-2-3. L1=0,R1=100, L2=10,R2=20, L3=15,R3=25.
    // Path 1->2: choose 100 at root, 10 at child -> |100-10|=90, then from 2 to 3: |10-25|=15 or |20-25|=5, best 15 total 105.
    // Also consider root=0, child=20 -> 20, then 20->15=5 total 25. So max 105.
    assert(maxPathSum(3, {0,10,15}, {100,20,25}, {{1,2},{2,3}}) == 105);

    // Test 4: Star with root 1, leaves 2,3. L=0,10,10, R=100,20,20.
    // Path to leaf2: max edge = |100-10|=90 or |100-20|=80 -> 90. Path to leaf3 same 90. Answer 90.
    assert(maxPathSum(3, {0,10,10}, {100,20,20}, {{1,2},{1,3}}) == 90);

    // Test 5: More complex: root with two children, each child has its own child.
    // Root L=0,R=100. Child2 L=10,R=20. Child3 L=15,R=25.
    // For path to grandchild under 2: best to 2 is 90 (100->10), then to grandchild (L=1,R=99): |10-99|=89 or |20-99|=79, best total 90+89=179.
    // For path to grandchild under 3: best to 3: |100-15|=85 or |100-25|=75, best 85. Then grandchild (L=2,R=98): |15-98|=83 or |25-98|=73, best 85+83=168. So max 179.
    assert(maxPathSum(5, {0,10,15,1,2}, {100,20,25,99,98}, {{1,2},{2,4},{1,3},{3,5}}) == 179);

    // Test 6: Negative values. Root L=-5,R=5, child L=-10,R=10.
    // Absolute differences: |-5-(-10)|=5, |-5-10|=15, |5-(-10)|=15, |5-10|=5 -> max 15.
    assert(maxPathSum(2, {-5,-10}, {5,10}, {{1,2}}) == 15);

    // Test 7: All same values, any choice yields 0.
    assert(maxPathSum(4, {3,3,3,3}, {3,3,3,3}, {{1,2},{2,3},{3,4}}) == 0);

    // Test 8: Larger tree with many branches, verify via brute force for small n? Here just basic consistency.
    // n=4: root 1 with children 2,3 and 3 has child 4.
    // L = {0, 50, 10, 20}, R = {100, 60, 90, 80}
    // Paths: 1->2: max(|0-50|=50,|0-60|=60,|100-50|=50,|100-60|=40) -> 60.
    // 1->3: max(|0-10|=10,|0-90|=90,|100-10|=90,|100-90|=10) -> 90.
    // 1->3->4: from root to 3 choose 100 (root) and 10 (node3) = 90. Then from 3 to 4: |10-20|=10 or |10-80|=70 or |90-20|=70 or |90-80|=10 -> max 70. Total 160.
    // So answer 160.
    assert(maxPathSum(4, {0,50,10,20}, {100,60,90,80}, {{1,2},{1,3},{3,4}}) == 160);

    return 0;
}
// The problem is a classic tree DP with two states per node because each node chooses between two possible values. For each node `u`, define `dp[u][0]` as the maximum sum of edge weights along any path from the root down to `u` when `u` takes value `L[u]`, and `dp[u][1]` similarly when `u` takes `R[u]`. For the root, both states are initialized to 0 (no incoming edge). For a non-root node `u` with parent `p`, the recurrence is:
// - `dp[u][0] = max( dp[p][0] + abs(L[p] - L[u]), dp[p][1] + abs(R[p] - L[u]) )`
// - `dp[u][1] = max( dp[p][0] + abs(L[p] - R[u]), dp[p][1] + abs(R[p] - R[u]) )`
// But because the tree is undirected and we want the maximum over all root-to-leaf paths, we need to combine contributions from all children. However, the problem statement says "maximum possible sum of edge weights along a path from the root to any node" — that means we are summing along a single path, not summing across all branches. Therefore, the DP should compute the best root-to-node sum for each node, and the answer is the maximum over all nodes of `max(dp[u][0], dp[u][1])`. Since the root has no parent, we set `dp[1][0] = dp[1][1] = 0` and then for each child we recursively compute their DP values using the parent's two states. The recurrence for a child `v` given parent `u` is exactly:
// - `dp[v][0] = max( dp[u][0] + abs(L[u] - L[v]), dp[u][1] + abs(R[u] - L[v]) )`
// - `dp[v][1] = max( dp[u][0] + abs(L[u] - R[v]), dp[u][1] + abs(R[u] - R[v]) )`
// We run a DFS from root, computing dp for each child and then after finishing the DFS, we scan all nodes for the maximum of `dp[node][0]` and `dp[node][1]`. Since the tree is undirected, we must avoid revisiting the parent node. Use adjacency list. Edge cases: single node tree — answer is 0 because there are no edges. Negative values in L and R? The problem says integers, but since we use abs, they can be negative; the DP works regardless. Large values up to 10^9 require 64-bit. Time complexity O(n) because each edge is visited twice in DFS, space O(n) for adjacency and dp arrays.
