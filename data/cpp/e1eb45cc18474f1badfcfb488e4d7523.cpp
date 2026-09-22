Given a tree with `n` vertices (numbered 1 to n) and an integer `k` (1 ≤ k ≤ n), write a C++ function that computes the sum over all unordered pairs of distinct vertices `(u, v)` of the number of connected subgraphs (subtrees) containing both `u` and `v`, modulo `1'000'000'007`. The tree is provided as adjacency lists. For each edge `(u, v)` in the tree, removing that edge splits the tree into two components of sizes `s` and `n-s`. For a fixed edge, a subtree containing both `u` and `v` must include vertices from both components. More precisely, for the pair `(u, v)`, the number of connected subgraphs containing both is the number of ways to choose exactly `k` vertices such that the induced subgraph is connected and includes both `u` and `v`. The final answer is the sum over all distinct `(u, v)` of this count. Computations must be done modulo `1'000'000'007`. The function signature is `int treeConnectedSubgraphs(int n, int k, const std::vector<std::vector<int>>& adj)`, where `adj` is 0-indexed.
#include <cassert>
#include <vector>

// Include your solution function here (e.g., paste the code above).

int main() {
    // Test 1: n=2, k=2, one edge -> only one connected subgraph of size 2, answer = C(2,2)=1 * 1 = 1
    {
        std::vector<std::vector<int>> adj(2);
        adj[0].push_back(1);
        adj[1].push_back(0);
        assert(treeConnectedSubgraphs(2, 2, adj) == 1);
    }
    // Test 2: n=3, path 0-1-2, k=2 -> edges: (0,1),(1,2) -> 2 subgraphs, answer = 1 * 2 = 2
    {
        std::vector<std::vector<int>> adj(3);
        adj[0].push_back(1);
        adj[1].push_back(0);
        adj[1].push_back(2);
        adj[2].push_back(1);
        assert(treeConnectedSubgraphs(3, 2, adj) == 2);
    }
    // Test 3: n=3, path, k=3 -> one subgraph (whole tree), answer = C(3,2)=3 * 1 = 3
    {
        std::vector<std::vector<int>> adj(3);
        adj[0].push_back(1);
        adj[1].push_back(0);
        adj[1].push_back(2);
        adj[2].push_back(1);
        assert(treeConnectedSubgraphs(3, 3, adj) == 3);
    }
    // Test 4: n=4, star centered at 0, k=2 -> edges: (0,1),(0,2),(0,3) -> 3 subgraphs, answer = 1*3=3
    {
        std::vector<std::vector<int>> adj(4);
        for (int i = 1; i < 4; ++i) {
            adj[0].push_back(i);
            adj[i].push_back(0);
        }
        assert(treeConnectedSubgraphs(4, 2, adj) == 3);
    }
    // Test 5: n=4, star, k=3 -> subgraphs: any two leaves plus center -> C(3,2)=3 such subgraphs, answer = C(3,2)=3 * 3 = 9
    {
        std::vector<std::vector<int>> adj(4);
        for (int i = 1; i < 4; ++i) {
            adj[0].push_back(i);
            adj[i].push_back(0);
        }
        assert(treeConnectedSubgraphs(4, 3, adj) == 9);
    }
    // Test 6: k=1 always 0
    {
        std::vector<std::vector<int>> adj(2);
        adj[0].push_back(1);
        adj[1].push_back(0);
        assert(treeConnectedSubgraphs(2, 1, adj) == 0);
    }
    // Test 7: n=1, k=1 -> 0
    {
        std::vector<std::vector<int>> adj(1);
        assert(treeConnectedSubgraphs(1, 1, adj) == 0);
    }
    // Test 8: n=5, chain 0-1-2-3-4, k=3 -> number of connected subgraphs of size 3? Chains: length 3 consecutive vertices: (0,1,2),(1,2,3),(2,3,4) -> 3 subgraphs, answer = C(3,2)=3 * 3 = 9
    {
        std::vector<std::vector<int>> adj(5);
        for (int i = 0; i < 4; ++i) {
            adj[i].push_back(i+1);
            adj[i+1].push_back(i);
        }
        assert(treeConnectedSubgraphs(5, 3, adj) == 9);
    }
    // Test 9: larger tree: n=6, path 0-1-2-3-4-5, k=4 -> subgraphs of size 4: (0,1,2,3),(1,2,3,4),(2,3,4,5) -> 3, answer = C(4,2)=6 * 3 = 18
    {
        std::vector<std::vector<int>> adj(6);
        for (int i = 0; i < 5; ++i) {
            adj[i].push_back(i+1);
            adj[i+1].push_back(i);
        }
        assert(treeConnectedSubgraphs(6, 4, adj) == 18);
    }
    // Test 10: k > n -> 0
    {
        std::vector<std::vector<int>> adj(3);
        adj[0].push_back(1);
        adj[1].push_back(0);
        adj[1].push_back(2);
        adj[2].push_back(1);
        assert(treeConnectedSubgraphs(3, 5, adj) == 0);
    }
    return 0;
}
#include <vector>
#include <cstdint>

