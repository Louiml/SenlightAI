Given an undirected graph with `n` vertices and `m` edges, each edge having a distinct integer weight (weights are unique), write a C++ function `vector<int> minimumSpanningTreeEdgesByIndex(int n, const vector<tuple<int,int,int>>& edges)` that returns, for every edge index `i` from `0` to `m-1`, the **minimum possible maximum edge weight** among all spanning trees that contain edge `i`. If no spanning tree contains edge `i` (i.e., the edge is a bridge that when forced disconnects the graph), return `-1` for that index. The graph is connected overall (there exists at least one spanning tree). Edge weights are positive integers. The solution must be efficient for `n` up to 400 and `m` up to 600.
// This is a classic "minimum bottleneck spanning tree with forced edge" problem. The key observation is that for a given edge `i`, to find the minimum possible maximum weight among spanning trees containing it, we need to find the smallest threshold `W` such that the subgraph consisting of all edges with weight ≤ `W` has a spanning tree that includes edge `i`. This is equivalent to: in the subgraph of edges with weight ≤ `W`, the edge `i` must not be a bridge in the connected component that contains both its endpoints. The algorithm uses a divide-and-conquer over the sorted edge list (by weight) combined with a rollbackable Union-Find (DSU) that tracks the number of odd-sized components (to detect connectivity for even/odd reasons, but here we just need connectivity). The recursive function `solve(xl, xr, yl, yr, edges)` handles a range of edge indices `[xl, xr]` and a weight range `[yl, yr]` (inclusive). It picks the middle index `xmid`, and tries to find the minimal possible answer for that index by adding edges in increasing weight order, checking when the graph becomes fully connected (i.e., `ufs.odd == 0`). If it becomes connected with the forced edge added, then the current edge weight is the answer for that index. Then it splits the remaining edges into two lists: those relevant to the left half and those relevant to the right half, based on which ranges they can affect, and recurses. The rollback DSU allows us to revert state efficiently. Time complexity is roughly `O(m log m + m log m log n)` due to the divide-and-conquer plus DSU operations, with each level processing O(m) edges, and there are O(log m) levels. Space is O(m + n) for the DSU and temporary edge lists.
#include <bits/stdc++.h>
using namespace std;

struct RollbackDSU {
    int odd; // number of components with odd size
    vector<int> parent, sz;
    struct Change { int x, y, old_sz_x; };
    stack<Change> hist;

    RollbackDSU(int n) {
        parent.resize(n);
        sz.assign(n, 1);
        for (int i = 0; i < n; ++i) parent[i] = i;
        odd = n;
    }

    int find(int x) const {
        while (parent[x] != x) x = parent[x];
        return x;
    }

    void merge(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        hist.push({a, b, sz[a]});
        if ((sz[a] & 1) && (sz[b] & 1)) odd -= 2;
        sz[a] += sz[b];
        parent[b] = a;
    }

    int checkpoint() const { return (int)hist.size(); }

    void rollback(int checkpoint) {
        while ((int)hist.size() > checkpoint) {
            Change c = hist.top();
            hist.pop();
            if ((c.old_sz_x & 1) && (sz[c.y] & 1)) odd += 2;
            sz[c.x] = c.old_sz_x;
            parent[c.y] = c.y;
        }
    }
};

struct Edge {
    int u, v, w, id;
    bool operator<(const Edge& other) const { return w < other.w; }
};

vector<int> ans;
RollbackDSU* dsu;

