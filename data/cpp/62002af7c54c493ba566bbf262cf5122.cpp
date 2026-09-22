// Given a rooted tree (1-indexed) with `N` nodes and a list of special nodes `S`, write a C++ function `buildVirtualTree(int N, const vector<vector<int>>& adj, const vector<int>& S)` that constructs the virtual tree induced by `S` (plus their necessary LCAs) and returns the adjacency list of the virtual tree, re-rooted at the node with the smallest DFS discovery time among the virtual nodes. The function must internally compute LCA using binary lifting (depth up to `1e5`, number of nodes up to `3e5`). The virtual tree edges must be directed from parent to child in the original rooted tree (rooted at 1). The returned adjacency list must be a `vector<vector<int>>` of size `N+1`, where only entries for nodes that appear in the virtual tree are non-empty. The input graph is guaranteed to be a tree, and `S` contains no duplicates. The tree is rooted at node 1. You must include all necessary preprocessing (tin, tout, depth, up table) inside the function or a helper class. The function should not modify the original adjacency list. The returned virtual tree must contain exactly the nodes from `S` plus all LCAs of adjacent pairs in the sorted order (by `tin`). The edges must connect parent-child relationships in the original rooted tree. The root of the returned virtual tree must be the node with the smallest `tin` among all virtual nodes.
The solution uses Euler tour to assign `tin` and `tout` for ancestor checks. Preprocess depth and binary lifting table `up[node][k]` for LCA queries in O(N log N) time. To build the virtual tree, sort the given special nodes by `tin`. Append LCAs of every adjacent pair in the sorted order. Sort the combined list again by `tin`, remove duplicates, and then use a stack to construct the tree: for each node `u` in sorted order, pop from stack until the top is an ancestor of `u`. If the stack is non-empty, add `u` as a child of the top. Push `u` onto the stack. This yields a virtual tree with edges directed from ancestor to descendant. The root is naturally the first element in the sorted order (smallest tin). Time complexity is O((|S| + K) log N) for sorting and O(|S| log N) for LCA queries, where K is the number of added LCA nodes (≤ |S|-1). Space complexity is O(N log N) for the up table plus O(N) for auxiliary arrays. Edge case: when `S` is empty, return empty adjacency list. When `S` has one node, the virtual tree has just that node (no edges). Duplicate LCAs must be removed using a set or sorting+unique. Ancestor check requires `tin[u] <= tin[v] && tout[v] <= tout[u]`.
#include <bits/stdc++.h>
using namespace std;

// Build virtual tree (parent->child directed edges) from a list of special nodes.
// Returns adjacency list of size N+1. Root is the node with smallest tin among virtual nodes.
// Assumes tree rooted at 1, nodes 1..N.
vector<vector<int>> buildVirtualTree(int N, const vector<vector<int>>& adj, const vector<int>& S) {
    if (S.empty()) return vector<vector<int>>(N + 1);

    // Preprocess LCA
    const int LOG = 20; // for N up to 3e5, log2(3e5) ~ 18.2, use 20
    vector<int> tin(N + 1), tout(N + 1), depth(N + 1);
    vector<vector<int>> up(N + 1, vector<int>(LOG));
    int timer = 0;
    function<void(int, int)> dfs = [&](int u, int p) {
        tin[u] = ++timer;
        up[u][0] = p;
        for (int k = 1; k < LOG; ++k) {
            up[u][k] = up[up[u][k-1]][k-1];
        }
        for (int v : adj[u]) {
            if (v != p) {
                depth[v] = depth[u] + 1;
                dfs(v, u);
            }
        }
        tout[u] = ++timer;
    };
    dfs(1, 1);

    auto is_ancestor = [&](int u, int v) {
        return tin[u] <= tin[v] && tout[v] <= tout[u];
    };

    auto lca = [&](int u, int v) {
        if (is_ancestor(u, v)) return u;
        if (is_ancestor(v, u)) return v;
        for (int k = LOG - 1; k >= 0; --k) {
            if (!is_ancestor(up[u][k], v)) {
                u = up[u][k];
            }
        }
        return up[u][0];
    };

    // Build virtual node list
    vector<int> nodes = S;
    sort(nodes.begin(), nodes.end(), [&](int a, int b) { return tin[a] < tin[b]; });
    for (size_t i = 1; i < nodes.size(); ++i) {
        nodes.push_back(lca(nodes[i-1], nodes[i]));
    }
    sort(nodes.begin(), nodes.end(), [&](int a, int b) { return tin[a] < tin[b]; });
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

    // Build virtual tree
    vector<vector<int>> vir(N + 1);
    vector<int> stk;
    for (int u : nodes) {
        while (!stk.empty() && !is_ancestor(stk.back(), u)) {
            stk.pop_back();
        }
        if (!stk.empty()) {
            vir[stk.back()].push_back(u);
        }
        stk.push_back(u);
    }
    return vir;
}
#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (copy from above)

