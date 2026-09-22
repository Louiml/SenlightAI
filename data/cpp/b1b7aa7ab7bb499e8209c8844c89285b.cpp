/*
Write a C++ function `int deepestLeavesSum(TreeNode* root)` that computes the sum of all node values located on the deepest level of a binary tree. The tree is represented using the `TreeNode` structure with `val`, `left`, and `right` members, where missing children are `nullptr`. The function should handle an empty tree (return 0) and trees with only one node. The result must be computed by performing a level-order traversal, collecting node values level by level, and then summing the values of the last level. The input tree is not modified, and the function should have a constant-time auxiliary space usage per level (excluding the output storage), with an overall time complexity proportional to the number of nodes in the tree.
*/
#include <vector>
#include <queue>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Compute the sum of all values on the deepest level of the binary tree.
// Returns 0 if the tree is empty.
int deepestLeavesSum(const TreeNode* root) {
    if (root == nullptr) return 0;
    
    std::queue<const TreeNode*> q;
    q.push(root);
    std::vector<int> lastLevelValues;
    
    while (!q.empty()) {
        int levelSize = q.size();
        lastLevelValues.clear();  // Reset for the current level
        
        for (int i = 0; i < levelSize; ++i) {
            const TreeNode* node = q.front();
            q.pop();
            lastLevelValues.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    
    int sum = 0;
    for (int value : lastLevelValues) {
        sum += value;
    }
    return sum;
}
#include <cassert>
#include <vector>

// The TreeNode struct and deepestLeavesSum function are assumed to be available here.

int main() {
    // Test 1: Empty tree
    assert(deepestLeavesSum(nullptr) == 0);

    // Test 2: Single node
    TreeNode* root2 = new TreeNode(5);
    assert(deepestLeavesSum(root2) == 5);
    delete root2;

    // Test 3: Root with two children (deepest level is level 1, sum = 3+4=7)
    TreeNode* root3 = new TreeNode(1);
    root3->left = new TreeNode(3);
    root3->right = new TreeNode(4);
    assert(deepestLeavesSum(root3) == 7);
    delete root3->right;
    delete root3->left;
    delete root3;

    // Test 4: A 3-level tree: root 1, left child 2 with child 5, right child 3 with child 6 and 7 at level 2 -> deepest leaves: 5,6,7 sum=18
    TreeNode* root4 = new TreeNode(1);
    root4->left = new TreeNode(2);
    root4->right = new TreeNode(3);
    root4->left->left = new TreeNode(5);
    root4->right->left = new TreeNode(6);
    root4->right->right = new TreeNode(7);
    assert(deepestLeavesSum(root4) == 18);
    delete root4->right->right;
    delete root4->right->left;
    delete root4->left->left;
    delete root4->right;
    delete root4->left;
    delete root4;

    // Test 5: Skewed tree (all left children): 1-2-3-4 -> deepest leaf is 4, sum=4
    TreeNode* root5 = new TreeNode(1);
    root5->left = new TreeNode(2);
    root5->left->left = new TreeNode(3);
    root5->left->left->left = new TreeNode(4);
    assert(deepestLeavesSum(root5) == 4);
    delete root5->left->left->left;
    delete root5->left->left;
    delete root5->left;
    delete root5;

    // Test 6: Tree with one side deeper: root 1, left child 2 with child 4, right child 3 -> deepest leaves: 4 (only) sum=4
    TreeNode* root6 = new TreeNode(1);
    root6->left = new TreeNode(2);
    root6->right = new TreeNode(3);
    root6->left->left = new TreeNode(4);
    assert(deepestLeavesSum(root6) == 4);
    delete root6->left->left;
    delete root6->right;
    delete root6->left;
    delete root6;

    // Test 7: Large values and duplicates: root 10, left 10 with child 10, right 20 -> deepest leaf: 10 and 20? Actually deepest level has only left child 10, so sum=10
    TreeNode* root7 = new TreeNode(10);
    root7->left = new TreeNode(10);
    root7->right = new TreeNode(20);
    root7->left->left = new TreeNode(10);
    assert(deepestLeavesSum(root7) == 10);
    delete root7->left->left;
    delete root7->right;
    delete root7->left;
    delete root7;

    return 0;
}
// The solution uses a queue-based level-order traversal (BFS). We maintain a queue starting with the root, and for each level, we record the number of nodes at that level (the size of the queue before processing). We then process exactly that many nodes, pushing their children (if any) to the queue and accumulating their values into a temporary vector for that level. After processing all nodes at the current level, we add this vector to a result vector of vectors. At the end of traversal, the last element of the result vector contains the values of the deepest level. We then sum those values and return the sum. Edge cases: if the root is `nullptr`, return 0 immediately; if the tree has only one level, the first (and only) level vector is used. The time complexity is O(n), where n is the number of nodes, because each node is visited once. The space complexity is O(m), where m is the maximum width of the tree (the largest number of nodes on any level), which is the maximum size of the queue at any point, plus the storage for the result vector of vectors, which is O(n) in the worst case (e.g., a skewed tree has only one node per level, so the result size is O(n) but the queue size is O(1); a complete tree has O(n) queue size at the last level). In practice, the auxiliary space (excluding the output) is O(m) for the queue, and the result vector is O(n) but that is often considered acceptable. We do not need to store all levels if only the last is needed; we could optimize by keeping only the current level values and overwriting them, but the code snippet provided collects all levels, so we follow that approach for clarity.
