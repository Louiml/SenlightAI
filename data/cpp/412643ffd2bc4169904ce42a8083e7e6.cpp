/*
Given a pointer to the root of a complete binary tree (all levels are completely filled except possibly the last, which is filled from left to right), write a C++ function `int countCompleteTreeNodes(const Node* root)` that returns the total number of nodes in the tree, achieving an average time complexity of \(O(\log^2 n)\) by using the property that left and right subtree heights can be computed in \(O(\log n)\) each, and combining them with recursion only when necessary. The function should be `const`‑correct and handle a null root by returning 0. You may assume the input is always a complete binary tree, so you do not need to validate the structure.
*/
#include <cmath>

// Node structure for a binary tree (kept within the solution for completeness).
struct Node {
    int key;
    Node* left;
    Node* right;
    explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

// Helper: returns height of the leftmost path (number of nodes from root to deepest left leaf).
int leftHeight(const Node* root) {
    int h = 0;
    while (root) {
        ++h;
        root = root->left;
    }
    return h;
}

// Helper: returns height of the rightmost path (number of nodes from root to deepest right leaf).
int rightHeight(const Node* root) {
    int h = 0;
    while (root) {
        ++h;
        root = root->right;
    }
    return h;
}

// Count nodes in a complete binary tree in O(log^2 n) average time.
int countCompleteTreeNodes(const Node* root) {
    if (!root) return 0;

    int lh = leftHeight(root);
    int rh = rightHeight(root);

    if (lh == rh) {
        // Perfect subtree: 2^lh - 1 nodes
        return (1 << lh) - 1;
    }

    // Otherwise, count recursively on left and right, plus current node.
    return 1 + countCompleteTreeNodes(root->left) + countCompleteTreeNodes(root->right);
}
#include <cassert>

int main() {
    // Test 1: Empty tree
    assert(countCompleteTreeNodes(nullptr) == 0);

    // Test 2: Single node
    Node* n1 = new Node(1);
    assert(countCompleteTreeNodes(n1) == 1);

    // Test 3: Perfect tree of height 2 (3 nodes)
    Node* n2 = new Node(2);
    n2->left = new Node(3);
    n2->right = new Node(4);
    assert(countCompleteTreeNodes(n2) == 3);

    // Test 4: Complete but not perfect: height 3, left subtree perfect, right incomplete
    Node* n3 = new Node(5);
    n3->left = new Node(6);
    n3->right = new Node(7);
    n3->left->left = new Node(8);
    n3->left->right = new Node(9);
    n3->right->left = new Node(10);
    // Nodes: 5,6,7,8,9,10 => total 6
    assert(countCompleteTreeNodes(n3) == 6);

    // Test 5: Larger perfect tree: height 3 (7 nodes)
    Node* n4 = new Node(11);
    n4->left = new Node(12);
    n4->right = new Node(13);
    n4->left->left = new Node(14);
    n4->left->right = new Node(15);
    n4->right->left = new Node(16);
    n4->right->right = new Node(17);
    assert(countCompleteTreeNodes(n4) == 7);

    // Test 6: Complete tree with only left child at root level (2 nodes)
    Node* n5 = new Node(18);
    n5->left = new Node(19);
    assert(countCompleteTreeNodes(n5) == 2);

    // Clean up (not necessary for assert but good practice)
    delete n1; delete n2->left; delete n2->right; delete n2;
    delete n3->left->left; delete n3->left->right; delete n3->right->left;
    delete n3->left; delete n3->right; delete n3;
    delete n4->left->left; delete n4->left->right; delete n4->right->left; delete n4->right->right;
    delete n4->left; delete n4->right; delete n4;
    delete n5->left; delete n5;

    return 0;
}
// The key observation is that in a complete binary tree, for any node, if the height of the left subtree equals the height of the right subtree, then the entire subtree rooted at that node is a perfect binary tree. For a perfect subtree of height \(h\) (measured as number of nodes on the longest path from the root to a leaf), the number of nodes is exactly \(2^h - 1\). By precomputing the left‑most height (`lh`) and right‑most height (`rh`) for a given node, we can decide in \(O(\log n)\) time whether the subtree is perfect. If it is perfect, we return the closed‑form count; otherwise, we recursively count the left and right children (which are also complete) and add 1 for the current node. Because each recursive call descends only along one “imperfect” branch, and at each level the heights are computed in \(O(\log n)\), the total time per node visited is \(O(\log n)\). The recursion visits at most \(O(\log n)\) nodes (one per level of the tree), giving an overall time of \(O(\log^2 n)\). Space complexity is \(O(\log n)\) due to recursion depth. Edge case: an empty tree (null pointer) returns 0. Also, when the left and right heights differ, the tree is not perfect, so we must recurse. The use of integer powers via `std::pow` is acceptable for typical sizes, but for large trees we can use left‑shift `(1 << h) - 1` to avoid floating‑point precision issues. The solution uses `std::pow` as in the original snippet but with `const Node*` and `int` return.
