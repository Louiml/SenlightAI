/*
Write a C++ function that takes the root of a binary tree and returns a vector containing the values of its nodes in preorder traversal order (root, left subtree, right subtree). The function must work for any binary tree, including empty trees (returning an empty vector) and trees with only one node. You may assume the tree nodes are defined as `struct TreeNode { int val; TreeNode *left; TreeNode *right; TreeNode() : val(0), left(nullptr), right(nullptr) {} TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {} };`. Implement the traversal iteratively using an explicit stack, not recursion, and do not modify the tree. Ensure the function is declared as `const`-correct (i.e., accepts `const TreeNode*` if possible, but for simplicity you may take `TreeNode*` since the given struct lacks `const` traversal; however, the function itself should not mutate the tree).
*/
#include <vector>
#include <stack>

// Definition for a binary tree node (as provided in the problem context).
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Perform an iterative preorder traversal of a binary tree.
// Returns a vector of node values in the order: root, left subtree, right subtree.
std::vector<int> preorderTraversal(TreeNode* root) {
    std::vector<int> result;
    if (!root) {
        return result;
    }
    std::stack<TreeNode*> nodeStack;
    nodeStack.push(root);
    while (!nodeStack.empty()) {
        TreeNode* current = nodeStack.top();
        nodeStack.pop();
        // Push right child first so that left child is processed next.
        if (current->right) {
            nodeStack.push(current->right);
        }
        if (current->left) {
            nodeStack.push(current->left);
        }
        result.push_back(current->val);
    }
    return result;
}
#include <cassert>
#include <vector>

// The definition and function are assumed to be included above.
// The following main function tests the solution with various tree shapes.
int main() {
    // Test 1: Empty tree -> empty vector
    TreeNode* empty = nullptr;
    assert(preorderTraversal(empty) == std::vector<int>{});

    // Test 2: Single node
    TreeNode single(42);
    assert(preorderTraversal(&single) == std::vector<int>{42});

    // Test 3: Two nodes - root with left child
    TreeNode leftChild(2);
    TreeNode rootL(1, &leftChild, nullptr);
    assert(preorderTraversal(&rootL) == std::vector<int>({1, 2}));

    // Test 4: Two nodes - root with right child
    TreeNode rightChild(3);
    TreeNode rootR(1, nullptr, &rightChild);
    assert(preorderTraversal(&rootR) == std::vector<int>({1, 3}));

    // Test 5: Full binary tree:         1
    //                                /   \
    //                               2     3
    //                              / \   / \
    //                             4   5 6   7
    TreeNode n4(4), n5(5), n6(6), n7(7);
    TreeNode n2(2, &n4, &n5);
    TreeNode n3(3, &n6, &n7);
    TreeNode n1(1, &n2, &n3);
    assert(preorderTraversal(&n1) == std::vector<int>({1, 2, 4, 5, 3, 6, 7}));

    // Test 6: Left-skewed tree: 1 -> 2 -> 3 -> 4
    TreeNode n44(4);
    TreeNode n33(3, &n44, nullptr);
    TreeNode n22(2, &n33, nullptr);
    TreeNode n11(1, &n22, nullptr);
    assert(preorderTraversal(&n11) == std::vector<int>({1, 2, 3, 4}));

    // Test 7: Right-skewed tree: 1 -> 2 -> 3 -> 4
    TreeNode m44(4);
    TreeNode m33(3, nullptr, &m44);
    TreeNode m22(2, nullptr, &m33);
    TreeNode m11(1, nullptr, &m22);
    assert(preorderTraversal(&m11) == std::vector<int>({1, 2, 3, 4}));

    // Test 8: Tree with only right children, but deeper
    TreeNode d3(3);
    TreeNode d2(2, nullptr, &d3);
    TreeNode d1(1, nullptr, &d2);
    assert(preorderTraversal(&d1) == std::vector<int>({1, 2, 3}));

    // Test 9: Tree with only left children
    TreeNode e3(3);
    TreeNode e2(2, &e3, nullptr);
    TreeNode e1(1, &e2, nullptr);
    assert(preorderTraversal(&e1) == std::vector<int>({1, 2, 3}));

    return 0;
}
// The solution uses an explicit stack to simulate the recursive preorder traversal. The algorithm starts by pushing the root onto the stack. While the stack is not empty, the top node is popped, its value is appended to the result vector, and then its right child is pushed first (if non-null), followed by its left child (if non-null). Pushing the right child before the left ensures that the left child is popped next, maintaining the correct preorder order (root → left → right). Edge cases include an empty tree (the initial root is null, so we immediately return an empty vector) and a tree with only one node (the stack will contain only that node, and it will be processed without any pushes). The time complexity is O(n), where n is the number of nodes, because each node is visited exactly once. The space complexity is O(h) in the worst case for the stack (where h is the tree height) and O(n) in the worst case for a skewed tree, but the auxiliary space used explicitly (excluding the output vector) is O(h); if counting the result vector, it is O(n) in total.
