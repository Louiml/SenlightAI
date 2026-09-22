/*
Given the root of a binary tree where nodes have an integer value, a `left` pointer, and a `right` pointer, write a C++ function that returns a vector of integers containing the in-order traversal (left subtree, root, right subtree) of the tree. The function must implement the traversal **iteratively** without using any auxiliary stack or recursion, modifying the tree temporarily during the traversal (like the Morris traversal) and restoring it to its original shape before returning. The tree may have up to 10^4 nodes, values may be negative, and empty nodes (nullptr) are allowed. After the function returns, the tree must be unchanged from its original structure (i.e., all left/right pointers must point to the same nodes as before the call).
*/
#include <vector>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Perform Morris in-order traversal, restoring the tree before returning.
std::vector<int> inorderTraversal(TreeNode* root) {
    std::vector<int> ans;
    TreeNode* current = root;
    while (current != nullptr) {
        if (current->left != nullptr) {
            // Find the rightmost node of the left subtree (predecessor)
            TreeNode* predecessor = current->left;
            while (predecessor->right != nullptr && predecessor->right != current) {
                predecessor = predecessor->right;
            }
            if (predecessor->right == nullptr) {
                // Create a temporary thread to the current node
                predecessor->right = current;
                current = current->left;
            } else {
                // The thread already exists; we've visited the left subtree
                // Break the thread and visit current
                predecessor->right = nullptr;
                ans.push_back(current->val);
                current = current->right;
            }
        } else {
            // No left child, visit current and move right
            ans.push_back(current->val);
            current = current->right;
        }
    }
    return ans;
}
#include <cassert>
#include <vector>

// TreeNode definition and inorderTraversal function are assumed to be included here.

int main() {
    // Test 1: Empty tree
    TreeNode* root1 = nullptr;
    assert(inorderTraversal(root1) == std::vector<int>{});

    // Test 2: Single node
    TreeNode* root2 = new TreeNode(5);
    assert(inorderTraversal(root2) == std::vector<int>{5});
    // Verify tree unchanged
    assert(root2->left == nullptr && root2->right == nullptr);
    delete root2;

    // Test 3: Left-skewed tree (1-2-3 as left children)
    TreeNode* n3 = new TreeNode(3);
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n1 = new TreeNode(1);
    n2->left = n3;
    n1->left = n2;
    //          1
    //         /
    //        2
    //       /
    //      3
    assert(inorderTraversal(n1) == std::vector<int>({3,2,1}));
    // Verify tree unchanged
    assert(n1->left == n2 && n2->left == n3 && n3->left == nullptr && n3->right == nullptr);
    delete n1; delete n2; delete n3;

    // Test 4: Right-skewed tree (1-2-3 as right children)
    TreeNode* m1 = new TreeNode(1);
    TreeNode* m2 = new TreeNode(2);
    TreeNode* m3 = new TreeNode(3);
    m1->right = m2;
    m2->right = m3;
    assert(inorderTraversal(m1) == std::vector<int>({1,2,3}));
    // Verify unchanged
    assert(m1->right == m2 && m2->right == m3 && m3->left == nullptr && m3->right == nullptr);
    delete m1; delete m2; delete m3;

    // Test 5: Full balanced tree
    //       4
    //      / \
    //     2   6
    //    / \ / \
    //   1  3 5  7
    TreeNode* a1 = new TreeNode(1);
    TreeNode* a3 = new TreeNode(3);
    TreeNode* a5 = new TreeNode(5);
    TreeNode* a7 = new TreeNode(7);
    TreeNode* a2 = new TreeNode(2);
    TreeNode* a6 = new TreeNode(6);
    TreeNode* a4 = new TreeNode(4);
    a2->left = a1; a2->right = a3;
    a6->left = a5; a6->right = a7;
    a4->left = a2; a4->right = a6;
    assert(inorderTraversal(a4) == std::vector<int>({1,2,3,4,5,6,7}));
    // Verify unchanged
    assert(a4->left == a2 && a4->right == a6);
    assert(a2->left == a1 && a2->right == a3);
    assert(a6->left == a5 && a6->right == a7);
    delete a1; delete a2; delete a3; delete a4; delete a5; delete a6; delete a7;

    // Test 6: Tree with negative values and single left child
    TreeNode* b1 = new TreeNode(-10);
    TreeNode* b2 = new TreeNode(-20);
    b1->left = b2;
    assert(inorderTraversal(b1) == std::vector<int>({-20,-10}));
    assert(b1->left == b2 && b2->left == nullptr && b2->right == nullptr);
    delete b1; delete b2;

    // Test 7: Tree with only right child, verify unchanged
    TreeNode* c1 = new TreeNode(100);
    TreeNode* c2 = new TreeNode(200);
    c1->right = c2;
    assert(inorderTraversal(c1) == std::vector<int>({100,200}));
    assert(c1->right == c2 && c2->left == nullptr && c2->right == nullptr);
    delete c1; delete c2;

    return 0;
}
// The algorithm is a classic Morris in-order traversal that avoids using a stack by temporarily rewiring the rightmost node of the left subtree to point back to the current root. The process:  
// - Start at the root.  
// - While current node is not null:  
//   - If the left child exists, find the rightmost node in the left subtree (the predecessor).  
//     - If the predecessor's right child is null, set it to the current node (creating a temporary thread), then move current to its left child.  
//     - If the predecessor's right child already points to the current node (we’ve already visited the left subtree), break the thread by setting predecessor's right to null, push the current node's value, and move current to its right child.  
//   - If the left child is null, push the current value and move to its right child.  
// This temporarily modifies the tree but always restores it before moving on. Edge cases: empty tree returns empty vector; a single-node tree pushes that value; skewed trees (left-heavy or right-heavy) work fine because the algorithm always reaches the rightmost predecessor correctly. Time complexity is O(n) because each edge is traversed at most twice (once to create the thread, once to break it). Space complexity is O(1) auxiliary (excluding the output vector), using no stack or recursion.
