/*
Write a C++ function `std::vector<int> inorderTraversal(const TreeNode* root)` that performs an iterative in-order traversal of a binary tree and returns the node values in the correct order as a vector of integers. The binary tree is defined by the provided `TreeNode` class (with `int data`, `TreeNode* left`, `TreeNode* right`). The function must not use recursion and must handle an empty tree (return an empty vector) as well as trees with only a left or only a right child. The input tree is immutable; do not modify the tree structure. The traversal order must be: left subtree, current node, right subtree.
*/
#include <vector>
#include <stack>

// Definition for a binary tree node.
struct TreeNode {
    int data;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int value) : data(value), left(nullptr), right(nullptr) {}
};

// Perform iterative in-order traversal and return node values in a vector.
std::vector<int> inorderTraversal(const TreeNode* root) {
    std::vector<int> result;
    std::stack<const TreeNode*> nodeStack;
    const TreeNode* current = root;

    while (!nodeStack.empty() || current != nullptr) {
        if (current != nullptr) {
            nodeStack.push(current);
            current = current->left;  // go to left subtree
        } else {
            current = nodeStack.top();
            nodeStack.pop();
            result.push_back(current->data);  // visit node
            current = current->right;         // go to right subtree
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// TreeNode and inorderTraversal definitions go here (from Solution section).

int main() {
    // Test 1: Empty tree
    assert(inorderTraversal(nullptr).empty());

    // Test 2: Single node
    TreeNode* single = new TreeNode(5);
    assert((inorderTraversal(single) == std::vector<int>{5}));

    // Test 3: Full binary tree from the code snippet (1-2-3-4-5-6-7)
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(7);
    assert((inorderTraversal(root) == std::vector<int>{4,2,5,1,6,3,7}));

    // Test 4: Only left child chain
    TreeNode* leftChain = new TreeNode(1);
    leftChain->left = new TreeNode(2);
    leftChain->left->left = new TreeNode(3);
    assert((inorderTraversal(leftChain) == std::vector<int>{3,2,1}));

    // Test 5: Only right child chain
    TreeNode* rightChain = new TreeNode(1);
    rightChain->right = new TreeNode(2);
    rightChain->right->right = new TreeNode(3);
    assert((inorderTraversal(rightChain) == std::vector<int>{1,2,3}));

    // Test 6: Left and right unbalanced
    TreeNode* unbalanced = new TreeNode(10);
    unbalanced->left = new TreeNode(20);
    unbalanced->right = new TreeNode(30);
    unbalanced->left->right = new TreeNode(40);
    assert((inorderTraversal(unbalanced) == std::vector<int>{20,40,10,30}));

    // Clean up memory (optional in a test but good practice)
    delete single;
    delete root->left->left;
    delete root->left->right;
    delete root->right->left;
    delete root->right->right;
    delete root->left;
    delete root->right;
    delete root;
    delete leftChain->left->left;
    delete leftChain->left;
    delete leftChain;
    delete rightChain->right->right;
    delete rightChain->right;
    delete rightChain;
    delete unbalanced->left->right;
    delete unbalanced->left;
    delete unbalanced->right;
    delete unbalanced;

    return 0;
}
// The algorithm uses an explicit stack to simulate the recursion's call stack. Starting from the root, we repeatedly push the current node onto the stack and move to its left child until we reach `nullptr`. At that point, we pop the top node from the stack, append its value to the result vector, and then move to its right child. This process continues while the stack is not empty or the current node is not null.
//
// Edge cases: The empty tree (root is `nullptr`) returns an empty vector. A tree with only a right child forces the stack to be empty before we process the right subtree, but the loop condition `!stack.empty() || curr != nullptr` handles this. A tree with only a left child will cause all left nodes to be pushed, then popped in reverse order, correctly outputting the in-order sequence. The algorithm is `O(n)` time and `O(h)` space, where `n` is the number of nodes and `h` is the tree height (worst-case `O(n)` for skewed trees). The tree is not modified, and the function is `const`-correct by taking `const TreeNode*`.
