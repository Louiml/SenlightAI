Write a C++ function named `collectPreorder` that takes a pointer to the root of a binary tree (defined by a `TreeNode` struct with integer `val` and `left`/`right` pointers) and returns a `std::vector<int>` containing the values of the nodes in preorder traversal order (root, then left subtree, then right subtree). The function must handle an empty tree (nullptr root) by returning an empty vector, and it must work correctly for trees with arbitrary depth, including skew trees. You may implement it recursively, but must ensure the function is `const`-correct (i.e., it does not modify the tree). The solution should be self-contained, include the definition of `TreeNode`, and include necessary headers.
The preorder traversal visits the current node first, then recursively traverses the left subtree, and then the right subtree. The main algorithm is straightforward recursion: if the current node is null, return immediately; otherwise, push the node’s value into the result vector, then recursively call on the left child, then on the right child. Edge cases include an empty tree (root is nullptr) where the recursion base case immediately returns, yielding an empty vector. Also, a tree with only one node works because it pushes the value and then both recursive calls return immediately. For a skew tree (each node has only one child), the recursion depth equals the number of nodes, which could risk stack overflow for very large inputs, but for typical test sizes it is fine. Time complexity is O(n) where n is the number of nodes, because each node is visited once. Space complexity is O(n) in the worst case for the recursion stack (skew tree) plus O(n) for the output vector, so overall O(n).
#include <vector>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper function to perform recursive preorder traversal.
// Appends values to the given vector in preorder order.
void preorderHelper(const TreeNode* curr, std::vector<int>& result) {
    if (curr == nullptr) {
        return;
    }
    result.push_back(curr->val);
    preorderHelper(curr->left, result);
    preorderHelper(curr->right, result);
}

// Public function: returns preorder traversal of binary tree as a vector.
// Takes const pointer to root to ensure the tree is not modified.
std::vector<int> collectPreorder(const TreeNode* root) {
    std::vector<int> result;
    preorderHelper(root, result);
    return result;
}
#include <cassert>
#include <vector>

// Assume collectPreorder and TreeNode are already available (include the solution code above).

int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    assert(collectPreorder(empty) == std::vector<int>());

    // Test 2: Single node
    TreeNode single(42);
    assert(collectPreorder(&single) == std::vector<int>({42}));

    // Test 3: Full tree root=1, left=2, right=3
    TreeNode n1(1), n2(2), n3(3);
    n1.left = &n2;
    n1.right = &n3;
    assert(collectPreorder(&n1) == std::vector<int>({1, 2, 3}));

    // Test 4: Left skew tree: 1->2->3
    TreeNode a(1), b(2), c(3);
    a.left = &b;
    b.left = &c;
    assert(collectPreorder(&a) == std::vector<int>({1, 2, 3}));

    // Test 5: Right skew tree: 1->2->3
    TreeNode x(1), y(2), z(3);
    x.right = &y;
    y.right = &z;
    assert(collectPreorder(&x) == std::vector<int>({1, 2, 3}));

    // Test 6: More complex tree: root=10, left=5 (left=3, right=7), right=15 (right=20)
    TreeNode r(10), l(5), ll(3), lr(7), rr(15), rrr(20);
    r.left = &l; r.right = &rr;
    l.left = &ll; l.right = &lr;
    rr.right = &rrr;
    assert(collectPreorder(&r) == std::vector<int>({10, 5, 3, 7, 15, 20}));

    // Test 7: All left children only but deeper
    TreeNode p1(1), p2(2), p3(3), p4(4);
    p1.left = &p2; p2.left = &p3; p3.left = &p4;
    assert(collectPreorder(&p1) == std::vector<int>({1, 2, 3, 4}));

    return 0;
}
