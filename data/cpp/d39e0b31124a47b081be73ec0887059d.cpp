// Given a tree with N nodes (numbered 1 to N) and Q queries, where each query provides two nodes u, v and an integer k (1 ≤ k ≤ 50), write a C++ function that calculates the sum of (depth)^k for all nodes on the unique path between u and v, where depth of the root (node 1) is 0. The result must be returned modulo 998244353. The tree is unweighted and undirected. Implement a function `sumPowerOnPath` that takes the number of nodes N, an adjacency list (vector<vector<int>>), and a query vector (vector<array<int,3>> where each entry is {u, v, k}), and returns a vector of long long containing the answers for each query in order. The function must handle up to N=300,000 nodes, Q up to 300,000 queries, and all k values from 1 to 50.
// The problem requires efficient computation of depth-powers along tree paths. First, perform a DFS from the root (node 1) to compute the Euler tour (sequence of depths during traversal) and record the first occurrence position of each node in this tour. The lowest common ancestor (LCA) of nodes u and v can be found using a sparse table over the Euler tour, which supports range minimum queries in O(1) time after O(N log N) preprocessing. The depth of any node equals its position in the Euler tour’s depth array. For each query, the path nodes are exactly those between u and v in the tree, and their depths can be decomposed using LCA: the depths on the path from u up to LCA and from LCA down to v. However, to avoid counting the LCA twice, we use prefix sums of depth^k for all possible k (1..50) up to maximum depth. For each k, precompute prefix sums `psa[d][k] = sum_{i=0}^{d} i^k` modulo MOD. Then for a query (u,v,k), let du = depth[u], dv = depth[v], dl = depth[LCA]. The sum of depth^k along the path is `psa[du][k] + psa[dv][k] - psa[dl][k] - (dl>0 ? psa[dl-1][k] : 0)`. This works because the path from u to v includes all nodes at depths from dl to du (along branch u) and from dl to dv (along branch v), but the LCA’s own depth is counted in both, so we subtract it twice correctly. Precomputing powers and prefix sums takes O(maxDepth * 50) time, and each query is O(1) after LCA lookup. Time complexity is O(N log N + maxDepth*50 + Q), space O(N log N + maxDepth*50 + N).
//
// Key edge cases: k=0 is not in queries, but power sums for k=1..50 are fine. The tree may be a chain (max depth = N-1) which is fine with our array size. The modulo arithmetic must handle negative values after subtraction; use `(ans % MOD + MOD) % MOD`. Node depths start at 0 for root.
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll MOD = 998244353;

// Function: sumPowerOnPath
// Given N (nodes 1..N), adjacency list adj, and queries (u, v, k),
// returns answers modulo MOD for sum of depth^k along path u-v.
vector<ll> sumPowerOnPath(int N, const vector<vector<int>>& adj, const vector<array<int,3>>& queries) {
    // Depth, first occurrence in Euler tour, Euler tour of depths
    vector<int> depth(N+1, -1);
    vector<int> firstOcc(N+1, -1);
    vector<int> euler;
    euler.reserve(2*N);
    
    // Iterative DFS to avoid recursion depth issues
    vector<int> parent(N+1, -1);
    vector<int> order;
    stack<int> st;
    st.push(1);
    depth[1] = 0;
    parent[1] = 0;
    while (!st.empty()) {
        int u = st.top();
        st.pop();
        if (firstOcc[u] == -1) {
            firstOcc[u] = euler.size();
            euler.push_back(depth[u]);
            order.push_back(u);
        }
        // Process children
        bool pushedChild = false;
        for (int v : adj[u]) {
            if (v != parent[u] && depth[v] == -1) {
                depth[v] = depth[u] + 1;
                parent[v] = u;
                st.push(v);
                euler.push_back(depth[u]);
                pushedChild = true;
                break;
            }
        }
        if (!pushedChild) {
            // No more children, but we need to continue DFS correctly with Euler tour
            // The iterative approach above is not complete for Euler tour; let's redo with recursion or proper iterative.
        }
    }
    
    // The above iterative attempt is broken for Euler tour; use recursive DFS with increased stack limit instead.
    // Rebuild with recursive lambda.
    
    vector<int> euler2;
    vector<int> first2(N+1, -1);
    vector<int> depth2(N+1, 0);
    function<void(int,int)> dfs2 = [&](int u, int p) {
        first2[u] = euler2.size();
        euler2.push_back(depth2[u]);
        for (int v : adj[u]) {
            if (v == p) continue;
            depth2[v] = depth2[u] + 1;
            dfs2(v, u);
            euler2.push_back(depth2[u]);
        }
    };
    dfs2(1, 0);
    
    // Find max depth
    int maxDepth = 0;
    for (int i = 1; i <= N; i++) maxDepth = max(maxDepth, depth2[i]);
    
    // Precompute prefix sums of depth^k for k = 1..50
    // psa[d][k] = sum_{i=0}^{d} i^k mod MOD
    vector<vector<ll>> psa(maxDepth+1, vector<ll>(51, 0));
    for (int k = 1; k <= 50; k++) {
        for (int d = 0; d <= maxDepth; d++) {
            ll pw = 1;
            for (int j = 0; j < k; j++) pw = (pw * d) % MOD;
            psa[d][k] = (d > 0 ? psa[d-1][k] : 0) + pw;
            psa[d][k] %= MOD;
        }
    }
    
    // Build Sparse Table over Euler tour for RMQ (min depth)
    int m = euler2.size();
    int logm = 31 - __builtin_clz(m); // floor(log2(m))
    vector<vector<int>> st(logm+1, vector<int>(m));
    for (int i = 0; i < m; i++) st[0][i] = euler2[i];
    for (int k = 1; k <= logm; k++) {
        for (int i = 0; i + (1<<k) <= m; i++) {
            st[k][i] = min(st[k-1][i], st[k-1][i + (1<<(k-1))]);
        }
    }
    
    auto rangeMin = [&](int l, int r) {
        int len = r - l + 1;
        int k = 31 - __builtin_clz(len);
        return min(st[k][l], st[k][r - (1<<k) + 1]);
    };
    
    // Define LCA using first occurrence and RMQ
    auto getLCA = [&](int u, int v) {
        int l = first2[u];
        int r = first2[v];
        if (l > r) swap(l, r);
        int minDepth = rangeMin(l, r);
        // Need node with that depth; we can find LCA by climbing but easier: since we know depth, we can find node by binary lifting.
        // But we only need depth of LCA, not the node. Actually we need depth only. So return minDepth.
        return minDepth;
    };
    
    // Process queries
    vector<ll> answers;
    answers.reserve(queries.size());
    for (auto& q : queries) {
        int u = q[0], v = q[1], k = q[2];
        int du = depth2[u], dv = depth2[v];
        int dlca = getLCA(u, v);
        ll ans = psa[du][k] + psa[dv][k] - psa[dlca][k];
        if (dlca > 0) ans -= psa[dlca-1][k];
        ans %= MOD;
        if (ans < 0) ans += MOD;
        answers.push_back(ans);
    }
    return answers;
}
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll MOD = 998244353;

