Write a C++ function `int houseRobber(TreeNode* root)` that computes the maximum amount of money a thief can rob from a binary tree of houses, where each `TreeNode` stores an integer `val` (the money in that house). The thief cannot rob two directly linked houses (i.e., if a house is robbed, its parent and children cannot be robbed), but can rob any other houses. The tree may be empty (`root == nullptr`), in which case the result is 0. The function must work for arbitrarily deep trees and large values, and should not modify the tree. You must design a solution using a bottom-up post-order traversal, returning the optimal total.

// The problem is a classic tree DP. For each node, we compute two values:  
// - `selected`: the maximum money obtainable from the subtree rooted at this node **if this node is robbed**.  
// - `unselected`: the maximum money obtainable from the subtree **if this node is not robbed**.  
//
// When the node is robbed, we cannot rob its children, so `selected = node->val + left.unselected + right.unselected`. When the node is not robbed, we are free to rob or not rob each child independently, so `unselected = max(left.selected, left.unselected) + max(right.selected, right.unselected)`. For a `nullptr` node, both values are 0. The answer for the whole tree is `max(root.selected, root.unselected)`. This recursively explores each node exactly once, so time complexity is O(n) and space complexity is O(h) for the call stack, where h is the tree height (worst-case O(n) for a skewed tree). Edge cases: empty tree returns 0; single node returns its value; nodes with only one child are handled naturally because the missing child contributes 0.

#include <algorithm>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Returns the maximum money that can be robbed from the binary tree.
// The thief cannot rob two directly connected houses.
int houseRobber(TreeNode* root) {
    // Helper struct to carry two DP values for a subtree.
    struct RobInfo {
        int selected;   // max money if this node is robbed
        int unselected; // max money if this node is not robbed
    };
    
    // Recursive post-order traversal.
    // This lambda is defined using std::function or a helper closure.
    // We use a recursive lambda with a wrapper.
    RobInfo dfs(TreeNode* node) {
        if (node == nullptr) {
            return {0, 0};
        }
        RobInfo left = dfs(node->left);
        RobInfo right = dfs(node->right);
        
        RobInfo result;
        result.selected = node->val + left.unselected + right.unselected;
        result.unselected = std::max(left.selected, left.unselected) 
                          + std::max(right.selected, right.unselected);
        return result;
    }
    
    RobInfo total = dfs(root);
    return std::max(total.selected, total.unselected);
}

#include <cassert>

// Assume the TreeNode and houseRobber declarations are above.

int main() {
    // Test 1: Empty tree
    assert(houseRobber(nullptr) == 0);
    
    // Test 2: Single node
    TreeNode* single = new TreeNode(4);
    assert(houseRobber(single) == 4);
    delete single;
    
    // Test 3: Simple tree [3,2,3,null,3,null,1] from the prompt
    TreeNode* root1 = new TreeNode(3);
    root1->left = new TreeNode(2);
    root1->right = new TreeNode(3);
    root1->left->right = new TreeNode(3);
    root1->right->right = new TreeNode(1);
    assert(houseRobber(root1) == 7);
    // Cleanup
    delete root1->left->right;
    delete root1->right->right;
    delete root1->left;
    delete root1->right;
    delete root1;
    
    // Test 4: Chain where alternating robbery is optimal
    // Tree: 1 - 2 - 3 (right chain)
    TreeNode* root2 = new TreeNode(1);
    root2->right = new TreeNode(2);
    root2->right->right = new TreeNode(3);
    // Rob 1 and 3 => 4
    assert(houseRobber(root2) == 4);
    delete root2->right->right;
    delete root2->right;
    delete root2;
    
    // Test 5: All negative? Not specified; values assumed non-negative? But we test anyway.
    // Here we just test a simple case with equal values.
    TreeNode* root3 = new TreeNode(5);
    root3->left = new TreeNode(5);
    assert(houseRobber(root3) == 5); // rob one of them (either 5) but not both
    delete root3->left;
    delete root3;
    
    // Test 6: Balanced tree with known optimal
    // Tree:     4
    //         /   \
    //        1     2
    //       / \   / \
    //      3   4 5   6
    // Rob level 0 and level 2? Let's compute: if rob root(4) -> 4 + (1's unsel? but we skip) 
    // Actually best is rob root + leaves? Let's just compute by manual: 
    // Option A: rob root(4) + max of each child's unselected: child 1 unselected = max(3,4)=4, child2 unselected = max(5,6)=6 -> total=4+4+6=14
    // Option B: not rob root -> rob best of each child: child1 selected=1+... but its children: 1+max(3,4)? Actually child1 selected = 1+unselected of its children (0+0=0) =>1; unselected = max(3,4)=4 => max=4. child2 similarly selected=2, unselected=6 => max=6. total=4+6=10. So answer 14.
    TreeNode* root4 = new TreeNode(4);
    root4->left = new TreeNode(1);
    root4->right = new TreeNode(2);
    root4->left->left = new TreeNode(3);
    root4->left->right = new TreeNode(4);
    root4->right->left = new TreeNode(5);
    root4->right->right = new TreeNode(6);
    assert(houseRobber(root4) == 14);
    // Cleanup recursively (not fully shown for brevity)
    // In real code you would delete all, but for test it's okay.
    delete root4->left->left;
    delete root4->left->right;
    delete root4->right->left;
    delete root4->right->right;
    delete root4->left;
    delete root4->right;
    delete root4;
    
    return 0;
}
