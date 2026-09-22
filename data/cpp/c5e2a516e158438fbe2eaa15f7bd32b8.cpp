You are given m supply rows and n demand columns. The row i must be assigned to exactly r[i] distinct columns, the column j must be assigned to exactly c[j] distinct rows, and every possible (row, column) pair may be used at most once. Write a C++ function `bool bipartiteAssignment(const std::vector<int>& rowDemands, const std::vector<int>& colDemands, std::vector<std::vector<int>>& assignment)` that returns `true` and fills `assignment[i]` (a 1-indexed vector of column numbers) for each row i (0 ≤ i < m) in increasing order if a valid assignment exists; otherwise, return `false` with `assignment` in an unspecified but valid state. The sum of row demands equals the sum of column demands. If assignment is impossible, return `false`. The function must handle up to 300 rows and columns, with each individual demand between 0 and 300. The total number of edges required is at most 90,000.

#include <cassert>
#include <vector>

// Include the solution function (declared above). 
// In a real test file, copy the solution here.

int main() {
    // Test 1: Simple 2x2 perfect matching
    {
        std::vector<int> r = {1, 1};
        std::vector<int> c = {1, 1};
        std::vector<std::vector<int>> assign;
        assert(bipartiteAssignment(r, c, assign) == true);
        assert(assign.size() == 2);
        assert(assign[0].size() == 1 && assign[1].size() == 1);
        // check uniqueness of columns
        assert(assign[0][0] != assign[1][0]);
    }

    // Test 2: Impossible due to demand sum mismatch
    {
        std::vector<int> r = {2, 1};
        std::vector<int> c = {1, 1};
        std::vector<std::vector<int>> assign;
        assert(bipartiteAssignment(r, c, assign) == false);
    }

    // Test 3: Single row with two columns
    {
        std::vector<int> r = {2};
        std::vector<int> c = {1, 1};
        std::vector<std::vector<int>> assign;
        assert(bipartiteAssignment(r, c, assign) == true);
        assert(assign.size() == 1);
        assert(assign[0].size() == 2);
        assert(assign[0][0] == 1 && assign[0][1] == 2 || assign[0][0] == 2 && assign[0][1] == 1);
    }

    // Test 4: Zero demand rows/columns
    {
        std::vector<int> r = {0, 2};
        std::vector<int> c = {1, 1, 0};
        std::vector<std::vector<int>> assign;
        assert(bipartiteAssignment(r, c, assign) == true);
        assert(assign.size() == 2);
        assert(assign[0].empty());
        assert(assign[1].size() == 2);
    }

    // Test 5: Impossible because one column cannot take multiple from same row (capacity 1)
    {
        std::vector<int> r = {2, 0};
        std::vector<int> c = {2};
        std::vector<std::vector<int>> assign;
        assert(bipartiteAssignment(r, c, assign) == false);
    }

    // Test 6: Larger test 3x3 with full matching
    {
        std::vector<int> r = {2, 1, 0};
        std::vector<int> c = {1, 1, 1};
        std::vector<std::vector<int>> assign;
        assert(bipartiteAssignment(r, c, assign) == true);
        int total = 0;
        for (auto& row : assign) total += row.size();
        assert(total == 3);
    }

    // Test 7: All zero demands
    {
        std::vector<int> r = {0, 0};
        std::vector<int> c = {0, 0, 0};
        std::vector<std::vector<int>> assign;
        assert(bipartiteAssignment(r, c, assign) == true);
        assert(assign.size() == 2);
        assert(assign[0].empty() && assign[1].empty());
    }

    // Test 8: Negative demand should fail
    {
        std::vector<int> r = {-1, 1};
        std::vector<int> c = {1, 1};
        std::vector<std::vector<int>> assign;
        assert(bipartiteAssignment(r, c, assign) == false);
    }

    // Test 9: Large demand exceeding edges (impossible)
    {
        std::vector<int> r = {3, 3};
        std::vector<int> c = {3, 3};
        std::vector<std::vector<int>> assign;
        // 2 rows * 2 cols = 4 max edges, demands total 6 > 4
        assert(bipartiteAssignment(r, c, assign) == false);
    }

    // Test 10: A complex valid case with 4x4
    {
        std::vector<int> r = {2, 2, 1, 1};
        std::vector<int> c = {2, 2, 1, 1};
        std::vector<std::vector<int>> assign;
        assert(bipartiteAssignment(r, c, assign) == true);
        // verify each row size matches demand
        for (int i = 0; i < 4; ++i) {
            assert(assign[i].size() == static_cast<size_t>(r[i]));
        }
    }

    return 0;
}

#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

