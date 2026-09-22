// Write a C++ function that takes the root of a binary tree (where each node has integer values, left and right child pointers, and may be null) and returns the minimum depth of the tree. The minimum depth is defined as the number of nodes along the shortest path from the root node down to the nearest leaf node. A leaf is a node with no children. If the tree is empty (root is null), return 0. The function should be efficient and handle trees with up to 10^5 nodes. You may assume the tree is a standard binary tree and you have access to a `TreeNode` structure with members `int val; TreeNode *left; TreeNode *right;` and a constructor initializing them. Implement the function with a descriptive name such as `minimumDepth` and ensure it is const-correct where appropriate.
The problem asks for the shortest path from root to any leaf. A breadth-first search (BFS) is ideal because it explores nodes level by level, and the first leaf encountered will be at the minimum depth. We use a queue to store nodes along with their depth, or alternatively process level by level. Start by pushing the root with depth 1. While the queue is not empty, pop the front node. If it is a leaf (both children null), return its depth. Otherwise, push its non-null children with depth+1. This guarantees we find the nearest leaf early. Edge cases: empty tree (return 0); a single node (root is a leaf, return 1); trees where the shortest path goes through one child missing at some levels. Time complexity is O(n) because each node is visited once in the worst case (when traversing the entire tree if the leaf is deep). Space complexity is O(w) where w is the maximum width of the tree, which in the worst case (a complete tree) is O(n/2) ~ O(n). More precisely, BFS queue can hold up to the number of nodes at the deepest level before finding a leaf; for a worst-case skewed tree, the queue holds at most 1 node at a time, so O(1) in that case, but overall O(n) in the worst case.
#include <queue>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Returns the minimum depth of a binary tree.
// An empty tree has depth 0. The depth is the number of nodes
// along the shortest path from the root to a leaf.
int minimumDepth(const TreeNode* root) {
    if (root == nullptr) return 0;
    
    std::queue<const TreeNode*> q;
    q.push(root);
    int depth = 1;
    
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            const TreeNode* node = q.front();
            q.pop();
            
            // If it's a leaf, we found the minimum depth.
            if (node->left == nullptr && node->right == nullptr) {
                return depth;
            }
            
            if (node->left != nullptr) q.push(node->left);
            if (node->right != nullptr) q.push(node->right);
        }
        ++depth;
    }
    
    return depth; // Should never reach here, but for safety.
}
#include <cassert>

int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    assert(minimumDepth(empty) == 0);

    // Test 2: Single node (leaf)
    TreeNode* single = new TreeNode(5);
    assert(minimumDepth(single) == 1);
    delete single;

    // Test 3: Left-skewed tree: 1-2-3-4
    TreeNode* root3 = new TreeNode(1);
    root3->left = new TreeNode(2);
    root3->left->left = new TreeNode(3);
    root3->left->left->left = new TreeNode(4);
    assert(minimumDepth(root3) == 4); // Only path goes to depth 4
    // Clean up
    delete root3->left->left->left;
    delete root3->left->left;
    delete root3->left;
    delete root3;

    // Test 4: Balanced tree: root with two children, left child has a leaf, right child has a deep path
    TreeNode* root4 = new TreeNode(1);
    root4->left = new TreeNode(2);
    root4->right = new TreeNode(3);
    root4->left->left = new TreeNode(4);
    root4->left->right = new TreeNode(5);
    root4->right->left = new TreeNode(6);
    root4->right->left->left = new TreeNode(7);
    // Leaf at depth 2: node 4 or 5 or 6? Actually node 4 and 5 are leaves (depth 3), node 6 has a child, node 7 is leaf (depth 4). The minimum depth is 2? No, wait: depth root=1, left=2, right=3. Left child has both leaves at depth 3? Actually left node (2) has children 4 and 5, both leaves at depth 3 (root depth 1, node 2 depth 2, leaf depth 3). Right node (3) has left child 6 (depth 3) which has leaf 7 (depth 4). So minimum depth is 3 (path 1-2-4 or 1-2-5). Let's verify.
    assert(minimumDepth(root4) == 3);
    // Clean up
    delete root4->left->left;
    delete root4->left->right;
    delete root4->left;
    delete root4->right->left->left;
    delete root4->right->left;
    delete root4->right;
    delete root4;

    // Test 5: Tree where one child is null at depth 2, other branch deep
    TreeNode* root5 = new TreeNode(1);
    root5->left = new TreeNode(2);
    root5->right = new TreeNode(3);
    root5->right->right = new TreeNode(4);
    // Left child 2 is a leaf at depth 2, right branch goes deeper. Minimum depth is 2.
    assert(minimumDepth(root5) == 2);
    delete root5->left;
    delete root5->right->right;
    delete root5->right;
    delete root5;

    // Test 6: Root with only right child, deep
    TreeNode* root6 = new TreeNode(1);
    root6->right = new TreeNode(2);
    root6->right->right = new TreeNode(3);
    assert(minimumDepth(root6) == 3);
    delete root6->right->right;
    delete root6->right;
    delete root6;
}