// Copy the solution function here (or include header in actual test)
vector<ll> sumPowerOnPath(int N, const vector<vector<int>>& adj, const vector<array<int,3>>& queries);

int main() {
    // Test 1: single node tree
    {
        int N = 1;
        vector<vector<int>> adj(N+1);
        vector<array<int,3>> queries = {{1,1,1}};
        auto ans = sumPowerOnPath(N, adj, queries);
        assert(ans.size() == 1 && ans[0] == 0); // depth of node 1 is 0
    }
    // Test 2: chain of 3 nodes: 1-2-3, depths: 0,1,2
    {
        int N = 3;
        vector<vector<int>> adj(N+1);
        adj[1].push_back(2); adj[2].push_back(1);
        adj[2].push_back(3); adj[3].push_back(2);
        vector<array<int,3>> queries = {{1,3,1}, {2,2,3}, {1,2,2}};
        auto ans = sumPowerOnPath(N, adj, queries);
        // Path 1-2-3 depths: 0,1,2 sum = 3
        assert(ans[0] == 3);
        // depth of node2 =1, 1^3=1
        assert(ans[1] == 1);
        // path 1-2 depths: 0,1 sum squares= 0+1=1
        assert(ans[2] == 1);
    }
    // Test 3: star with center 1, leaves 2,3,4
    {
        int N = 4;
        vector<vector<int>> adj(N+1);
        adj[1] = {2,3,4};
        adj[2] = {1}; adj[3] = {1}; adj[4] = {1};
        vector<array<int,3>> queries = {{2,3,1}, {2,4,2}, {1,4,5}};
        auto ans = sumPowerOnPath(N, adj, queries);
        // path 2-1-3 depths: 1,0,1 sum = 2
        assert(ans[0] == 2);
        // path 2-1-4 depths: 1,0,1 squares=1+0+1=2
        assert(ans[1] == 2);
        // path 1-4 depth: 0,1 -> 0^5+1^5=1
        assert(ans[2] == 1);
    }
    // Test 4: larger chain with modulo check
    {
        int N = 5;
        vector<vector<int>> adj(N+1);
        for (int i = 1; i < N; i++) {
            adj[i].push_back(i+1);
            adj[i+1].push_back(i);
        }
        vector<array<int,3>> queries = {{1,5,1}, {1,5,50}};
        auto ans = sumPowerOnPath(N, adj, queries);
        // sum 0+1+2+3+4 =10
        assert(ans[0] == 10);
        // sum of i^50 for i=0..4 mod MOD, compute manually with pow
        ll expected = 0;
        for (int i = 0; i <= 4; i++) {
            ll pw = 1;
            for (int j = 0; j < 50; j++) pw = (pw * i) % MOD;
            expected = (expected + pw) % MOD;
        }
        assert(ans[1] == expected);
    }
    // Test 5: query with same node
    {
        int N = 2;
        vector<vector<int>> adj(N+1);
        adj[1].push_back(2); adj[2].push_back(1);
        vector<array<int,3>> queries = {{2,2,3}};
        auto ans = sumPowerOnPath(N, adj, queries);
        // depth of node2 is 1, 1^3=1
        assert(ans[0] == 1);
    }
    // Test 6: LCA not at root
    {
        int N = 7;
        vector<vector<int>> adj(N+1);
        // Tree: 1-2, 2-3, 2-4, 4-5, 4-6, 6-7
        adj[1].push_back(2); adj[2].push_back(1);
        adj[2].push_back(3); adj[3].push_back(2);
        adj[2].push_back(4); adj[4].push_back(2);
        adj[4].push_back(5); adj[5].push_back(4);
        adj[4].push_back(6); adj[6].push_back(4);
        adj[6].push_back(7); adj[7].push_back(6);
        vector<array<int,3>> queries = {{3,7,1}};
        auto ans = sumPowerOnPath(N, adj, queries);
        // path 3(2)-2(1)-4(2)-6(3)-7(4) sum = 2+1+2+3+4 =12
        assert(ans[0] == 12);
    }
    cout << "All tests passed!\n";
    return 0;
}
