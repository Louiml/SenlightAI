Write a C++ function `maxTargetNodes` that takes two vectors of undirected edges, `edges1` and `edges2`, where each edge is a pair of 0-indexed node IDs, and returns a vector `res` such that for each node `i` in the first tree (with `n = edges1.size() + 1` nodes), `res[i]` equals the maximum possible number of target nodes that can be selected if you are allowed to choose one node from the first tree and one node from the second tree (with `m = edges2.size() + 1` nodes), under the rule that two nodes can both be selected only if the distance between them in their respective tree is odd (since the depth parity determines color compatibility: a node at even depth can pair with nodes at odd depth in another tree). More precisely, color each tree’s nodes by depth parity (0 for even depth from an arbitrarily chosen root at node 0, 1 for odd). For a given node `i` in tree 1 with color `c`, it can pair with all nodes in tree 2 whose color is not `c` (i.e., opposite parity). Thus `res[i] = count1[c] + max(count2[0], count2[1])`, where `count1` and `count2` are the counts of nodes of each color in tree 1 and tree 2 respectively. The number of nodes in tree 2 that can be paired with node `i` is the larger of the two color counts in tree 2, because you can choose the root of tree 2 arbitrarily (the tree is undirected, so you can root it at any node to flip its color assignments). Implement this function with the exact same semantics as the provided code, but make it a free function (not a class method) named `maxTargetNodes`, taking `std::vector<std::vector<int>>` for edges, and returning `std::vector<int>`. The input edges are guaranteed to form a tree (no cycles, connected). Handle the trivial case where a tree has 1 node. The function must be efficient for up to `10^5` nodes.

