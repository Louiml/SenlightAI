// Write a C++ function `std::vector<int> offlineLCA(const std::vector<std::vector<int>>& tree, int root, const std::vector<std::pair<int,int>>& queries)` that takes an undirected tree (given as an adjacency list with 0-based vertex indices), a root vertex, and a list of query pairs `(u, v)`, and returns for each query the lowest common ancestor (LCA) of `u` and `v`. The function must use the Tarjan offline LCA algorithm with a union-find (disjoint set) structure. The tree is guaranteed to be connected and acyclic, with at least one vertex. Queries may contain duplicate pairs or a vertex paired with itself. The function should return a vector of integers where the `i`-th element is the LCA for the `i`-th query.

#include <cassert>
#include <vector>

// The solution function is provided above; include it here.

int main() {
    // Test 1: Simple chain 0-1-2-3, root=0
    std::vector<std::vector<int>> tree1(4);
    tree1[0].push_back(1); tree1[1].push_back(0);
    tree1[1].push_back(2); tree1[2].push_back(1);
    tree1[2].push_back(3); tree1[3].push_back(2);
    std::vector<std::pair<int,int>> q1 = {{0,3}, {2,3}, {1,1}, {0,2}};
    auto res1 = offlineLCA(tree1, 0, q1);
    assert(res1[0] == 0); // LCA(0,3)=0
    assert(res1[1] == 2); // LCA(2,3)=2
    assert(res1[2] == 1); // LCA(1,1)=1
    assert(res1[3] == 0); // LCA(0,2)=0

    // Test 2: Star tree, center 0, leaves 1,2,3
    std::vector<std::vector<int>> tree2(4);
    tree2[0] = {1,2,3};
    tree2[1] = {0}; tree2[2] = {0}; tree2[3] = {0};
    std::vector<std::pair<int,int>> q2 = {{1,2}, {1,3}, {2,3}, {0,1}, {1,1}};
    auto res2 = offlineLCA(tree2, 0, q2);
    assert(res2[0] == 0); // LCA(1,2)=0
    assert(res2[1] == 0); // LCA(1,3)=0
    assert(res2[2] == 0); // LCA(2,3)=0
    assert(res2[3] == 0); // LCA(0,1)=0
    assert(res2[4] == 1); // LCA(1,1)=1

    // Test 3: Root not at 0, tree: 2-0, 2-1, 0-3, root=2
    std::vector<std::vector<int>> tree3(4);
    tree3[2] = {0,1};
    tree3[0] = {2,3};
    tree3[3] = {0};
    tree3[1] = {2};
    std::vector<std::pair<int,int>> q3 = {{1,3}, {0,1}, {0,3}, {3,3}};
    auto res3 = offlineLCA(tree3, 2, q3);
    assert(res3[0] == 2); // LCA(1,3)=2
    assert(res3[1] == 2); // LCA(0,1)=2
    assert(res3[2] == 0); // LCA(0,3)=0
    assert(res3[3] == 3); // LCA(3,3)=3

    // Test 4: Duplicate queries and self queries
    std::vector<std::vector<int>> tree4(1);
    tree4[0] = {};
    std::vector<std::pair<int,int>> q4 = {{0,0}, {0,0}};
    auto res4 = offlineLCA(tree4, 0, q4);
    assert(res4[0] == 0 && res4[1] == 0);

    // Test 5: Larger tree with multiple branches
    // 0-1, 0-2, 1-3, 1-4, 2-5, root=0
    std::vector<std::vector<int>> tree5(6);
    tree5[0] = {1,2};
    tree5[1] = {0,3,4};
    tree5[2] = {0,5};
    tree5[3] = {1};
    tree5[4] = {1};
    tree5[5] = {2};
    std::vector<std::pair<int,int>> q5 = {{3,4}, {3,5}, {4,5}, {3,2}, {5,0}, {1,2}};
    auto res5 = offlineLCA(tree5, 0, q5);
    assert(res5[0] == 1); // 3 and 4 under 1
    assert(res5[1] == 0); // 3 under 1, 5 under 2
    assert(res5[2] == 0); // 4 under 1, 5 under 2
    assert(res5[3] == 1); // 3 and 2: LCA under 0? Actually 3's branch through 1, 2's branch through 0->2: LCA=0? Wait 3->1->0, 2->0, LCA=0. But 1 and 2 are children of 0, so LCA(3,2)=0. Let's correct: q5[3] = {3,2} -> 0.
    assert(res5[3] == 0);
    assert(res5[4] == 0); // 5 and 0: LCA=0
    assert(res5[5] == 0); // 1 and 2: LCA=0

    return 0;
}