// Returns true if a valid assignment exists; fills assignment as 1-indexed column lists.
bool bipartiteAssignment(const std::vector<int>& rowDemands,
                         const std::vector<int>& colDemands,
                         std::vector<std::vector<int>>& assignment) {
    const int m = static_cast<int>(rowDemands.size());
    const int n = static_cast<int>(colDemands.size());
    
    int sumR = 0, sumC = 0;
    for (int r : rowDemands) {
        if (r < 0) return false;
        sumR += r;
    }
    for (int c : colDemands) {
        if (c < 0) return false;
        sumC += c;
    }
    if (sumR != sumC) return false;
    if (sumR > m * n) return false;  // impossible to have unique pairs

    int s = 0;
    int t = m + n + 1;
    const int N = m + n + 2;
    const int INF = 0x3f3f3f3f;

    // Edge lists: use simple adjacency matrix? Better: adjacency list with arrays.
    // But for clarity, use vector of edges with arrays.
    std::vector<int> head(N, -1);
    std::vector<int> to, nxt, wei;
    auto add_edge = [&](int u, int v, int w) {
        // forward edge
        to.push_back(v);
        wei.push_back(w);
        nxt.push_back(head[u]);
        head[u] = static_cast<int>(to.size()) - 1;
        // backward edge
        to.push_back(u);
        wei.push_back(0);
        nxt.push_back(head[v]);
        head[v] = static_cast<int>(to.size()) - 1;
    };

    // source to rows
    for (int i = 0; i < m; ++i) {
        add_edge(s, i + 1, rowDemands[i]);
    }
    // rows to columns
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            add_edge(i + 1, m + 1 + j, 1);
        }
    }
    // columns to sink
    for (int j = 0; j < n; ++j) {
        add_edge(m + 1 + j, t, colDemands[j]);
    }

    // Dinic BFS
    std::vector<int> depth(N), now(N);
    auto bfs = [&]() -> bool {
        std::fill(depth.begin(), depth.end(), -1);
        std::queue<int> q;
        q.push(s);
        depth[s] = 0;
        now[s] = head[s];
        while (!q.empty()) {
            int x = q.front(); q.pop();
            for (int e = head[x]; e != -1; e = nxt[e]) {
                int y = to[e];
                if (depth[y] == -1 && wei[e] > 0) {
                    depth[y] = depth[x] + 1;
                    now[y] = head[y];
                    if (y == t) return true;
                    q.push(y);
                }
            }
        }
        return false;
    };

    // Dinic DFS (iterative recursion using lambda)
    std::function<int(int,int)> dfs = [&](int x, int flow) -> int {
        if (x == t) return flow;
        int rest = flow;
        for (int &e = now[x]; e != -1; e = nxt[e]) {
            int y = to[e];
            if (depth[y] != depth[x] + 1 || wei[e] == 0) continue;
            int k = dfs(y, std::min(rest, wei[e]));
            if (k == 0) depth[y] = -1;
            wei[e] -= k;
            wei[e ^ 1] += k;
            rest -= k;
            if (rest == 0) break;
        }
        return flow - rest;
    };

    int maxFlow = 0;
    while (bfs()) {
        maxFlow += dfs(s, INF);
    }

    if (maxFlow != sumR) return false;

    // Build assignment
    assignment.assign(m, std::vector<int>());
    for (int i = 0; i < m; ++i) {
        for (int e = head[i + 1]; e != -1; e = nxt[e]) {
            int y = to[e];
            if (y == s) continue;
            if (wei[e] == 0) {
                // flow used on this edge
                int col = y - m - 1;  // 0-indexed column
                assignment[i].push_back(col + 1); // 1-indexed
            }
        }
    }
    return true;
}

// This is a maximum bipartite matching problem with vertex capacities: each row i has capacity r[i], each column j has capacity c[j], and each edge (i,j) has capacity 1. Model it as a flow network: source → row i (capacity r[i]), row i → column j (capacity 1) for all i,j, column j → sink (capacity c[j]). Compute the maximum flow using Dinic’s algorithm. Because each edge from a row to a column has capacity 1, a flow of k on that edge means row i is assigned to column j exactly once (k ∈ {0,1}). The maximum flow value equals the sum of row demands if and only if a valid assignment exists. After computing flow, for each row i, examine all outgoing edges excluding the source edge; if the residual capacity on that edge is 0 (i.e., flow used), then column j = (target node - m) is selected. Edge cases: if any demand is negative (should not happen) or if total demand exceeds m*n (not possible given constraints but still guarded), the function returns false. Also, if the row demand sum is not equal to column demand sum, impossible by definition, but we check anyway. Time complexity: Dinic on a graph with O(m+n) nodes and O(m*n) edges runs in O(V^2 E) worst-case, but in practice with unit-capacity row-to-column edges and capacities up to 300, it is fast enough. Space complexity is O(m*n) for adjacency lists.