int main() {
    // Example tree: 1-2, 1-3, 2-4, 2-5, 3-6
    int N = 6;
    vector<vector<int>> adj(N+1);
    adj[1] = {2,3};
    adj[2] = {1,4,5};
    adj[3] = {1,6};
    adj[4] = {2};
    adj[5] = {2};
    adj[6] = {3};

    // Test 1: Special nodes {4,5} -> virtual tree: 2->4, 2->5
    auto vir1 = buildVirtualTree(N, adj, {4,5});
    assert(vir1[2].size() == 2 && (vir1[2][0]==4 || vir1[2][0]==5) && (vir1[2][1]==4 || vir1[2][1]==5));
    assert(vir1[4].empty() && vir1[5].empty());

    // Test 2: Special nodes {4,6} -> LCA=1, virtual tree: 1->2, 1->3, 2->4, 3->6
    auto vir2 = buildVirtualTree(N, adj, {4,6});
    assert(vir2[1].size() == 2 && vir2[2].size()==1 && vir2[3].size()==1);
    assert(vir2[2][0]==4 && vir2[3][0]==6);

    // Test 3: Special nodes {4,5,6} -> virtual tree: 1->2, 1->3, 2->4, 2->5, 3->6
    auto vir3 = buildVirtualTree(N, adj, {4,5,6});
    assert(vir3[1].size()==2 && vir3[2].size()==2 && vir3[3].size()==1);
    assert(vir3[4].empty() && vir3[5].empty() && vir3[6].empty());

    // Test 4: Single node {4} -> virtual tree has only node 4, no edges
    auto vir4 = buildVirtualTree(N, adj, {4});
    assert(vir4[4].empty());

    // Test 5: Empty set -> all empty
    auto vir5 = buildVirtualTree(N, adj, {});
    for (int i=1;i<=N;i++) assert(vir5[i].empty());

    // Test 6: All nodes {1,2,3,4,5,6} -> original tree structure
    auto vir6 = buildVirtualTree(N, adj, {1,2,3,4,5,6});
    assert(vir6[1].size()==2 && vir6[2].size()==2 && vir6[3].size()==1);
    assert(vir6[4].empty() && vir6[5].empty() && vir6[6].empty());

    // Test 7: Special nodes {2,3} -> virtual tree: 1->2,1->3
    auto vir7 = buildVirtualTree(N, adj, {2,3});
    assert(vir7[1].size()==2 && vir7[2].empty() && vir7[3].empty());

    // Test 8: Special nodes {5,6} -> LCA=1, virtual tree: 1->2,1->3,2->5,3->6
    auto vir8 = buildVirtualTree(N, adj, {5,6});
    assert(vir8[1].size()==2 && vir8[2].size()==1 && vir8[2][0]==5 && vir8[3].size()==1 && vir8[3][0]==6);

    // Test 9: Special nodes {2,4} -> virtual tree: 1->2,2->4
    auto vir9 = buildVirtualTree(N, adj, {2,4});
    assert(vir9[1].size()==1 && vir9[2].size()==1 && vir9[2][0]==4);

    // Test 10: Special nodes {1} -> just root
    auto vir10 = buildVirtualTree(N, adj, {1});
    assert(vir10[1].empty());

    return 0;
}
