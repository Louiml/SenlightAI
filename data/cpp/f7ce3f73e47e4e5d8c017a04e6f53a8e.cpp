// Implement a C++ function `int findKthSmallest(const BST* root, int k)` that, given a pointer to the root of a binary search tree (BST) and an integer `k` (1‑indexed), returns the `k`‑th smallest value in the tree. The BST is defined by the class `BST` shown in the snippet (each node has `value`, `left`, `right`; left subtree contains strictly smaller values, right subtree contains values greater than or equal). You may assume the tree is non-empty and that `k` is always a valid index (1 ≤ k ≤ number of nodes). The function should not modify the tree and must work correctly even if the tree contains duplicate values. If `k` is invalid (less than 1 or greater than the number of nodes), return `-1` (the problem guarantees this case does not occur, but handle it gracefully for safety). The implementation must be iterative (no recursion), using an explicit stack, and should have time complexity O(n) and space complexity O(h), where `h` is the tree height.
// The main algorithm uses an in-order traversal of the BST. In a BST, an in-order traversal visits nodes in non-decreasing order (because left values are smaller, and right values are larger or equal). To find the `k`‑th smallest, we perform an iterative in-order traversal using a stack: start at the root and push all left children onto the stack, then pop a node, decrement `k`, and if `k` becomes zero, that node’s value is the answer. Then move to the right child and repeat the left‑push process. This correctly handles duplicates because equal values appear consecutively in in-order, and each occurrence is counted separately.
//
// Edge cases: 
// - `k` might be 1 (smallest) – works.
// - `k` equals the total number of nodes (largest) – works because the traversal visits all nodes.
// - If the tree is a degenerate chain (height = n), the stack depth reaches at most n, but that is acceptable for O(h).
// - If `k` is out of range, after the traversal finishes we return -1, but because the problem guarantees valid input, this is just safety.
//
// Time complexity is O(n) in the worst case (visiting all nodes if `k` is the largest). Space complexity is O(h) for the stack, since at any moment we store at most one path from root to a leaf.
//
// The solution function takes `const BST*` to enforce not modifying the tree, and uses `std::stack<const BST*>` to store pointers.
#include <stack>

// Returns the k-th smallest value in the BST, or -1 if k is out of range.
int findKthSmallest(const BST* root, int k) {
    if (root == nullptr || k <= 0) {
        return -1;
    }

    std::stack<const BST*> stk;
    const BST* cur = root;

    while (cur != nullptr || !stk.empty()) {
        // Push all left children onto the stack.
        while (cur != nullptr) {
            stk.push(cur);
            cur = cur->left;
        }

        // Process the top node.
        cur = stk.top();
        stk.pop();

        --k;
        if (k == 0) {
            return cur->value;
        }

        // Move to the right subtree.
        cur = cur->right;
    }

    // If we exit the loop without finding, k was too large.
    return -1;
}
#include <cassert>

int main() {
    // Build a simple BST: root=10, left=5, right=15, left's right=7, right's left=12.
    BST root(10);
    root.insert(5);
    root.insert(15);
    root.insert(7);
    root.insert(12);

    // In-order order: 5,7,10,12,15
    assert(findKthSmallest(&root, 1) == 5);
    assert(findKthSmallest(&root, 2) == 7);
    assert(findKthSmallest(&root, 3) == 10);
    assert(findKthSmallest(&root, 4) == 12);
    assert(findKthSmallest(&root, 5) == 15);

    // Test with duplicates: insert 10 again (right subtree of root since >=)
    root.insert(10);
    // Now in-order: 5,7,10,10,12,15
    assert(findKthSmallest(&root, 3) == 10);
    assert(findKthSmallest(&root, 4) == 10);

    // Test single-node tree.
    BST single(42);
    assert(findKthSmallest(&single, 1) == 42);

    // Test degenerate left chain: 3->2->1
    BST chainRoot(3);
    chainRoot.insert(2);
    chainRoot.insert(1);
    assert(findKthSmallest(&chainRoot, 1) == 1);
    assert(findKthSmallest(&chainRoot, 3) == 3);

    // Test k out of range (should not happen per spec but check).
    assert(findKthSmallest(&root, 100) == -1);
    assert(findKthSmallest(&root, 0) == -1);

    return 0;
}
