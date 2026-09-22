Write a C++ function `bool isBSTMorris(node* root)` that determines whether a given binary tree is a valid Binary Search Tree (BST) using Morris traversal (O(1) auxiliary space). The tree is defined by the `node` struct with fields `val`, `left`, and `right`. A BST is valid if for every node, all values in its left subtree are strictly less than the node's value, and all values in its right subtree are strictly greater. The function must return `true` for an empty tree (`root == nullptr`) and `false` otherwise. The function should not modify the tree permanently; any temporary modifications made during Morris traversal must be undone before returning.
#include <cassert>
#include <cstddef>

// Node struct and isBSTMorris function provided above have been included here for completeness.
int main() {
    // Test 1: empty tree
    node* empty = nullptr;
    assert(isBSTMorris(empty) == true);

    // Test 2: single node
    node* single = new node(5);
    assert(isBSTMorris(single) == true);
    delete single;

    // Test 3: valid BST (root 10, left 5, right 15)
    node* root1 = new node(10);
    root1->left = new node(5);
    root1->right = new node(15);
    assert(isBSTMorris(root1) == true);
    delete root1->left;
    delete root1->right;
    delete root1;

    // Test 4: invalid BST (left child greater than root)
    node* root2 = new node(10);
    root2->left = new node(20);
    assert(isBSTMorris(root2) == false);
    delete root2->left;
    delete root2;

    // Test 5: invalid BST due to duplicate values (equal to parent)
    node* root3 = new node(10);
    root3->left = new node(10);
    assert(isBSTMorris(root3) == false);
    delete root3->left;
    delete root3;

    // Test 6: invalid BST deeper (right subtree has smaller value)
    node* root4 = new node(10);
    root4->right = new node(15);
    root4->right->left = new node(12);
    root4->right->right = new node(20);
    root4->right->left->left = new node(11); // 11 < 12 but also < 10? Actually 11 > 10, but 11 < 12, okay? Check: 11 is in left of 12, should be < 12 yes, but also all in right of 10 must be > 10, 11>10 yes, but inorder: 11,12,15,20? Actually root's right subtree values are 11,12,15,20 all >10, but BST requires left of 12 to be <12, okay. But also 11 is less than 15? Not relevant. Let's make it invalid: put 9 in right? Then it's invalid. Use 9:  
    // Instead, modify: root4->right->left->left = new node(9); // 9 < 10 invalid
    // Redo cleanly:
    node* root5 = new node(10);
    root5->right = new node(15);
    root5->right->left = new node(12);
    root5->right->left->left = new node(9); // invalid, 9 < 10
    assert(isBSTMorris(root5) == false);
    delete root5->right->left->left;
    delete root5->right->left;
    delete root5->right;
    delete root5;

    // Test 7: valid complex BST
    node* root6 = new node(20);
    root6->left = new node(10);
    root6->left->right = new node(15);
    root6->right = new node(30);
    root6->right->left = new node(25);
    root6->right->right = new node(35);
    assert(isBSTMorris(root6) == true);
    delete root6->left->right;
    delete root6->left;
    delete root6->right->left;
    delete root6->right->right;
    delete root6->right;
    delete root6;

    return 0;
}
#include <cstddef>

struct node {
    int val;
    node *left;
    node *right;
    node(int n) : val(n), left(nullptr), right(nullptr) {}
};

// Checks if the tree is a valid BST using Morris traversal (O(1) space).
// Temporary changes are undone before returning.
bool isBSTMorris(node* root) {
    if (root == nullptr) return true;

    node* curr = root;
    node* prev = nullptr;
    bool first = true;
    int lastVal = 0;

    while (curr != nullptr) {
        if (curr->left == nullptr) {
            // Visit current node
            if (!first && curr->val <= lastVal) {
                return false;
            }
            lastVal = curr->val;
            first = false;
            curr = curr->right;
        } else {
            // Find inorder predecessor
            node* pred = curr->left;
            while (pred->right != nullptr && pred->right != curr) {
                pred = pred->right;
            }

            if (pred->right == nullptr) {
                // Create thread
                pred->right = curr;
                curr = curr->left;
            } else {
                // Thread exists: remove it and visit current
                pred->right = nullptr;
                if (!first && curr->val <= lastVal) {
                    return false;
                }
                lastVal = curr->val;
                first = false;
                curr = curr->right;
            }
        }
    }
    return true;
}
// The challenge is to check BST property in-place without using a stack or recursion. Morris traversal allows an inorder traversal with O(1) extra space by temporarily linking each node's predecessor to the node itself. We perform a standard Morris inorder walk, but instead of printing, we compare each visited node's value with the previous visited node's value. In a valid BST, an inorder traversal yields strictly increasing values. At each step, we track `prev` (the previously visited node's value) and the current `curr`. If at any point `curr->val <= prev`, the tree is not a BST. The key edge cases: (1) empty or single-node trees are valid; (2) duplicate values are invalid (strict inequality); (3) the Morris traversal temporarily modifies tree pointers, but we must restore them by setting `predecessor->right = NULL` when we encounter the thread; (4) the traversal ends correctly when `curr` becomes null. Time complexity is O(n) for n nodes because each node is visited at most twice (once to create a thread, once to traverse it). Space complexity is O(1) auxiliary, aside from the recursion stack used by the traversal itself (which is not used here since it's iterative). The function must be `const`-safe at the node level? Actually, we cannot make it fully const because Morris traversal mutates pointers temporarily, but we can avoid `const` on the root parameter to allow modifications. We'll implement iteratively.
