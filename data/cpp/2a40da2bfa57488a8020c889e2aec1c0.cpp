// Write a C++ function `flattenTree(TreeNode* root)` that transforms a binary tree in-place into a right-skewed linked list structure using the same `TreeNode` nodes, where each node’s `right` pointer points to the next node in the original preorder traversal order, and every node’s `left` pointer is set to `nullptr`. The function must operate iteratively (not recursively) and achieve \(O(n)\) time and \(O(1)\) extra space (excluding the recursion stack, which is not used). The input may be an empty tree (`root == nullptr`), a tree with only left children, only right children, or a balanced tree. You must not allocate new nodes or use auxiliary containers (e.g., vectors, stacks, queues). After the operation, the resulting tree should be a valid linked list where traversing `right` pointers from the original root visits nodes in the same order as a preorder traversal of the original tree. The function signature is `void flattenTree(TreeNode* root);`.
The algorithm is based on Morris traversal-style threading. For each current node, if it has a left child, we find the rightmost node in that left subtree (the predecessor in preorder order). We then link that predecessor’s `right` pointer to the current node’s original right subtree. Next, we move the left subtree to the right: set `curr->right = curr->left`, and set `curr->left = nullptr`. This effectively places the left subtree immediately after the current node in the linked list. Then we advance `curr` to `curr->right` (which is now the former left child). This process repeats until `curr` becomes null. The key insight is that after moving a left subtree, the current node no longer has a left child, so future iterations only process right children. Edge cases: an empty tree requires no action; a tree with only left children will have each left child moved to the right one by one; a tree with only right children is already a valid list and no changes occur. The time complexity is \(O(n)\) because each edge is traversed at most twice (once to find the predecessor and once during the main traversal), and space complexity is \(O(1)\) auxiliary since no recursion or extra data structures are used.
#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Flatten a binary tree into a right-skewed linked list in preorder order.
void flattenTree(TreeNode* root) {
    TreeNode* current = root;
    while (current != nullptr) {
        if (current->left != nullptr) {
            // Find the rightmost node in the left subtree.
            TreeNode* predecessor = current->left;
            while (predecessor->right != nullptr) {
                predecessor = predecessor->right;
            }
            // Connect the rightmost node to the current node's right subtree.
            predecessor->right = current->right;
            // Move the left subtree to the right.
            current->right = current->left;
            current->left = nullptr;
        }
        // Move to the next node in the flattened list.
        current = current->right;
    }
}
#include <cassert>

// Helper to build a tree from a vector (preorder with -1 for null) — not needed for tests, we build manually.
int main() {
    // Test 1: Empty tree.
    TreeNode* empty = nullptr;
    flattenTree(empty);
    assert(empty == nullptr);

    // Test 2: Single node.
    TreeNode single(5);
    flattenTree(&single);
    assert(single.left == nullptr && single.right == nullptr && single.val == 5);

    // Test 3: Tree with left and right children: 1 -> left 2, right 3.
    // Preorder: 1,2,3. After flatten: 1->right=2, 2->right=3, all left=null.
    TreeNode n3(3);
    TreeNode n2(2);
    TreeNode n1(1, &n2, &n3);
    flattenTree(&n1);
    assert(n1.left == nullptr && n1.right == &n2);
    assert(n2.left == nullptr && n2.right == &n3);
    assert(n3.left == nullptr && n3.right == nullptr);

    // Test 4: Left-only chain: 1->left=2, 2->left=3. Preorder: 1,2,3.
    TreeNode c3(3);
    TreeNode c2(2, &c3, nullptr);
    TreeNode c1(1, &c2, nullptr);
    flattenTree(&c1);
    assert(c1.left == nullptr && c1.right == &c2);
    assert(c2.left == nullptr && c2.right == &c3);
    assert(c3.left == nullptr && c3.right == nullptr);

    // Test 5: Right-only chain: 1->right=2, 2->right=3. Already flattened.
    TreeNode r3(3);
    TreeNode r2(2, nullptr, &r3);
    TreeNode r1(1, nullptr, &r2);
    flattenTree(&r1);
    assert(r1.left == nullptr && r1.right == &r2);
    assert(r2.left == nullptr && r2.right == &r3);
    assert(r3.left == nullptr && r3.right == nullptr);

    // Test 6: Balanced tree: 1 left=2, right=3; 2 left=4, right=5; 3 left=6, right=7.
    // Preorder: 1,2,4,5,3,6,7.
    TreeNode b7(7);
    TreeNode b6(6);
    TreeNode b5(5);
    TreeNode b4(4);
    TreeNode b3(3, &b6, &b7);
    TreeNode b2(2, &b4, &b5);
    TreeNode b1(1, &b2, &b3);
    flattenTree(&b1);
    // Verify sequence by traversing right pointers.
    int expected[] = {1, 2, 4, 5, 3, 6, 7};
    TreeNode* ptr = &b1;
    for (int i = 0; i < 7; ++i) {
        assert(ptr != nullptr);
        assert(ptr->val == expected[i]);
        assert(ptr->left == nullptr);
        ptr = ptr->right;
    }
    assert(ptr == nullptr);

    // Test 7: Complex tree with deep left and right: 1 left=2, 2 left=3, 3 right=4, 1 right=5.
    // Preorder: 1,2,3,4,5.
    TreeNode d4(4);
    TreeNode d3(3, nullptr, &d4);
    TreeNode d2(2, &d3, nullptr);
    TreeNode d5(5);
    TreeNode d1(1, &d2, &d5);
    flattenTree(&d1);
    int expected2[] = {1, 2, 3, 4, 5};
    TreeNode* ptr2 = &d1;
    for (int i = 0; i < 5; ++i) {
        assert(ptr2 != nullptr);
        assert(ptr2->val == expected2[i]);
        assert(ptr2->left == nullptr);
        ptr2 = ptr2->right;
    }
    assert(ptr2 == nullptr);

    return 0;
}
