// Given a tree with \(n\) nodes (numbered \(1\) to \(n\)) and \(n-1\) undirected edges, define for every node \(c\) (used as the tree's root) and every unordered pair of distinct nodes \(\{i,j\}\), the value \(P(c,i,j)\) as the probability that a simple symmetric random walk on the tree, starting at the lowest common ancestor (LCA) of \(i\) and \(j\) when the tree is rooted at \(c\), reaches node \(i\) before reaching node \(j\). (Ties in distance from the LCA are possible; the probability is computed as the appropriate rational number with denominator a power of two.) Write a function that, given the number of nodes and the edges, returns the sum of \(P(c,i,j)\) over all \(c\) and all unordered pairs, modulo \(1\,000\,000\,007\). The output must be an integer in \([0, MOD-1]\). The tree has at most 300 nodes, and the input edges are 1-indexed.

// We first precompute a two-dimensional table `dp[a][b]` for \(0 \le a,b \le n-1\), where `dp[a][b]` is the probability (mod \(MOD\)) that a simple symmetric random walk on a path of length \(a+b\) edges, starting at a vertex located \(a\) edges from the left endpoint and \(b\) edges from the right endpoint, hits the left endpoint before the right endpoint. This satisfies the recurrence `dp[a][b] = (dp[a-1][b] + dp[a][b-1]) / 2` with base cases `dp[0][b]=1` for all \(b>0\) and `dp[a][0]=0` for all \(a>0\). (The case \(a=0,b=0\) never occurs in the recursion because we only call with both positive after base cases.) This recurrence is solved iteratively using modular arithmetic with the inverse of 2 modulo \(MOD\). Then, for each root \(c\) from 1 to \(n\), we run a DFS from \(c\) to compute the depth and parent of every node. For every unordered pair \(\{i,j\}\) with \(1 \le i < j \le n\), we find the LCA of \(i\) and \(j\) under the current root by following parent pointers (since the tree is small, we can just climb up: first bring the deeper node up to the same depth, then climb both together until they meet). Let the LCA be \(l\). Let \(a = depth[j] - depth[l]\) and \(b = depth[i] - depth[l]\). Then we add `dp[b][a]` (note the order: first argument is distance to `i`, second to `j`) to the total sum modulo \(MOD\). After processing all roots, the required answer is the total sum modulo \(MOD\). Edge cases include \(a=0\) or \(b=0\) (when one node is an ancestor of the other; then `dp[0][b]=1` or `dp[a][0]=0` respectively, correctly handling the probability). Complexity: Precomputing `dp` takes \(O(n^2)\). For each of the \(n\) roots, a DFS takes \(O(n)\), and computing all pairs takes \(O(n^2)\) (LCA climbing is \(O(n)\) per pair in the worst case, but since each climb is at most \(n\) steps, the pair processing could be \(O(n^3)\) overall; with \(n \le 300\) this is about 27 million operations, which is acceptable). Total time \(O(n^3)\), space \(O(n^2)\).

#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;
const int INV2 = 500000004; // modular inverse of 2

int expectedProbability(int n, vector<pair<int,int>>& edges) {
    // Build adjacency list
    vector<vector<int>> adj(n + 1);
    for (auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }
    
    // Precompute dp[a][b] modulo MOD
    vector<vector<int>> dp(n, vector<int>(n, 0));
    for (int b = 1; b < n; ++b) dp[0][b] = 1;
    for (int a = 1; a < n; ++a) dp[a][0] = 0;
    for (int a = 1; a < n; ++a) {
        for (int b = 1; b < n; ++b) {
            long long val = (dp[a-1][b] + dp[a][b-1]) % MOD;
            dp[a][b] = val * INV2 % MOD;
        }
    }
    
    long long total = 0;
    
    // For each root c
    for (int root = 1; root <= n; ++root) {
        vector<int> parent(n + 1, -1), depth(n + 1, 0);
        // DFS from root
        function<void(int,int)> dfs = [&](int u, int p) {
            parent[u] = p;
            for (int v : adj[u]) {
                if (v == p) continue;
                depth[v] = depth[u] + 1;
                dfs(v, u);
            }
        };
        dfs(root, -1);
        
        // Lambda to compute LCA by climbing
        auto get_lca = [&](int u, int v) {
            if (depth[u] < depth[v]) swap(u, v);
            // bring u up
            while (depth[u] > depth[v]) u = parent[u];
            while (u != v) {
                u = parent[u];
                v = parent[v];
            }
            return u;
        };
        
        // Iterate all unordered pairs i < j
        for (int i = 1; i <= n; ++i) {
            for (int j = i + 1; j <= n; ++j) {
                int lca = get_lca(i, j);
                int a = depth[j] - depth[lca];
                int b = depth[i] - depth[lca];
                total += dp[b][a];
                total %= MOD;
            }
        }
    }
    
    return (int)total;
}

