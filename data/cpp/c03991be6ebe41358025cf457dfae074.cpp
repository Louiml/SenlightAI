/*
Write a C++ function `long long countColorSegments(int n, const std::vector<int>& initialColor, const std::vector<std::pair<int,int>>& edges, const std::vector<Query>& queries)` that processes a tree of `n` nodes (numbered 1..n) with each node initially colored by an integer. The queries are either of type `'C' a b c` meaning set the color of every node on the simple path between `a` and `b` to color `c`, or of type `'Q' a b` meaning output the number of maximal contiguous segments of equal color along the simple path from `a` to `b` (i.e., count how many times the color changes as you walk from `a` to `b`, including the first node). The function must return a vector of answers for the `'Q'` queries in order. The tree is unrooted, but for processing we root it at node 1. You may assume the input is valid (tree, node indices within range), colors can be any 32-bit integer, and `n` up to 2000, number of queries up to 5000.
*/

#include <vector>
#include <queue>
#include <cstdint>
#include <functional>

struct Query {
    char type;      // 'C' or 'Q'
    int a, b, c;    // c only used for 'C'
};

// Process tree queries: return answers for 'Q' queries in order.
std::vector<long long> countColorSegments(
    int n,
    const std::vector<int>& initialColor,
    const std::vector<std::pair<int,int>>& edges,
    const std::vector<Query>& queries)
{
    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // Root the tree at 1 using BFS/DFS to compute parent and depth
    std::vector<int> parent(n + 1, 0);
    std::vector<int> depth(n + 1, 0);
    std::vector<bool> visited(n + 1, false);
    std::queue<int> q;
    q.push(1);
    visited[1] = true;
    parent[1] = 0;
    depth[1] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                depth[v] = depth[u] + 1;
                q.push(v);
            }
        }
    }

    // Simple LCA by climbing (since n is small)
    auto getLCA = [&](int a, int b) {
        if (depth[a] < depth[b]) std::swap(a, b);
        // bring a up to same depth as b
        int diff = depth[a] - depth[b];
        while (diff--) a = parent[a];
        while (a != b) {
            a = parent[a];
            b = parent[b];
        }
        return a;
    };

    // Color array (1-indexed)
    std::vector<int> color = initialColor;
    color.insert(color.begin(), 0); // shift to 1-indexed

    std::vector<long long> answers;
    for (const auto& query : queries) {
        if (query.type == 'C') {
            int a = query.a, b = query.b, c = query.c;
            int lca = getLCA(a, b);
            // Update path a -> lca (inclusive of lca)
            int u = a;
            while (u != lca) {
                color[u] = c;
                u = parent[u];
            }
            // Update path b -> lca (inclusive of lca)
            u = b;
            while (u != lca) {
                color[u] = c;
                u = parent[u];
            }
            color[lca] = c;
        } else { // 'Q'
            int a = query.a, b = query.b;
            if (a == b) {
                answers.push_back(1);
                continue;
            }
            int lca = getLCA(a, b);
            long long changes = 0;
            int lastA = -1; // last node visited on a-side (before lca)
            int lastB = -1; // last node visited on b-side (before lca)

            // Walk from a up to lca (exclusive)
            int u = a;
            int prev = -1;
            while (u != lca) {
                if (prev != -1 && color[u] != prev) changes++;
                prev = color[u];
                lastA = u;
                u = parent[u];
            }
            // Now u == lca
            // Walk from b up to lca (exclusive)
            u = b;
            prev = -1;
            while (u != lca) {
                if (prev != -1 && color[u] != prev) changes++;
                prev = color[u];
                lastB = u;
                u = parent[u];
            }

            // Count lca itself
            changes++; // for the lca node as a segment start
            // Adjust if lastA color equals lca color (then lca merges with a-side)
            if (lastA != -1 && color[lastA] == color[lca]) changes--;
            // Adjust if lastB color equals lca color
            if (lastB != -1 && color[lastB] == color[lca]) changes--;
            // If both lastA and lastB exist and have same color, we might have double counted? No, because we already added lca as +1 and subtracted twice if both equal lca. But consider if both sides have same color as each other but different from lca? Then we have two separate segments on each side, plus lca segment, so total = changesA + changesB + 1, which is correct. Edge case: if a-side has only one node (a==lca) then lastA=-1, no adjustment.
            answers.push_back(changes);
        }
    }
    return answers;
}

