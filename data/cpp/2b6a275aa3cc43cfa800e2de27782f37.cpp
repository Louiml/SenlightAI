Given a binary tree where each node stores an integer value and has pointers to its left and right children (possibly `nullptr`), write a C++ function named `nodesAtDistanceK` that takes three parameters: a pointer to the root of the tree, a pointer to a specific target node within that tree, and a non-negative integer K. The function must return a `std::vector<int>` containing the values of all nodes that are exactly K edges away from the target node. The order of the returned values does not matter. The tree has at least one node, the target is guaranteed to exist in the tree, and K can be zero (in which case the result should contain only the target's value). You may assume the tree has no cycles and each node has a unique integer value for simplicity, but you must not rely on that for correctness — the algorithm should work with duplicate values as well.

// The solution uses a two-phase Breadth-First Search (BFS) approach. First, we perform a BFS from the root to build a parent map for each node, recording for every node its parent pointer. This allows us to move upward in the tree during the second phase. Then, starting from the target node, we perform another BFS that explores three neighbors for each node: its left child, its right child, and its parent (if it exists). We maintain a `visited` set to avoid revisiting nodes and to prevent infinite loops. We run the second BFS for K levels; when we have processed exactly K edges, the nodes currently in the BFS queue are exactly those at distance K from the target. We then collect their values into a result vector. Edge cases: K=0 should return just the target's value; leaf nodes with no children but having a parent still need to explore the parent; if K is larger than the maximum distance from target to any node, the queue becomes empty before reaching K, and we return an empty vector. Time complexity: O(N) for the first BFS to build parent map and O(N) for the second BFS, where N is the number of nodes, since each node is visited at most once. Space complexity: O(N) for the parent map, visited map, and the BFS queue.

#include <vector>
#include <queue>
#include <unordered_map>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Returns the values of all nodes exactly K edges away from target in the binary tree
// rooted at `root`. The target is guaranteed to be present. K >= 0.
std::vector<int> nodesAtDistanceK(TreeNode* root, TreeNode* target, int K) {
    if (root == nullptr || target == nullptr) {
        return {};
    }
    
    // Phase 1: Build parent pointers using BFS from root.
    std::unordered_map<TreeNode*, TreeNode*> parent;
    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        if (node->left) {
            parent[node->left] = node;
            q.push(node->left);
        }
        if (node->right) {
            parent[node->right] = node;
            q.push(node->right);
        }
    }
    
    // Phase 2: BFS from target, moving to left, right, and parent.
    std::unordered_map<TreeNode*, bool> visited;
    std::queue<TreeNode*> bfs;
    bfs.push(target);
    visited[target] = true;
    int level = 0;
    while (!bfs.empty() && level < K) {
        int size = bfs.size();
        for (int i = 0; i < size; ++i) {
            TreeNode* node = bfs.front();
            bfs.pop();
            if (node->left && !visited[node->left]) {
                bfs.push(node->left);
                visited[node->left] = true;
            }
            if (node->right && !visited[node->right]) {
                bfs.push(node->right);
                visited[node->right] = true;
            }
            if (parent.count(node) && !visited[parent[node]]) {
                bfs.push(parent[node]);
                visited[parent[node]] = true;
            }
        }
        ++level;
    }
    
    // Collect results from the current queue (nodes at distance K).
    std::vector<int> result;
    while (!bfs.empty()) {
        result.push_back(bfs.front()->val);
        bfs.pop();
    }
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Test 1: Simple tree, target at root, K=1
    TreeNode* root1 = new TreeNode(3);
    root1->left = new TreeNode(5);
    root1->right = new TreeNode(1);
    root1->left->left = new TreeNode(6);
    root1->left->right = new TreeNode(2);
    root1->right->left = new TreeNode(0);
    root1->right->right = new TreeNode(8);
    root1->left->right->left = new TreeNode(7);
    root1->left->right->right = new TreeNode(4);
    std::vector<int> res1 = nodesAtDistanceK(root1, root1, 1);
    std::sort(res1.begin(), res1.end());
    std::vector<int> expected1 = {1, 5};
    assert(res1 == expected1);
    
    // Test 2: K=0 returns only target
    std::vector<int> res2 = nodesAtDistanceK(root1, root1->left, 0);
    assert(res2.size() == 1 && res2[0] == 5);
    
    // Test 3: Target is a leaf, K=2 (should go through parent)
    std::vector<int> res3 = nodesAtDistanceK(root1, root1->left->right->left, 2);
    std::sort(res3.begin(), res3.end());
    std::vector<int> expected3 = {1, 3, 4};
    assert(res3 == expected3);
    
    // Test 4: K is too large -> empty result
    std::vector<int> res4 = nodesAtDistanceK(root1, root1->right, 10);
    assert(res4.empty());
    
    // Test 5: Single node tree
    TreeNode* solo = new TreeNode(42);
    std::vector<int> res5 = nodesAtDistanceK(solo, solo, 0);
    assert(res5.size() == 1 && res5[0] == 42);
    std::vector<int> res6 = nodesAtDistanceK(solo, solo, 1);
    assert(res6.empty());
    
    // Test 6: left-skewed tree, target in middle
    TreeNode* skew = new TreeNode(1);
    skew->right = new TreeNode(2);
    skew->right->right = new TreeNode(3);
    skew->right->right->right = new TreeNode(4);
    // target=2, K=1 -> should get {1,3}
    std::vector<int> res7 = nodesAtDistanceK(skew, skew->right, 1);
    std::sort(res7.begin(), res7.end());
    std::vector<int> expected7 = {1, 3};
    assert(res7 == expected7);
    
    // Cleanup (not strictly needed for asserts but good practice)
    // In a real test you'd delete all nodes; here we skip for brevity.
    
    return 0;
}