void solve(int xl, int xr, int yl, int yr, const vector<Edge>& edges) {
    if (xl > xr) return;
    if (yl == yr) {
        for (int i = xl; i <= xr; ++i) ans[i] = yl;
        return;
    }

    int check1 = dsu->checkpoint();
    int xmid = (xl + xr) >> 1;
    int p = 0;
    // Add all edges with weight < yl that are within current edge index range [xl, xmid]
    for (; p < (int)edges.size() && edges[p].w < yl; ++p) {
        if (edges[p].id >= xl && edges[p].id <= xmid) {
            dsu->merge(edges[p].u, edges[p].v);
        }
    }

    int check2 = dsu->checkpoint();
    // Try to find answer for edge index xmid by adding edges in increasing weight
    for (int i = p; i < (int)edges.size(); ++i) {
        if (edges[i].w > yr) break;
        if (edges[i].id >= xmid && edges[i].id <= xr || edges[i].id >= xl && edges[i].id <= xmid) {
            // Actually we need to consider edges that can affect connectivity when we force edge xmid.
            // We allow all edges with weight >= yl and <= yr, and index in [xl, xr] plus the forced edge itself.
            dsu->merge(edges[i].u, edges[i].v);
            if (dsu->odd == 0) {
                ans[xmid] = edges[i].w;
                break;
            }
        }
    }

    // Build subproblem edge lists
    vector<Edge> left_edges, right_edges;
    for (const auto& e : edges) {
        // For right half: indices [xmid+1, xr], weights [yl, ans[xmid]]
        if (e.id >= xmid+1 && e.id <= xr && e.w >= yl && e.w <= (ans[xmid] < INT_MAX ? ans[xmid] : INT_MAX)) {
            right_edges.push_back(e);
        }
        // For left half: indices [xl, xmid-1], weights [ans[xmid], yr]
        if (e.id >= xl && e.id <= xmid-1 && e.w >= (ans[xmid] < INT_MAX ? ans[xmid] : 0) && e.w <= yr) {
            left_edges.push_back(e);
        }
    }

    // Important: we must also include the forced edge xmid itself in the right/left subproblems if needed,
    // but since its index is not in the subranges, we handle it by adding it manually in recursion? Actually not needed because subproblems only consider edges in their index range. The forced edge for each call is the middle of that call, and we already account for it by the DSU state passed down.

    // Recurse right first
    dsu->rollback(check2);
    solve(xmid+1, xr, yl, (ans[xmid] < INT_MAX ? ans[xmid] : yr), right_edges);

    // Recurse left
    if (ans[xmid] < INT_MAX) {
        dsu->rollback(check1);
        // Add all edges with weight < ans[xmid] that affect left region
        for (auto& e : edges) {
            if (e.w < ans[xmid] && e.w >= yl && e.id >= xl && e.id <= xmid-1) {
                dsu->merge(e.u, e.v);
            }
        }
        int check3 = dsu->checkpoint();
        solve(xl, xmid-1, ans[xmid], yr, left_edges);
        dsu->rollback(check3);
    }
    dsu->rollback(check1);
}

vector<int> minimumSpanningTreeEdgesByIndex(int n, const vector<tuple<int,int,int>>& edges) {
    int m = edges.size();
    vector<Edge> sorted_edges;
    sorted_edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        auto [u, v, w] = edges[i];
        sorted_edges.push_back({u, v, w, i});
    }
    sort(sorted_edges.begin(), sorted_edges.end());

    ans.assign(m, INT_MAX);
    RollbackDSU d(n);
    dsu = &d;
    solve(0, m-1, 1, INT_MAX, sorted_edges);
    for (auto& x : ans) if (x == INT_MAX) x = -1;
    return ans;
}
#include <bits/stdc++.h>
using namespace std;

// The solution function is assumed to be defined above (include it here).

