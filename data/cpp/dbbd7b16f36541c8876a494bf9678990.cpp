/*
Write a C++ function `kthLargestInBST` that takes a pointer to the root of a binary search tree (BST) and a positive integer `k`, and returns the `k`-th largest value in the tree (1-indexed, where 1 means the maximum). The tree nodes are defined as `struct Node { int data; Node* left; Node* right; };` with a constructor that initializes `data` and sets both children to `nullptr`. The function must handle cases where `k` is larger than the number of nodes, in which case it should return a sentinel value of `-1` (assuming all tree values are non‑negative). The tree may contain duplicate values; each occurrence counts separately. The function should not modify the tree and must use iterative traversal or recursion with a counter. The solution must be provided as a standalone free function (no `main`), and a test harness will call it with various BSTs to verify correctness.
*/
#include <stack>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Return the k-th largest value in BST, or -1 if k is out of range.
int kthLargestInBST(const Node* root, int k) {
    if (root == nullptr || k <= 0) return -1;

    std::stack<const Node*> st;
    const Node* current = root;
    int visited = 0;

    while (current != nullptr || !st.empty()) {
        // Go to the rightmost node
        while (current != nullptr) {
            st.push(current);
            current = current->right;
        }
        current = st.top();
        st.pop();

        visited++;
        if (visited == k) {
            return current->data;
        }

        // Move to left subtree
        current = current->left;
    }

    // k is larger than number of nodes
    return -1;
}
#include <cassert>
#include <iostream>

// Node definition already given in the solution; include it here for completeness
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper to insert (not used by tests but for building trees)
Node* insert(Node* root, int val) {
    if (!root) return new Node(val);
    if (val > root->data) root->right = insert(root->right, val);
    else root->left = insert(root->left, val);
    return root;
}

// Free helper to delete tree
void deleteTree(Node* root) {
    if(!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// The function to test is declared here (must match the provided solution)
int kthLargestInBST(const Node* root, int k);

int main() {
    // Test 1: Basic tree: 5,3,8,1,4,7,9  (in-order: 1,3,4,5,7,8,9)
    Node* root1 = nullptr;
    root1 = insert(root1, 5);
    insert(root1, 3); insert(root1, 8);
    insert(root1, 1); insert(root1, 4);
    insert(root1, 7); insert(root1, 9);
    assert(kthLargestInBST(root1, 1) == 9);
    assert(kthLargestInBST(root1, 2) == 8);
    assert(kthLargestInBST(root1, 3) == 7);
    assert(kthLargestInBST(root1, 7) == 1);
    assert(kthLargestInBST(root1, 8) == -1); // out of range

    // Test 2: Skewed left (only right children? Actually a chain left)
    Node* root2 = nullptr;
    root2 = insert(root2, 10);
    root2->left = new Node(9);
    root2->left->left = new Node(8);
    assert(kthLargestInBST(root2, 1) == 10);
    assert(kthLargestInBST(root2, 3) == 8);

    // Test 3: Duplicate values
    Node* root3 = nullptr;
    root3 = new Node(5);
    root3->left = new Node(5);
    root3->right = new Node(7);
    root3->left->left = new Node(5);
    assert(kthLargestInBST(root3, 1) == 7);
    assert(kthLargestInBST(root3, 2) == 5);
    assert(kthLargestInBST(root3, 4) == 5);

    // Test 4: Empty tree and invalid k
    assert(kthLargestInBST(nullptr, 1) == -1);
    assert(kthLargestInBST(root1, 0) == -1);
    assert(kthLargestInBST(root1, -5) == -1);

    // Test 5: Single node
    Node* root4 = new Node(42);
    assert(kthLargestInBST(root4, 1) == 42);
    assert(kthLargestInBST(root4, 2) == -1);

    // Clean up
    deleteTree(root1); deleteTree(root2); deleteTree(root3); delete root4;

    std::cout << "All tests passed." << std::endl;
    return 0;
}
// The approach is to perform a reverse in‑order traversal of the BST (right subtree, current node, left subtree), which visits nodes in decreasing order. Maintain a counter; when the counter reaches `k`, the current node’s value is the answer. If the traversal ends without reaching `k`, return `-1`. Implement iteratively using an explicit stack to avoid recursion depth issues on skewed trees, or recursively with a reference counter. Edge cases: empty tree (root == nullptr) → return `-1` immediately; `k <= 0` → return `-1`; duplicates are handled naturally because each node is visited exactly once. Time complexity is O(n) in the worst case (when `k` is the number of nodes), but on average O(h + k) where h is height if we prune early; however, for simplicity and robustness, we accept O(n) worst‑case. Space complexity is O(h) for the recursion stack or O(h) for the explicit stack.
