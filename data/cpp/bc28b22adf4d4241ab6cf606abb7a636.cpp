// Write a C++ function `binary_tree_traversal_order` that, given the root of a binary tree (where each node contains an integer value and pointers to its left and right children, with `nullptr` representing an empty subtree), returns a `std::vector<int>` containing the node values in **pre-order** traversal order. Pre-order visits the current node, then the left subtree, then the right subtree. The function must not use recursion (to avoid stack overflow on deep trees) and must handle an empty tree (root `nullptr`) by returning an empty vector. The tree structure is defined by the provided `BinaryTreeNode` struct with fields `val`, `left`, and `right`. You may not modify the tree; the input is read-only. Ensure the implementation is iterative using an explicit stack.

// The solution uses an explicit stack to simulate recursion while avoiding call-stack depth limits. Start by pushing the root onto the stack. While the stack is not empty, pop the top node. If the popped node is `nullptr` (which can occur when pushing children that do not exist), skip it. Otherwise, append its value to the result vector, then push its right child and then its left child onto the stack (right first because the stack is LIFO; left is popped first to maintain pre-order: node → left → right). This guarantees that when the left subtree is fully processed, the right subtree is visited next. Edge cases: empty tree (root `nullptr`) → immediately return empty vector; nodes with only one child → pushed `nullptr` gets skipped. Time complexity is \(O(n)\) because each node is pushed and popped once. Space complexity is \(O(h)\) in the worst case for a skewed tree (stack holds the path) and \(O(n)\) for a completely balanced tree’s frontier, but typically \(O(n)\) worst-case for a degenerate tree where we push both children each time. The implementation uses `std::stack` and `std::vector`, with `const` correctness on the input pointer (though pointer itself can be non-const, we treat tree as read-only).

#include <vector>
#include <stack>

struct BinaryTreeNode {
    int val;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
    BinaryTreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Performs iterative pre-order traversal of a binary tree.
// Returns node values in pre-order: root, left subtree, right subtree.
std::vector<int> pre_order_traversal(const BinaryTreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::stack<const BinaryTreeNode*> node_stack;
    node_stack.push(root);

    while (!node_stack.empty()) {
        const BinaryTreeNode* node = node_stack.top();
        node_stack.pop();

        if (node == nullptr) {
            continue;
        }

        result.push_back(node->val);
        node_stack.push(node->right);
        node_stack.push(node->left);
    }

    return result;
}

#include <cassert>
#include <vector>

// BinaryTreeNode definition included for testing (same as in solution).
struct BinaryTreeNode {
    int val;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
    BinaryTreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

std::vector<int> pre_order_traversal(const BinaryTreeNode* root);

int main() {
    // Test 1: Empty tree
    assert(pre_order_traversal(nullptr).empty());

    // Test 2: Single node
    BinaryTreeNode n1(5);
    assert(pre_order_traversal(&n1) == std::vector<int>({5}));

    // Test 3: Left-skewed tree: 1 -> 2 -> 3 (all left children)
    BinaryTreeNode n3(3);
    BinaryTreeNode n2(2);
    n2.left = &n3;
    BinaryTreeNode n1_skew(1);
    n1_skew.left = &n2;
    std::vector<int> left_skew = pre_order_traversal(&n1_skew);
    assert(left_skew == std::vector<int>({1, 2, 3}));

    // Test 4: Right-skewed tree: 1 -> 2 -> 3 (all right children)
    BinaryTreeNode m3(3);
    BinaryTreeNode m2(2);
    m2.right = &m3;
    BinaryTreeNode m1(1);
    m1.right = &m2;
    assert(pre_order_traversal(&m1) == std::vector<int>({1, 2, 3}));

    // Test 5: Full binary tree: root=10, left subtree root=5 (left=3, right=7), right subtree root=15 (left=12, right=18)
    BinaryTreeNode leaf_l(3);
    BinaryTreeNode leaf_r(7);
    BinaryTreeNode left_sub(5);
    left_sub.left = &leaf_l;
    left_sub.right = &leaf_r;

    BinaryTreeNode leaf_rl(12);
    BinaryTreeNode leaf_rr(18);
    BinaryTreeNode right_sub(15);
    right_sub.left = &leaf_rl;
    right_sub.right = &leaf_rr;

    BinaryTreeNode root(10);
    root.left = &left_sub;
    root.right = &right_sub;

    std::vector<int> expected = {10, 5, 3, 7, 15, 12, 18};
    assert(pre_order_traversal(&root) == expected);

    // Test 6: Tree with only left child at some nodes
    BinaryTreeNode a(1);
    BinaryTreeNode b(2);
    BinaryTreeNode c(3);
    a.left = &b;
    b.right = &c; // 1 -> left 2 -> right 3
    assert(pre_order_traversal(&a) == std::vector<int>({1, 2, 3}));

    return 0;
}
