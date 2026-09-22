/*
Given a tree represented as an undirected adjacency list, write a C++ function `std::vector<int> allNodeLongestPaths(int n, const std::vector<std::vector<int>>& adj)` that, for each node in the tree (0-indexed), computes the length of the longest path that starts at that node and ends at any leaf in the tree. Here, "length" is measured as the number of nodes on the path, including both the starting node and the leaf. The tree has exactly `n` nodes and is connected, with `n-1` edges. If a node is itself a leaf, its longest path length is 1. The function should return a vector of length `n` where the i-th element is the longest path length starting from node i. The tree may have up to 10^5 nodes, so the solution must be efficient.
*/

#include <vector>
#include <algorithm>

// Computes for each node the length of the longest path starting at that node
// and ending at some leaf. The tree has n nodes (0..n-1) and is given as an
// undirected adjacency list. Path length is measured in number of nodes.
std::vector<int> allNodeLongestPaths(int n, const std::vector<std::vector<int>>& adj) {
    if (n == 0) return {};

    std::vector<int> down(n, 0);   // longest path from node to leaf in its subtree (counting node)
    std::vector<int> up(n, 0);     // longest path from node to leaf that goes through its parent first
    std::vector<int> ans(n, 0);

    // First DFS: compute down[] (post-order).
    // Use iterative stack to avoid recursion depth issues for large n.
    std::vector<int> parent(n, -1);
    std::vector<int> order;
    std::vector<int> stack = {0};
    parent[0] = 0; // root is 0 (any node works; tree is connected)
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        order.push_back(u);
        for (int v : adj[u]) {
            if (v == parent[u]) continue;
            parent[v] = u;
            stack.push_back(v);
        }
    }
    // Process in reverse order (post-order)
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        down[u] = 1; // at least the node itself
        for (int v : adj[u]) {
            if (v == parent[u]) continue;
            down[u] = std::max(down[u], down[v] + 1);
        }
    }

    // Second DFS: compute up[] and ans[] (pre-order).
    // For the root, up[root] = 0 (no parent path), but we can set it to 0 and handle.
    // We'll do a BFS/DFS from root to children.
    std::vector<int> queue;
    queue.push_back(0);
    std::vector<bool> visited(n, false);
    visited[0] = true;
    while (!queue.empty()) {
        int u = queue.back();
        queue.pop_back();

        // Compute the best downward path from u not going into each child.
        // We need top two down values among children of u.
        // Additionally, up[u] may give a path going above u.
        // Best path starting at u that does not go into a specific child c is:
        //   max( up[u] , 1 + max( down[child] for child != c ) )
        // But up[u] already includes u? Let's define up[u] as the length of the path from u to a leaf
        // that goes through its parent first, counting nodes from u. For root, up[0]=0 (no such path).
        // For a child c, up[c] = 1 + (best path starting at u that does not go into c).
        // That best path starting at u (not going into c) is:
        //   max( up[u] , 1 + max_down_excluding_c )
        // where max_down_excluding_c is the maximum down[child] over children != c.
        // Note: down[child] already includes the child node.

        // Gather children and their down values
        std::vector<int> children;
        std::vector<int> downValues;
        for (int v : adj[u]) {
            if (v == parent[u]) continue;
            children.push_back(v);
            downValues.push_back(down[v]);
        }
        // Find the largest and second largest down values among children
        int max1 = -1, max2 = -1; // sentinel values; down values are at least 1
        for (int val : downValues) {
            if (val > max1) {
                max2 = max1;
                max1 = val;
            } else if (val > max2) {
                max2 = val;
            }
        }
        // For each child, compute the best path from u not going into it
        for (size_t idx = 0; idx < children.size(); ++idx) {
            int c = children[idx];
            int bestDownExcluding = (downValues[idx] == max1) ? max2 : max1;
            // best path from u going to a leaf not through c:
            // either go above u (up[u]) or go into another child (1 + bestDownExcluding)
            int bestPathFromU = up[u]; // for root, up[0]=0, so this may be 0; that's okay because if no above path, we ignore
            if (bestDownExcluding != -1) {
                bestPathFromU = std::max(bestPathFromU, 1 + bestDownExcluding);
            }
            // up[c] = 1 + bestPathFromU (adding the edge from c to u and counting c itself)
            // But careful: bestPathFromU already includes node u. So the path from c to that leaf will be:
            // c -> u -> ... (leaf) => length = 1 (c) + length of path from u to that leaf.
            // The length from u to that leaf is bestPathFromU. So up[c] = 1 + bestPathFromU.
            up[c] = 1 + bestPathFromU;
            // Enqueue child for further pre-order
            queue.push_back(c);
        }
    }

    // Now compute final answer: ans[u] = max(down[u], up[u]) but up[u] might be 0 for root;
    // for root, down[root] is correct (longest path from root to leaf in subtree).
    // Actually up[u] is defined as path going through parent first; for root, no parent, so up[0]=0.
    // So ans[u] = max(down[u], up[u]) works because up[u] will be >=1 for non-root (since at least child->parent->... path exists).
    for (int u = 0; u < n; ++u) {
        ans[u] = std::max(down[u], up[u]);
    }
    return ans;
}