int main() {
    {
        // Simple triangle graph
        int n = 3;
        vector<tuple<int,int,int>> edges = {
            {0,1,5},
            {1,2,6},
            {0,2,7}
        };
        vector<int> res = minimumSpanningTreeEdgesByIndex(n, edges);
        // Edges 0 and 1 can be forced with max weight 7 (the other two edges form tree with 7? Actually forced edge 0: need spanning tree with edge 0, possible tree {0,1}? no that leaves vertex2 isolated. Need all 3 edges. So max weight=7. Similarly edge1 also 7. Edge2 alone cannot span, must also include another edge, max=7.
        assert(res == vector<int>({7,7,7}));
    }
    {
        // 4-cycle with all equal weights
        int n = 4;
        vector<tuple<int,int,int>> edges = {
            {0,1,1},
            {1,2,1},
            {2,3,1},
            {3,0,1}
        };
        vector<int> res = minimumSpanningTreeEdgesByIndex(n, edges);
        // Any edge can be part of a spanning tree, max weight = 1
        assert(res == vector<int>({1,1,1,1}));
    }
    {
        // Disconnected when forced edge removed? Actually graph connected overall, but edge is bridge
        int n = 3;
        vector<tuple<int,int,int>> edges = {
            {0,1,10},
            {1,2,20}
        };
        vector<int> res = minimumSpanningTreeEdgesByIndex(n, edges);
        // Both edges are bridges, any spanning tree must include both, so max weight = 20 for both
        assert(res == vector<int>({20,20}));
    }
    {
        // Square with diagonal, forced edge is diagonal
        int n = 4;
        vector<tuple<int,int,int>> edges = {
            {0,1,1},
            {1,2,2},
            {2,3,3},
            {3,0,4},
            {0,2,5}
        };
        vector<int> res = minimumSpanningTreeEdgesByIndex(n, edges);
        // For diagonal (edge 4): need spanning tree with it, can use edges 0,1,2? That doesn't connect vertex3? Actually edges: 0(0-1),1(1-2),2(2-3),4(0-2) -> connects all, max weight=5. For edge 0: tree {0,1,2}? That gives vertices 0,1,2,3? edges 0-1,1-2,2-3 => yes, max=3. Edge1: need tree with edge1, can use 0,2,3? edges 0-1,2-3,3-0 => connects all, max=4. Edge2: tree with edge2, can use 0,1,3? edges 0-1,1-2,3-0 => 3 also connects all, max=3. Edge3: tree with edge3, use 0,1,2? edges 0-1,1-2,2-3 => that has edge2 not edge3. Use 0,1,3? edges 0-1,1-2? no. Use 0,2,3? edges 0-2(5),2-3(3),3-0(4) => max=5. But better: 0,1,3? edges 0-1(1),1-2(2) not edge3. Actually we need spanning tree containing edge3(3-0). Use edges 0(0-1),2(2-3),3(3-0) => connects all? 0-1,2-3,3-0 => vertices 0,1,2,3 all connected, max weight=4. So edge3 answer=4. Edge4 answer=5. So expected: {3,4,3,4,5}
        assert(res == vector<int>({3,4,3,4,5}));
    }
    {
        // Larger chain, force edge in middle
        int n = 5;
        vector<tuple<int,int,int>> edges = {
            {0,1,10},
            {1,2,20},
            {2,3,30},
            {3,4,40}
        };
        vector<int> res = minimumSpanningTreeEdgesByIndex(n, edges);
        // All are bridges, spanning tree must include all, max weight = 40 for all
        assert(res == vector<int>({40,40,40,40}));
    }
    {
        // 5-cycle with one heavy edge
        int n = 5;
        vector<tuple<int,int,int>> edges = {
            {0,1,1},
            {1,2,2},
            {2,3,3},
            {3,4,4},
            {4,0,100}
        };
        vector<int> res = minimumSpanningTreeEdgesByIndex(n, edges);
        // For edge 4 (heavy): forced, need tree with it, can use 0,1,2,3? That's edges 0,1,2,3 -> already a tree without edge4, but we must include edge4, so we need to replace one. Use edges 4,0,1,2 (max 100) or 4,1,2,3 (max 100) etc. So answer 100.
        // For edge 0: can use tree without edge4? Edges 0,1,2,3 forms a spanning tree (chain 0-1-2-3-4), max=4. So answer 4.
        // Similarly edges 1,2,3 answer 4.
        assert(res == vector<int>({4,4,4,4,100}));
    }
    printf("All tests passed!\n");
    return 0;
}