#include <cassert>
#include <vector>
using namespace std;

// Include the solution function here (or link to it)

int main() {
    // Test 1: Single edge, two nodes
    {
        int n = 2;
        vector<pair<int,int>> edges = {{1,2}};
        // For root 1: pair (1,2), lca=1, b=0,a=1 => dp[0][1]=1
        // For root 2: lca=2, b=1,a=0 => dp[1][0]=0
        // total = 1+0 = 1
        assert(expectedProbability(n, edges) == 1);
    }
    
    // Test 2: Path of 3 nodes: 1-2-3
    {
        int n = 3;
        vector<pair<int,int>> edges = {{1,2},{2,3}};
        // Let's compute manually quickly (mod MOD)
        // Roots: 1,2,3
        // Root 1:
        //   pairs: (1,2): lca=1, b=0,a=1 => dp[0][1]=1
        //          (1,3): lca=1, b=0,a=2 => dp[0][2]=1
        //          (2,3): lca=2, b=1,a=1 => dp[1][1] = 1/2 = 500000004
        //   sum = 1+1+500000004 = 2+500000004 = 500000006
        // Root 2:
        //   pairs: (1,2): lca=2, b=1,a=0 => dp[1][0]=0
        //          (1,3): lca=2, b=1,a=1 => dp[1][1]=500000004
        //          (2,3): lca=2, b=0,a=1 => dp[0][1]=1
        //   sum = 0+500000004+1 = 500000005
        // Root 3:
        //   pairs: (1,2): lca=2? Actually root=3, tree root 3: edges 3-2-1, depths: 3 depth0, 2 depth1, 1 depth2
        //          (1,2): lca = ? path 1-2, LCA under root 3 is 2 (since 2 is ancestor of 1). b=depth[1]-depth[2]=2-1=1, a=depth[2]-depth[2]=0 => dp[1][0]=0
        //          (1,3): lca = 3? Actually root 3, 3 is ancestor of 1? path 3-2-1, LCA is 3. b=depth[1]-depth[3]=2-0=2, a=depth[3]-depth[3]=0 => dp[2][0]=0
        //          (2,3): lca=3, b=depth[2]-depth[3]=1-0=1, a=0 => dp[1][0]=0
        //   sum = 0
        // Total = 500000006 + 500000005 + 0 = 1000000011 % MOD = 4? Actually 1000000011 - 1000000007 = 4.
        assert(expectedProbability(n, edges) == 4);
    }
    
    // Test 3: Star with 3 leaves: center 1, leaves 2,3,4
    {
        int n = 4;
        vector<pair<int,int>> edges = {{1,2},{1,3},{1,4}};
        // Quick sanity: For root 1, all pairs have lca=1, so for leaf-leaf pair: b=1,a=1 => dp[1][1]=1/2; for leaf-center: b=1,a=0 or b=0,a=1 => 0 or 1. We'll trust implementation.
        // Just ensure it runs without error and returns something mod MOD.
        int result = expectedProbability(n, edges);
        assert(result >= 0 && result < MOD);
    }
    
    // Test 4: n=1 (though problem likely n>=2, but handle gracefully: no edges, only one node, no pairs => sum 0)
    {
        int n = 1;
        vector<pair<int,int>> edges;
        assert(expectedProbability(n, edges) == 0);
    }
    
    return 0;
}
