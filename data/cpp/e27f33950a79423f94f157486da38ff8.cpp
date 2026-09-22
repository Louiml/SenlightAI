Given the root of a binary tree, write a C++ function `countGoodNodes(const TreeNode* root)` that returns the number of *good* nodes. A node is considered *good* if its value is greater than or equal to the maximum value found on the path from the root to that node (inclusive of both endpoints). The tree is defined using the standard `TreeNode` struct with integer `val`, `left`, and `right` pointers. The tree may be empty (root is `nullptr`), may contain negative values, and may have duplicate values. The function must not modify the tree and should be `const`-correct. For example, in a tree where root has value 3, left child 1, right child 4, then only the root and the right child are good because 3≥3 and 4≥max(3,4)=4, while 1 is not good since 1 < max(3,1)=3.
The solution uses a depth-first search (DFS) traversal. We maintain a parameter `currentMax` representing the maximum node value encountered so far along the path from the root to the current node. At each node, we compare its `val` with `currentMax`. If `val >= currentMax`, then the node is good, so we increment the count by 1 and update `currentMax` to `val` for the recursive calls to its children; otherwise, we do not increment and keep `currentMax` unchanged. The recursion terminates at `nullptr`, returning 0. Because the maximum path value is passed down, each node is visited exactly once, leading to **O(n)** time complexity, where n is the number of nodes, and **O(h)** auxiliary space due to the recursion stack, where h is the tree height (O(n) for skewed trees, O(log n) for balanced). The initial call starts with `currentMax = INT_MIN` (from `<climits>`) to ensure the root is always considered good even if its value is the smallest possible integer. Edge cases include an empty tree (returns 0) and negative values (handled correctly by INT_MIN initialization).
#include <climits>   // for INT_MIN
#include <algorithm> // for std::max

// Definition for a binary tree node (must be provided by the user or included here)
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Helper function: returns the number of good nodes in the subtree rooted at 'node',
// given that the maximum value seen on the path from the root to this node is 'currentMax'.
int countGoodNodesHelper(const TreeNode* node, int currentMax) {
    if (node == nullptr) {
        return 0;
    }

    int count = 0;
    // If the node's value is at least as large as the current maximum along the path,
    // then this node is considered good.
    if (node->val >= currentMax) {
        count = 1;
        currentMax = node->val; // update the maximum for the path going down
    }

    return count + countGoodNodesHelper(node->left, currentMax) + countGoodNodesHelper(node->right, currentMax);
}

// Public function: counts the number of good nodes in the entire tree.
// A node is good if its value is >= the maximum value on the path from the root to that node.
int countGoodNodes(const TreeNode* root) {
    // INT_MIN ensures the root is always good even if it is the smallest possible integer.
    return countGoodNodesHelper(root, INT_MIN);
}
#include <cassert>

// The TreeNode definition and countGoodNodes are assumed to be available here.

int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    assert(countGoodNodes(empty) == 0);

    // Test 2: Single node
    TreeNode n5(5);
    assert(countGoodNodes(&n5) == 1);

    // Test 3: Simple tree: 3 (root), left=1, right=4
    TreeNode n1(1);
    TreeNode n4(4);
    TreeNode n3(3, &n1, &n4);
    assert(countGoodNodes(&n3) == 2); // root and right child are good

    // Test 4: Tree with negative values: -5 (root), left=-10, right=-3
    TreeNode neg10(-10);
    TreeNode neg3(-3);
    TreeNode neg5(-5, &neg10, &neg3);
    assert(countGoodNodes(&neg5) == 2); // root (-5) and right (-3) are good

    // Test 5: All equal values: 2 (root), left=2, right=2, left-left=2
    TreeNode eqLeaf(2);
    TreeNode eqLeft(2, &eqLeaf, nullptr);
    TreeNode eqRight(2);
    TreeNode eqRoot(2, &eqLeft, &eqRight);
    assert(countGoodNodes(&eqRoot) == 4); // all nodes are good

    // Test 6: Left-skewed descending: 10 -> 8 -> 7
    TreeNode d1(7);
    TreeNode d2(8, &d1, nullptr);
    TreeNode d3(10, &d2, nullptr);
    assert(countGoodNodes(&d3) == 1); // only root is good

    // Test 7: Right-skewed ascending: 1 -> 2 -> 3
    TreeNode a1(3);
    TreeNode a2(2, nullptr, &a1);
    TreeNode a3(1, nullptr, &a2);
    assert(countGoodNodes(&a3) == 3); // all are good

    // Test 8: Root with only one child, child has larger value
    TreeNode c1(9);
    TreeNode cRoot(4, nullptr, &c1);
    assert(countGoodNodes(&cRoot) == 2); // both good

    // Test 9: Root with only one child, child has smaller value
    TreeNode s1(2);
    TreeNode sRoot(4, &s1, nullptr);
    assert(countGoodNodes(&sRoot) == 1); // only root good

    // Test 10: Complex tree: 3(root), left subtree: 1 with right child 5, right subtree: 4 with left child 2
    TreeNode t5(5);
    TreeNode t1(1, nullptr, &t5);
    TreeNode t2(2);
    TreeNode t4(4, &t2, nullptr);
    TreeNode tRoot(3, &t1, &t4);
    // Paths: root(3) good, then left 1 not good, then 5 good, then right 4 good, then 2 not good => total 3
    assert(countGoodNodes(&tRoot) == 3);

    return 0;
}