#include <cassert>
#include <vector>

// Function under test is assumed to be declared above

int main() {
    // Test 1: single node
    {
        std::vector<std::vector<int>> adj(1);
        auto res = allNodeLongestPaths(1, adj);
        assert(res.size() == 1);
        assert(res[0] == 1);
    }

    // Test 2: simple path 0-1-2
    {
        int n = 3;
        std::vector<std::vector<int>> adj(n);
        adj[0].push_back(1); adj[1].push_back(0);
        adj[1].push_back(2); adj[2].push_back(1);
        auto res = allNodeLongestPaths(n, adj);
        // Node 0: longest to leaf 2 is 0-1-2 = 3
        // Node 1: longest to leaf 0 or 2 is 2 (1-0 or 1-2)
        // Node 2: longest to leaf 0 is 3
        assert(res[0] == 3);
        assert(res[1] == 2);
        assert(res[2] == 3);
    }

    // Test 3: star with center 0 and leaves 1,2,3
    {
        int n = 4;
        std::vector<std::vector<int>> adj(n);
        adj[0] = {1,2,3};
        adj[1] = {0};
        adj[2] = {0};
        adj[3] = {0};
        auto res = allNodeLongestPaths(n, adj);
        // Center: longest to any leaf is 2 (center+leaf)
        // Leaves: longest to another leaf is 3 (leaf-center-leaf)
        assert(res[0] == 2);
        assert(res[1] == 3);
        assert(res[2] == 3);
        assert(res[3] == 3);
    }

    // Test 4: balanced binary tree with 7 nodes (0 root, children 1,2; 1 children 3,4; 2 children 5,6)
    {
        int n = 7;
        std::vector<std::vector<int>> adj(n);
        adj[0] = {1,2};
        adj[1] = {0,3,4};
        adj[2] = {0,5,6};
        adj[3] = {1};
        adj[4] = {1};
        adj[5] = {2};
        adj[6] = {2};
        auto res = allNodeLongestPaths(n, adj);
        // Leaves (3,4,5,6): longest path to another leaf is 3+? e.g., 3-1-0-2-5 = 5 nodes
        // Internal nodes (1,2): longest to leaf is 3 (1 to 3,4,5,6 via 0? Actually 1 to 5 goes 1-0-2-5 = 4 nodes)
        // Root (0): longest to leaf is 3 (0 to 3 or 4 via 1, or to 5,6 via 2)
        // Let's compute manually:
        // Node 0: down = 1+max(down[1]=3? Actually down[1]=2? Let's see:
        // down[3]=1, down[4]=1, so down[1]=2, down[2]=2, so down[0]=3.
        // up[0]=0, ans[0]=3.
        // Node 1: down=2, up[1]=? Since 1's parent is 0, best path from 0 not through 1 is down[2]+1=3? Actually down[2]=2, so best from 0 not through 1 = 1+2=3, then up[1]=1+3=4. ans[1]=4.
        // Node 2: similarly ans[2]=4.
        // Node 3: down=1, up[3]=? Parent 1 has children 3 and 4; best from 1 not through 3 is down[4]+1=2, or up[1]=4? Actually up[1]=4, so best from 1 not through 3 is max(4, 1+down[4]=2) =4, so up[3]=1+4=5. ans[3]=5.
        // Similarly ans[4]=5, ans[5]=5, ans[6]=5.
        assert(res[0] == 3);
        assert(res[1] == 4);
        assert(res[2] == 4);
        assert(res[3] == 5);
        assert(res[4] == 5);
        assert(res[5] == 5);
        assert(res[6] == 5);
    }

    // Test 5: path of 5 nodes (0-1-2-3-4)
    {
        int n = 5;
        std::vector<std::vector<int>> adj(n);
        for (int i = 0; i < n-1; ++i) {
            adj[i].push_back(i+1);
            adj[i+1].push_back(i);
        }
        auto res = allNodeLongestPaths(n, adj);
        // Node 0: longest to 4 is 5 nodes
        // Node 1: longest to 4 is 1-2-3-4 = 4 nodes
        // Node 2: longest to 0 or 4 is 3 nodes (2-1-0 or 2-3-4)
        // Node 3: longest to 0 is 4 nodes
        // Node 4: longest to 0 is 5 nodes
        assert(res[0] == 5);
        assert(res[1] == 4);
        assert(res[2] == 3);
        assert(res[3] == 4);
        assert(res[4] == 5);
    }

    // Test 6: large chain of 100 nodes, check endpoints
    {
        int n = 100;
        std::vector<std::vector<int>> adj(n);
        for (int i = 0; i < n-1; ++i) {
            adj[i].push_back(i+1);
            adj[i+1].push_back(i);
        }
        auto res = allNodeLongestPaths(n, adj);
        assert(res.front() == n);
        assert(res.back() == n);
        // Middle node should have length max(i+1, n-i) for i=49: max(50,51)=51? Actually node 49: path to 0 is 50 nodes, to 99 is 51 nodes, so max=51
        assert(res[49] == 51); // 49 is 0-indexed, distance to 0 is 50 nodes, to 99 is 51 nodes
    }

    // Test 7: tree with root having one child that is a leaf, and that leaf is also a leaf (just 2 nodes) - already covered by path test? Let's explicitly do 2 nodes.
    {
        int n = 2;
        std::vector<std::vector<int>> adj(n);
        adj[0].push_back(1);
        adj[1].push_back(0);
        auto res = allNodeLongestPaths(n, adj);
        assert(res[0] == 2);
        assert(res[1] == 2);
    }

    return 0;
}

