// Write a C++ function `TreeNode* createTreeFromDescriptions(vector<vector<int>>& descriptions)` that takes a list of descriptions, where each description is a vector of three integers `{parent, child, isLeft}`. The `parent` and `child` are node values (each unique integer represents exactly one node in the final binary tree), and `isLeft` is either 1 (child is the left child of parent) or 0 (child is the right child). The input describes all edges of a non-empty binary tree, and there is exactly one root (a node with no parent). The function must build the complete binary tree from these descriptions, returning a pointer to the root `TreeNode*`. You may assume that the `TreeNode` struct is already defined as given in the code snippet (with `val`, `left`, `right` and three constructors). Your function must handle any valid tree with up to 10^4 nodes and return `nullptr` if no valid root is found (though the input guarantees a valid tree).

The main idea is to first create or retrieve `TreeNode` pointers for every parent and child value from a hash map that stores the mapping from integer value to the corresponding dynamically allocated node. As we iterate through the descriptions, we create nodes for parent and child if they don’t exist yet, then link the child to either the left or right pointer of the parent based on `isLeft`. Simultaneously, we record every child value in a hash set to track which values are not roots. After processing all descriptions, the root is the only node value that never appears as a child; we find it by iterating through the descriptions again and checking if the parent is not in the child set. If such a parent is found, return its pointer. If none is found (should not happen with valid input), return `nullptr`. Edge cases include multiple descriptions referencing the same parent/child (handled by the map), single-node tree with no edges (but the problem guarantees non-empty so there is at least one edge, but the algorithm still works), and large input sizes—using `unordered_map` and `unordered_set` gives average O(1) operations. The time complexity is O(N) where N is the number of descriptions (two passes), and the space complexity is O(M) where M is the number of distinct node values (since we store one node pointer per value and the child set).

#include <unordered_map>
#include <unordered_set>
#include <vector>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Build a binary tree from a list of (parent, child, isLeft) descriptions.
// Returns pointer to the root node, or nullptr if no root exists.
TreeNode* createTreeFromDescriptions(const std::vector<std::vector<int>>& descriptions) {
    std::unordered_map<int, TreeNode*> nodeMap;
    std::unordered_set<int> childSet;
    
    for (const auto& desc : descriptions) {
        int parent = desc[0];
        int child = desc[1];
        int isLeft = desc[2];
        
        // Create parent node if not exists
        if (nodeMap.find(parent) == nodeMap.end()) {
            nodeMap[parent] = new TreeNode(parent);
        }
        // Create child node if not exists
        if (nodeMap.find(child) == nodeMap.end()) {
            nodeMap[child] = new TreeNode(child);
        }
        
        // Link child to parent
        if (isLeft == 1) {
            nodeMap[parent]->left = nodeMap[child];
        } else {
            nodeMap[parent]->right = nodeMap[child];
        }
        
        childSet.insert(child);
    }
    
    // Find the root: a node that is never a child
    for (const auto& desc : descriptions) {
        int parent = desc[0];
        if (childSet.find(parent) == childSet.end()) {
            return nodeMap[parent];
        }
    }
    
    return nullptr; // Should not reach here for valid input
}

#include <cassert>

int main() {
    // Test 1: Simple tree: root=1, left=2, right=3
    std::vector<std::vector<int>> desc1 = {{1,2,1},{1,3,0}};
    TreeNode* root1 = createTreeFromDescriptions(desc1);
    assert(root1 != nullptr);
    assert(root1->val == 1);
    assert(root1->left != nullptr && root1->left->val == 2);
    assert(root1->right != nullptr && root1->right->val == 3);

    // Test 2: Deeper tree: 1 -> left 2 -> left 3
    std::vector<std::vector<int>> desc2 = {{1,2,1},{2,3,1}};
    TreeNode* root2 = createTreeFromDescriptions(desc2);
    assert(root2 != nullptr && root2->val == 1);
    assert(root2->left != nullptr && root2->left->val == 2);
    assert(root2->left->left != nullptr && root2->left->left->val == 3);
    assert(root2->left->right == nullptr);

    // Test 3: Single edge, parent=10, child=20 as right
    std::vector<std::vector<int>> desc3 = {{10,20,0}};
    TreeNode* root3 = createTreeFromDescriptions(desc3);
    assert(root3 != nullptr && root3->val == 10);
    assert(root3->right != nullptr && root3->right->val == 20);
    assert(root3->left == nullptr);

    // Test 4: Multiple children same parent, ensure both linked
    std::vector<std::vector<int>> desc4 = {{5,1,1},{5,2,0},{5,3,0}};
    TreeNode* root4 = createTreeFromDescriptions(desc4);
    assert(root4 != nullptr && root4->val == 5);
    assert(root4->left != nullptr && root4->left->val == 1);
    assert(root4->right != nullptr && root4->right->val == 2);
    // Note: if both right children specified, the second overwrites the first, so only last right remains
    // But to test correctly, the second right child (for 5,3,0) overwrites the right pointer, so assert that
    assert(root4->right->val == 3);

    // Test 5: Larger tree with multiple levels
    std::vector<std::vector<int>> desc5 = {{1,2,1},{1,3,0},{2,4,1},{2,5,0},{3,6,1}};
    TreeNode* root5 = createTreeFromDescriptions(desc5);
    assert(root5 != nullptr && root5->val == 1);
    assert(root5->left->val == 2);
    assert(root5->right->val == 3);
    assert(root5->left->left->val == 4);
    assert(root5->left->right->val == 5);
    assert(root5->right->left->val == 6);
    assert(root5->right->right == nullptr);

    // Test 6: Node order in descriptions arbitrary (parent may appear later)
    std::vector<std::vector<int>> desc6 = {{4,5,0},{2,3,1},{1,2,1},{1,4,0}};
    TreeNode* root6 = createTreeFromDescriptions(desc6);
    assert(root6 != nullptr && root6->val == 1);
    assert(root6->left->val == 2);
    assert(root6->right->val == 4);
    assert(root6->left->left->val == 3);
    assert(root6->right->right->val == 5);

    // Test 7: Duplicate descriptions of the same edge (should still work)
    std::vector<std::vector<int>> desc7 = {{1,2,1},{1,2,1}};
    TreeNode* root7 = createTreeFromDescriptions(desc7);
    assert(root7 != nullptr && root7->val == 1);
    assert(root7->left->val == 2);
    assert(root7->right == nullptr);

    // Test 8: Values can be negative or large
    std::vector<std::vector<int>> desc8 = {{-100, 50, 1}, {-200, -100, 0}};
    TreeNode* root8 = createTreeFromDescriptions(desc8);
    assert(root8 != nullptr && root8->val == -200);
    assert(root8->right->val == -100);
    assert(root8->right->left->val == 50);

    // Cleanup (optional, but good practice)
    // For simplicity in a test, memory is not freed here.

    return 0;
}
