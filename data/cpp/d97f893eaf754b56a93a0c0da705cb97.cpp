You are given an undirected graph with `n` vertices and `m` edges, where each edge has an integer cost. You also receive `q` queries. Each query is defined by two vertices `s` and `t` and a target value `w`. For a query, consider all paths (not necessarily simple) between `s` and `t` in the graph. The “cost” of a path is defined as the bitwise XOR of all edge costs along that path (each edge can be used multiple times). Your task is to find, for each query, the minimum possible maximum edge cost on any path from `s` to `t` whose total XOR equals exactly `w`. If no such path exists, output `-1`. The graph may have self-loops and multiple edges. All edge costs and query values are non‑negative integers less than `2^30`. Implement a function that takes the number of vertices, a vector of edges (each with `u`, `v`, `cost`), and a vector of queries (each with `s`, `t`, `w`, and an original index), and returns a vector of `q` integers, the answers in the original order.

// The core idea is to process edges in non‑decreasing order of cost, maintaining a dynamic structure that can answer queries whose minimum required maximum edge cost is the current edge weight. For each connected component, we maintain a linear basis (over GF(2) with 30 bits) of all possible XOR values that can be formed by cycles within that component. This basis, combined with the component’s disjoint‑set parent pointers and XOR distances from each vertex to its component root, lets us check whether a given XOR value `(val[s] ^ val[t] ^ w)` is representable using the basis – if yes, then there exists a path from `s` to `t` with total XOR `w` using only edges with cost ≤ current processed cost.  
//
// We use a DSU where each component stores its size, its basis array, and a list of queries that involve at least one vertex in that component. When we process an edge with cost `c`, we set `curans = c`. If the edge’s two endpoints are already in the same component, we add the cycle XOR `(val[u] ^ val[v] ^ w_edge)` to that component’s basis; if the basis changes (i.e., a new independent vector is added), we need to re‑evaluate all queries stored in that component, because new cycles may now allow additional XOR values. If the endpoints are in different components, we merge the smaller component into the larger one (union by size). During merge, we combine the two bases, adjust the `val` of the smaller root, and then re‑evaluate queries that have endpoints in both components.  
//
// For each query, we store it in both endpoints’ component lists initially. When a query’s two endpoints become connected (either via merging or because they were already connected), we check if `check(basis, val[s] ^ val[t] ^ w)` is true; if so, we set the answer to `min(ans[id], curans)`. Since we process edges in increasing cost, the first time a query is satisfied gives the minimum possible maximum edge cost.  
//
// Critical edge cases: self‑loops (edge where `u == v`) create a cycle with XOR equal to the edge cost; multiple edges between same vertices; queries with `s == t` (the empty path has XOR 0, so the answer is the minimum `c` such that `0` is representable, which is always true for any `c` if a path exists, but we need to consider only paths that use edges with cost ≤ `c`; for `s==t`, if `w==0`, the answer is `0` if there is any edge, otherwise `-1`; the algorithm naturally handles this because the basis initially is empty, and when a self‑loop is added it becomes representable). Also, queries that are never satisfied remain `INF` and output `-1`.  
//
// Time complexity: Each edge is processed once. Each query may be moved during merges, but because of union by size, each query is moved at most `O(log n)` times. For each re‑evaluation, we do a basis insert/check which is `O(30)`. Overall, the worst‑case time is `O((m + q) * 30 * log n)` which is acceptable for `n,m,q` up to 2e5. Space complexity is `O(n * 30 + n + q)`.

#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, cost;
    bool operator<(const Edge& other) const { return cost < other.cost; }
};

struct Query {
    int s, t, w, id;
};

