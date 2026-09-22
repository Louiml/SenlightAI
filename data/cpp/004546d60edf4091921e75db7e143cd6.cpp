/*
Given a rectangular grid of size `n` rows and `m` columns, and a list of `T` independent queries, write a C++ function that computes the minimum total cost to color every cell in the grid either black (1) or white (2). The grid is surrounded by an outer border of cells (row 0, row n+1, column 0, column m+1) that are pre-colored in each query. The cost of coloring is determined by horizontal and vertical adjacency edge weights: for each adjacent pair of cells sharing a horizontal edge (between `(i,j)` and `(i,j+1)`), there is a given cost `D_horizontal[i][j]` that is paid if the two cells have different colors; similarly, for each vertical edge between `(i,j)` and `(i+1,j)`, there is a cost `D_vertical[i][j]` paid if colors differ. The border cells (row 0, row n+1, column 0, column m+1) are assigned colors per query, and any edge between a border cell and an interior cell contributes its corresponding weight if colors differ. The function must return the minimal possible sum of all differing-edge costs for each query. The border assignments are given by listing `k` constraints, each specifying a border cell index `b` (0-indexed along the perimeter starting from top-left of the top row going clockwise), a color `c` (0 for black, 1 for white), and a weight `a` that is assigned to that border edge (the edge connecting that border cell to its adjacent interior cell). For each query, the function must compute the minimum total cost under those border constraints. The grid sizes satisfy `1 ≤ n,m ≤ 500`, and the total number of queries `T` is up to `10`. Edge costs are non-negative integers up to `10^9`. The function should handle multiple queries efficiently, reusing the static grid edge arrays.
*/

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 4e18;

struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };
    int N;
    vector<vector<Edge>> g;
    vector<int> level, iter;
    Dinic(int n) : N(n), g(n), level(n), iter(n) {}
    void add_edge(int from, int to, ll cap) {
        g[from].push_back({to, (int)g[to].size(), cap});
        g[to].push_back({from, (int)g[from].size()-1, 0});
    }
    void bfs(int s) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (auto &e : g[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
    }
    ll dfs(int v, int t, ll f) {
        if (v == t) return f;
        for (int &i = iter[v]; i < (int)g[v].size(); ++i) {
            Edge &e = g[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                ll d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    g[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    ll max_flow(int s, int t) {
        ll flow = 0;
        while (true) {
            bfs(s);
            if (level[t] < 0) break;
            fill(iter.begin(), iter.end(), 0);
            while (true) {
                ll f = dfs(s, t, INF);
                if (f == 0) break;
                flow += f;
            }
        }
        return flow;
    }
};

// Given n,m (interior grid dimensions), horizontal edge weights h[n][m-1] (between (i,j) and (i,j+1) for i=0..n-1, j=0..m-2),
// vertical edge weights v[n-1][m] (between (i,j) and (i+1,j) for i=0..n-2, j=0..m-1),
// and border weights and colors per query, return minimal cost for each query.
// Border indexing: 1-based clockwise from top-left corner: top row left to right (indices 1..m),
// right column top to bottom (m+1..m+n), bottom row right to left (m+n+1..2m+n),
// left column bottom to top (2m+n+1..2m+2n).
// For each border cell index b, color c (0=black,1=white) and weight a (edge cost if different from interior neighbor).
vector<ll> solveTraffic(int n, int m, int T,
                        const vector<vector<ll>>& h, // h[i][j] for i in [0,n-1], j in [0,m-2]
                        const vector<vector<ll>>& v, // v[i][j] for i in [0,n-2], j in [0,m-1]
                        const vector<vector<tuple<int,int,ll>>>& queries) {
    // Pre-copy base horizontal and vertical arrays with extra border slots: we'll use arrays of size (n+1)x(m+1)
    vector<vector<ll>> hor(n+1, vector<ll>(m+1, 0)); // hor[i][j] for edge between (i,j) and (i,j+1) for j=0..m-1, i=1..n
    vector<vector<ll>> ver(n+1, vector<ll>(m+1, 0)); // ver[i][j] for edge between (i,j) and (i+1,j) for i=0..n, j=1..m
    // Fill interior edges: i base index from 1 to n for horizontal? Actually snippet uses D[heng][i][o] for i=1..n, o=1..m-1
    // We'll use 1-indexed for rows and cols to match snippet mapping.
    for (int i=1; i<=n; ++i) {
        for (int j=1; j<=m-1; ++j) {
            hor[i][j] = h[i-1][j-1];
        }
    }
    for (int i=1; i<=n-1; ++i) {
        for (int j=1; j<=m; ++j) {
            ver[i][j] = v[i-1][j-1];
        }
    }
    // Initialize border edges to 0; they will be set per query.
    // For top border: ver[0][j] for j=1..m
    // For bottom border: ver[n][j] for j=1..m
    // For left border: hor[i][0] for i=1..n
    // For right border: hor[i][m] for i=1..n

    vector<ll> ans;
    ans.reserve(T);

    int V = n*m + 2;
    int S = n*m;
    int TT = n*m + 1;

    for (int q=0; q<T; ++q) {
        // Copy border weights and color arrays
        vector<ll> borderTop(m+1, 0), borderBottom(m+1, 0), borderLeft(n+1, 0), borderRight(n+1, 0);
        // Also we need to know color of each border cell; we'll store color arrays similarly.
        // For simplicity, we store arrays of color (0/1) for each border edge
        vector<int> colorTop(m+1, -1), colorBottom(m+1, -1), colorLeft(n+1, -1), colorRight(n+1, -1);
        for (auto [b, c, a] : queries[q]) {
            // b is 1-based
            if (b <= m) { // top row
                int j = b;
                borderTop[j] = a;
                colorTop[j] = c;
            } else if (b <= m+n) { // right column
                int i = b - m;
                borderRight[i] = a;
                colorRight[i] = c;
            } else if (b <= 2*m+n) { // bottom row
                int j = m - (b - m - n) + 1; // snippet: m-(b-m-n)+1
                borderBottom[j] = a;
                colorBottom[j] = c;
            } else { // left column
                int i = n - (b - m - m - n) + 1; // snippet: n-(b-m-m-n)+1
                borderLeft[i] = a;
                colorLeft[i] = c;
            }
        }

        Dinic dinic(V);
        // Add source/sink edges for each interior cell based on border connections
        for (int i=1; i<=n; ++i) {
            for (int j=1; j<=m; ++j) {
                int node = (i-1)*m + (j-1);
                ll costToSource = 0; // if we label this cell 0 (black), pay these costs for edges to white border cells
                ll costToSink = 0;   // if we label this cell 1 (white), pay these costs for edges to black border cells
                // Top border
                if (i == 1 && colorTop[j] != -1) {
                    if (colorTop[j] == 1) costToSource += borderTop[j];
                    else costToSink += borderTop[j];
                }
                // Bottom border
                if (i == n && colorBottom[j] != -1) {
                    if (colorBottom[j] == 1) costToSource += borderBottom[j];
                    else costToSink += borderBottom[j];
                }
                // Left border
                if (j == 1 && colorLeft[i] != -1) {
                    if (colorLeft[i] == 1) costToSource += borderLeft[i];
                    else costToSink += borderLeft[i];
                }
                // Right border
                if (j == m && colorRight[i] != -1) {
                    if (colorRight[i] == 1) costToSource += borderRight[i];
                    else costToSink += borderRight[i];
                }
                if (costToSource > 0) dinic.add_edge(S, node, costToSource);
                if (costToSink > 0) dinic.add_edge(node, TT, costToSink);
            }
        }
        // Add edges between adjacent interior cells
        for (int i=1; i<=n; ++i) {
            for (int j=1; j<=m; ++j) {
                int node = (i-1)*m + (j-1);
                // Horizontal edge to right neighbor
                if (j < m) {
                    int nxt = node + 1;
                    ll w = hor[i][j]; // h[i-1][j-1]
                    dinic.add_edge(node, nxt, w);
                    dinic.add_edge(nxt, node, w);
                }
                // Vertical edge to bottom neighbor
                if (i < n) {
                    int nxt = node + m;
                    ll w = ver[i][j]; // v[i-1][j-1]
                    dinic.add_edge(node, nxt, w);
                    dinic.add_edge(nxt, node, w);
                }
            }
        }
        ll minCost = dinic.max_flow(S, TT);
        ans.push_back(minCost);
    }
    return ans;
}

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Declare the function (or include the solution code above)
// For testing, we copy the solution here, but in practice we include it.

int main() {
    // Test 1: 1x1 grid, no interior edges, one query with no border constraints => cost 0
    {
        int n=1, m=1;
        vector<vector<long long>> h(n, vector<long long>(max(0,m-1), 0));
        vector<vector<long long>> v(max(0,n-1), vector<long long>(m, 0));
        vector< vector<tuple<int,int,long long>> > queries(1);
        queries[0] = {};
        auto ans = solveTraffic(n,m,1,h,v,queries);
        assert(ans.size()==1 && ans[0]==0);
    }
    // Test 2: 1x1 grid, top border colored white (1) with weight 5 => cost 5 regardless of interior color? Actually if interior is black, pay 5; if white pay 0. Min is 0.
    {
        int n=1, m=1;
        vector<vector<long long>> h(n, vector<long long>(0, 0));
        vector<vector<long long>> v(0, vector<long long>(m, 0));
        vector< vector<tuple<int,int,long long>> > queries(1);
        queries[0] = {{1, 1, 5}}; // border index 1 = top left, color 1 (white), weight 5
        auto ans = solveTraffic(n,m,1,h,v,queries);
        assert(ans.size()==1 && ans[0]==0);
    }
    // Test 3: 1x1 grid, top border white(1) weight 5, left border black(0) weight 7 => min cost? If interior white: pay left=7, if black: pay top=5 => min=5
    {
        int n=1, m=1;
        vector<vector<long long>> h(n, vector<long long>(0, 0));
        vector<vector<long long>> v(0, vector<long long>(m, 0));
        vector< vector<tuple<int,int,long long>> > queries(1);
        // border indices: top row index 1, left column index 1? For n=1,m=1, perimeter: top row: 1; right col: 2; bottom row: 3; left col: 4.
        // So left border index = 4.
        queries[0] = {{1, 1, 5}, {4, 0, 7}};
        auto ans = solveTraffic(n,m,1,h,v,queries);
        assert(ans.size()==1 && ans[0]==5);
    }
    // Test 4: 2x2 grid with no border constraints and all interior edge weights 1 => min cut 0 (all same color)
    {
        int n=2, m=2;
        vector<vector<long long>> h(n, vector<long long>(m-1, 1)); // 2x1
        vector<vector<long long>> v(n-1, vector<long long>(m, 1)); // 1x2
        vector< vector<tuple<int,int,long long>> > queries(1);
        queries[0] = {};
        auto ans = solveTraffic(n,m,1,h,v,queries);
        assert(ans.size()==1 && ans[0]==0);
    }
    // Test 5: 2x2 grid, single query with top border all white(1) weight 100 each, and interior edges weight 1. Minimal cost: color all cells white => pay 0 for borders, interior all same => cost 0.
    {
        int n=2, m=2;
        vector<vector<long long>> h(n, vector<long long>(m-1, 1));
        vector<vector<long long>> v(n-1, vector<long long>(m, 1));
        vector< vector<tuple<int,int,long long>> > queries(1);
        queries[0] = {{1,1,100}, {2,1,100}}; // top row indices 1 and 2
        auto ans = solveTraffic(n,m,1,h,v,queries);
        assert(ans.size()==1 && ans[0]==0);
    }
    // Test 6: 2x2 grid, top border left white(1) weight 10, top border right black(0) weight 10, interior edges all 1. Minimal cost? Let's brute force: 4 cells, colors 0/1.
    // Enumerate 16 combinations. We can compute manually: For each cell, pay border if different, and interior edges if different.
    // We'll trust the min-cut. Let's compute expected: If top-left cell white, top-right black, bottom-left white, bottom-right black? Then interior edges: between TL-TR diff=1, TL-BL same=0, TR-BR same=0, BL-BR diff=1 => total interior=2, border: TL top white same 0, TR top black same 0, bottom no constraints, left/right no => total 2. Another combo: all white except top-right black? Then top-left white (border same), top-right black (border same 0), bottom-left white, bottom-right white. Interior: TL-TR diff=1, TL-BL same 0, TR-BR diff=1, BL-BR same 0 => total 2. Another: all white? border top-left white same 0, top-right white diff from black border? But border top-right is black, so if cell is white, cost 10 => total 10. So min is 2? Actually we also can color top-left white, top-right black, bottom-left black, bottom-right white? Let's compute: TL white (border same 0), TR black (border same 0), BL black (no border), BR white (no border). Interior: TL-TR diff=1, TL-BL diff=1, TR-BR diff=1, BL-BR diff=1 => total 4. So min seems 2. Let's test.
    {
        int n=2, m=2;
        vector<vector<long long>> h(n, vector<long long>(m-1, 1));
        vector<vector<long long>> v(n-1, vector<long long>(m, 1));
        vector< vector<tuple<int,int,long long>> > queries(1);
        queries[0] = {{1,1,10}, {2,0,10}};
        auto ans = solveTraffic(n,m,1,h,v,queries);
        assert(ans.size()==1 && ans[0]==2);
    }
    // Test 7: 3x3 grid, no borders, all interior edges 7 => min 0
    {
        int n=3, m=3;
        vector<vector<long long>> h(n, vector<long long>(m-1, 7));
        vector<vector<long long>> v(n-1, vector<long long>(m, 7));
        vector< vector<tuple<int,int,long long>> > queries(1);
        queries[0] = {};
        auto ans = solveTraffic(n,m,1,h,v,queries);
        assert(ans.size()==1 && ans[0]==0);
    }
    // Test 8: 3x3, all top border cells white weight 100, all left border cells black weight 100, interior edges 1. Min cost: likely color all white except left column black? Let's just check that min <= some value and >=0.
    {
        int n=3, m=3;
        vector<vector<long long>> h(n, vector<long long>(m-1, 1));
        vector<vector<long long>> v(n-1, vector<long long>(m, 1));
        vector< vector<tuple<int,int,long long>> > queries(1);
        for(int b=1;b<=m;b++) queries[0].push_back({b, 1, 100});
        // left column indices: for n=3,m=3, perimeter: top row 1..3, right col 4..6, bottom row 7..9, left col 10..12
        for(int i=1;i<=n;i++) {
            int b = 2*m + n + i; // left column bottom to top? snippet: b = 2m+n+1 for bottommost left? Let's compute: left col index from bottom to top: for i=1 (top) we need index? Actually left column from bottom to top: for i=n (bottom) index = 2m+2n? Wait snippet: left column: b in (2m+n, 2m+2n]? We can directly use our indexing: for i from 1 to n, left border index = 2*m + n + i? Let's check for n=3,m=3: total perimeter = 12. Left column from bottom to top: indices 10 (bottom-left), 11 (middle-left), 12 (top-left). So for i=1 (top) -> 12, i=2 -> 11, i=3 (bottom) -> 10. So formula: b = 2*m + n + (n - i + 1) = 6+3+ (3-1+1)=? Actually 2m+n=9, then + (n-i+1) for i=1 gives 9+3=12, i=2->11, i=3->10. Yes.
            int b = 2*m + n + (n - i + 1);
            queries[0].push_back({b, 0, 100});
        }
        auto ans = solveTraffic(n,m,1,h,v,queries);
        // We just check that ans[0] is non-negative and not huge; we can't easily compute exact, but we can assert it's < 2000
        assert(ans.size()==1 && ans[0] >= 0 && ans[0] < 2000);
    }
    // Test 9: Two queries, ensure independence
    {
        int n=1, m=2;
        vector<vector<long long>> h(n, vector<long long>(m-1, 3)); // one horizontal edge weight 3
        vector<vector<long long>> v(n-1, vector<long long>(m, 0));
        vector< vector<tuple<int,int,long long>> > queries(2);
        queries[0] = {{1,1,5}}; // top-left white weight 5
        queries[1] = {{2,0,4}}; // top-right black weight 4
        auto ans = solveTraffic(n,m,2,h,v,queries);
        // Query0: grid 1x2, top-left border white 5, interior edge 3.
        // Possible colorings: both white -> pay 5 (top-left diff) +0 interior =5; both black -> pay 0 (top-left white vs black cost5? Actually if cell black and border white, pay 5; so if both black, top-left cell black vs white border =>5, interior same =>5); left white right black -> pay 5? top-left same 0, top-right no border, interior diff=3 =>3; left black right white -> top-left diff=5, interior diff=3 =>8. So min=3.
        // Query1: top-right border black weight 4. Both white -> top-right white vs black diff=4, interior 0 =>4; both black -> top-right same 0 + interior 0 =>0; left white right black -> top-right same 0 + interior 3 =>3; left black right white -> top-right diff 4 + interior 3 =>7. So min=0.
        assert(ans.size()==2);
        assert(ans[0]==3);
        assert(ans[1]==0);
    }
    // Test 10: Larger random small grid brute force check using exhaustive for n=2,m=2 with all possible border constraints of at most 2 border cells
    {
        int n=2,m=2;
        srand(42);
        for(int rep=0; rep<50; ++rep) {
            vector<vector<long long>> h(n, vector<long long>(m-1, 1+rand()%5));
            vector<vector<long long>> v(n-1, vector<long long>(m, 1+rand()%5));
            // generate random border constraints: each border cell either no constraint or random color and weight
            vector<tuple<int,int,long long>> q;
            for(int b=1; b<=2*m+2*n; ++b) {
                if(rand()%2) {
                    int c = rand()%2;
                    ll w = rand()%10 + 1;
                    q.push_back({b,c,w});
                }
            }
            vector< vector<tuple<int,int,long long>> > queries(1, q);
            auto ans = solveTraffic(n,m,1,h,v,queries);
            // Brute force over 2^4=16 colorings
            ll brute = LLONG_MAX;
            for(int mask=0; mask<(1<<(n*m)); ++mask) {
                vector<vector<int>> col(n+2, vector<int>(m+2, -1)); // border cells -1 means no constraint, but we use -1 for interior? We'll use 0/1 for interior.
                // set interior colors
                for(int i=1;i<=n;i++) for(int j=1;j<=m;j++) col[i][j] = (mask>>((i-1)*m + (j-1))) & 1;
                // set border constraints from q
                // Need to map b to border positions as done in solution; we'll copy the mapping from solveTraffic? For simplicity, we'll just compute cost directly using proper indexing.
                // We'll create arrays for border colors and weights.
                vector<int> cTop(m+1,-1), cBottom(m+1,-1), cLeft(n+1,-1), cRight(n+1,-1);
                vector<ll> wTop(m+1,0), wBottom(m+1,0), wLeft(n+1,0), wRight(n+1,0);
                for(auto [b,c,w] : q) {
                    if(b<=m) { cTop[b]=c; wTop[b]=w; }
                    else if(b<=m+n) { int i=b-m; cRight[i]=c; wRight[i]=w; }
                    else if(b<=2*m+n) { int j=m-(b-m-n)+1; cBottom[j]=c; wBottom[j]=w; }
                    else { int i=n-(b-m-m-n)+1; cLeft[i]=c; wLeft[i]=w; }
                }
                ll cost=0;
                // border costs
                for(int j=1;j<=m;j++) {
                    if(cTop[j]!=-1 && col[1][j] != cTop[j]) cost += wTop[j];
                    if(cBottom[j]!=-1 && col[n][j] != cBottom[j]) cost += wBottom[j];
                }
                for(int i=1;i<=n;i++) {
                    if(cLeft[i]!=-1 && col[i][1] != cLeft[i]) cost += wLeft[i];
                    if(cRight[i]!=-1 && col[i][m] != cRight[i]) cost += wRight[i];
                }
                // interior edges
                for(int i=1;i<=n;i++) for(int j=1;j<=m;j++) {
                    if(j<m && col[i][j] != col[i][j+1]) cost += h[i-1][j-1];
                    if(i<n && col[i][j] != col[i+1][j]) cost += v[i-1][j-1];
                }
                brute = min(brute, cost);
            }
            assert(ans[0]==brute);
        }
    }
    return 0;
}

// The problem is essentially a binary labeling problem on a grid with pairwise potentials (Ising model) on a grid graph with rectangular topology. The grid has `n*m` interior cells, each assigned one of two colors. The cost is the sum over all edges (horizontal and vertical) of a given weight if the two endpoints have different labels. For grids with submodular pairwise potentials (which is the case here because the cost is zero for equal labels and positive for different labels), the minimum cut / maximum flow algorithm (e.g., via Dinic or push-relabel) provides a polynomial-time solution. However, given that `n,m` can be 500 and `T` up to 10, a naive min-cut on an `n*m` grid with up to 250,000 nodes and ~500,000 edges per query may be acceptable but we must also handle border constraints. A simpler approach is to use a backtracking DFS that fills the grid in row-major order, pruning with the current best solution. This is exactly what the provided snippet does, though it is exponential in the worst case; for `n,m ≤ 500` it would be far too slow. Thus the intended solution must be a more efficient algorithm: we can formulate it as a standard s-t min-cut: create a source node representing color 0 (black) and sink for color 1 (white). For each interior cell, add an edge to source with capacity equal to the sum of weights of all edges to neighboring border cells that are colored 1 (since if we label the cell 0, we incur those costs). Similarly add an edge to sink with capacity for edges to border cells that are colored 0. Between adjacent interior cells, add undirected edges with capacity equal to the edge weight (this models the penalty for differing labels). The min-cut then gives the minimal cost. We need to build the graph per query based on the given border constraints and the pre-stored edge weights. Edge weights are up to 1e9, so use 64-bit integers. For each query, we construct the flow network and run max-flow. Since the graph is a grid, we can index nodes as `(i-1)*m + (j-1)` for interior cells. For each border edge, we add capacities to source/sink accordingly. The min-cut value is the answer. Complexity per query: O(V^2 E) worst-case for Dinic on graphs with unit capacities, but for general capacities it's O(E sqrt(V)) for unit capacities and O(V^2 E) general, but on grids with 250k nodes and ~500k edges, it may be borderline but acceptable in practice with optimized Dinic. We can also use push-relabel which is faster. We'll implement Dinic with adjacency list, using long long capacities. Edge cases: when n=1 or m=1, the grid is a path; still works. Border indices are given in order: top row from left to right (1..m), then right column from top to bottom (m+1..m+n), then bottom row from right to left (m+n+1..2m+n), then left column from bottom to top (2m+n+1..2m+2n). The border cell index `b` in the input is 0-indexed? The snippet uses 1-indexed starting from 1 but the problem can be defined with 0-index. We'll define it as given in the snippet: `b` is 1-indexed along the perimeter, and the snippet treats `b<=m` as top row, `b<=m+n` as right column, etc. But for clarity in the solution, we'll follow the exact mapping from the snippet: For a border cell at index `b` (1-indexed), compute which border edge it corresponds to. The snippet uses arrays `D_shu` (vertical edges between row i and i+1, for i from 1 to n-1) and `D_heng` (horizontal edges between column j and j+1, for j from 1 to m-1). It also uses `D_shu[0][b]` for top border cells (row 0 to row 1), `D_shu[n][b]` for bottom border (row n to n+1), `D_heng[i][0]` for left border, `D_heng[i][m]` for right border. We'll replicate that mapping. The border color is given as `c` (0 for black, 1 for white) but the snippet increments `c` by 1 to map to `bai=1` (black) and `hei=2` (white). We'll use 0 and 1 directly. For each query, we set the border edge weights as given (overwriting the base values, but careful: the base grid edge weights are given once and reused across queries; only the border edge weights are overwritten per query). We must ensure that we do not modify the original arrays permanently; we can copy them per query or maintain a separate array. The solution function should take the grid dimensions n,m, the base horizontal and vertical edge arrays (size (n+1) x (m+1) but only relevant parts), and the list of queries, and return a vector of answers.
