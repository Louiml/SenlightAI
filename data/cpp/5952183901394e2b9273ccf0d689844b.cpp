// Implement a C++ function `simulatePackageManager` that models installing and uninstalling software packages in a dependency tree. The input is a rooted tree with nodes numbered `0` to `n-1` (node 0 is the root), where each node's parent is given, a list of operations ('i' for install, 'u' for uninstall) applied to specific nodes, and an initial count of installed packages (0). Installing a node installs it and all of its ancestors (path to root), while uninstalling a node removes it and all of its descendants (subtree). After each operation, you must output the absolute change in the total number of installed packages compared to before the operation. The function should take `n`, a vector of parents for nodes 1..n-1 (parent of node 0 is -1), a vector of operations (each a pair of char and node index), and return a vector of integers representing the change after each operation. The tree is rooted at 0 and edges are given such that parent[i] < i for all i (but your solution must handle arbitrary valid trees). The final state is not needed, only the changes.
// The problem requires efficient range updates and queries on a tree. We use Heavy-Light Decomposition (HLD) to map each node to a contiguous segment in a linear array so that any path (for install) and any subtree (for uninstall) can be represented as a union of `O(log n)` contiguous intervals. For install, we repeatedly update the path from node x up to the root, which in HLD is decomposed into at most `O(log n)` chains; for each chain segment, we set all values to `1`. For uninstall, we set the entire subtree interval to `0` (since in HLD, a subtree is a contiguous range when nodes are numbered by DFS order). We maintain a segment tree that supports range assignment (set to 0 or 1) and range sum query. After each operation, we query the total sum (number of installed nodes) and compare with the previous sum to return the absolute difference. Edge cases: installing an already-installed node should have no net effect (assigning 1 over a segment that already has 1s is idempotent), and uninstalling an uninstalled node similarly. The initial state is all zeros. Time complexity: O((n + q) log n) for building and each operation, with O(n) space. Ensure proper lazy propagation for range assignment.
#include <vector>
#include <string>
#include <cstdint>

namespace {
constexpr int MAXN = 100005;
int head[MAXN], to[2 * MAXN], nxt[2 * MAXN], ecnt;
int parent[MAXN], depth[MAXN], sz[MAXN], heavy[MAXN];
int top[MAXN], dfn[MAXN], dfs_clock;
int seg_tree[4 * MAXN], lazy_set[4 * MAXN];
bool has_lazy[4 * MAXN];

inline void add_edge(int u, int v) {
    to[ecnt] = v;
    nxt[ecnt] = head[u];
    head[u] = ecnt++;
}

void dfs_size(int u, int p) {
    parent[u] = p;
    sz[u] = 1;
    int max_sub = 0;
    for (int e = head[u]; e != -1; e = nxt[e]) {
        int v = to[e];
        if (v == p) continue;
        depth[v] = depth[u] + 1;
        dfs_size(v, u);
        sz[u] += sz[v];
        if (sz[v] > max_sub) {
            max_sub = sz[v];
            heavy[u] = v;
        }
    }
}

void dfs_hld(int u, int h) {
    top[u] = h;
    dfn[u] = ++dfs_clock;
    if (heavy[u]) dfs_hld(heavy[u], h);
    for (int e = head[u]; e != -1; e = nxt[e]) {
        int v = to[e];
        if (v != parent[u] && v != heavy[u]) {
            dfs_hld(v, v);
        }
    }
}

void build_seg(int idx, int l, int r) {
    lazy_set[idx] = 0;
    has_lazy[idx] = false;
    seg_tree[idx] = 0;
    if (l == r) return;
    int mid = (l + r) >> 1;
    build_seg(idx * 2, l, mid);
    build_seg(idx * 2 + 1, mid + 1, r);
}

void apply_lazy(int idx, int l, int r, int val) {
    seg_tree[idx] = (r - l + 1) * val;
    lazy_set[idx] = val;
    has_lazy[idx] = true;
}

void push_down(int idx, int l, int r) {
    if (has_lazy[idx] && l != r) {
        int mid = (l + r) >> 1;
        apply_lazy(idx * 2, l, mid, lazy_set[idx]);
        apply_lazy(idx * 2 + 1, mid + 1, r, lazy_set[idx]);
        has_lazy[idx] = false;
    }
}

void update(int idx, int l, int r, int ql, int qr, int val) {
    if (ql > r || qr < l) return;
    if (ql <= l && r <= qr) {
        apply_lazy(idx, l, r, val);
        return;
    }
    push_down(idx, l, r);
    int mid = (l + r) >> 1;
    update(idx * 2, l, mid, ql, qr, val);
    update(idx * 2 + 1, mid + 1, r, ql, qr, val);
    seg_tree[idx] = seg_tree[idx * 2] + seg_tree[idx * 2 + 1];
}

int query(int idx, int l, int r, int ql, int qr) {
    if (ql > r || qr < l) return 0;
    if (ql <= l && r <= qr) return seg_tree[idx];
    push_down(idx, l, r);
    int mid = (l + r) >> 1;
    return query(idx * 2, l, mid, ql, qr) + query(idx * 2 + 1, mid + 1, r, ql, qr);
}

void path_update(int u, int v) {
    while (top[u] != top[v]) {
        if (depth[top[u]] < depth[top[v]]) {
            std::swap(u, v);
        }
        update(1, 1, dfs_clock, dfn[top[u]], dfn[u], 1);
        u = parent[top[u]];
    }
    if (depth[u] > depth[v]) std::swap(u, v);
    update(1, 1, dfs_clock, dfn[u], dfn[v], 1);
}
} // namespace

