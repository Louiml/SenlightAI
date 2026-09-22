Given an integer `N` (number of elements initially forming `N` singleton sets labeled `0` to `N-1`), and a list of `Q` queries, write a C++ function `std::vector<int> solvePersistentUnionFind(int N, const std::vector<std::array<int,4>>& queries)` that processes the queries in a version-tree fashion. Each query is represented as `{t, k, u, v}` where `t` is the operation type (`0` = union, `1` = same), `k` is the 0-based version index to start from (the current operation will be version `i+1`), and `u`, `v` are node indices. For each `type 1` query, record the answer (1 if `u` and `v` are in the same set in that query’s version, 0 otherwise). Queries of type `0` create a new version by performing a union of `u` and `v` on top of version `k`. For type `1`, the version does not change the state; it just answers based on version `k`. Return all answers in the order of type‑1 queries. The function must handle up to `N` and `Q` in the order of `2×10^5`, with the guarantee that union operations may be undone when moving between branches of the version tree (hence the need for a rollback (undo) union-find). The implementation must be efficient, since the total number of edges in the version tree is exactly `Q` and each node’s children are visited exactly once. Note that version indices are 1-based, and version 0 is the initial state with no unions. You may assume all inputs are valid (`k` is a valid prior version, `u` and `v` are in range).

The core challenge is that the problem is *persistent union-find*, but because the version graph is a tree (each new version is created from exactly one previous version, and queries don’t create new versions), we can process it with a depth‑first traversal of that version tree, using a *rollback union-find* (also called undoable DSU). The rollback DSU supports `unite(a,b)`, `same(a,b)`, and `undo()` to revert the last union, all in amortized `O(log N)` time (using union by size/rank and a stack of changes). The approach:  
1. Build an adjacency list `children` of the version tree. Version `i` has a parent version `k` (from the query). The initial version `0` has no associated query, but we treat it as the root. Each node stores its query details `{t, u, v, ans_index}`. For type `0`, the new version `i` has a union operation to be applied when we enter that node in DFS. For type `1`, we record the answer when we enter that node.  
2. Perform a DFS starting at version 0. When entering a node: if the node represents a union, perform `unite(u,v)` before recursing into its children. If the node represents a same query, record `uf.same(u,v)` and recurse (no state change). After processing all children, if the node was a union, call `undo()` to revert that union and allow the DFS to explore other branches correctly.  
3. Edge cases: The initial version 0 has no operation, so it is just a root for DFS. Queries may have `k` that refers to any previously created version, possibly causing a branching tree (not just a line). The rollback DSU ensures that each branch’s unions are isolated. The DFS visits each node exactly once, so total complexity is `O(Q α(N))` if union/undo are amortized constant (with union by size). Actually, with rollback, each union and undo is `O(log N)` worst-case, but overall `O(Q log N)`. The space is `O(N + Q)` for the adjacency lists and DSU arrays.  
4. The answer is stored in a vector in the order of type‑1 queries encountered in the input, not DFS order. So we need a mapping from node index to answer index (or we can additionally store the original query position). Simpler: when reading type‑1, we push the node index to a list `ans_nodes`, and during DFS we compute the answer and place it at the correct position using a counter (or we can store the answer in a map from node to answer). We’ll use a separate `std::vector<int> ans(Q+1)` initialized to `-1` and fill it directly when DFS reaches that node.

#include <vector>
#include <array>
#include <functional>

class RollbackUnionFind {
    int n;
    std::vector<int> parent, sz;
    std::vector<std::pair<int,int>> history;
public:
    RollbackUnionFind(int n) : n(n), parent(n), sz(n, 1) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) const {
        while (parent[x] != x) x = parent[x];
        return x;
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) {
            history.emplace_back(-1, -1);
            return false;
        }
        if (sz[a] < sz[b]) std::swap(a, b);
        history.emplace_back(b, a);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
    bool same(int a, int b) const {
        return find(a) == find(b);
    }
    void undo() {
        auto [b, a] = history.back();
        history.pop_back();
        if (b == -1) return;
        parent[b] = b;
        sz[a] -= sz[b];
    }
};

