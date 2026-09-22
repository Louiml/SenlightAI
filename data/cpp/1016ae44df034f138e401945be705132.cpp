Write a C++ function that, given integers `n`, `q`, a list of `n-1` undirected edges forming a tree on vertices labeled `1..n`, and `q` queries each specifying two vertices `u` and `v`, returns a `vector<long long>` of size `n+1` where the value at index `i` is the number of times vertex `i` lies on the simple path between `u` and `v` across all queries. For each query, the path includes both endpoints.

#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared externally (included from the solution file)
std::vector<long long> countPathVertices(int, const std::vector<std::pair<int,int>>&, const std::vector<std::pair<int,int>>&);

int main() {
    // Single node tree, one self-query
    {
        int n = 1;
        std::vector<std::pair<int,int>> edges;
        std::vector<std::pair<int,int>> queries = {{1,1}};
        auto res = countPathVertices(n, edges, queries);
        assert(res.size() == 2);
        assert(res[1] == 1);
    }

    // Two-node tree, path 1-2 both directions and self-query
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{1,2}};
        std::vector<std::pair<int,int>> queries = {{1,2}, {2,1}, {1,1}};
        auto res = countPathVertices(n, edges, queries);
        assert(res[1] == 3); // path 1-2 + 2-1 + 1-1 => 1 appears 3 times
        assert(res[2] == 2); // only in first two queries
    }

    // Chain 1-2-3, query 1-3 and 2-2
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        std::vector<std::pair<int,int>> queries = {{1,3},{2,2}};
        auto res = countPathVertices(n, edges, queries);
        assert(res[1] == 1);
        assert(res[2] == 2);
        assert(res[3] == 1);
    }

    // Star centered at 1: leaves 2,3,4
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{1,4}};
        std::vector<std::pair<int,int>> queries = {{2,3},{3,4},{2,4}};
        auto res = countPathVertices(n, edges, queries);
        // paths: 2-1-3, 3-1-4, 2-1-4 -> center 1 appears 3 times, leaves each once
        assert(res[1] == 3);
        assert(res[2] == 2); // in first and third
        assert(res[3] == 2); // in first and second
        assert(res[4] == 2); // in second and third
    }

    // Queries with same LCA as endpoint
    {
        int n = 5;
        // Tree: 1-2, 1-3, 2-4, 2-5
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{2,4},{2,5}};
        std::vector<std::pair<int,int>> queries = {{4,5},{1,3},{5,5}};
        auto res = countPathVertices(n, edges, queries);
        // Query 4-5: path 4-2-5 => 4,2,5 each +1
        // Query 1-3: 1,3 each +1
        // Query 5-5: 5 +1
        // Totals: 1:1, 2:1, 3:1, 4:1, 5:2
        assert(res[1] == 1);
        assert(res[2] == 1);
        assert(res[3] == 1);
        assert(res[4] == 1);
        assert(res[5] == 2);
    }

    // Larger random tree test with small brute force
    {
        int n = 6;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5},{5,6}}; // chain
        std::vector<std::pair<int,int>> queries = {{1,6},{2,5},{3,4},{6,1}};
        auto res = countPathVertices(n, edges, queries);
        // Brute force: manually compute
        // 1-6: all 1..6 each +1
        // 2-5: 2,3,4,5 each +1
        // 3-4: 3,4 each +1
        // 6-1: all again each +1
        std::vector<long long> expected(n + 1, 0);
        for (int i = 1; i <= 6; ++i) expected[i] += 2; // from 1-6 and 6-1
        expected[2]++; expected[3]++; expected[4]++; expected[5]++; // from 2-5
        expected[3]++; expected[4]++; // from 3-4
        for (int i = 1; i <= 6; ++i) assert(res[i] == expected[i]);
    }

    return 0;
}

#include <vector>
#include <algorithm>

/**
 * @brief Count how many times each vertex lies on the path of each query.
 * 
 * @param n Number of vertices (labeled 1..n).
 * @param edges Vector of pairs (u, v) forming an undirected tree on n vertices.
 * @param queries Vector of pairs (u, v) – each represents a path to count.
 * @return std::vector<long long> of size n+1, result[i] = count for vertex i.
 */
