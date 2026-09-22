// Given a rooted tree where nodes are numbered from 1 to n, write a C++ function that answers range query problems: for a query interval [l, r] of the node numbers (not Euler indices), your function must return the node that, when temporarily "removed" (excluded) from consideration, maximizes the depth of the Lowest Common Ancestor (LCA) among the remaining nodes in that interval. Specifically, consider the set of nodes with IDs in [l, r]. You must try removing exactly one node from this set: either the node with minimum DFS-entry time (tin) or the node with maximum DFS-entry time (tout), whichever gives a deeper LCA when excluded. The function should return that optimal node ID and the depth (level) of the LCA of the remaining nodes (after exclusion). If the interval has exactly one node, the LCA is the node itself (depth = its level). The tree is given as a parent array where node i (2≤i≤n) has parent p_i (1≤p_i<i). You must implement the solution as a standalone function that accepts n, the parent list, and a list of queries, and returns a vector of pairs (nodeID, maxLCA_depth) for each query.
#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (from above)

int main() {
    // Test 1: Simple chain 1-2-3-4
    {
        int n = 4;
        vector<int> parent = {1, 2, 3}; // parent[0] for node2, parent[1] for node3, parent[2] for node4
        vector<pair<int,int>> queries = {{1,4}, {2,3}, {1,1}};
        auto res = solve_tree_queries(n, parent, queries);
        assert(res.size() == 3);
        // Query [1,4]: remove min or max; min tin is node1 (tin=1), max tin is node4 (tin=4). Removing min (node1) leaves {2,3,4}, LCA=2 depth1. Removing max (node4) leaves {1,2,3}, LCA=1 depth0. So choose node1 depth1.
        assert(res[0].first == 1 && res[0].second == 1);
        // Query [2,3]: nodes {2,3}. min tin is node2 (tin=2), max tin is node3 (tin=3). Removing min (node2) leaves {3}, LCA=3 depth2; removing max (node3) leaves {2}, LCA=2 depth1. So choose node2 depth2 (since depth2 > depth1? actually remove min gives depth2? Let's compute: level[2]=1, level[3]=2, LCA of {3}=3 depth2; LCA of {2}=2 depth1, so choose min_node (2) with depth1? Actually depth1 computed from removing min_node (node2) gives new_min = node3, max_node = node3 => LCA=3 depth2. Wait we swap: case1 is remove min_node -> new_min, LCA(new_min, max_node) = LCA(node3, node3) = node3 depth2. case2 remove max_node (node3) -> new_max=node2, LCA(min_node=node2, node2)=node2 depth1. So depth1=2 > depth2=1, so output min_node=2, depth=2. But note that after removing node2, remaining set is {3} and LCA depth is level[3]=2. So res[1] should be (2,2). Let's check: our code outputs min_node (node2) with depth1=2. Yes.
        assert(res[1].first == 2 && res[1].second == 2);
        // Query [1,1]: single node, both cases same, output node1 depth0.
        assert(res[2].first == 1 && res[2].second == 0);
    }

    // Test 2: Star tree with root 1 and leaves 2,3,4
    {
        int n = 4;
        vector<int> parent = {1, 1, 1}; // nodes 2,3,4 have parent 1
        vector<pair<int,int>> queries = {{2,4}, {1,4}};
        auto res = solve_tree_queries(n, parent, queries);
        assert(res.size() == 2);
        // Query [2,4]: nodes 2,3,4 all have depth1. Min tin order: DFS visits children in adjacency order. Since we push parent edges, and adjacency from 1 to 2,3,4 but DFS order depends on input order. But tin values depend on traversal. Let's compute: DFS(1) visits children in order of adjacency. Since we push child->parent and parent->child, adjacency of 1 is [2,3,4] in order? Actually we push for i=2..n: g[i].push_back(p); g[p].push_back(i); so g[1] gets 2,3,4 in increasing order. So DFS order: tin[1]=1, then child 2 -> tin[2]=2, child 3 -> tin[3]=3, child 4 -> tin[4]=4. So min_node=2, max_node=4. Removing min (2) leaves {3,4}, LCA=1 depth0. Removing max (4) leaves {2,3}, LCA=1 depth0. Both depth0, tie -> choose max_node (4) depth0? Our code picks else branch when depth2 >= depth1, so outputs max_node. res[0] = (4,0). Let's check: LCA1 = LCA(new_min=3, max_node=4) = 1 depth0; LCA2 = LCA(min_node=2, new_max=3) = 1 depth0; depth1==depth2 so else branch picks (max_node, depth2=0). So assert (4,0).
        assert(res[0].first == 4 && res[0].second == 0);
        // Query [1,4]: nodes 1,2,3,4. min_node=1, max_node=4. Removing min (1) leaves {2,3,4}, LCA=1 depth0. Removing max (4) leaves {1,2,3}, LCA=1 depth0. Tie -> choose max_node=4 depth0.
        assert(res[1].first == 4 && res[1].second == 0);
    }

    // Test 3: Larger tree with query not covering root
    {
        int n = 6;
        // Tree: 1-2, 1-3, 2-4, 2-5, 3-6
        vector<int> parent = {1, 1, 2, 2, 3}; // node2 parent1, node3 parent1, node4 parent2, node5 parent2, node6 parent3
        vector<pair<int,int>> queries = {{4,6}, {2,5}};
        auto res = solve_tree_queries(n, parent, queries);
        // Compute manually: DFS order depends on adjacency. adjacency: 1->[2,3], 2->[1,4,5], 3->[1,6], 4->[2],5->[2],6->[3]. DFS(1): tin[1]=1; child2: tin[2]=2; child4: tin[4]=3; child5: tin[5]=4; back to 1; child3: tin[3]=5; child6: tin[6]=6. So tin: 1:1,2:2,3:5,4:3,5:4,6:6.
        // Query [4,6]: nodes 4,5,6 with tin 3,4,6. min_node=4, max_node=6. Remove min (4): remaining {5,6}, LCA(5,6)=? 5 parent 2, 6 parent 3, LCA=1 depth0. Remove max (6): remaining {4,5}, LCA=2 depth1. So choose max_node=6 depth1.
        assert(res[0].first == 6 && res[0].second == 1);
        // Query [2,5]: nodes 2,3,4,5 with tin 2,5,3,4. min_node=2, max_node=3. Remove min (2): remaining {3,4,5}, LCA(4,5)=2? Actually new_min among {3,4,5} is node4 (tin3), max_node is node3 (tin5). LCA(4,3)=1 depth0. Remove max (3): remaining {2,4,5}, new_max is node5 (tin4), min_node=2. LCA(2,5)=2 depth1. Choose max_node=3 depth1.
        assert(res[1].first == 3 && res[1].second == 1);
    }

    cout << "All tests passed!" << endl;
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

// Return for each query the optimal node to remove and the maximum LCA depth of the remaining set.
vector<pair<int,int>> solve_tree_queries(int n, const vector<int>& parent, const vector<pair<int,int>>& queries) {
    // Build adjacency list (1-indexed)
    vector<vector<int>> g(n+1);
    for (int i=2; i<=n; ++i) {
        int p = parent[i-2]; // parent array is for i=2..n, so index i-2
        g[i].push_back(p);
        g[p].push_back(i);
    }

    // DFS to compute tin, tout, level, and store node by tin
    vector<int> tin(n+1), tout(n+1), level(n+1), node_at_tin(n+1);
    int tim = 0;
    function<void(int,int,int)> dfs = [&](int u, int par, int lvl) {
        tin[u] = ++tim;
        node_at_tin[tim] = u;
        level[u] = lvl;
        for (int v : g[u]) {
            if (v == par) continue;
            dfs(v, u, lvl+1);
        }
        tout[u] = tim;
    };
    dfs(1, 1, 0);

    // Precompute binary lifting for LCA
    int LG = 0;
    while ((1<<LG) <= n) LG++;
    vector<vector<int>> up(LG, vector<int>(n+1));
    up[0][1] = 1; // root's parent is itself
    function<void(int,int)> dfs2 = [&](int u, int par) {
        for (int v : g[u]) {
            if (v == par) continue;
            up[0][v] = u;
            dfs2(v, u);
        }
    };
    dfs2(1, 1);
    for (int i=1; i<LG; ++i) {
        for (int v=1; v<=n; ++v) {
            up[i][v] = up[i-1][up[i-1][v]];
        }
    }

    // LCA function
    auto lca = [&](int u, int v) {
        if (level[u] < level[v]) swap(u,v);
        int diff = level[u] - level[v];
        for (int i=0; i<LG; ++i) {
            if (diff & (1<<i)) u = up[i][u];
        }
        if (u == v) return u;
        for (int i=LG-1; i>=0; --i) {
            if (up[i][u] != up[i][v]) {
                u = up[i][u];
                v = up[i][v];
            }
        }
        return up[0][u];
    };

    // Segment tree for min tin (stores pair: tin, nodeID)
    struct MinSeg {
        vector<pair<int,int>> tree;
        int n;
        MinSeg(int size) : n(size), tree(4*size, {INT_MAX, 0}) {}
        void build(int node, int l, int r, const vector<pair<int,int>>& arr) {
            if (l == r) {
                tree[node] = arr[l];
                return;
            }
            int mid = (l+r)/2;
            build(node*2, l, mid, arr);
            build(node*2+1, mid+1, r, arr);
            tree[node] = min(tree[node*2], tree[node*2+1]);
        }
        void update(int node, int l, int r, int pos, pair<int,int> val) {
            if (l == r) {
                tree[node] = val;
                return;
            }
            int mid = (l+r)/2;
            if (pos <= mid) update(node*2, l, mid, pos, val);
            else update(node*2+1, mid+1, r, pos, val);
            tree[node] = min(tree[node*2], tree[node*2+1]);
        }
        pair<int,int> query(int node, int l, int r, int ql, int qr) {
            if (ql > r || qr < l) return {INT_MAX, 0};
            if (ql <= l && r <= qr) return tree[node];
            int mid = (l+r)/2;
            return min(query(node*2, l, mid, ql, qr), query(node*2+1, mid+1, r, ql, qr));
        }
    };

    // Segment tree for max tin (stores pair: tin, nodeID)
    struct MaxSeg {
        vector<pair<int,int>> tree;
        int n;
        MaxSeg(int size) : n(size), tree(4*size, {INT_MIN, 0}) {}
        void build(int node, int l, int r, const vector<pair<int,int>>& arr) {
            if (l == r) {
                tree[node] = arr[l];
                return;
            }
            int mid = (l+r)/2;
            build(node*2, l, mid, arr);
            build(node*2+1, mid+1, r, arr);
            tree[node] = max(tree[node*2], tree[node*2+1]);
        }
        void update(int node, int l, int r, int pos, pair<int,int> val) {
            if (l == r) {
                tree[node] = val;
                return;
            }
            int mid = (l+r)/2;
            if (pos <= mid) update(node*2, l, mid, pos, val);
            else update(node*2+1, mid+1, r, pos, val);
            tree[node] = max(tree[node*2], tree[node*2+1]);
        }
        pair<int,int> query(int node, int l, int r, int ql, int qr) {
            if (ql > r || qr < l) return {INT_MIN, 0};
            if (ql <= l && r <= qr) return tree[node];
            int mid = (l+r)/2;
            return max(query(node*2, l, mid, ql, qr), query(node*2+1, mid+1, r, ql, qr));
        }
    };

    // Build initial arrays: for node id i in 1..n, store (tin[i], i)
    vector<pair<int,int>> arr_min(n+1), arr_max(n+1);
    for (int i=1; i<=n; ++i) {
        arr_min[i] = {tin[i], i};
        arr_max[i] = {tin[i], i};
    }
    MinSeg min_seg(n);
    MaxSeg max_seg(n);
    min_seg.build(1, 1, n, arr_min);
    max_seg.build(1, 1, n, arr_max);

    vector<pair<int,int>> results;
    results.reserve(queries.size());

    for (auto& [l, r] : queries) {
        // Query original min and max nodes in [l,r]
        auto p1 = min_seg.query(1, 1, n, l, r); // min tin node
        auto p2 = max_seg.query(1, 1, n, l, r); // max tin node
        int min_node = p1.second;
        int max_node = p2.second;

        // Case 1: remove min_node, find new min in interval
        int orig_min_val = p1.first;
        min_seg.update(1, 1, n, min_node, {INT_MAX, 0}); // remove it
        auto new_min = min_seg.query(1, 1, n, l, r);
        min_seg.update(1, 1, n, min_node, {orig_min_val, min_node}); // restore
        int LCA1 = lca(new_min.second, max_node);
        int depth1 = level[LCA1];

        // Case 2: remove max_node, find new max in interval
        int orig_max_val = p2.first;
        max_seg.update(1, 1, n, max_node, {INT_MIN, 0}); // remove it
        auto new_max = max_seg.query(1, 1, n, l, r);
        max_seg.update(1, 1, n, max_node, {orig_max_val, max_node}); // restore
        int LCA2 = lca(min_node, new_max.second);
        int depth2 = level[LCA2];

        if (depth1 > depth2) {
            results.push_back({min_node, depth1});
        } else {
            results.push_back({max_node, depth2});
        }
    }
    return results;
}
// The key observation is that for any set of nodes, the LCA of all nodes in that set is determined by the nodes with the minimum and maximum DFS entry times (tin) in that set. Specifically, the LCA of the entire set is the LCA of the pair (node with min tin, node with max tin). If we remove one node to maximize the depth of the LCA of the remaining nodes, the candidate node to remove must be either the one with minimum tin or the one with maximum tin; removing any other node leaves the min and max tin unchanged, so the LCA remains the same. Therefore, for each query interval [l,r], we compute the min-tin node and max-tin node using two segment trees (one for min, one for max, each also storing the node ID). Then we simulate removal: (a) temporarily set the min-tin node's tin to +infinity and find the new min-tin node in the interval; the LCA of the remaining set is LCA(newMinNode, maxTinNode). (b) similarly, temporarily set the max-tin node's tin to 0 (or -infinity) and find the new max-tin node; the LCA of the remaining set is LCA(minTinNode, newMaxNode). Compare the depths of these two candidate LCAs and pick the deeper one. If the interval has only one node, both candidates yield the same node itself as LCA. Implementation requires building the tree, computing tin/tout/level via DFS, precomputing binary lifting ancestors for LCA, and building two segment trees over node IDs (1..n) that store (tinValue, nodeID) for min and max. Updates temporarily modify the segment tree and then restore. Time complexity per query is O(log n) for two segment tree queries and two LCA calls (each O(log n)), so total O((n+q) log n). Space complexity is O(n log n) for the LCA table plus O(n) for segment trees.
