Write a C++ function that performs a preorder traversal of a binary tree and returns a vector of the node values in the order they are visited. The tree is represented using the `TreeNode` struct with `val`, `left`, and `right` pointers. The function should be iterative (not recursive) to avoid stack overflow on deeply skewed trees, and it must handle an empty tree and trees with only one node correctly. The signature should be `std::vector<int> preorderTraversalIterative(const TreeNode* root)`, where the root pointer is passed as `const` to indicate that the tree is not modified.
#include <cassert>
#include <vector>

int main() {
    // Test empty tree
    TreeNode* root = nullptr;
    std::vector<int> result = preorderTraversalIterative(root);
    assert(result.empty());

    // Test single node
    TreeNode node1(5);
    result = preorderTraversalIterative(&node1);
    assert(result.size() == 1 && result[0] == 5);

    // Test balanced tree:   1
    //                      / \
    //                     2   3
    //                    / \   \
    //                   4   5   6
    TreeNode n1(1), n2(2), n3(3), n4(4), n5(5), n6(6);
    n1.left = &n2;
    n1.right = &n3;
    n2.left = &n4;
    n2.right = &n5;
    n3.right = &n6;
    result = preorderTraversalIterative(&n1);
    assert(result == (std::vector<int>{1, 2, 4, 5, 3, 6}));

    // Test left-skewed tree: 1 -> 2 -> 3 -> 4
    TreeNode m1(1), m2(2), m3(3), m4(4);
    m1.right = &m2;
    m2.right = &m3;
    m3.right = &m4;
    result = preorderTraversalIterative(&m1);
    assert(result == (std::vector<int>{1, 2, 3, 4}));

    // Test right-skewed tree: 1 -> 2 -> 3
    TreeNode r1(1), r2(2), r3(3);
    r1.right = &r2;
    r2.right = &r3;
    result = preorderTraversalIterative(&r1);
    assert(result == (std::vector<int>{1, 2, 3}));

    // Test tree with only left children
    TreeNode l1(10), l2(20), l3(30);
    l1.left = &l2;
    l2.left = &l3;
    result = preorderTraversalIterative(&l1);
    assert(result == (std::vector<int>{10, 20, 30}));

    // Test const correctness: calling with a const tree pointer
    const TreeNode* constRoot = &l1;
    result = preorderTraversalIterative(constRoot);
    assert(result == (std::vector<int>{10, 20, 30}));

    return 0;
}
#include <vector>
#include <stack>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Perform iterative preorder traversal of a binary tree.
// Returns the node values in preorder (root, left, right) sequence.
std::vector<int> preorderTraversalIterative(const TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }
    
    std::stack<const TreeNode*> nodeStack;
    nodeStack.push(root);
    
    while (!nodeStack.empty()) {
        const TreeNode* current = nodeStack.top();
        nodeStack.pop();
        result.push_back(current->val);
        
        // Push right child first, then left, so left is processed first (LIFO).
        if (current->right != nullptr) {
            nodeStack.push(current->right);
        }
        if (current->left != nullptr) {
            nodeStack.push(current->left);
        }
    }
    
    return result;
}
// Preorder traversal visits the root node first, then the left subtree, and finally the right subtree. For an iterative solution, we use an explicit stack. The algorithm is: if the root is null, return an empty vector. Otherwise, push the root onto the stack. While the stack is not empty, pop the top node, add its value to the result, then push its right child (if it exists) and then its left child (if it exists). Pushing the right child first ensures that the left child is processed next, because the stack is LIFO. This yields the correct order: root, then entire left subtree, then right subtree. Edge cases include an empty tree (returns empty vector) and a single-node tree (returns a single-element vector). The algorithm uses \(O(n)\) time, where \(n\) is the number of nodes, because each node is visited exactly once. The auxiliary space is \(O(h)\) for the stack in the worst case, where \(h\) is the height of the tree (for a skewed tree, \(O(n)\); for a balanced tree, \(O(\log n)\)).
