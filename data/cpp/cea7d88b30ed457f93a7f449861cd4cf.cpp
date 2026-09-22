Write a C++ function `std::vector<int> spiralOrder(TreeNode* root)` that takes the root of a binary tree (where each node has an integer `data`, and pointers `left` and `right` that may be `nullptr`) and returns a vector containing the node values visited in a spiral (or zigzag) level-order traversal. In a spiral traversal, you process nodes from left to right on the first level, then right to left on the second level, then left to right on the third, and so on. The function should handle an empty tree (return an empty vector) and a single-node tree. Your solution must not use recursion for the traversal itself; instead, use two stacks to simulate alternating direction changes. You may define the node structure yourself inside the solution, but ensure it is compatible with a standard binary tree node having integer data. The returned vector should contain exactly all `n` node values in the correct spiral order, where `n` is the number of nodes in the tree. Time complexity must be O(n), and auxiliary space must be O(n) in the worst case (for the two stacks combined).
#include <cassert>
#include <vector>

// Include the TreeNode definition and spiralOrder function here (or link it).

int main() {
    // Test 1: Empty tree
    TreeNode* root1 = nullptr;
    assert(spiralOrder(root1).empty());

    // Test 2: Single node
    TreeNode* root2 = new TreeNode(1);
    assert(spiralOrder(root2) == std::vector<int>({1}));

    // Test 3: Simple tree: 1 -> left 2, right 3
    TreeNode* root3 = new TreeNode(1);
    root3->left = new TreeNode(2);
    root3->right = new TreeNode(3);
    assert(spiralOrder(root3) == std::vector<int>({1, 3, 2}));

    // Test 4: Full binary tree with 3 levels
    //         1
    //        / \
    //       2   3
    //      / \ / \
    //     4  5 6  7
    TreeNode* root4 = new TreeNode(1);
    root4->left = new TreeNode(2);
    root4->right = new TreeNode(3);
    root4->left->left = new TreeNode(4);
    root4->left->right = new TreeNode(5);
    root4->right->left = new TreeNode(6);
    root4->right->right = new TreeNode(7);
    assert(spiralOrder(root4) == std::vector<int>({1, 3, 2, 4, 5, 6, 7}));

    // Test 5: Unbalanced tree with missing children
    //      10
    //     /
    //    20
    //     \
    //      30
    TreeNode* root5 = new TreeNode(10);
    root5->left = new TreeNode(20);
    root5->left->right = new TreeNode(30);
    assert(spiralOrder(root5) == std::vector<int>({10, 20, 30}));

    // Test 6: Larger irregular tree
    //       1
    //      / \
    //     2   3
    //    /     \
    //   4       5
    //  / \
    // 6   7
    TreeNode* root6 = new TreeNode(1);
    root6->left = new TreeNode(2);
    root6->right = new TreeNode(3);
    root6->left->left = new TreeNode(4);
    root6->right->right = new TreeNode(5);
    root6->left->left->left = new TreeNode(6);
    root6->left->left->right = new TreeNode(7);
    assert(spiralOrder(root6) == std::vector<int>({1, 3, 2, 4, 5, 6, 7}));

    // Cleanup not shown for brevity; in real code, delete all nodes properly.
    return 0;
}
#include <vector>
#include <stack>

// Definition for a binary tree node.
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Perform spiral (zigzag) level-order traversal iteratively.
// Returns a vector of node values in spiral order.
std::vector<int> spiralOrder(const TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::stack<const TreeNode*> s1;
    std::stack<const TreeNode*> s2;
    s1.push(root);

    while (!s1.empty() || !s2.empty()) {
        // Process current level from left to right (s1)
        while (!s1.empty()) {
            const TreeNode* node = s1.top();
            s1.pop();
            result.push_back(node->data);
            // Push children in left-then-right order to s2
            if (node->left) s2.push(node->left);
            if (node->right) s2.push(node->right);
        }

        // Process next level from right to left (s2)
        while (!s2.empty()) {
            const TreeNode* node = s2.top();
            s2.pop();
            result.push_back(node->data);
            // Push children in right-then-left order to s1
            if (node->right) s1.push(node->right);
            if (node->left) s1.push(node->left);
        }
    }

    return result;
}
// The spiral zigzag traversal can be performed iteratively using two stacks, `s1` and `s2`. Push the root onto `s1`. While either stack is non-empty: pop all nodes from `s1`, process (append to result) each, and push their children onto `s2` in **left-then-right** order (so that when `s2` is popped next, the rightmost child comes first, giving right-to-left order for that level). Then, pop all nodes from `s2`, process each, and push their children onto `s1` in **right-then-left** order (so that when `s1` is popped again, the leftmost child comes first, restoring left-to-right order). This alternates direction level by level. Key edge cases: an empty tree returns an empty vector; a single node returns that node’s value; nodes with only one child are handled because we check for null before pushing. The algorithm visits each node exactly once, so time complexity is O(n) where n is the number of nodes. Space complexity is O(n) in the worst case (e.g., a complete binary tree holds roughly n/2 nodes in one stack at a time), but the two stacks combined never exceed n nodes. The approach avoids recursion and is robust to arbitrary tree shapes.
