Write a C++ function `std::vector<int> colorDominance(const std::vector<int>& colors, const std::vector<std::pair<int,int>>& edges)` that takes a list of vertex colors (0-indexed, values assumed to be 0 or 1) and undirected edges (vertices numbered 0..n-1), builds the tree (the input is guaranteed to be a valid tree), and returns for each vertex the number of color-0 vertices in its subtree when the tree is rooted arbitrarily (root at vertex 0). The function must use the DSU-on-tree (small-to-large) technique. The input colors are given for each vertex in order, and edges are given as pairs of vertex indices. The function should return a vector of length n where the i-th element is the count of color-0 vertices in the subtree rooted at vertex i (with root 0).

#include <cassert>
#include <vector>

// The solution function is assumed to be declared as above.
// Include the solution code or link it appropriately.

int main() {
    // Test 1: simple chain with two colors
    std::vector<int> colors1 = {0, 1, 0};
    std::vector<std::pair<int,int>> edges1 = {{0,1},{1,2}};
    auto res1 = colorDominance(colors1, edges1);
    assert(res1 == std::vector<int>({2, 1, 1})); // root0:0,1,0 => zeros 2; node1:1,0 => zeros1; node2:0 => zeros1

    // Test 2: star with center color 1, leaves all 0
    std::vector<int> colors2 = {1, 0, 0, 0};
    std::vector<std::pair<int,int>> edges2 = {{0,1},{0,2},{0,3}};
    auto res2 = colorDominance(colors2, edges2);
    assert(res2 == std::vector<int>({3, 1, 1, 1}));

    // Test 3: single vertex
    std::vector<int> colors3 = {0};
    std::vector<std::pair<int,int>> edges3 = {};
    auto res3 = colorDominance(colors3, edges3);
    assert(res3 == std::vector<int>({1}));

    // Test 4: all zeros
    std::vector<int> colors4 = {0, 0, 0, 0};
    std::vector<std::pair<int,int>> edges4 = {{0,1},{1,2},{2,3}};
    auto res4 = colorDominance(colors4, edges4);
    assert(res4 == std::vector<int>({4,3,2,1}));

    // Test 5: balanced binary tree with mixed colors
    std::vector<int> colors5 = {1, 0, 0, 1, 0, 1, 0};
    // Edges: 0-1,0-2,1-3,1-4,2-5,2-6
    std::vector<std::pair<int,int>> edges5 = {{0,1},{0,2},{1,3},{1,4},{2,5},{2,6}};
    auto res5 = colorDominance(colors5, edges5);
    // Compute manually: subtree of 0: zeros at 1,2,4,6 => 4 zeros
    // subtree of 1: zeros at 1,4 => 2 zeros
    // subtree of 2: zeros at 2,6 => 2 zeros
    // node3: color1 =>0, node4:0=>1, node5:1=>0, node6:0=>1
    assert(res5 == std::vector<int>({4,2,2,0,1,0,1}));

    // Test 6: chain all ones
    std::vector<int> colors6 = {1,1,1,1};
    std::vector<std::pair<int,int>> edges6 = {{0,1},{1,2},{2,3}};
    auto res6 = colorDominance(colors6, edges6);
    assert(res6 == std::vector<int>({0,0,0,0}));

    return 0;
}

#include <vector>
#include <functional>

// Computes for each vertex the number of color-0 vertices in its subtree when rooted at 0.
std::vector<int> colorDominance(const std::vector<int>& colors, const std::vector<std::pair<int,int>>& edges) {
    int n = static_cast<int>(colors.size());
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    std::vector<int> sz(n, 0);
    std::function<void(int,int)> dfs_size = [&](int u, int p) {
        sz[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs_size(v, u);
            sz[u] += sz[v];
        }
    };
    dfs_size(0, -1);

    std::vector<int> cntZero(2, 0); // cntZero[0] tracks zeros, but we only need zeros count
    std::vector<int> ans(n, 0);
    std::vector<int> big(n, 0); // big[u]=1 if u is a heavy child in current context

    // add subtree of u, excluding big children, with delta
    std::function<void(int,int,int)> add = [&](int u, int p, int delta) {
        cntZero[0] += (colors[u] == 0 ? delta : 0);
        for (int v : adj[u]) {
            if (v == p || big[v]) continue;
            add(v, u, delta);
        }
    };

    std::function<void(int,int,int)> dsu = [&](int u, int p, int keep) {
        // find heavy child
        int heavy = -1, maxSz = -1;
        for (int v : adj[u]) {
            if (v != p && sz[v] > maxSz) {
                maxSz = sz[v];
                heavy = v;
            }
        }
        // process light children first with keep=0
        for (int v : adj[u]) {
            if (v != p && v != heavy) {
                dsu(v, u, 0);
            }
        }
        // process heavy child with keep=1
        if (heavy != -1) {
            big[heavy] = 1;
            dsu(heavy, u, 1);
        }
        // add current node and all light subtrees
        add(u, p, 1);
        ans[u] = cntZero[0];
        if (heavy != -1) big[heavy] = 0;
        if (!keep) add(u, p, -1); // clean up if needed
    };

    dsu(0, -1, 0);
    return ans;
}

// The solution uses the DSU-on-tree (Sack) algorithm to efficiently compute subtree queries for all vertices. First, compute subtree sizes via a recursive DFS. Then, perform a second DFS that processes light children first (keeping=false) to clear their contribution, then processes the heavy child (keeping=true) to retain its data, then adds the current node and all light subtrees. Since colors are only 0 or 1, maintain a count of zeros. The answer for each vertex is the current count of zeros after adding its entire subtree. Edge cases: single vertex tree, all colors same, colors alternating. The recursion depth can be O(n) for a chain, so use an iterative stack or increase stack (but for typical constraints, recursion is fine). Time complexity is O(n log n) for n up to 1e5, space O(n).