std::vector<long long> countPathVertices(
    int n,
    const std::vector<std::pair<int,int>>& edges,
    const std::vector<std::pair<int,int>>& queries
) {
    constexpr int LOG = 20; // enough for n <= 2e5

    // Build adjacency list
    std::vector<std::vector<int>> g(n + 1);
    for (const auto& e : edges) {
        g[e.first].push_back(e.second);
        g[e.second].push_back(e.first);
    }

    // Binary lifting table and depth
    std::vector<std::vector<int>> up(LOG, std::vector<int>(n + 1, 0));
    std::vector<int> depth(n + 1, 0);

    // DFS from root 1 to set depth and immediate parent
    std::vector<int> parent(n + 1, 0);
    std::vector<bool> visited(n + 1, false);
    std::vector<int> stack;
    stack.push_back(1);
    visited[1] = true;
    depth[1] = 0;
    up[0][1] = 0;

    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        for (int v : g[u]) {
            if (!visited[v]) {
                visited[v] = true;
                depth[v] = depth[u] + 1;
                up[0][v] = u;
                stack.push_back(v);
            }
        }
    }

    // Fill binary lifting table
    for (int k = 1; k < LOG; ++k) {
        for (int v = 1; v <= n; ++v) {
            int mid = up[k - 1][v];
            up[k][v] = (mid != 0) ? up[k - 1][mid] : 0;
        }
    }

    // Helper lambda to lift a node up by d steps
    auto lift = [&](int u, int d) -> int {
        for (int k = 0; k < LOG; ++k) {
            if (d & (1 << k)) {
                u = up[k][u];
                if (u == 0) break;
            }
        }
        return u;
    };

    // LCA function
    auto lca = [&](int u, int v) -> int {
        if (depth[u] < depth[v]) std::swap(u, v);
        u = lift(u, depth[u] - depth[v]);
        if (u == v) return u;
        for (int k = LOG - 1; k >= 0; --k) {
            if (up[k][u] != up[k][v]) {
                u = up[k][u];
                v = up[k][v];
            }
        }
        return up[0][u];
    };

    // Difference array for path updates
    std::vector<long long> val(n + 1, 0);

    for (const auto& q : queries) {
        int u = q.first, v = q.second;
        int w = lca(u, v);
        val[u]++;
        val[v]++;
        val[w]--;
        if (up[0][w] != 0) val[up[0][w]]--;
    }

    // DFS post-order accumulation
    // We'll use iterative DFS to avoid recursion depth issues
    std::vector<int> order;
    std::vector<bool> done(n + 1, false);
    std::vector<int> stack2;
    stack2.push_back(1);
    std::vector<int> post;

    while (!stack2.empty()) {
        int u = stack2.back();
        stack2.pop_back();
        post.push_back(u);
        for (int v : g[u]) {
            if (!done[v]) {
                done[v] = true;
                stack2.push_back(v);
            }
        }
    }

    // Reverse order to get post-order (children before parent)
    for (auto it = post.rbegin(); it != post.rend(); ++it) {
        int u = *it;
        if (up[0][u] != 0) {
            val[up[0][u]] += val[u];
        }
    }

    return val;
}

// The task is essentially a “path update with point query” problem on a tree, solved efficiently using a difference array on trees combined with binary lifting for LCA. For each query `(u, v)`, we need to add +1 to every vertex on the path from `u` to `v`. A common trick: increment a difference array `val[u]++`, `val[v]++`, then decrement at the LCA and at the parent of the LCA: `val[lca]--` and `val[parent(lca)]--`. After processing all queries, one final DFS post-order traversal accumulates the difference array so that `val[i]` becomes the total count for vertex `i`. To find the LCA, we preprocess the tree with a depth array and a binary lifting table `dp[u][k]` storing the 2^k-th ancestor. Edge cases: when `u == v`, the LCA is `u`, so we do `val[u]++`, `val[u]++`, `val[u]--`, `val[parent(u)]--` which correctly leaves only `u` incremented; when the tree has only one vertex, handle parent as 0 (no decrement). Time complexity: `O((n+q) log n)` for preprocessing and per‑query LCA, plus `O(n)` for the DFS accumulation. Space: `O(n log n)` for the binary lifting table and `O(n)` for adjacency and difference arrays.