The key insight is that in any tree, if we root it at an arbitrary node (say node 0), the distance between any two nodes is determined by the parity of their depths relative to that root. Specifically, the distance between two nodes is odd if and only if their depth parities differ. Therefore, if we color each node by depth parity, then two nodes in the same tree are "compatible" (i.e., can be selected together) if and only if their colors differ. When combining two trees, we can select one node from tree 1 and one from tree 2, and they are compatible if their colors differ. For a fixed node `i` in tree 1 with color `c`, the number of nodes in tree 2 it can pair with is all nodes whose color is `1 - c`. However, since we can choose to root tree 2 at any node (the tree is undirected and unrooted; we just need a depth function to define colors), we can flip the colors of tree 2 by choosing a different root (e.g., root at a neighbor of the original root flips all parities). Thus the maximum number of nodes in tree 2 that can pair with any node is `max(count2[0], count2[1])`. For tree 1, the number of nodes in tree 1 that can pair with `i` is `count1[color1[i]]` (since only same-color nodes in tree 1 are compatible with each other? Wait: Actually, the rule is: two nodes can both be selected if the distance between them is odd. In tree 1, if we select node `i`, we can also select any other node `j` from tree 1 such that distance(i,j) is odd. Since distance parity is determined by depth parity difference, distance(i,j) odd iff color1[i] != color1[j]. Therefore, the number of nodes in tree 1 that can be paired with `i` is the count of nodes in tree 1 with the opposite color, i.e., `count1[1 - color1[i]]`, not `count1[color1[i]]`. But wait, the provided code does `res[i] = count1[color1[i]] + max(count2[0], count2[1])`. That seems incorrect? Let's re-examine: The problem statement in the snippet's original context likely says: For each node in tree 1, you can select that node plus a set of other nodes such that any two selected nodes in the combined graph (tree1 and tree2 are separate? Actually, the snippet seems to be from a problem about "maxTargetNodes" where you pick a starting node, and then you can also pick all nodes at odd distance from it in both trees? Let's infer: In the code, `dfs` returns `1 - depth % 2` initially, then adds results from children. That means `dfs` computes the number of nodes in the subtree (including the node itself) that are at even depth? Wait, `1 - depth % 2` is 1 if depth is even, 0 if odd. Then it sums over children. So `dfs` returns the number of nodes in the subtree rooted at `node` that have even depth relative to root 0. For the root at depth 0, it returns the total number of nodes at even depth in the entire tree (since it visits all). But in `build`, it computes `res = dfs(0,...)`, and returns `{res, n - res}`, i.e., counts of even and odd depth nodes. So `count1[0]` is number of nodes at even depth, `count1[1]` is number at odd depth. Now in `maxTargetNodes`, for node `i`, it does `res[i] = count1[color1[i]] + max(count2[0], count2[1])`. Here `color1[i]` is 0 if even depth, 1 if odd. So for a node `i` at even depth, it adds `count1[0]` (all even-depth nodes) plus max of tree2 colors. That means: The problem likely says: You are allowed to pick a set of nodes such that any two picked nodes have odd distance? No, if you pick a starting node, then you can also pick all nodes at odd distance from it? Let's not guess. The given code is the reference solution; we need to replicate its behavior. So we will use exactly that logic: For node `i`, the number of target nodes is `count1[color1[i]]` (i.e., number of nodes in tree1 with the same parity as `i`) plus the larger of the two color counts in tree2. Why same parity? Because if you pick node `i`, you can also pick any other node in tree1 that has the same parity? That would mean distance between them is even, so they are NOT at odd distance. So the problem must be about picking nodes such that the distance between any two selected nodes is even? Let's look at the code's `dfs`: it returns `1 - depth % 2` and sums. That counts even-depth nodes, not odd. So the code is counting nodes with even depth from root. So for a node `i` at even depth, `count1[0]` is the number of even-depth nodes, which includes `i` itself. So the code assumes you can pick all nodes with the same parity as `i` in tree1, and all nodes in tree2 that are in the larger parity group. That implies the rule is: distance between selected nodes must be even? Let's not overthink; the task is to replicate the given behavior. So in the analysis, we'll explain that the algorithm computes for each tree the count of nodes at even and odd depth from an arbitrary root (node 0), then for each node `i` in tree1, the answer is the count of nodes in tree1 with the same depth parity as `i` (which is `count1[color1[i]]`) plus the maximum count of either parity in tree2 (because you can choose the root of tree2 arbitrarily to flip parities, thus you can pair with the larger group). This matches the code. The time complexity is O(n + m) for the two DFS traversals plus O(n) to fill the result, so O(n + m). Space is O(n + m) for adjacency lists, plus O(n + m) for recursion stack (worst-case tree depth could be O(n) if not balanced), but we can use iterative DFS if needed; the given code uses recursion, so we'll mention that. Edge case: single-node trees (n=1 or m=1). For n=1, the DFS works, count1 = {1,0} if root depth 0. For m=1, count2 = {1,0}. The max is 1. The result for node 0 is `count1[0] + 1` which is 2. That means you can pick node 0 from tree1 and the single node from tree2, total 2.

#include <vector>
#include <algorithm>
#include <functional>

// Computes for each node in the first tree the maximum number of target nodes
// that can be selected together with it, using the parity-based matching rule.
std::vector<int> maxTargetNodes(const std::vector<std::vector<int>>& edges1,
                                const std::vector<std::vector<int>>& edges2) {
    auto build = [](const std::vector<std::vector<int>>& edges,
                    std::vector<int>& color) -> std::vector<int> {
        int n = static_cast<int>(edges.size()) + 1;
        std::vector<std::vector<int>> adj(n);
        for (const auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        std::function<int(int, int, int)> dfs =
            [&](int node, int parent, int depth) -> int {
                color[node] = depth % 2;
                int count = 1 - (depth % 2); // counts nodes at even depth from root
                for (int child : adj[node]) {
                    if (child == parent) continue;
                    count += dfs(child, node, depth + 1);
                }
                return count;
            };

        color.assign(n, 0);
        int evenCount = dfs(0, -1, 0);
        return {evenCount, n - evenCount};
    };

    int n = static_cast<int>(edges1.size()) + 1;
    int m = static_cast<int>(edges2.size()) + 1;

    std::vector<int> color1(n);
    std::vector<int> color2(m);
    std::vector<int> count1 = build(edges1, color1);
    std::vector<int> count2 = build(edges2, color2);

    std::vector<int> result(n);
    int best2 = std::max(count2[0], count2[1]);
    for (int i = 0; i < n; ++i) {
        result[i] = count1[color1[i]] + best2;
    }
    return result;
}

#include <cassert>
#include <vector>

// function declaration (already defined above)
std::vector<int> maxTargetNodes(const std::vector<std::vector<int>>& edges1,
                                const std::vector<std::vector<int>>& edges2);

int main() {
    // Example 1: Both trees are single nodes
    {
        std::vector<std::vector<int>> e1, e2;
        std::vector<int> res = maxTargetNodes(e1, e2);
        assert(res.size() == 1);
        assert(res[0] == 2); // 1 node from tree1 + 1 node from tree2
    }

    // Example 2: tree1 is a path of 2 nodes (0-1), tree2 is a single node
    {
        std::vector<std::vector<int>> e1 = {{0,1}};
        std::vector<std::vector<int>> e2 = {};
        std::vector<int> res = maxTargetNodes(e1, e2);
        assert(res.size() == 2);
        // For node 0 (depth 0, color 0): count1[0]=1 (node0 only), best2=1 -> 2
        // For node 1 (depth 1, color 1): count1[1]=1 (node1 only), best2=1 -> 2
        assert(res[0] == 2 && res[1] == 2);
    }

    // Example 3: tree1 is a star with center 0 and leaves 1,2 (edges: 0-1, 0-2)
    // tree2 is a path of 3 nodes (0-1, 1-2)
    {
        std::vector<std::vector<int>> e1 = {{0,1},{0,2}}; // n=3
        std::vector<std::vector<int>> e2 = {{0,1},{1,2}}; // m=3
        std::vector<int> res = maxTargetNodes(e1, e2);
        assert(res.size() == 3);
        // For tree1: root 0 depth0 -> color0; nodes 1,2 depth1 -> color1.
        // count1[0]=1, count1[1]=2.
        // For tree2: root 0 depth0->color0; node1 depth1->color1; node2 depth2->color0.
        // count2[0]=2 (nodes 0,2), count2[1]=1 (node1). best2 = max(2,1)=2.
        // For node0 (color0): res = 1 + 2 = 3.
        // For node1/2 (color1): res = 2 + 2 = 4.
        assert(res[0] == 3);
        assert(res[1] == 4 && res[2] == 4);
    }

    // Example 4: Both trees are paths of 4 nodes
    {
        std::vector<std::vector<int>> e1 = {{0,1},{1,2},{2,3}}; // n=4
        std::vector<std::vector<int>> e2 = {{0,1},{1,2},{2,3}}; // m=4
        // tree1: depths: 0->0,1->1,2->0,3->1 => count1[0]=2, count1[1]=2
        // tree2: same => count2[0]=2, count2[1]=2 => best2=2
        // For node0 (color0): res = 2+2=4
        // For node1 (color1): res = 2+2=4
        std::vector<int> res = maxTargetNodes(e1, e2);
        assert(res.size() == 4);
        for (int v : res) assert(v == 4);
    }

    // Example 5: tree1 has 1 node, tree2 has 5 nodes forming a star (center 0)
    {
        std::vector<std::vector<int>> e1;
        std::vector<std::vector<int>> e2 = {{0,1},{0,2},{0,3},{0,4}}; // m=5
        // tree2: root0 depth0 color0; leaves depth1 color1. count2[0]=1, count2[1]=4 => best2=4
        // tree1: count1[0]=1
        std::vector<int> res = maxTargetNodes(e1, e2);
        assert(res.size() == 1);
        assert(res[0] == 1 + 4); // 5
    }

    return 0;
}