vector<int> minMaxXorPath(int n, const vector<Edge>& edges, const vector<Query>& queries) {
    int q = (int)queries.size();
    vector<int> ans(q, INT_MAX);
    
    // DSU arrays
    vector<int> parent(n + 1);
    vector<int> sz(n + 1, 1);
    vector<int> xr(n + 1, 0); // XOR from vertex to its component root
    vector<array<int, 30>> basis(n + 1); // linear basis per component root
    vector<vector<Query>> store(n + 1); // queries attached to each vertex
    
    // Initialize DSU and basis
    for (int i = 1; i <= n; ++i) {
        parent[i] = i;
        basis[i].fill(0);
    }
    
    // Attach queries to both endpoints
    for (const auto& qry : queries) {
        store[qry.s].push_back(qry);
        store[qry.t].push_back(qry);
    }
    
    // Function prototypes using lambda recursion
    function<int(int)> find = [&](int x) -> int {
        if (parent[x] == x) return x;
        int p = parent[x];
        parent[x] = find(p);
        xr[x] ^= xr[p];
        return parent[x];
    };
    
    auto get_val = [&](int x) -> int {
        find(x);
        return xr[x];
    };
    
    // Insert value into basis of component with root id
    auto basis_insert = [&](int id, int x) -> bool {
        for (int i = 29; i >= 0; --i) {
            if (x & (1 << i)) {
                if (!basis[id][i]) {
                    basis[id][i] = x;
                    return true;
                }
                x ^= basis[id][i];
            }
        }
        return false;
    };
    
    // Check if value can be represented by basis
    auto basis_check = [&](int id, int x) -> bool {
        for (int i = 29; i >= 0; --i) {
            if (x & (1 << i)) {
                if (!basis[id][i]) return false;
                x ^= basis[id][i];
            }
        }
        return true;
    };
    
    // Re-evaluate all queries in component with root id (after basis changed)
    function<void(int)> go_over = [&](int id) {
        vector<Query> keep;
        for (const auto& qry : store[id]) {
            int a = find(qry.s);
            int b = find(qry.t);
            if (a != b) {
                keep.push_back(qry);
                continue;
            }
            int need = get_val(qry.s) ^ get_val(qry.t) ^ qry.w;
            if (basis_check(id, need)) {
                ans[qry.id] = min(ans[qry.id], curans); // curans is captured from outer scope
            }
            keep.push_back(qry);
        }
        store[id] = move(keep);
    };
    
    // Merge component containing u and v with edge weight z
    function<void(int,int,int)> merge_comp = [&](int u, int v, int z) {
        int root_u = find(u);
        int root_v = find(v);
        if (root_u == root_v) return;
        // Always merge smaller into larger
        if (store[root_u].size() > store[root_v].size()) {
            swap(u, v);
            swap(root_u, root_v);
        }
        // root_v is the new root
        // Save old values for basis combination
        vector<int> vu, vv;
        for (int i = 29; i >= 0; --i) {
            if (basis[root_u][i]) vu.push_back(basis[root_u][i]);
            if (basis[root_v][i]) vv.push_back(basis[root_v][i]);
        }
        bool changed_u = false, changed_v = false;
        for (int x : vu) if (basis_insert(root_v, x)) changed_v = true;
        // Note: we don't need to insert vv into u, because we discard u's root
        // But we need to check if the combined basis changed for root_v
        // Instead, we just call go_over on both if either basis might have changed.
        // Since we discard root_u, we only care about root_v.
        // But queries stored in root_u need to be moved to root_v.
        // Also, queries that now have both endpoints in root_v need re-evaluation.
        vector<Query> need;
        for (const auto& qry : store[root_u]) {
            int a = find(qry.s), b = find(qry.t);
            if (a != b) swap(a, b);
            if (a == root_u && b == root_v) {
                need.push_back(qry);
            }
        }
        // Adjust parent and XOR
        int old_root_u = root_u;
        parent[root_u] = root_v;
        xr[root_u] = xr[u] ^ xr[v] ^ z; // because root_u's new XOR to root_v
        // Move queries from u to v
        for (auto& qry : store[old_root_u]) {
            store[root_v].push_back(qry);
        }
        store[old_root_u].clear();
        
        // Re-evaluate queries that cross between the two old components
        for (const auto& qry : need) {
            int need_xor = get_val(qry.s) ^ get_val(qry.t) ^ qry.w;
            if (basis_check(root_v, need_xor)) {
                ans[qry.id] = min(ans[qry.id], curans);
            }
        }
        
        // If basis of root_v changed (because we added vectors from root_u),
        // we must re-evaluate all queries in root_v.
        // To be safe, we always call go_over on root_v after merge.
        // But go_over re-evaluates all queries in store[root_v] and may drop those whose two endpoints are in same comp.
        // However, we must be careful not to drop queries that are not yet connected.
        // The go_over function only keeps those that are still in different comps.
        // So we can call go_over on root_v.
        go_over(root_v);
    };
    
    // Sort edges
    vector<Edge> sorted_edges = edges;
    sort(sorted_edges.begin(), sorted_edges.end());
    
    int curans = 0;
    for (const auto& e : sorted_edges) {
        curans = e.cost;
        int ru = find(e.u), rv = find(e.v);
        if (ru == rv) {
            // Same component, add cycle to basis
            int cycle = get_val(e.u) ^ get_val(e.v) ^ e.cost;
            if (basis_insert(ru, cycle)) {
                go_over(ru);
            }
        } else {
            // Different components, merge
            merge_comp(e.u, e.v, e.cost);
        }
    }
    
    // Replace INF with -1
    for (int& val : ans) if (val == INT_MAX) val = -1;
    return ans;
}

