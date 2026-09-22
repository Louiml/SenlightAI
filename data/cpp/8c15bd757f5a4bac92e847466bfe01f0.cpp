Implement a C++ function that performs an iterative in-order traversal of a binary tree and returns a vector of its node values in the correct order. The function should accept a pointer to the root node of the tree (which may be `nullptr`). You must replicate the behavior of the provided recursive in-order traversal but use an explicit stack to avoid recursion. The tree nodes are defined by the given `TreeNode` struct with integer values and left/right child pointers. Handle all cases including empty trees, skewed trees (e.g., only left children), and balanced trees. The function should be named `inorderTraversal` and must be a free function (not a class method) that takes a `TreeNode*` parameter and returns a `std::vector<int>`.

The algorithm uses an explicit stack to simulate recursion. Start with a `node` pointer set to the root and an empty stack of `TreeNode*`. While either `node` is not `nullptr` or the stack is not empty, do the following: first, push all left children of the current `node` onto the stack, moving `node` to its left child each time. This descends to the leftmost node. Once `node` becomes `nullptr`, pop the top of the stack, append its value to the result vector, and then set `node` to the popped node's right child. This ensures we visit left subtree, then root, then right subtree. The loop terminates when `node` is `nullptr` and the stack is empty. Edge cases: an empty tree returns an empty vector; a tree with one node pushes it, pops it, appends its value, and finishes. The algorithm visits each node exactly once. Time complexity is O(n) where n is the number of nodes, and space complexity is O(h) for the stack in the worst case (skewed tree) or O(log n) for balanced trees; the output vector itself uses O(n) space.

#include <vector>
#include <stack>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Perform iterative in-order traversal of a binary tree.
// Returns a vector containing node values in in-order sequence.
std::vector<int> inorderTraversal(TreeNode* root) {
    std::vector<int> result;
    std::stack<TreeNode*> nodeStack;
    TreeNode* current = root;
    
    while (current != nullptr || !nodeStack.empty()) {
        // Traverse to the leftmost node, pushing each node onto the stack.
        while (current != nullptr) {
            nodeStack.push(current);
            current = current->left;
        }
        // current is nullptr; pop the stack and process the node.
        current = nodeStack.top();
        nodeStack.pop();
        result.push_back(current->val);
        // Move to the right subtree.
        current = current->right;
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty tree
    assert(inorderTraversal(nullptr).empty());

    // Test 2: Single node tree
    TreeNode* single = new TreeNode(5);
    assert((inorderTraversal(single) == std::vector<int>{5}));
    delete single;

    // Test 3: Left-skewed tree: 1 -> 2 -> 3 (root=1, left=2, left=3)
    TreeNode* n3 = new TreeNode(3);
    TreeNode* n2 = new TreeNode(2, n3, nullptr);
    TreeNode* n1 = new TreeNode(1, n2, nullptr);
    assert((inorderTraversal(n1) == std::vector<int>{3, 2, 1}));
    delete n3; delete n2; delete n1;

    // Test 4: Right-skewed tree: 1 -> 2 -> 3 (root=1, right=2, right=3)
    TreeNode* r3 = new TreeNode(3);
    TreeNode* r2 = new TreeNode(2, nullptr, r3);
    TreeNode* r1 = new TreeNode(1, nullptr, r2);
    assert((inorderTraversal(r1) == std::vector<int>{1, 2, 3}));
    delete r3; delete r2; delete r1;

    // Test 5: Balanced tree: root=2, left=1, right=3
    TreeNode* b1 = new TreeNode(1);
    TreeNode* b3 = new TreeNode(3);
    TreeNode* b2 = new TreeNode(2, b1, b3);
    assert((inorderTraversal(b2) == std::vector<int>{1, 2, 3}));
    delete b1; delete b3; delete b2;

    // Test 6: More complex tree: root=4, left=2 (left=1, right=3), right=6 (left=5, right=7)
    TreeNode* c1 = new TreeNode(1);
    TreeNode* c3 = new TreeNode(3);
    TreeNode* c2 = new TreeNode(2, c1, c3);
    TreeNode* c5 = new TreeNode(5);
    TreeNode* c7 = new TreeNode(7);
    TreeNode* c6 = new TreeNode(6, c5, c7);
    TreeNode* c4 = new TreeNode(4, c2, c6);
    assert((inorderTraversal(c4) == std::vector<int>{1, 2, 3, 4, 5, 6, 7}));
    delete c1; delete c2; delete c3; delete c4; delete c5; delete c6; delete c7;

    return 0;
}
