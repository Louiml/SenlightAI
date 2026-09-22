// Write a C++ function `vector<int> nodesAtDistanceK(TreeNode* root, TreeNode* target, int k)` that, given a binary tree (where each `TreeNode` has an integer `val` and left/right child pointers), returns a vector containing the values of all nodes whose distance from the given `target` node is exactly `k`. The distance between two nodes is defined as the number of edges along the shortest path connecting them (i.e., moving up to a common ancestor and then down). The tree may have up to 10^5 nodes, and node values are not necessarily unique. Return the result in any order. If no nodes are at distance `k`, return an empty vector. The function must handle `k = 0` (returning the target’s own value) and large trees efficiently. Assume `root` and `target` are non-null and `target` is guaranteed to be in the tree.
// The key challenge is that nodes at distance `k` from the target can be in any direction: in the target’s own subtree (downward), or in other parts of the tree via ancestors (upward then down). A simple DFS from the target only reaches descendants, so we need a way to traverse upward too. The solution uses a two-phase approach:
//
// 1. **Distance mapping**: Perform a DFS from the root to locate the target. While unwinding the recursion, store in a hash map `distFromRoot` the distance from the root to the target for every node on the path from root to target. Specifically, when the target is found, its distance is 0; for any ancestor, the distance is the child’s distance + 1. This gives us, for each ancestor, the exact distance from that ancestor to the target.
//
// 2. **Modified DFS from root**: Traverse the entire tree from the root using DFS. At each node, we need to know the distance from that node to the target. If the node is in the `distFromRoot` map (i.e., on the path from root to target), its distance is directly given by the map. Otherwise, if the node is not on that path, its distance is the distance of its parent plus 1 (since moving down increases distance). To implement this, pass a running `currentDistance` parameter that represents the distance from the parent to the target, then increment by 1 when going to a child. However, if the child is on the path, we override `currentDistance` with the stored value from the map (because that distance is shorter). This way, each node gets its correct distance to the target exactly once.
//
// 3. **Collect results**: Whenever a node’s distance equals `k`, append its value to the result vector.
//
// Edge cases: `k = 0` returns only the target’s value. If the target is the root, the map has only one entry, and the DFS works naturally. If `k` exceeds the tree height, the result is empty. The map ensures O(1) lookup per node.
//
// Time complexity: O(n) for both phases (each node visited once), where n is the number of nodes. Space complexity: O(n) for the hash map and the recursion stack in the worst case (skewed tree).
#include <vector>
#include <unordered_map>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper: Find target and store distances from target to all nodes on the path from root to target.
void findDistances(TreeNode* node, TreeNode* target, std::unordered_map<TreeNode*, int>& distances) {
    if (node == nullptr) {
        return;
    }
    if (node == target) {
        distances[node] = 0;
        return;
    }
    findDistances(node->left, target, distances);
    if (node->left != nullptr && distances.find(node->left) != distances.end()) {
        distances[node] = distances[node->left] + 1;
        return;
    }
    findDistances(node->right, target, distances);
    if (node->right != nullptr && distances.find(node->right) != distances.end()) {
        distances[node] = distances[node->right] + 1;
    }
}

// Helper: DFS the whole tree, computing distance to target for each node.
void collectNodes(TreeNode* node, int k, int currentDistance, const std::unordered_map<TreeNode*, int>& distances, std::vector<int>& result) {
    if (node == nullptr) {
        return;
    }
    // If this node is on the path from root to target, its distance is exact from map.
    auto it = distances.find(node);
    if (it != distances.end()) {
        currentDistance = it->second;
    }
    if (currentDistance == k) {
        result.push_back(node->val);
    }
    collectNodes(node->left, k, currentDistance + 1, distances, result);
    collectNodes(node->right, k, currentDistance + 1, distances, result);
}

// Main solution: Return all node values at distance k from target.
std::vector<int> nodesAtDistanceK(TreeNode* root, TreeNode* target, int k) {
    std::unordered_map<TreeNode*, int> distances;
    findDistances(root, target, distances);
    std::vector<int> result;
    collectNodes(root, k, 0, distances, result);
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple tree:
    //        3
    //       / \
    //      5   1
    //     / \   \
    //    6   2   8
    //       / \
    //      7   4
    TreeNode n3(3), n5(5), n1(1), n6(6), n2(2), n8(8), n7(7), n4(4);
    n3.left = &n5; n3.right = &n1;
    n5.left = &n6; n5.right = &n2;
    n1.right = &n8;
    n2.left = &n7; n2.right = &n4;

    // Distance 0 from target 5 -> {5}
    assert(nodesAtDistanceK(&n3, &n5, 0) == std::vector<int>{5});
    // Distance 1 from target 5 -> {3, 6, 2}
    auto res1 = nodesAtDistanceK(&n3, &n5, 1);
    std::sort(res1.begin(), res1.end());
    assert((res1 == std::vector<int>{2, 3, 6}));
    // Distance 2 from target 5 -> {1, 7, 4}
    auto res2 = nodesAtDistanceK(&n3, &n5, 2);
    std::sort(res2.begin(), res2.end());
    assert((res2 == std::vector<int>{1, 4, 7}));
    // Distance 3 from target 5 -> {8}
    auto res3 = nodesAtDistanceK(&n3, &n5, 3);
    std::sort(res3.begin(), res3.end());
    assert((res3 == std::vector<int>{8}));
    // Distance 4 from target 5 -> empty
    assert(nodesAtDistanceK(&n3, &n5, 4).empty());

    // Test 2: Target is root
    //      1
    //     / \
    //    2   3
    TreeNode r1(1), r2(2), r3(3);
    r1.left = &r2; r1.right = &r3;
    assert(nodesAtDistanceK(&r1, &r1, 0) == std::vector<int>{1});
    auto resRoot = nodesAtDistanceK(&r1, &r1, 1);
    std::sort(resRoot.begin(), resRoot.end());
    assert((resRoot == std::vector<int>{2, 3}));
    assert(nodesAtDistanceK(&r1, &r1, 2).empty());

    // Test 3: Single node
    TreeNode single(42);
    assert(nodesAtDistanceK(&single, &single, 0) == std::vector<int>{42});
    assert(nodesAtDistanceK(&single, &single, 1).empty());

    // Test 4: Skewed tree (target at leaf)
    // 1 -> 2 -> 3 -> 4 (target = 4)
    TreeNode s1(1), s2(2), s3(3), s4(4);
    s1.right = &s2; s2.right = &s3; s3.right = &s4;
    auto resSkew = nodesAtDistanceK(&s1, &s4, 2);
    std::sort(resSkew.begin(), resSkew.end());
    assert((resSkew == std::vector<int>{2}));
    assert(nodesAtDistanceK(&s1, &s4, 3) == std::vector<int>{1});

    // Test 5: Duplicate values (tree with nodes having same val)
    //       5
    //      / \
    //     5   5
    TreeNode d1(5), d2(5), d3(5);
    d1.left = &d2; d1.right = &d3;
    auto resDup = nodesAtDistanceK(&d1, &d2, 1);
    std::sort(resDup.begin(), resDup.end());
    assert((resDup == std::vector<int>{5, 5}));

    return 0;
}