#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here (or link appropriately)
// For testing, we copy the function definition from above.
// ... (solution code here)

int main() {
    // Test 1: small linear tree 1-2-3, queries
    {
        int n = 3;
        std::vector<int> init = {1, 2, 3};
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}};
        std::vector<Query> queries;
        queries.push_back({'Q', 1, 3, 0}); // path 1-2-3 colors 1,2,3 => 3 segments
        auto ans = countColorSegments(n, init, edges, queries);
        assert(ans.size() == 1);
        assert(ans[0] == 3);
    }
    // Test 2: update path then query
    {
        int n = 3;
        std::vector<int> init = {1, 2, 3};
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}};
        std::vector<Query> queries;
        queries.push_back({'C', 1, 3, 5}); // set all to 5
        queries.push_back({'Q', 1, 3, 0}); // now all 5 => 1 segment
        auto ans = countColorSegments(n, init, edges, queries);
        assert(ans.size() == 1);
        assert(ans[0] == 1);
    }
    // Test 3: path with repeated colors
    {
        int n = 4;
        std::vector<int> init = {5, 5, 5, 5};
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}, {3,4}};
        std::vector<Query> queries;
        queries.push_back({'Q', 1, 4, 0}); // all same => 1
        auto ans = countColorSegments(n, init, edges, queries);
        assert(ans[0] == 1);
    }
    // Test 4: tree with branches, query a node to itself
    {
        int n = 5;
        // tree: 1-2, 1-3, 3-4, 3-5
        std::vector<int> init = {1,2,3,4,5};
        std::vector<std::pair<int,int>> edges = {{1,2}, {1,3}, {3,4}, {3,5}};
        std::vector<Query> queries;
        queries.push_back({'Q', 2, 2, 0}); // single node => 1
        queries.push_back({'Q', 2, 4, 0}); // path 2-1-3-4 colors 2,1,3,4 => 4
        auto ans = countColorSegments(n, init, edges, queries);
        assert(ans.size() == 2);
        assert(ans[0] == 1);
        assert(ans[1] == 4);
    }
    // Test 5: update on branch path, then query
    {
        int n = 5;
        std::vector<int> init = {1,2,3,4,5};
        std::vector<std::pair<int,int>> edges = {{1,2}, {1,3}, {3,4}, {3,5}};
        std::vector<Query> queries;
        queries.push_back({'C', 2, 4, 9}); // path 2-1-3-4 all set to 9
        queries.push_back({'Q', 2, 4, 0}); // now all 9 => 1
        auto ans = countColorSegments(n, init, edges, queries);
        assert(ans.size() == 1);
        assert(ans[0] == 1);
    }
    // Test 6: larger n with random-ish but deterministic
    {
        int n = 6;
        std::vector<int> init = {1,1,2,2,3,3};
        // tree: 1-2, 2-3, 3-4, 4-5, 5-6 (path)
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5},{5,6}};
        std::vector<Query> queries;
        queries.push_back({'Q', 1, 6, 0}); // colors 1,1,2,2,3,3 => segments 1,2,3 => 3
        queries.push_back({'C', 2, 5, 7}); // set nodes 2,3,4,5 to 7
        queries.push_back({'Q', 1, 6, 0}); // now colors: 1,7,7,7,7,3 => segments 1,7,3 => 3
        auto ans = countColorSegments(n, init, edges, queries);
        assert(ans.size() == 2);
        assert(ans[0] == 3);
        assert(ans[1] == 3);
    }
    // Test 7: LCA is one endpoint
    {
        int n = 4;
        std::vector<int> init = {1,2,3,4};
        // tree with 1 root, 2 child of 1, 3 child of 2, 4 child of 2
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{2,4}};
        std::vector<Query> queries;
        queries.push_back({'Q', 1, 3, 0}); // path 1-2-3 colors 1,2,3 => 3
        queries.push_back({'Q', 3, 4, 0}); // path 3-2-4 colors 3,2,4 => 3
        auto ans = countColorSegments(n, init, edges, queries);
        assert(ans.size() == 2);
        assert(ans[0] == 3);
        assert(ans[1] == 3);
    }
    // Test 8: update all nodes via root to leaf, then query
    {
        int n = 3;
        std::vector<int> init = {1,1,1};
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        std::vector<Query> queries;
        queries.push_back({'C', 3, 1, 8}); // set all to 8
        queries.push_back({'Q', 1, 3, 0}); // all 8 => 1
        auto ans = countColorSegments(n, init, edges, queries);
        assert(ans.size() == 1);
        assert(ans[0] == 1);
    }
    // Test 9: query where colors alternate
    {
        int n = 5;
        std::vector<int> init = {1,2,1,2,1};
        // path 1-2-3-4-5
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5}};
        std::vector<Query> queries;
        queries.push_back({'Q', 1, 5, 0}); // colors 1,2,1,2,1 => 5 segments
        queries.push_back({'Q', 2, 4, 0}); // colors 2,1,2 => 3 segments
        auto ans = countColorSegments(n, init, edges, queries);
        assert(ans.size() == 2);
        assert(ans[0] == 5);
        assert(ans[1] == 3);
    }
    // Test 10: update only a part, then query
    {
        int n = 4;
        std::vector<int> init = {1,2,3,4};
        // tree: 1-2, 2-3, 3-4
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        std::vector<Query> queries;
        queries.push_back({'C', 2, 3, 9}); // set nodes 2 and 3 to 9
        queries.push_back({'Q', 1, 4, 0}); // colors 1,9,9,4 => segments 1,9,4 => 3
        auto ans = countColorSegments(n, init, edges, queries);
        assert(ans.size() == 1);
        assert(ans[0] == 3);
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// We need to handle path updates (color assignment) and path queries (count color changes). Since `n` is small (≤2000), a straightforward approach works: build adjacency list, root the tree using DFS to compute parent array. For each query, we can walk up from `a` and `b` to the LCA, updating or reading colors along the path. But we need LCA. We can compute LCA using a naive method: since `n` is small, we can precompute parent and depth (via DFS from root) and then for each LCA query, climb the deeper node up until depths equal, then climb both together until they meet. That is O(n) per query, acceptable. For path update: walk from `a` up to LCA, set each node's color to `c`, do same from `b`. For query: walk from `a` up to LCA, count changes while moving, also keep track of last node's color from `a` side; do same from `b`, then combine: total changes = changes from `a` side + changes from `b` side + 1 (for the LCA node), but we need to subtract 1 if the last node on `a` side (which is the child of LCA on that side, or if `a==LCA` then the color at LCA) has same color as LCA, and similarly for `b` side. Implement carefully: when walking from `a` to LCA (exclusive), we compare current node's color with previous node's color along the path. After the loop, we have `lastA` as the last node visited (which is the child of LCA on `a` side, or if `a==LCA` then we didn't walk). Then count = changesA + changesB + 1 (for LCA) minus adjustments if `lastA` color equals LCA color and similarly for `lastB`. Edge cases: if `a==b`, then path is single node, answer is 1. If LCA is `a` or `b`, handle accordingly. Time complexity: O(n + q * n) due to climbing, with O(n) space for adjacency, parent, depth, color array. Since n≤2000 and q≤5000, worst-case 10 million operations, fine.