using int64 = long long;
const int MOD = 1000000007;

// Count connected subgraphs of size k in a tree, then return C(k,2) * count mod MOD.
int treeConnectedSubgraphs(int n, int k, const std::vector<std::vector<int>>& adj) {
    if (k > n || k < 2) return 0;

    std::vector<int> parent(n, -1);
    std::vector<int> order;
    order.reserve(n);
    // DFS to get parent and traversal order (iterative to avoid recursion depth).
    std::vector<int> stack = {0};
    parent[0] = -2; // sentinel
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        order.push_back(u);
        for (int v : adj[u]) {
            if (v != parent[u]) {
                parent[v] = u;
                stack.push_back(v);
            }
        }
    }

    // dp[u][c] = number of connected subgraphs of size c in subtree of u that include u.
    std::vector<std::vector<int>> dp(n, std::vector<int>(k + 1, 0));
    std::vector<int> sub_size(n, 0);

    // Process in reverse order (post-order).
    for (int idx = n - 1; idx >= 0; --idx) {
        int u = order[idx];
        dp[u][1] = 1;
        sub_size[u] = 1;
        for (int v : adj[u]) {
            if (v == parent[u]) continue;
            // Merge child v into u.
            int max_u = std::min(sub_size[u], k);
            int max_v = std::min(sub_size[v], k);
            std::vector<int> new_dp(k + 1, 0);
            for (int a = 0; a <= max_u; ++a) {
                if (dp[u][a] == 0) continue;
                for (int b = 0; b <= max_v; ++b) {
                    if (a + b > k) break;
                    if (dp[v][b] == 0) continue;
                    new_dp[a + b] = (new_dp[a + b] + (int64)dp[u][a] * dp[v][b]) % MOD;
                }
            }
            // We must include u itself, so we carry over the combined result but shift? Actually dp[u][a] already includes u. We combine by adding child's choices.
            // Standard tree DP: start with dp[u][1]=1, then for each child, combine: new_dp[a+b] += dp[u][a] * dp[v][b], where v's dp[v][b] includes v.
            // We then assign dp[u] = new_dp, but careful: we also keep the original dp[u] for cases where we don't take any nodes from child? Actually the combination loop above already includes the option b=0? We did not include b=0 in the loop. Let's fix: We need to consider taking 0 nodes from child. So we need a separate handling.
            // Better: start with dp[u] as current, then for each child, create new_dp initialized to dp[u] (taking 0 from child), then for a>0 and b>0, add products.
            // Let's redo properly in a separate implementation.
        }
    }

    // Redo with correct merging:
    // Reset dp
    for (int i = 0; i < n; ++i) {
        std::fill(dp[i].begin(), dp[i].end(), 0);
        sub_size[i] = 1;
    }
    for (int idx = n - 1; idx >= 0; --idx) {
        int u = order[idx];
        dp[u][1] = 1;
        sub_size[u] = 1;
        for (int v : adj[u]) {
            if (v == parent[u]) continue;
            // Merge child
            int max_u = std::min(sub_size[u], k);
            int max_v = std::min(sub_size[v], k);
            std::vector<int> new_dp = dp[u]; // take 0 from child
            for (int a = 1; a <= max_u; ++a) {
                if (dp[u][a] == 0) continue;
                for (int b = 1; b <= max_v; ++b) {
                    if (a + b > k) break;
                    if (dp[v][b] == 0) continue;
                    new_dp[a + b] = (new_dp[a + b] + (int64)dp[u][a] * dp[v][b]) % MOD;
                }
            }
            dp[u] = std::move(new_dp);
            sub_size[u] += sub_size[v];
        }
    }

    int64 total = 0;
    for (int u = 0; u < n; ++u) {
        total = (total + (u == 0 ? dp[u][k] : dp[u][k])) % MOD;
    }
    // Actually each connected subgraph has a unique topmost node (closest to root 0). Since we root at 0, dp[u][k] counts subgraphs where u is the highest node. So sum all u.
    total = 0;
    for (int u = 0; u < n; ++u) {
        total = (total + dp[u][k]) % MOD;
    }

    // Compute C(k, 2) modulo MOD
    int64 comb = (int64)k * (k - 1) / 2 % MOD;
    return (int)(total * comb % MOD);
}
// The problem can be solved by re-interpreting the sum over all pairs `(u, v)` of the number of connected subgraphs of size `k` containing both `u` and `v`. For each connected subgraph `S` of size `k`, it contributes to the sum for every pair `(u, v)` that lies inside `S`. Therefore, the total sum equals the sum over all connected subgraphs `S` of size `k` of `C(|S|, 2) = C(k, 2)`, which is constant per subgraph. So the answer is simply `C(k, 2) * (number of connected subgraphs of size k in the tree)`. Counting all connected subgraphs of a given size in a tree can be done with tree DP. Root the tree at 0. For each node `u`, compute `dp[u][c]` = number of connected subgraphs of size `c` that are entirely within the subtree of `u` and include `u`. This is computed by combining children: start with `dp[u][1] = 1`, then for each child `v`, combine polynomials using knapsack: initialize a temporary array `newdp` of size up to subtree size, and for each possible `a` (already chosen from processed children) and `b` (from child's subtree), `newdp[a+b] += dp[u][a] * dp[v][b]`. After processing all children, the total number of connected subgraphs of size `k` in the whole tree is `sum_{u=0}^{n-1} dp[u][k]` minus the overcount? Actually careful: A connected subgraph has a unique highest node (closest to root). Therefore each connected subgraph is counted exactly once by the `dp` at its topmost node. So the total count is `sum_{u=0}^{n-1} dp[u][k]`. Thus the answer is `C(k,2) * sum_u dp[u][k] mod MOD`. The DP runs in `O(n k^2)` if done naively, but with careful merging it is `O(n k)` because the total work over all merges is bounded by `n * k` (each pair of nodes from different subtrees is combined at most once, and we cap at k). More precisely, the sum over all merging steps of `min(subtree_size, k) * min(child_subtree_size, k)` is `O(n k)` because it's like a quadratic cost that is bounded by `n k` when k is small, but if k can be up to n, worst-case is O(n^2) (star graph merging can cost O(n*k)). However the intended solution uses the provided snippet's approach, which computes the sum directly without enumerating all subgraphs, using the identity that the sum over pairs equals a linear combination of contributions per edge and per vertex. The snippet computes for each edge `(u,v)` the number of connected subgraphs of size `k` that cross the edge, and for each vertex the number of subgraphs containing that vertex but not crossing any incident edge in a certain way. The total sum is computed as: for each edge `(u,v)`, let `s` be size of one component (say the one containing `v` when root at 0). The number of connected subgraphs of size `k` that contain both endpoints of the edge is `C(n, k) - C(s, k) - C(n-s, k) + 0? Actually the number of connected subgraphs that contain both `u` and `v` equals the number of subgraphs that include at least one vertex from each side of the edge. But the snippet's approach is different: it counts for each edge the number of subgraphs that include exactly the edge? Wait, the snippet computes sums of `binom(size[v], k)` etc. Let me re-analyze: The snippet computes for a pair `(u,v)` the number of connected subgraphs of size k containing both. However the actual trick: For a fixed pair `(a,b)`, the number of connected subgraphs containing both is equal to `C(n, k) - (#subgraphs not containing a) - (#subgraphs not containing b) + (#subgraphs containing neither)`. But since subgraphs are connected, those counts depend on the tree structure. The snippet instead iterates over all vertices and edges and uses combinatorial identities to compute the sum over all pairs directly in O(n) after precomputing binomials. The key identity is: The sum over all pairs `(u,v)` of the number of connected subgraphs containing both equals `sum_{edge (u,v)} size(u_side)*size(v_side) * W_edge` where `W_edge` is the number of ways to choose a connected subgraph that crosses the edge, but careful. Let me derive: A connected subgraph of size k has exactly `k-1` edges. For each pair `(a,b)` inside the subgraph, it contributes 1. So total sum = sum over subgraphs S of size k of `C(k,2)`. Therefore answer = `C(k,2) * (number of connected subgraphs of size k)`. So the problem reduces to counting connected subgraphs of size k in a tree. The snippet appears to compute that count indirectly via a different formula. However for the task, we can implement a simpler O(n k) DP to count connected subgraphs of size k. Since the original snippet uses a more complex O(n) method after precomputing binomials, but we are free to choose any correct solution. For constraints not specified, we can assume n up to maybe 1000 or 2000, so O(n k) is fine. But to be safe, we can implement the DP with complexity O(n k^2) worst-case but optimize to O(n k) by capping at k. Edge cases: k=1 gives answer 0 because C(1,2)=0, correct because no pair can be in a size-1 subgraph. For k=2, each connected subgraph of size 2 is exactly an edge, so number of such subgraphs is n-1, and answer = 1 * (n-1) = n-1. For k > n, answer 0. The DP must handle modulo arithmetic. Complexity: O(n * k) time and O(n * k) space if we store dp for all nodes, but we can use a 2D array of size n x (k+1). With n up to maybe 5000 and k up to n, O(n^2) could be heavy, but we'll assume reasonable constraints. The solution will use modular arithmetic with a constant MOD = 1'000'000'007.
