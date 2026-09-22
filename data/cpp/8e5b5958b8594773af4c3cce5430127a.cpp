// Write a standalone C++ function that takes a pointer to the root of a binary tree where each node stores an integer value, and returns the sum of all node values that are strictly greater than the value of their parent node (i.e., for each edge where the child's value is larger than the parent's, add the child's value to the total). The function should handle an empty tree (returning 0) and work correctly for both left and right children. You may assume the tree is acyclic and that node values are within the range of `int`. Do not modify the tree or use any global state.

// The solution requires a recursive traversal of the binary tree. For each node, if it has a parent (i.e., it is not the root), compare its value with the parent's value; if the child's value is greater, add the child's value to a running sum. The function can be implemented as a helper that takes the current node and its parent's value (or a sentinel like `std::nullopt` for the root). Starting from the root with no parent (so no addition for the root itself), recursively visit left and right children, passing the current node's value as the new parent value. The base case is a null node, returning 0. The time complexity is O(n) where n is the number of nodes, because each node is visited once. The space complexity is O(h) for the recursive call stack, where h is the tree height; in the worst case (a skewed tree) this is O(n), but for balanced trees it is O(log n). Edge cases include an empty tree (return 0), a single-node tree (return 0, since no parent exists), and handling negative or duplicate values correctly (strict > comparison). No need to handle cycles, as the problem states the tree is acyclic.

#include <cstddef>  // for nullptr
#include <optional> // for std::optional (optional<T> as parent value sentinel)

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper that recursively accumulates the sum of child values that are greater than their parent.
int sumGreaterThanParentRecursive(const TreeNode* node, std::optional<int> parentVal) {
    if (node == nullptr) {
        return 0;
    }
    int sum = 0;
    // If this node has a parent and its value is strictly greater than the parent's, add it.
    if (parentVal.has_value() && node->val > *parentVal) {
        sum += node->val;
    }
    // Recurse into left and right children, passing the current node's value as the new parent.
    sum += sumGreaterThanParentRecursive(node->left, node->val);
    sum += sumGreaterThanParentRecursive(node->right, node->val);
    return sum;
}

// Public function: given the root of a binary tree, return the sum of all node values
// that are strictly greater than the value of their parent node.
int sumGreaterThanParent(const TreeNode* root) {
    // The root has no parent, so pass an empty optional.
    return sumGreaterThanParentRecursive(root, std::nullopt);
}

#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(sumGreaterThanParent(nullptr) == 0);

    // Test 2: Single node tree (no parent)
    TreeNode* single = new TreeNode(5);
    assert(sumGreaterThanParent(single) == 0);
    delete single;

    // Test 3: Small tree: root=1, left child=2 ( >1 ), right child=0 (not >1)
    TreeNode* n1 = new TreeNode(1);
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n3 = new TreeNode(0);
    n1->left = n2;
    n1->right = n3;
    assert(sumGreaterThanParent(n1) == 2); // only 2 is greater than parent 1
    // Cleanup
    delete n1; delete n2; delete n3;

    // Test 4: Chain: 1 -> 2 -> 3 (each child greater than parent)
    TreeNode* a = new TreeNode(1);
    TreeNode* b = new TreeNode(2);
    TreeNode* c = new TreeNode(3);
    a->right = b;
    b->right = c;
    assert(sumGreaterThanParent(a) == 5); // 2 + 3
    delete a; delete b; delete c;

    // Test 5: Larger tree with negative and duplicate values
    //        5
    //       / \
    //      3   7
    //     / \   \
    //    4   2   8
    //  Check: 3<5 (not add), 7>5 (add 7), 4>3 (add 4), 2<3 (not add), 8>7 (add 8) => total 19
    TreeNode* r = new TreeNode(5);
    TreeNode* l1 = new TreeNode(3);
    TreeNode* r1 = new TreeNode(7);
    TreeNode* l2 = new TreeNode(4);
    TreeNode* r2 = new TreeNode(2);
    TreeNode* r3 = new TreeNode(8);
    r->left = l1; r->right = r1;
    l1->left = l2; l1->right = r2;
    r1->right = r3;
    assert(sumGreaterThanParent(r) == 19);
    delete r; delete l1; delete r1; delete l2; delete r2; delete r3;

    // Test 6: All children less than parents => sum 0
    TreeNode* p = new TreeNode(10);
    TreeNode* q = new TreeNode(5);
    TreeNode* s = new TreeNode(1);
    p->left = q; q->right = s;
    assert(sumGreaterThanParent(p) == 0);
    delete p; delete q; delete s;

    // Test 7: Mixed equal values: parent=5, child=5 (not greater), other child=6 (greater)
    TreeNode* e1 = new TreeNode(5);
    TreeNode* e2 = new TreeNode(5);
    TreeNode* e3 = new TreeNode(6);
    e1->left = e2; e1->right = e3;
    assert(sumGreaterThanParent(e1) == 6);
    delete e1; delete e2; delete e3;

    return 0;
}
