Write a C++ function `bool isValidBST(const TreeNode* root)` that checks whether a given binary tree is a valid binary search tree (BST). The tree is represented using the provided `TreeNode` structure: each node contains an integer `val`, and pointers to left and right children (which may be `nullptr`). A valid BST must satisfy the property that for every node, all values in its left subtree are strictly less than the node's value, and all values in its right subtree are strictly greater. The function must handle an empty tree (return `true`), nodes with equal values (such duplicates are invalid), and must not modify the tree. Implement the function iteratively using an explicit stack (i.e., a simulated in-order traversal), avoiding recursion to demonstrate stack-based traversal. The solution must be `const`-correct and include necessary headers only (e.g., `<stack>`, `<TreeNode>` definition if needed, but the function signature is provided separately). You are allowed to define a `TreeNode` struct in your solution if one is not given globally.

// The key insight is that an in-order traversal of a valid BST yields a strictly increasing sequence of values. If we traverse the tree iteratively using a stack (simulating the recursive in-order process), we can track the previously visited node's value and compare it with the current node's value. The algorithm: initialize an empty stack and set `cur` to `root`, and `pre` to `nullptr`. While `cur` is not null or the stack is not empty: if `cur` is non-null, push it onto the stack and move to its left child (descend left); otherwise, pop the top node (the current in-order node), set `cur` to that popped node, then check: if `pre` is not null and `pre->val >= cur->val`, then the tree is invalid (we use `>=` because duplicates are not allowed). After the check, set `pre = cur`, then move `cur` to its right child. If the loop finishes without violation, return `true`. Edge cases: empty tree returns true; a single node returns true; a tree with duplicate values returns false; a left subtree containing a value greater than the root (or right subtree with smaller) must be caught because in-order visits all nodes in sorted order. Time complexity is O(n) where n is the number of nodes, as each node is visited once. Space complexity is O(h) for the stack in the worst case (h = height of tree), which is O(n) for a skewed tree and O(log n) for a balanced tree.

#include <stack>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Checks whether the binary tree rooted at 'root' is a valid BST.
// Uses iterative in-order traversal with an explicit stack.
bool isValidBST(const TreeNode* root) {
    std::stack<const TreeNode*> st;
    const TreeNode* cur = root;
    const TreeNode* pre = nullptr;

    while (cur != nullptr || !st.empty()) {
        if (cur != nullptr) {
            st.push(cur);
            cur = cur->left;          // Traverse left subtree
        } else {
            cur = st.top();           // Process current node
            st.pop();

            // In-order sequence must be strictly increasing
            if (pre != nullptr && pre->val >= cur->val) {
                return false;
            }
            pre = cur;

            cur = cur->right;         // Traverse right subtree
        }
    }
    return true;
}

#include <cassert>

int main() {
    // Test 1: Empty tree is a valid BST
    assert(isValidBST(nullptr));

    // Test 2: Single node
    TreeNode a(5);
    assert(isValidBST(&a));

    // Test 3: Valid BST (balanced)
    TreeNode b(2), c(7), d(1), e(3), f(6), g(9);
    a.left = &b; a.right = &c;
    b.left = &d; b.right = &e;
    c.left = &f; c.right = &g;
    assert(isValidBST(&a));

    // Test 4: Invalid BST - left child greater than root
    TreeNode h(10), i(20), j(5);
    h.left = &i; h.right = &j; // i.val=20 > h.val=10, invalid
    assert(!isValidBST(&h));

    // Test 5: Invalid BST - duplicate values (right child equal to root)
    TreeNode k(7), l(7);
    k.right = &l; // duplicate, invalid
    assert(!isValidBST(&k));

    // Test 6: Invalid BST - right subtree contains smaller value
    TreeNode m(10), n(15), o(12);
    m.right = &n; n.left = &o; // o.val=12 < n.val=15 but > m.val=10, order fails: in-order gives 10,12,15? Actually 12>10 and 12<15, but 12 is in right subtree of 10, so it must be >10, and it is, but it's in left subtree of 15, so it must be <15, so valid? Wait, 12 is less than 15 and greater than 10, so this is actually valid. Let's use a clearer invalid case: 
    TreeNode p(10), q(15), r(8);
    p.right = &q; q.left = &r; // r.val=8 < 10, invalid
    assert(!isValidBST(&p));

    // Test 7: Valid BST with negative and positive numbers
    TreeNode s(-3), t(-10), u(0);
    s.left = &t; s.right = &u; // -10 < -3 < 0
    assert(isValidBST(&s));

    // Test 8: Complex invalid - deep violation
    TreeNode v(20), w(10), x(30), y(5), z(25);
    v.left = &w; v.right = &x; w.right = &y; x.left = &z; 
    // y.val=5 is in right subtree of 10? Actually y is left? Let's arrange: v.root=20, left=10, right=30, left's right=5 (invalid because 5 < 10), so false
    assert(!isValidBST(&v));

    // Reset pointers to avoid dangling in test 7/8? Not necessary since all heap-less.

    return 0;
}
