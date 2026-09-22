// Write a C++ function `bool isMaxHeapTree(const Node* root)` that determines whether a given binary tree satisfies both the **complete binary tree** property (all levels filled except possibly the last, which is filled from left to right) and the **max-heap** property (every node's value is greater than or equal to the values of its children). The function should work on an arbitrary binary tree, not just a heap, and return `true` only if both conditions hold. You may assume the tree is non-empty. Implement the tree node structure as `struct Node` with integer `data` and pointers to left and right children (initialized to `nullptr`). Handle edge cases such as a single node (which is both complete and a max-heap), a tree with missing left child but present right child (which violates completeness), and trees where a parent is smaller than a child.
#include <cassert>

int main() {
    // Test 1: Single node is both complete and a max-heap.
    Node* single = new Node(42);
    assert(isMaxHeapTree(single) == true);

    // Test 2: Complete and correct max-heap (example from prompt, custom).
    Node* heap = new Node(10);
    heap->left = new Node(9);
    heap->right = new Node(8);
    heap->left->left = new Node(7);
    heap->left->right = new Node(6);
    assert(isMaxHeapTree(heap) == true);

    // Test 3: Not a max-heap because parent < child.
    Node* notHeap = new Node(5);
    notHeap->left = new Node(10);
    assert(isMaxHeapTree(notHeap) == false);

    // Test 4: Not complete (right child without left child).
    Node* incomplete = new Node(1);
    incomplete->right = new Node(2);
    assert(isMaxHeapTree(incomplete) == false);

    // Test 5: Complete but missing right child at last level is allowed (single left child).
    Node* completeWithLeftOnly = new Node(10);
    completeWithLeftOnly->left = new Node(9);
    assert(isMaxHeapTree(completeWithLeftOnly) == true);

    // Test 6: Complete but heap property fails deeper in the tree.
    Node* deepFail = new Node(50);
    deepFail->left = new Node(40);
    deepFail->right = new Node(30);
    deepFail->left->left = new Node(35); // 40 < 35? Actually 35 < 40, fine
    deepFail->left->right = new Node(45); // 40 < 45 → fails
    assert(isMaxHeapTree(deepFail) == false);

    // Test 7: Empty tree (nullptr) returns true by definition.
    assert(isMaxHeapTree(nullptr) == true);

    // Test 8: Larger valid heap with many levels.
    Node* bigHeap = new Node(100);
    bigHeap->left = new Node(90);
    bigHeap->right = new Node(80);
    bigHeap->left->left = new Node(70);
    bigHeap->left->right = new Node(60);
    bigHeap->right->left = new Node(50);
    bigHeap->right->right = new Node(40);
    bigHeap->left->left->left = new Node(30);
    bigHeap->left->left->right = new Node(20);
    assert(isMaxHeapTree(bigHeap) == true);

    // Test 9: Complete but right subtree has a larger child than parent.
    Node* rightViolation = new Node(15);
    rightViolation->left = new Node(12);
    rightViolation->right = new Node(18); // violates heap
    assert(isMaxHeapTree(rightViolation) == false);

    // Test 10: Skewed left tree is complete only if full levels? This one is not full at last level but left-only deeper is okay.
    Node* leftSkew = new Node(10);
    leftSkew->left = new Node(9);
    leftSkew->left->left = new Node(8);
    assert(isMaxHeapTree(leftSkew) == true); // complete (left only at last level) and heap ok

    // Cleanup is omitted for brevity in tests, but in practice you would delete nodes.
}
#include <queue>
#include <cstddef>

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper to count total number of nodes in the tree.
static int countNodes(const Node* root) {
    if (!root) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// Check if the tree is a complete binary tree using index-based recursion.
static bool isCompleteBinaryTree(const Node* root, int index, int totalNodes) {
    if (!root) return true;

    if (index >= totalNodes) return false;

    return isCompleteBinaryTree(root->left, 2 * index + 1, totalNodes) &&
           isCompleteBinaryTree(root->right, 2 * index + 2, totalNodes);
}

// Check if the tree satisfies the max-heap property (parent >= children).
static bool isMaxHeapProperty(const Node* root) {
    if (!root || (!root->left && !root->right)) return true;

    if (root->left && root->data < root->left->data) return false;
    if (root->right && root->data < root->right->data) return false;

    return isMaxHeapProperty(root->left) && isMaxHeapProperty(root->right);
}

// Public function to check whether the given binary tree is a max-heap.
bool isMaxHeapTree(const Node* root) {
    if (!root) return true;
    int total = countNodes(root);
    return isCompleteBinaryTree(root, 0, total) && isMaxHeapProperty(root);
}
// The solution requires two independent checks that must both pass.  
// **1. Completeness check (isCBT):** Count the total number of nodes first (via a level-order traversal or recursion). Then perform a recursive traversal starting at index `0` for the root. For each node at index `i`, its left child would be at index `2*i+1` and right child at `2*i+2` (0-based indexing for a complete binary tree). If any node is `nullptr` at an index that is less than the total node count, the tree is not complete. Recursively verify both subtrees.  
// **2. Max-heap property check (isMaxHeap):** For every node, ensure it is not smaller than any existing child. If the right child exists and the parent is smaller than the right child, fail. Similarly for the left child. Then recursively check both subtrees.  
// **Edge cases:**  
// - A `nullptr` tree is vacuously complete and a max-heap (return `true`).  
// - A single node: no children, so both checks pass.  
// - Leaves are always valid.  
// - A tree with only a right child and missing left child fails completeness (because the node count index will exceed, or index of right child is not allowed if left is missing).  
// - A tree where left child exists but right is missing may still be complete (since last level fills left to right), but this only holds if the missing right child is at the bottom level and all preceding levels are full—our index-based check handles this automatically.  
// **Complexity:** Counting nodes takes `O(n)` time and `O(w)` space for the queue where `w` is the maximum width (in worst case `O(n)`). The completeness and heap checks each run in `O(n)` time and `O(h)` recursion stack space (`h` is height, worst-case `O(n)` for skewed trees). Overall time `O(n)`, space `O(n)` in worst case.