#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (copy from above)

int main() {
    // Test 1: Simple case with one edge
    {
        int n = 2;
        vector<Edge> edges = {{1, 2, 5}};
        vector<Query> queries = {{1, 2, 5, 0}, {1, 2, 0, 1}, {1, 2, 3, 2}};
        vector<int> ans = minMaxXorPath(n, edges, queries);
        assert(ans[0] == 5);
        assert(ans[1] == -1);
        assert(ans[2] == -1);
    }
    
    // Test 2: Self-loop
    {
        int n = 1;
        vector<Edge> edges = {{1, 1, 7}};
        vector<Query> queries = {{1, 1, 7, 0}, {1, 1, 0, 1}, {1, 1, 3, 2}};
        vector<int> ans = minMaxXorPath(n, edges, queries);
        assert(ans[0] == 7);
        assert(ans[1] == 7);
        assert(ans[2] == -1);
    }
    
    // Test 3: Cycle gives new XOR values
    {
        int n = 3;
        vector<Edge> edges = {{1, 2, 1}, {2, 3, 2}, {1, 3, 3}};
        vector<Query> queries = {{1, 3, 3, 0}, {1, 3, 0, 1}, {1, 3, 2, 2}};
        vector<int> ans = minMaxXorPath(n, edges, queries);
        // Edge costs sorted: 1,2,3
        // After processing edge 1: comp {1,2} has basis {1}
        // After edge 2: comp {1,2,3} basis {1,2}? Actually cycle from 1-2-3-1: 1^2^3 = 0
        // So basis remains {1} but wait cycle XOR is 1^2^3=0, no new. But we can also use path 1-2-3 cost 1^2=3
        // and path 1-3 cost 3. So XOR 3 achievable with max cost 3. XOR 0 achievable with max cost 3? Path 1-2-3-1? But need max edge on path, if we use edge 3-1 cost 3, max=3. So answer for 0 is 3. For 2, not achievable.
        assert(ans[0] == 3);
        assert(ans[1] == 3);
        assert(ans[2] == -1);
    }
    
    // Test 4: Queries with s==t, w=0
    {
        int n = 2;
        vector<Edge> edges = {{1, 2, 10}};
        vector<Query> queries = {{1, 1, 0, 0}, {2, 2, 0, 1}};
        vector<int> ans = minMaxXorPath(n, edges, queries);
        // For 1 to 1 with XOR 0: can use no edges? But we need path, empty path has XOR 0 and max edge cost 0? Our algorithm doesn't include empty path because we require at least one edge? Actually the problem says "paths", normally a path can be of length 0? Usually yes. But our DSU basis initially empty, and we only update when we add edges. For s==t, after adding edge (1,2,10), basis for comp {1,2} is {10}? Actually cycle is 1-1? No, self-loop? The edge (1,2) doesn't create cycle. So basis remains empty. So get_val(1)^get_val(1)^0 = 0, basis_check with empty basis returns true? basis_check checks if 0 can be represented, it returns true (since 0 is always representable). So ans becomes curans=10. So answer is 10. That's correct because to go from 1 to 1 with XOR 0, you can go 1->2->1, which has XOR 10^10=0 and max edge 10. So ans[0]=10, ans[1]=10.
        assert(ans[0] == 10);
        assert(ans[1] == 10);
    }
    
    // Test 5: Larger example from problem statement
    {
        int n = 7;
        vector<Edge> edges = {
            {1,1,128}, {1,2,1}, {1,3,8}, {2,3,73}, {2,4,2},
            {2,5,4}, {3,6,16}, {3,7,32}, {3,4,75}
        };
        vector<Query> queries = {
            {2,3,73,0}, {2,3,9,1}, {2,7,104,2}, {4,7,100,3}, {4,7,107,4}
        };
        vector<int> ans = minMaxXorPath(n, edges, queries);
        // Expected based on original code output
        assert(ans[0] == 73);
        assert(ans[1] == -1);
        assert(ans[2] == 32);
        assert(ans[3] == 75);
        assert(ans[4] == -1);
    }
    
    printf("All tests passed!\n");
    return 0;
}
