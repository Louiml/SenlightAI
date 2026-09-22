/*
Write a standalone C++ function that computes the width of a binary search tree, where width is defined as the largest number of nodes at any single level (depth). The function should accept a pointer to the root node of the tree (where each node has integer data and left/right child pointers), be const-correct (not modify the tree), and return an integer. The tree may be empty, in which case the width is 0. For example, a tree with root 5, left child 3, right child 8, and a right child of 8 being 12 (and 12's left child 9) has widths: level 0 = 1 node, level 1 = 2 nodes, level 2 = 1 node, level 3 = 1 node, so width = 2. Handle trees where all nodes are on one side (e.g., a skewed tree) and trees with no nodes.
*/
#include <queue>

// Definition for a binary tree node.
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Compute the width of a binary search tree: the maximum number of nodes
// at any single level. Returns 0 for an empty tree.
int treeWidth(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }

    std::queue<const TreeNode*> q;
    q.push(root);
    int maxWidth = 0;

    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        if (levelSize > maxWidth) {
            maxWidth = levelSize;
        }

        // Process all nodes at the current level.
        for (int i = 0; i < levelSize; ++i) {
            const TreeNode* current = q.front();
            q.pop();
            if (current->left != nullptr) {
                q.push(current->left);
            }
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }
    }

    return maxWidth;
}
#include <cassert>
#include <queue>

// Definition for a binary tree node (same as in solution).
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// The solution function (copied here for testing).
int treeWidth(const TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    std::queue<const TreeNode*> q;
    q.push(root);
    int maxWidth = 0;
    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        if (levelSize > maxWidth) {
            maxWidth = levelSize;
        }
        for (int i = 0; i < levelSize; ++i) {
            const TreeNode* current = q.front();
            q.pop();
            if (current->left != nullptr) {
                q.push(current->left);
            }
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }
    }
    return maxWidth;
}

int main() {
    // Test 1: Empty tree.
    assert(treeWidth(nullptr) == 0);

    // Test 2: Single node tree.
    TreeNode n1(5);
    assert(treeWidth(&n1) == 1);

    // Test 3: Balanced tree with width 2 (root with two children).
    TreeNode n2(3);
    TreeNode n3(8);
    TreeNode root(5);
    root.left = &n2;
    root.right = &n3;
    assert(treeWidth(&root) == 2);

    // Test 4: Skewed tree (all left children) - width 1.
    TreeNode a(1);
    TreeNode b(2);
    TreeNode c(3);
    a.right = &b; // 1 -> 2 -> 3 (right chain)
    b.right = &c;
    assert(treeWidth(&a) == 1);

    // Test 5: More complex tree: root 5, left 3, right 8, right child 12, 12's left 9.
    TreeNode t1(5);
    TreeNode t2(3);
    TreeNode t3(8);
    TreeNode t4(12);
    TreeNode t5(9);
    t1.left = &t2;
    t1.right = &t3;
    t3.right = &t4;
    t4.left = &t5;
    assert(treeWidth(&t1) == 2); // Level 1 has two nodes (3 and 8).

    // Test 6: Perfect tree of height 2 (root + two children + four grandchildren) - width 4.
    TreeNode p1(1), p2(2), p3(3), p4(4), p5(5), p6(6), p7(7);
    TreeNode proot(0);
    proot.left = &p1; proot.right = &p2;
    p1.left = &p3; p1.right = &p4;
    p2.left = &p5; p2.right = &p6;
    assert(treeWidth(&proot) == 4);

    return 0;
}
// The solution uses a level-order traversal (BFS) with a queue to count nodes at each depth. Initialize the queue with the root (if non-null), and maintain a variable `maxWidth` starting at 0. While the queue is not empty, record the current queue size, which equals the number of nodes at the current level. Compare this size to `maxWidth` and update if larger. Then dequeue all nodes at that level and enqueue their non-null children. This process continues until all levels are processed. Edge cases: empty tree (root = nullptr) returns 0 immediately; a single node has width 1; a skewed tree has a maximum width of 1 at every level, so the answer is 1 (or 0 for empty). Time complexity is O(n) where n is the number of nodes, since each node is visited exactly once. Space complexity is O(w) where w is the maximum width of the tree, due to the queue storing at most one level at a time; in the worst case (a perfect tree), w ≈ n/2, so O(n) auxiliary space.