std::vector<int> solvePersistentUnionFind(
    int N,
    const std::vector<std::array<int,4>>& queries
) {
    int Q = queries.size();
    // Build version tree: children[i] = list of queries that have parent version i.
    std::vector<std::vector<int>> children(Q+1);
    // For each version 1..Q, store whether it's union or same and the nodes.
    // We'll store as: op_type (0=union, 1=same), u, v, and original query index (for answer order).
    std::vector<std::array<int,4>> node_info(Q+1);
    std::vector<int> answer_idx_for_node(Q+1, -1);
    int ans_cnt = 0;
    for (int i = 0; i < Q; ++i) {
        int t = queries[i][0];
        int k = queries[i][1];
        int u = queries[i][2];
        int v = queries[i][3];
        int node = i+1;
        children[k].push_back(node);
        node_info[node] = {t, u, v};
        if (t == 1) {
            answer_idx_for_node[node] = ans_cnt++;
        }
    }
    std::vector<int> ans(ans_cnt, -1);
    RollbackUnionFind uf(N);
    std::function<void(int)> dfs = [&](int node) {
        int t = node_info[node][0];
        int u = node_info[node][1];
        int v = node_info[node][2];
        if (t == 1) {
            ans[answer_idx_for_node[node]] = uf.same(u, v) ? 1 : 0;
            // Note: no state change, so just recurse.
        } else if (t == 0) {
            uf.unite(u, v);
        }
        for (int child : children[node]) {
            dfs(child);
        }
        if (t == 0) {
            uf.undo();
        }
    };
    dfs(0);
    return ans;
}

int main() {
    // Test 1: Simple union then same
    {
        std::vector<std::array<int,4>> q = {
            {0, 0, 0, 1}, // version 1: union(0,1) based on version 0
            {1, 0, 0, 1}, // version 2: same(0,1) based on version 0 -> should be 0
            {1, 1, 0, 1}  // version 3: same(0,1) based on version 1 -> should be 1
        };
        auto ans = solvePersistentUnionFind(3, q);
        std::vector<int> expected = {0, 1};
        assert(ans == expected);
    }
    // Test 2: Branching (persistent separation)
    {
        // Start with 4 nodes 0..3
        // version1: union(0,1) from 0
        // version2: union(2,3) from 0 (parallel to v1)
        // version3: same(0,1) from version1 -> true
        // version4: same(0,1) from version2 -> false (since v2 only unions 2,3)
        std::vector<std::array<int,4>> q = {
            {0, 0, 0, 1}, // v1
            {0, 0, 2, 3}, // v2
            {1, 1, 0, 1}, // v3 (from v1)
            {1, 2, 0, 1}  // v4 (from v2)
        };
        auto ans = solvePersistentUnionFind(4, q);
        std::vector<int> expected = {1, 0};
        assert(ans == expected);
    }
    // Test 3: Undo along same branch (multiple unions)
    {
        // version1: union(0,1)
        // version2: union(1,2) (so after v2: 0,1,2 all connected)
        // version3: same(0,2) from version2 -> true
        // version4: same(0,2) from version1 -> false (only 0,1 connected)
        std::vector<std::array<int,4>> q = {
            {0, 0, 0, 1},
            {0, 1, 1, 2},
            {1, 2, 0, 2},
            {1, 1, 0, 2}
        };
        auto ans = solvePersistentUnionFind(3, q);
        std::vector<int> expected = {1, 0};
        assert(ans == expected);
    }
    // Test 4: No unions, just queries
    {
        std::vector<std::array<int,4>> q = {
            {1, 0, 0, 1},
            {1, 0, 1, 0},
            {1, 0, 2, 2}
        };
        auto ans = solvePersistentUnionFind(3, q);
        std::vector<int> expected = {0, 0, 1};
        assert(ans == expected);
    }
    // Test 5: Chain of unions all meaningful
    {
        std::vector<std::array<int,4>> q = {
            {0, 0, 0, 1},
            {0, 1, 1, 2},
            {0, 2, 2, 3},
            {1, 3, 0, 3}, // all connected
            {1, 2, 0, 3}  // only up to version2: 0-1-2, 3 separate
        };
        auto ans = solvePersistentUnionFind(4, q);
        std::vector<int> expected = {1, 0};
        assert(ans == expected);
    }
    // Test 6: Large N but small Q – just a sanity
    {
        std::vector<std::array<int,4>> q = {
            {0, 0, 0, 999},
            {1, 1, 0, 999},
            {1, 0, 0, 999}
        };
        auto ans = solvePersistentUnionFind(1000, q);
        std::vector<int> expected = {1, 0};
        assert(ans == expected);
    }
    return 0;
}
