// Write a C++ function that, given the root of a binary search tree (BST) and a target integer value, returns the subtree rooted at the node containing that value if it exists; otherwise, it returns `nullptr`. The function must preserve the structure of the subtree (i.e., return the exact node with its left and right children intact). The BST nodes are defined as `struct TreeNode { int val; TreeNode *left; TreeNode *right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };`. The function should be named `findSubtreeByValue` and must handle an empty tree (`root == nullptr`) by returning `nullptr`. The function must work recursively or iteratively, but must not rely on the BST property for searching (i.e., it must search both left and right subtrees even though it could be optimized); the goal is to practice general tree traversal. The solution must be self-contained, include all necessary headers (`<cstddef>` for `nullptr`), and be `const`‑correct where appropriate.

// The core algorithm is a pre‑order traversal of the binary tree. At each node, we check if its value equals the target; if yes, we return that node immediately because it is the root of the desired subtree. If not, we recursively search the left subtree and then the right subtree. If either recursive call returns a non‑null pointer, we propagate that result upward; if both return `nullptr`, we return `nullptr` to indicate absence.  
//
// Edge cases:  
// - The root is `nullptr` (empty tree) → return `nullptr`.  
// - The target value appears multiple times in the tree → the function returns the first node encountered via pre‑order (root first, then left, then right). This is acceptable because the problem specifies no uniqueness requirement.  
// - The target value appears only in the right subtree or deeper in the left subtree → recursive search handles it.  
// - The target value does not exist → all recursive calls return `nullptr`, and the function ultimately returns `nullptr`.  
//
// Time complexity: O(n) in the worst case, where n is the number of nodes, because we may need to visit every node if the target is not present or is the last visited. Space complexity: O(h) due to recursion stack depth, where h is the tree height; worst‑case O(n) for a skewed tree. The function does not modify the tree, so it can be `const`‑qualified on the `TreeNode` pointer (i.e., `const TreeNode*` parameters), but to match typical LeetCode‑style signatures we keep a non‑const pointer. However, we can apply `const` to the function itself if it were a member, but as a free function we can use `const TreeNode*` to indicate read‑only access. The implementation below uses `const TreeNode*` for clarity and safety.

#include <cstddef>  // for nullptr

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Search for a node with given value in a binary tree (not necessarily BST).
// Returns the subtree rooted at that node, or nullptr if not found.
const TreeNode* findSubtreeByValue(const TreeNode* root, int val) {
    if (root == nullptr) return nullptr;
    if (root->val == val) return root;

    const TreeNode* leftResult = findSubtreeByValue(root->left, val);
    if (leftResult != nullptr) return leftResult;

    const TreeNode* rightResult = findSubtreeByValue(root->right, val);
    if (rightResult != nullptr) return rightResult;

    return nullptr;
}

#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(findSubtreeByValue(nullptr, 5) == nullptr);

    // Test 2: Single node tree, value exists
    TreeNode root1(10);
    assert(findSubtreeByValue(&root1, 10) == &root1);

    // Test 3: Single node tree, value not found
    TreeNode root2(7);
    assert(findSubtreeByValue(&root2, 3) == nullptr);

    // Test 4: Tree with left child only
    TreeNode nodeA(5);
    TreeNode nodeB(3);
    nodeA.left = &nodeB;
    assert(findSubtreeByValue(&nodeA, 3) == &nodeB);
    assert(findSubtreeByValue(&nodeA, 5) == &nodeA);
    assert(findSubtreeByValue(&nodeA, 99) == nullptr);

    // Test 5: Tree with right child only
    TreeNode nodeC(8);
    TreeNode nodeD(4);
    nodeC.right = &nodeD;
    assert(findSubtreeByValue(&nodeC, 4) == &nodeD);

    // Test 6: Full tree, value in left subtree deeper
    TreeNode n1(20);
    TreeNode n2(10);
    TreeNode n3(30);
    TreeNode n4(5);
    TreeNode n5(15);
    n1.left = &n2;
    n1.right = &n3;
    n2.left = &n4;
    n2.right = &n5;
    assert(findSubtreeByValue(&n1, 15) == &n5);
    assert(findSubtreeByValue(&n1, 30) == &n3);
    assert(findSubtreeByValue(&n1, 100) == nullptr);

    // Test 7: Duplicate values, returns first in pre-order (root, left, right)
    TreeNode m1(5);
    TreeNode m2(5);
    TreeNode m3(7);
    m1.left = &m2;
    m1.right = &m3;
    assert(findSubtreeByValue(&m1, 5) == &m1);  // root found first

    return 0;
}
