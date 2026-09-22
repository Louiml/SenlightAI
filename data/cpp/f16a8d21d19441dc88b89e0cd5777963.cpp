/*
Write a C++ function `vector<int> longestSpecialPath(vector<vector<int>>& edges, vector<int>& nums)` that takes an undirected tree with `n` nodes (nodes labeled `0` to `n-1`), where each edge is given as `{u, v, weight}` with positive integer weight, and each node has a color value `nums[i]`. A "special path" is a simple path where no color value appears more than once along the path. The function must return a vector of two integers: the maximum total edge weight among all special paths, and the minimum number of nodes among all special paths that achieve that maximum weight. If there are multiple paths with the same maximum weight, choose the one with the fewest nodes. If no edges exist (single node), return `{0, 1}`. The tree is connected, `1 <= n <= 1000`, edge weights up to 10^6, and color values up to 10^5.
*/
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> longestSpecialPath(vector<vector<int>>& edges, vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return {0, 1};
        
        // Build adjacency list: each entry is (neighbor, weight)
        vector<vector<pair<int, int>>> adj(n);
        for (const auto& e : edges) {
            int u = e[0], v = e[1], w = e[2];
            adj[u].push_back({v, w});
            adj[v].push_back({u, w});
        }
        
        long long bestWeight = 0;
        int bestNodes = 1; // single-node path
        
        // DFS from every possible start node
        for (int start = 0; start < n; ++start) {
            unordered_set<int> colors; // colors on current path
            colors.insert(nums[start]);
            dfs(start, -1, 0LL, 1, adj, nums, colors, bestWeight, bestNodes);
        }
        
        return {static_cast<int>(bestWeight), bestNodes};
    }
    
private:
    void dfs(int u, int parent, long long curWeight, int curNodes,
             const vector<vector<pair<int, int>>>& adj,
             const vector<int>& nums,
             unordered_set<int>& colors,
             long long& bestWeight, int& bestNodes) {
        // Update answer with the path that ends at u
        if (curWeight > bestWeight || (curWeight == bestWeight && curNodes < bestNodes)) {
            bestWeight = curWeight;
            bestNodes = curNodes;
        }
        
        for (const auto& [v, w] : adj[u]) {
            if (v == parent) continue;
            // If the child's color already appears on the current path, cannot extend
            if (colors.count(nums[v])) continue;
            // Extend path
            colors.insert(nums[v]);
            dfs(v, u, curWeight + w, curNodes + 1, adj, nums, colors, bestWeight, bestNodes);
            colors.erase(nums[v]);
        }
    }
};
#include <cassert>
#include <vector>
using namespace std;

// Assume Solution class from above is included.