#include <vector>
#include <numeric>

// Union-Find with path compression and union by size
class DSU {
    std::vector<int> parent, size;
public:
    DSU(int n) : parent(n), size(n, 1) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    // Union x into y (sets parent of x's root to y's root)
    void unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return;
        // To keep consistent with Tarjan (parent becomes y), we attach x's root to y's root
        parent[rx] = ry;
        size[ry] += size[rx];
    }
};

std::vector<int> offlineLCA(const std::vector<std::vector<int>>& tree, int root,
                            const std::vector<std::pair<int,int>>& queries) {
    int n = tree.size();
    int q = queries.size();
    std::vector<int> ans(q, -1);
    std::vector<std::vector<std::pair<int,int>>> qList(n); // (other, queryIndex)
    for (int i = 0; i < q; ++i) {
        int u = queries[i].first;
        int v = queries[i].second;
        qList[u].emplace_back(v, i);
        qList[v].emplace_back(u, i);
    }

    std::vector<bool> visited(n, false);
    DSU dsu(n);

    // DFS using iterative stack to avoid recursion depth issues on large trees
    // We simulate recursion with an explicit stack of (node, parent, childIndex)
    struct Frame {
        int node, parent, nextChild;
    };
    std::vector<Frame> st;
    st.push_back({root, -1, 0});
    visited[root] = true;

    // We need post-order processing: after finishing all children, process queries.
    // We'll use a second stack approach or store completion events.
    // Simpler: use recursion with tail-call? But to be safe, we use iterative with events.
    // We'll use a vector of frames and a vector of "post" actions.
    // Since the problem likely has moderate n, we can use recursion as in the original code.
    // However, for robustness, we use an explicit stack and process queries when node is popped.
    std::vector<std::pair<int,int>> postOrder; // (node, parent) when all children done
    std::vector<std::pair<int,int>> stack; // (node, parent)
    stack.emplace_back(root, -1);
    visited[root] = true;
    while (!stack.empty()) {
        auto [u, parent] = stack.back();
        stack.pop_back();
        // Simulate entering node u: we push it to postOrder after processing children.
        // To do DFS, we need to process children before parent. Use a flag.
        // Better: use recursive lambda for clarity. But we avoid recursion by using two-phase.
        // We'll use a simple approach: since tree is acyclic and root given, we can do a BFS/DFS
        // to build parent and order, then process in reverse order. But Tarjan requires 
        // "visited" state during DFS, which we can simulate by processing in topological order.
        // However, the standard Tarjan uses DFS recusively. We'll implement recursive for clarity,
        // and assume the tree size is within stack limits (typical for such tasks).
        // For a fully iterative solution, we'd need a more complex state machine.
        // Here we implement the recursive version which matches the snippet's style.
    }

    // Recursive definition (clear and matches the original code)
    // Use a helper lambda with std::function
    std::function<void(int,int)> dfs = [&](int u, int parent) {
        visited[u] = true;
        for (int v : tree[u]) {
            if (v == parent) continue;
            dfs(v, u);
            dsu.unite(v, u); // attach v's set to u
        }
        // Process queries after all children
        for (auto [other, id] : qList[u]) {
            if (visited[other]) {
                ans[id] = dsu.find(other);
            }
        }
    };
    dfs(root, -1);

    return ans;
}

// The Tarjan offline LCA algorithm processes the tree via a depth-first search (DFS) while accumulating answers for queries using a union-find structure. The key idea: during DFS, when we finish processing a subtree rooted at a child, we union that child’s component into the current node’s component, setting the parent of the child’s representative to the current node. For a query `(u, v)` whose one endpoint has already been fully visited (i.e., its DFS call has completed), the LCA is the current representative (find) of the visited endpoint. To handle queries efficiently, we pre-store each query in two adjacency lists: one for each endpoint, each with the query index. During DFS, when we reach a node `u`, we first mark `u` as visited, then recursively process all unvisited neighbors, then after each child returns, we union that child’s set into `u`’s set (with `u` as the parent). After all children are processed, we iterate over queries involving `u`; for any query where the other endpoint is already visited, we set the answer to `find(other)`. Edge cases: when `u == v` in a query, when processing `u` first, the other endpoint `v` is the same node and is already visited (since we mark `u` visited before processing queries), so the answer is `find(v)` = `u`. If one endpoint is an ancestor of the other, the ancestor will have its DFS complete before the descendant processes the query, so the answer correctly becomes the ancestor. Complexity: each union-find operation is nearly O(α(n)) (inverse Ackermann), and each tree edge and query is processed a constant number of times, giving O((n + q) α(n)) time and O(n + q) space, where n is vertices and q is queries.