// Performs install/uninstall operations on a package tree.
std::vector<int> simulatePackageManager(
    int n,
    const std::vector<int>& parents,
    const std::vector<std::pair<char, int>>& ops) {
    
    ecnt = 0;
    dfs_clock = 0;
    for (int i = 0; i <= n; ++i) {
        head[i] = -1;
        heavy[i] = 0;
        depth[i] = sz[i] = parent[i] = 0;
    }
    for (int i = 1; i < n; ++i) {
        add_edge(parents[i], i);
        add_edge(i, parents[i]);
    }
    depth[0] = 0;
    dfs_size(0, -1);
    dfs_hld(0, 0);
    build_seg(1, 1, n);

    std::vector<int> result;
    int current = 0;
    for (const auto& op : ops) {
        char typ = op.first;
        int x = op.second;
        if (typ == 'i') {
            path_update(0, x);
        } else {
            // subtree of x: dfn[x] .. dfn[x] + sz[x] - 1
            update(1, 1, n, dfn[x], dfn[x] + sz[x] - 1, 0);
        }
        int new_val = query(1, 1, n, 1, n);
        result.push_back(new_val > current ? new_val - current : current - new_val);
        current = new_val;
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Example 1: n=1, root only
    {
        std::vector<int> parents = {-1};
        std::vector<std::pair<char,int>> ops = {{'i',0},{'u',0},{'i',0}};
        auto res = simulatePackageManager(1, parents, ops);
        assert((res == std::vector<int>{1,1,1}));
    }
    // Example 2: chain 0-1-2
    {
        std::vector<int> parents = {-1,0,1};
        std::vector<std::pair<char,int>> ops = {{'i',2},{'i',1},{'u',2},{'i',2}};
        auto res = simulatePackageManager(3, parents, ops);
        // Initial: 0
        // i2 -> 0,1,2 installed -> 3 -> diff 3
        // i1 -> still 3 (0,1,2) -> diff 0
        // u2 -> removes 2 only -> 2 installed -> diff 1
        // i2 -> installs path 2->1->0 all already 0,1 installed except 2 -> 3 installed -> diff 1
        assert((res == std::vector<int>{3,0,1,1}));
    }
    // Example 3: star with root 0 and leaves 1,2,3
    {
        std::vector<int> parents = {-1,0,0,0};
        std::vector<std::pair<char,int>> ops = {{'i',1},{'i',2},{'u',1},{'u',0}};
        auto res = simulatePackageManager(4, parents, ops);
        // i1 -> install 0,1 -> 2 -> diff 2
        // i2 -> install 0,2 (0 already) -> 3 -> diff 1
        // u1 -> uninstall 1 alone -> 2 -> diff 1
        // u0 -> uninstall subtree of 0 (0,2,3) -> 0 -> diff 2
        assert((res == std::vector<int>{2,1,1,2}));
    }
    // Example 4: deep tree, uninstall on internal node
    {
        // 0 - 1 - 2, and 1 - 3
        std::vector<int> parents = {-1,0,1,1};
        std::vector<std::pair<char,int>> ops = {{'i',2},{'u',1},{'i',3}};
        auto res = simulatePackageManager(4, parents, ops);
        // i2 -> install 0,1,2 -> 3 -> diff 3
        // u1 -> uninstall subtree of 1 (1,2,3) -> only 0 -> diff 2
        // i3 -> install path 3->1->0 -> 0,1,3 installed -> 3 -> diff 2
        assert((res == std::vector<int>{3,2,2}));
    }
    // Example 5: empty operations
    {
        std::vector<int> parents = {-1,0};
        std::vector<std::pair<char,int>> ops = {};
        auto res = simulatePackageManager(2, parents, ops);
        assert(res.empty());
    }
    return 0;
}
