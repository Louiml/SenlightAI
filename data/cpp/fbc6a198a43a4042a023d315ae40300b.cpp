// Write a C++ function that takes the root of a binary tree and returns a vector of integers containing the tree’s values in postorder traversal order (left subtree, right subtree, root). The function must work for an arbitrary binary tree, including an empty tree (nullptr root), a tree with a single node, and trees with more complex structure. Assume the tree nodes are defined by the provided `TreeNode` structure. Implement the traversal recursively. Your solution should be a free function (not a class method) named `postorderTraversal` that accepts a pointer to the root node and returns a `std::vector<int>`. Use `const` correctness where appropriate, and include all necessary headers.
The core algorithm is a classic depth-first postorder traversal. Given a node, we first recursively traverse its left child, then its right child, and finally append the node’s value to the result vector. The base case occurs when the node is `nullptr`, in which case we simply return without doing anything. This ensures that recursion terminates and handles empty subtrees gracefully. For an empty tree (root is `nullptr`), the function returns an empty vector because the recursive calls never push any value. For a single-node tree, the function calls `traverse` on both null children (which return immediately) and then pushes the root’s value. Time complexity is O(n) because each node is visited exactly once. Space complexity is O(h) for the recursion stack, where h is the height of the tree; in the worst case (skewed tree) h = n, and in the best case (balanced tree) h = log n. The auxiliary vector itself uses O(n) space for the result.
#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Perform postorder traversal of a binary tree.
// The traversal visits left subtree, then right subtree, then root.
void traversePostOrder(const TreeNode* root, std::vector<int>& result) {
    if (root == nullptr) {
        return;
    }
    traversePostOrder(root->left, result);
    traversePostOrder(root->right, result);
    result.push_back(root->val);
}

// Return a vector of node values in postorder traversal order.
std::vector<int> postorderTraversal(const TreeNode* root) {
    std::vector<int> result;
    traversePostOrder(root, result);
    return result;
}
#include <cassert>
#include <vector>

// TreeNode definition and postorderTraversal function from the solution above are assumed.

int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    assert(postorderTraversal(empty) == std::vector<int>{});

    // Test 2: Single node
    TreeNode single(42);
    assert(postorderTraversal(&single) == std::vector<int>{42});

    // Test 3: Two-node tree (root with left child)
    TreeNode leftChild(2);
    TreeNode rootWithLeft(1, &leftChild, nullptr);
    assert(postorderTraversal(&rootWithLeft) == std::vector<int>{2, 1});

    // Test 4: Two-node tree (root with right child)
    TreeNode rightChild(3);
    TreeNode rootWithRight(1, nullptr, &rightChild);
    assert(postorderTraversal(&rootWithRight) == std::vector<int>{3, 1});

    // Test 5: Full binary tree: root=1, left=2, right=3; left has left=4, right=5
    TreeNode node4(4);
    TreeNode node5(5);
    TreeNode node2(2, &node4, &node5);
    TreeNode node3(3);
    TreeNode root(1, &node2, &node3);
    std::vector<int> expected = {4, 5, 2, 3, 1};
    assert(postorderTraversal(&root) == expected);

    // Test 6: Left-skewed tree: 1 -> 2 -> 3 (each node has only left child)
    TreeNode node3s(3);
    TreeNode node2s(2, &node3s, nullptr);
    TreeNode rootSkewed(1, &node2s, nullptr);
    assert(postorderTraversal(&rootSkewed) == std::vector<int>{3, 2, 1});

    // Test 7: Right-skewed tree: 1 -> 2 -> 3 (each node has only right child)
    TreeNode node3r(3);
    TreeNode node2r(2, nullptr, &node3r);
    TreeNode rootSkewedRight(1, nullptr, &node2r);
    assert(postorderTraversal(&rootSkewedRight) == std::vector<int>{3, 2, 1});

    // Test 8: More complex tree: root=10, left=20 (left=40, right=50), right=30 (left=60, right=70)
    TreeNode n40(40), n50(50), n60(60), n70(70);
    TreeNode n20(20, &n40, &n50);
    TreeNode n30(30, &n60, &n70);
    TreeNode rootComplex(10, &n20, &n30);
    assert(postorderTraversal(&rootComplex) == std::vector<int>{40, 50, 20, 60, 70, 30, 10});

    return 0;
}