int main() {
    Solution sol;
    
    // Test 1: Example from problem statement
    vector<vector<int>> edges1 = {{0,1,2},{1,2,3},{1,3,5},{1,4,4},{2,5,6}};
    vector<int> nums1 = {2,1,2,1,3,1};
    vector<int> res1 = sol.longestSpecialPath(edges1, nums1);
    // The maximum special path: 3-1 (5) + 1-0 (2) = 7? Or 2-5 (6) + 2-1 (3) = 9? Let's compute manually.
    // Best path: 5->2 (6) + 2->1 (3) + 1->3 (5) = 14? But color at 2 is 2, at 5 is 1, at 1 is 1? duplicate color 1 between 5 and 1? Actually nums: 0:2, 1:1, 2:2, 3:1, 4:3, 5:1. Path 5-2-1-3: colors 1,2,1,1? has duplicates (1 appears at 5,1,3). Not valid.
    // Valid path: 0-1-4: colors 2,1,3 all distinct, weight 2+4=6, nodes 3.
    // Path 5-2-1-4: colors 1,2,1,3? duplicate 1 (5 and 1) not allowed.
    // Path 2-5: colors 2,1 weight 6, nodes 2.
    // Path 1-3: weight 5, nodes 2.
    // Path 5-2-1: colors 1,2,1? duplicate 1, no.
    // Path 0-1-3: colors 2,1,1? duplicate 1, no.
    // Path 4-1-0: colors 3,1,2 all distinct, weight 4+2=6, nodes 3.
    // Path 4-1-3: colors 3,1,1 duplicate 1, no.
    // Path 5-2-1-4? not allowed.
    // So max weight is 6? But there is also path 0-1 (2), 1-2 (3), 2-5 (6) - path 0-1-2-5: colors 2 (0), 1 (1), 2 (2) duplicate 2? Actually nums[2]=2, duplicate with 0, invalid. So max weight 6. But what about path 3-1-4: 5+4=9, colors 1,1,3? duplicate 1 (node1 and node3) invalid.
    // Wait, there is path 0-1-2: colors 2,1,2 duplicate 2, invalid.
    // Path 3-1-0: 5+2=7 colors 1,1,2 duplicate 1, invalid.
    // Path 3-1-2-5: 5+3+6=14 colors 1,1,2,1 duplicates many.
    // So max weight is 6, with node count 3 (0-1-4 or 4-1-0). So expected {6,3}.
    assert(res1 == vector<int>({6, 3}));
    
    // Test 2: Simple 3-node line with distinct colors
    // edges: 0-1 weight 1, 1-2 weight 2; nums: 4,3,5
    vector<vector<int>> edges2 = {{1,0,1},{1,2,6}};
    vector<int> nums2 = {4,3,5};
    vector<int> res2 = sol.longestSpecialPath(edges2, nums2);
    // All paths distinct colors, max path from 0 to 2: weight 1+6=7, nodes 3. Also 0-1 (1) etc. So {7,3}
    assert(res2 == vector<int>({7, 3}));
    
    // Test 3: Single node
    vector<vector<int>> edges3 = {};
    vector<int> nums3 = {7};
    vector<int> res3 = sol.longestSpecialPath(edges3, nums3);
    assert(res3 == vector<int>({0, 1}));
    
    // Test 4: Two nodes with same color? Not allowed since colors distinct? Actually edges: 0-1 weight 5, nums both 1 -> path 0-1 is invalid because duplicate color. So only single nodes, max weight 0, nodes 1.
    vector<vector<int>> edges4 = {{0,1,5}};
    vector<int> nums4 = {1,1};
    vector<int> res4 = sol.longestSpecialPath(edges4, nums4);
    assert(res4 == vector<int>({0, 1}));
    
    // Test 5: Two nodes with different colors
    vector<vector<int>> edges5 = {{0,1,5}};
    vector<int> nums5 = {1,2};
    vector<int> res5 = sol.longestSpecialPath(edges5, nums5);
    assert(res5 == vector<int>({5, 2}));
    
    // Test 6: Star with 4 nodes, center 0, leaves 1,2,3 with weights 1,2,3, colors: center 1, leaves all 2 -> no path with center and leaf because duplicate color, so max path is leaf-leaf? But leaves are not connected directly; path leaf1 - center - leaf2 has colors 2,1,2 duplicate 2, invalid. So only single leaves, weight 0, nodes 1. But there are edges between leaves? No. So {0,1}
    vector<vector<int>> edges6 = {{0,1,1},{0,2,2},{0,3,3}};
    vector<int> nums6 = {1,2,2,2};
    vector<int> res6 = sol.longestSpecialPath(edges6, nums6);
    assert(res6 == vector<int>({0, 1}));
    
    // Test 7: Star with distinct colors, max path between two leaves: weight sum of two largest edges.
    vector<vector<int>> edges7 = {{0,1,1},{0,2,2},{0,3,3}};
    vector<int> nums7 = {1,2,3,4};
    vector<int> res7 = sol.longestSpecialPath(edges7, nums7);
    // Path 1-0-3: weight 1+3=4, nodes 3. Path 2-0-3: 2+3=5, nodes 3. So {5,3}
    assert(res7 == vector<int>({5, 3}));
    
    // Test 8: Ties in weight, different node counts
    // Tree: 0-1 weight 2, 1-2 weight 2, also 1-3 weight 4? Need two paths of same weight but different lengths.
    // Let's do: edges: 0-1 weight 3, 1-2 weight 3 (path 0-1-2 weight 6), and also 1-3 weight 6 (path 0-1-3 weight 9? no). To get tie, make two paths of same weight: e.g., 0-1 weight 5 (nodes 2), and 0-2 weight 5 (nodes 2) but that requires node0 connected to both, but then path 1-0-2 has weight 10? Not tie.
    // Simpler: line 0-1-2-3, weights 1,1,1 → path 0 to 3 weight 3, nodes 4, also path 1 to 3 weight 2, etc. No tie.
    // Create: 0-1 weight 2 (path 0-1 weight2 nodes2), and 1-2 weight 2 (path 0-1-2 weight4), but also another edge 0-3 weight4 (path 0-3 weight4 nodes2). So two paths of weight 4: nodes 3 (0-1-2) and nodes 2 (0-3). We expect max weight 4, min nodes 2.
    vector<vector<int>> edges8 = {{0,1,2},{1,2,2},{0,3,4}};
    vector<int> nums8 = {1,2,3,4}; // all distinct
    vector<int> res8 = sol.longestSpecialPath(edges8, nums8);
    // Paths: 0-3 weight4 nodes2; 0-1-2 weight4 nodes3; also 3-0-1 weight6 nodes3? Weight 4+2=6, nodes3, so max is 6 actually! Because 3-0-1 is weight 4+2=6, nodes3. Also 3-0-1-2 weight 4+2+2=8 nodes4. So max weight 8. So not a tie. Need careful design. But for test we can just check that the function returns correct for known example 1.
    
    // Test 9: Chain with all distinct colors
    vector<vector<int>> edges9 = {{0,1,1},{1,2,2},{2,3,3}};
    vector<int> nums9 = {1,2,3,4};
    vector<int> res9 = sol.longestSpecialPath(edges9, nums9);
    // Path 0-3 weight 1+2+3=6 nodes4. So {6,4}
    assert(res9 == vector<int>({6, 4}));
    
    // Test 10: Chain with duplicate colors but max path skips
    vector<vector<int>> edges10 = {{0,1,5},{1,2,5}};
    vector<int> nums10 = {1,2,1}; // colors 0 and2 same
    vector<int> res10 = sol.longestSpecialPath(edges10, nums10);
    // Path 0-1 weight 5 nodes2, 1-2 weight5 nodes2, but 0-1-2 has duplicate 1, so max weight 5, min nodes2 (two equal paths). So {5,2}
    assert(res10 == vector<int>({5, 2}));
    
    return 0;
}
// The problem is a classic tree DFS with backtracking to enforce unique color constraint. The key idea is to perform a depth-first search from an arbitrary root (node 0), maintaining a hash map `last_occ` that maps each color value to the most recent node index on the current DFS path that has that color. When we traverse an edge from parent `u` to child `v` with weight `w`, we must ensure the color `nums[v]` hasn't appeared earlier on the current path (i.e., not in `last_occ`). If it hasn't, we recurse deeper. If it has, we cannot go deeper through that child because it would violate the "no duplicate color" rule; instead, we consider the path that ends at the parent `u` and starts at the child `v`? Wait, careful: The special path must be a simple path (no repeated nodes), and no two nodes on the path can have the same color. Since we are doing DFS and backtracking, we can consider all paths that start at some node and go down the tree. When we are at node `u`, we consider all possible downward paths that go through one or more children. However, to cover all paths, we need to consider paths that go from one subtree through `u` to another subtree (i.e., paths that have `u` as their highest point in the DFS tree). But the original code snippet treats it as purely downward paths starting from the root? Actually, the code's approach is a bit flawed: it only considers paths that go from a node down to its descendants and also a branch where it encounters a color that has been visited before, but it doesn't properly handle the general case of a path that goes up and down the tree. However, since the tree is undirected and we root it at node 0, any simple path has a unique highest node relative to that root (the LCA). A special path can be decomposed into two downward paths from that LCA to two nodes in different subtrees (or one downward path if it ends at the LCA). The constraint is that no color repeats along the entire path, which means: (1) the colors along each downward branch must be distinct, and (2) no color from one branch appears in the other branch. The original code does not handle this correctly because it only tracks a single path from root down and doesn't combine two branches. However, the given code is incomplete and buggy. For a proper solution, we need a more careful algorithm.
//
// A correct approach: Root the tree at 0. For each node, we compute for each child subtree the best downward special path that starts at that child and goes down, and also the path that ends exactly at the child. But to combine two branches at a node, we need to ensure the colors in the two combined paths are disjoint. This is combinatorially complex. Another simpler approach for n <= 1000: Since the tree has at most 1000 nodes, we can brute-force all pairs of nodes as endpoints and check if the unique path between them has all distinct colors, using BFS/DFS per pair. That would be O(n^2 * n) = O(n^3) which for 1000 is too high (1e9). But we can do O(n^2) by doing DFS from each node as start, expanding outward until a duplicate color appears, and tracking max weight and min node count. Since the graph is a tree, from each starting node we can do a DFS with a set to track colors on the current path, and compute the maximum weight path starting at that node (i.e., any path that has that node as one endpoint). This covers all paths because every path has two endpoints; by iterating each node as one endpoint, we consider all paths. The complexity: For each start node, do a DFS over the entire tree (which is O(n) per start) because we explore all nodes reachable without color duplicates, but we can prune. However, in a tree of size 1000, doing O(n^2) = 1e6 operations is totally feasible. For each start node, we perform a DFS that goes outward from that node, maintaining a set of colors on the current path. When we move to a neighbor, if the neighbor's color is already in the set, we cannot include that neighbor in the path, so we skip it (we don't continue deeper through that neighbor). Otherwise, we include it, update the total weight and node count, and recurse. We also update the global answer whenever we reach a node (each node reached represents a valid path from start to that node). Since the graph is a tree, there is only one path from start to any node, and the DFS naturally explores all nodes reachable without color repeats. However, we must be careful: When we skip a neighbor due to duplicate color, we cannot go deeper through that neighbor, but we should still explore other neighbors. This is essentially a DFS with a color set, and we explore all nodes that can be reached from start via a path with distinct colors. The total number of nodes explored per start is at most n, but we might explore some nodes multiple times? Actually, because it's a tree, once we skip an edge due to a color conflict, we won't traverse that subtree, but other subtrees are explored. So per start, we do O(n) work because each edge is considered at most once (since the graph is a tree). So total O(n^2) time. For n=1000, that's 1e6, fine. We need to track the maximum weight and the minimum node count among paths with that weight. The answer is the maximum weight over all paths (with distinct colors) and the minimum node count among those max-weight paths. We start from each node, and for each node we update the answer with the path that has that node as one endpoint. This is correct because any simple path has two endpoints, and we will consider it when we start from one endpoint (the other endpoint will be reached during DFS). However, we must ensure we consider both directions? Starting from one endpoint, the DFS reaches the other endpoint exactly once, and the weight and node count are computed correctly. So iterating all start nodes covers all paths. Edge case: single node, no edges: the answer is {0, 1}. For n>1, the minimum node count for a path of length 0 (single node) is 1, but we want the max weight, which might be >0, so we prefer paths with edges. We initialize ans = {0, 1} (empty path at node 0). For each start node, we do DFS and update ans for every node visited (including the start, weight 0, node count 1). Complexity: O(n^2) time, O(n) auxiliary space for the DFS stack and color set per start.
//
// Let's implement a helper DFS that takes the current node, parent, current weight, node count, and a set of colors on the current path. At each step, we update global ans. For neighbors, if neighbor != parent and nums[neighbor] not in set, we recurse. This is straightforward.
//
// But we must ensure we don't double count the same path? But it's fine because we only take max weight and min node count. For a given start node, we can reach multiple nodes; each path is unique because tree has unique simple path. So we update ans for each reachable node. When we start from a node that is not the endpoint of a max path, we might still reach the other endpoint, but the path from start to that endpoint might be shorter (or longer) than the true max path? Actually, the path from start (which is one endpoint) to any other node is a valid path, and the max weight path with start as one endpoint will be considered when we start at that endpoint. Since we iterate all possible starts, we will consider every possible path at least once (when start is one of its endpoints). So we don't need to worry about paths that have start as an internal node; they will be covered when start is one of its endpoints. Thus this method is correct.
//
// Potential pitfall: For a given start node, the DFS explores all nodes reachable via paths with distinct colors, but it might revisit nodes? Because it's a tree, there are no cycles, so DFS without a visited set (except parent) works fine; we just don't go back to parent. But we must avoid revisiting nodes that have already been visited via a different path? Since it's a tree, there is only one simple path from start to any node, so we won't revisit nodes; the graph is acyclic. So we can simply use a parent parameter to avoid going back. However, we also need to skip going into a neighbor if its color is already in the set. If we skip that neighbor, we don't recurse into it, but that neighbor's subtree might contain nodes that are reachable from start via a different path? In a tree, the only way to reach a node in that skipped subtree is through that neighbor, and since we skip that neighbor, we cannot reach any node in that subtree from start without encountering the duplicate color. So it's correct to prune that entire subtree. Also, we need to remove the color from the set after exploring a neighbor (backtracking). So per start, we do a DFS with backtracking.
//
// Time complexity: For each start node, we may explore all nodes in the tree in the worst case (if all colors are distinct and the tree is a line), so O(n) per start, total O(n^2) = 1e6 operations for n=1000, fine. Space: O(n) for the recursion stack and set.
//
// We need to handle the case where the tree has multiple edges? No, it's a tree, so n-1 edges. We are given edges as vector of {u, v, w}. We'll build adjacency list as vector of pairs (neighbor, weight).
//
// We output a vector<int> ans = {maxWeight, minNodesAmongMaxWeightPaths}. Initialize maxWeight = 0, minNodes = 1 (since a single node path has weight 0 and 1 node). For each start, we call DFS. When we reach a node with current weight > maxWeight, we update both. If current weight == maxWeight, we take minNodes = min(minNodes, currentNodes).
//
// Make sure to use long long for weights to avoid overflow? Max n=1000, each edge weight up to 1e6, path can have up to n-1 edges, so max weight ~1e9, fits in int but better use long long. But output expects int? The problem statement likely uses int, but to be safe we'll use long long internally and cast to int when returning. However, the function signature returns vector<int>, so we'll store maxWeight as long long and convert. Actually, since n<=1000 and weight up to 1e6, max total 1e9 < 2^31-1, so int is fine. But we'll use long long for clarity.
//
// Now, produce the solution.