// The problem requires finding, for each node, the maximum distance (in terms of node count) from that node down to any leaf. This is a classic tree DP problem. The key is to compute two pieces of information for each node:
// 1. **Downward maximum**: The longest path starting at the node and going strictly down into its subtree (i.e., not going back to the parent). This is computed by a post-order DFS: for node `u`, the downward maximum is `1 + max(downward_max[v])` over all children `v` of `u`. If there are no children, it is 1.
// 2. **Upward/alternative maximum**: For each node, the longest path that starts at the node and goes upward toward its parent (and possibly then down into another branch of the ancestor) must also be considered. This requires a second DFS (pre-order) that propagates "best path from outside the subtree" values from parent to children. For a child `v` of `u`, the best path starting at `v` and going up through `u` is either:
//    - The best path starting at `u` and going upward from `u` (i.e., the value computed for `u` when considering paths not going through `v`), or
//    - The best downward path from `u` into a child other than `v`, then plus 1 (for the edge from `v` to `u`) and plus 1 (for `u` itself), but care: the downward path from `u` already counts `u`, so we need to be precise.
//
// A clean approach is to do two DFS passes:
// - First DFS (post-order): compute `down[u]` = longest path from `u` to a leaf in its subtree (number of nodes). For a leaf, `down[u]=1`. For internal node, `down[u] = 1 + max(down[child])`.
// - Second DFS (pre-order): compute an `up` value for each node representing the longest path from that node to a leaf that goes through its parent first. More precisely, for a node `u`, we want the final answer `ans[u] = max(down[u], up[u])`, where `up[u]` is the longest path starting at `u` that goes to its parent (and then possibly down elsewhere). To compute `up[child]` for each child `child` of `u`, we need the best path starting at `u` that does not go into `child`. That best path is either `up[u]` (if `u` is not the root, i.e., a path that goes above `u`) or `1 + max(down[w])` for all children `w` of `u` with `w != child` (a path that goes down into a sibling subtree). Also, the path starting at `u` going into that sibling already counts `u`, so when transferring to `child`, we add 1 for the edge from `child` to `u`, so `up[child] = 1 + best_path_from_u_not_through_child`. This works because `best_path_from_u_not_through_child` is the length of the path from `u` to a leaf in the "other side", and adding `child` adds one node. This can be computed efficiently by, for each node, storing the top two downward values among children, so that we can quickly get the best downward value excluding a specific child.
//
// Edge cases: The tree may be a path (so leaves are only the two endpoints), or a star (center with many leaves). For a leaf, `down=1` and `up` may be large if the leaf is attached to a long chain upward. The algorithm handles all cases.
//
// Time complexity: Two DFS traversals visit each node and edge constant times, so O(n). Space complexity: O(n) for adjacency list and DP arrays.
