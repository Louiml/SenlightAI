Write a C++ function `findBottomLeftValue` that takes a pointer to the root of a non-empty binary tree (where each node has an integer `val`, and `left`/`right` child pointers, possibly `nullptr`) and returns the value of the leftmost node on the last (deepest) level of the tree. The tree may be unbalanced (e.g., a skewed chain) or have only one node. Your function must perform a level-order (breadth-first) traversal, but instead of storing all levels fully, you should track only the first value of each level, and after the traversal completes return the first value of the last visited level. The function signature should be `int findBottomLeftValue(TreeNode* root)` and must be declared as `const`-correct where appropriate (i.e., you may mark the parameter as `const TreeNode*` if you do not modify the tree). You may define a helper function for the traversal if desired. Do not include `main` in the solution; only the function(s).
// The core idea is to perform a level-order traversal using a queue. For each level, we need to capture the value of the first node encountered on that level, because that is the leftmost node of that level. We can process the tree level by level: while the queue is not empty, record the current queue size (number of nodes in this level), then pop that many nodes, pushing their non-null children for the next level. The first node popped in each level gives its leftmost value. We keep updating a variable `leftmost` with the first node's value for every level. After the queue becomes empty, `leftmost` will contain the value from the last processed level, which is exactly the deepest level. Edge cases: (1) empty tree is not allowed per problem (root is guaranteed non-null), but we handle it defensively by returning a sentinel or checking; (2) a single-node tree: the first (and only) level's leftmost value is that node's value; (3) unbalanced trees: the queue naturally handles any shape because we only enqueue existing children; (4) if all nodes on the last level are right children and no left children, the leftmost is still taken from the first node in that level (which might be a right child if there are no left ones on that level—but since we process nodes in left-to-right order at each level, the first one is indeed the leftmost). Time complexity: O(n) where n is the number of nodes, since each node is visited once and pushed/popped once. Space complexity: O(w) where w is the maximum width of the tree (the maximum number of nodes in a level), because the queue holds at most one level's nodes at a time.
#include <queue>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Return the value of the leftmost node on the deepest level of the tree.
int findBottomLeftValue(const TreeNode* root) {
    if (root == nullptr) {
        // Problem assumes non-empty, but handle defensively.
        return -1; // or throw, but better to assume valid input.
    }

    std::queue<const TreeNode*> q;
    q.push(root);
    int leftmost = root->val;

    while (!q.empty()) {
        int levelSize = q.size();
        // Record the first node of this level (leftmost).
        leftmost = q.front()->val;
        for (int i = 0; i < levelSize; ++i) {
            const TreeNode* current = q.front();
            q.pop();
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }
    return leftmost;
}
#include <cassert>
#include <memory>

// Definition of TreeNode (assume it is already provided from the solution).
// For testing, we include the same struct definition here (in a real test, it would be included once).
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Helper to build a tree from a vector (level order, -1 for null) – for testing convenience.
TreeNode* buildTree(const std::vector<int>& values) {
    if (values.empty() || values[0] == -1) return nullptr;
    TreeNode* root = new TreeNode(values[0]);
    std::queue<TreeNode*> q;
    q.push(root);
    int i = 1;
    while (!q.empty() && i < (int)values.size()) {
        TreeNode* current = q.front();
        q.pop();
        if (values[i] != -1) {
            current->left = new TreeNode(values[i]);
            q.push(current->left);
        }
        ++i;
        if (i < (int)values.size() && values[i] != -1) {
            current->right = new TreeNode(values[i]);
            q.push(current->right);
        }
        ++i;
    }
    return root;
}

// Helper to delete tree and prevent memory leaks in tests.
void deleteTree(TreeNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    // Test 1: Single node.
    TreeNode* t1 = new TreeNode(5);
    assert(findBottomLeftValue(t1) == 5);
    delete t1;

    // Test 2: Full balanced tree of height 2: root=1, left child=2, right child=3.
    // Levels: [1] then [2,3]. Leftmost of last level is 2.
    TreeNode* t2 = buildTree({1, 2, 3});
    assert(findBottomLeftValue(t2) == 2);
    deleteTree(t2);

    // Test 3: Unbalanced, deeper right side.
    // Tree: 1 -> right 2 -> right 3. Last level has only [3], leftmost is 3.
    TreeNode* t3 = buildTree({1, -1, 2, -1, -1, -1, 3});
    // Note: buildTree with vector {1,-1,2,-1,-1,-1,3} is tricky; better to manually build.
    // Manually build chain: 1->2->3 (all right children).
    TreeNode* t3b = new TreeNode(1);
    t3b->right = new TreeNode(2);
    t3b->right->right = new TreeNode(3);
    assert(findBottomLeftValue(t3b) == 3);
    deleteTree(t3b);

    // Test 4: Deeper left side: 1->2->4 and 1->3, where 4 is leftmost at deepest level.
    // Tree: root=1, left=2 (left=4), right=3. Levels: [1], [2,3], [4]. Leftmost is 4.
    TreeNode* t4 = new TreeNode(1);
    t4->left = new TreeNode(2);
    t4->right = new TreeNode(3);
    t4->left->left = new TreeNode(4);
    assert(findBottomLeftValue(t4) == 4);
    deleteTree(t4);

    // Test 5: Larger tree where deepest level has multiple nodes; leftmost is the first in that level.
    // root=1, left=2 (left=4, right=5), right=3 (left=6, right=7). Deepest level [4,5,6,7], leftmost=4.
    TreeNode* t5 = new TreeNode(1);
    t5->left = new TreeNode(2);
    t5->right = new TreeNode(3);
    t5->left->left = new TreeNode(4);
    t5->left->right = new TreeNode(5);
    t5->right->left = new TreeNode(6);
    t5->right->right = new TreeNode(7);
    assert(findBottomLeftValue(t5) == 4);
    deleteTree(t5);

    // Test 6: Deepest level has only right children (no left children).
    // root=1, left=2 (right=4), right=3 (right=5). Levels: [1], [2,3], [4,5] (since 2's left is null, 4 is leftmost? Actually level order: first 2 then 3, then 4 (left of 2? no it's right) and 5. Leftmost of level [4,5] is 4 because 4 appears before 5 in level order.)
    TreeNode* t6 = new TreeNode(1);
    t6->left = new TreeNode(2);
    t6->right = new TreeNode(3);
    t6->left->right = new TreeNode(4);
    t6->right->right = new TreeNode(5);
    assert(findBottomLeftValue(t6) == 4);
    deleteTree(t6);

    return 0;
}
