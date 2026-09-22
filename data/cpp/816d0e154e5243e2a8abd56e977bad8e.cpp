/*
Given a binary tree where each node stores an integer value, write a C++ function `TreeNode* replaceValueInTree(TreeNode* root)` that replaces each node's value with the sum of all values of its siblings' children (i.e., for each node, the new value equals the total sum of all values at the same depth minus the sum of values of its own parent's children). The root's value becomes 0 since it has no siblings. The function should modify the tree in place and return the root. The tree is represented using the standard `TreeNode` struct with `val`, `left`, and `right` pointers.
*/
#include <queue>
#include <vector>
#include <unordered_map>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Replace each node's value with the sum of all values at the same depth
// excluding the sum of its own parent's children. The root's new value is 0.
// Modifies the tree in place and returns the root pointer.
TreeNode* replaceValueInTree(TreeNode* root) {
    if (!root) return nullptr;

    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        std::vector<TreeNode*> nodesAtLevel;
        nodesAtLevel.reserve(levelSize);

        // Collect all nodes at current depth and compute total sum
        long long totalSum = 0;
        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front();
            q.pop();
            nodesAtLevel.push_back(node);
            totalSum += node->val;
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        // For each node, compute sum of its parent's children
        // We need a map from parent to sum of its two children values
        std::unordered_map<TreeNode*, long long> parentToChildSum;
        for (TreeNode* node : nodesAtLevel) {
            long long childSum = 0;
            if (node->left) childSum += node->left->val;
            if (node->right) childSum += node->right->val;
            parentToChildSum[node] = childSum;
        }

        // Now update each node at this level to totalSum minus parent's childSum
        // For root level (depth 0), there is no parent, so we set root to 0 (or via totalSum - totalSum since its parent's childSum = totalSum? Actually we handle root separately below)
        // But since root's parent doesn't exist, we can't use parentToChildSum. Instead, for root level we just set each node's value to totalSum - (its own value if it's root? Wait we need careful)
        // Actually: the rule says: new value = totalSum - sum of values of siblings' children.
        // For a node at depth d > 0, its siblings' children are all children of its parent.
        // So new value = totalSum - (sum of parent's children).
        // For root (depth 0), its parent doesn't exist; the set of siblings' children is empty, so new value = totalSum (which is just root's original value? But we want root to become 0). So we handle root separately.
        // But we are in a loop over all levels. At the deepest level, nodes have no children, so parentToChildSum is 0 for all parents (since we only add children if they exist, but actually those nodes are the leaves, and they have no children, so childSum=0). That's correct.
        // For root level, we set root's new value to 0. For all other levels, we update using the map.
        if (nodesAtLevel.size() == 1 && nodesAtLevel[0] == root) {
            // root level
            root->val = 0;
        } else {
            for (TreeNode* node : nodesAtLevel) {
                // We need to find this node's parent, but we don't have parent pointers.
                // Instead, we can store parent when we push children.
                // We'll modify the traversal to store parent pointers.
                // Therefore, better approach: use a map from child to parent during BFS.
            }
        }
    }
    return root;
}

Wait, the above is incomplete. Let me provide a clean correct solution:

#include <queue>
#include <vector>
#include <unordered_map>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Replace each node's value with sum of all values at its depth minus sum of its parent's children.
// The root becomes 0. Operates in-place and returns root.
TreeNode* replaceValueInTree(TreeNode* root) {
    if (!root) return nullptr;

    std::queue<TreeNode*> q;
    q.push(root);

    // Map from child node to its parent
    std::unordered_map<TreeNode*, TreeNode*> parent;

    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        std::vector<TreeNode*> nodesAtLevel;
        nodesAtLevel.reserve(levelSize);
        long long totalSum = 0;

        // Collect all nodes at current level and push children
        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front();
            q.pop();
            nodesAtLevel.push_back(node);
            totalSum += node->val;
            if (node->left) {
                parent[node->left] = node;
                q.push(node->left);
            }
            if (node->right) {
                parent[node->right] = node;
                q.push(node->right);
            }
        }

        // Compute sum of children for each parent in this level
        std::unordered_map<TreeNode*, long long> parentChildSum;
        for (TreeNode* node : nodesAtLevel) {
            long long sum = 0;
            if (node->left) sum += node->left->val;
            if (node->right) sum += node->right->val;
            parentChildSum[node] = sum;
        }

        // Update each node at this level
        if (nodesAtLevel.size() == 1 && nodesAtLevel[0] == root) {
            // Root level: no parent, so new value = totalSum - 0 = totalSum? But we want 0.
            // Actually totalSum for root level is just root's original value. Since no siblings' children, new value is totalSum - sum of parent's children (0). That gives root's original value, but we want 0. So special case: root becomes 0.
            root->val = 0;
        } else {
            for (TreeNode* node : nodesAtLevel) {
                TreeNode* p = parent[node];
                long long siblingsChildrenSum = parentChildSum[p]; // sum of p's children
                node->val = static_cast<int>(totalSum - siblingsChildrenSum);
            }
        }
    }
    return root;
}
#include <cassert>

// Helper to create a tree from level-order vector (with -1 for null)
TreeNode* buildTree(const std::vector<int>& values) {
    if (values.empty() || values[0] == -1) return nullptr;
    TreeNode* root = new TreeNode(values[0]);
    std::queue<TreeNode*> q;
    q.push(root);
    int idx = 1;
    while (!q.empty() && idx < static_cast<int>(values.size())) {
        TreeNode* node = q.front();
        q.pop();
        if (values[idx] != -1) {
            node->left = new TreeNode(values[idx]);
            q.push(node->left);
        }
        idx++;
        if (idx < static_cast<int>(values.size()) && values[idx] != -1) {
            node->right = new TreeNode(values[idx]);
            q.push(node->right);
        }
        idx++;
    }
    return root;
}

// Helper to print tree in level-order (for debugging)
std::vector<int> treeToLevelOrder(TreeNode* root) {
    if (!root) return {};
    std::vector<int> result;
    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        if (node) {
            result.push_back(node->val);
            q.push(node->left);
            q.push(node->right);
        } else {
            result.push_back(-1);
        }
    }
    // Trim trailing -1s
    while (!result.empty() && result.back() == -1) result.pop_back();
    return result;
}

int main() {
    // Test 1: Single node
    TreeNode* t1 = new TreeNode(5);
    replaceValueInTree(t1);
    assert(treeToLevelOrder(t1) == std::vector<int>{0});
    delete t1;

    // Test 2: Root with two children
    TreeNode* t2 = buildTree({5, 3, 2});
    replaceValueInTree(t2);
    assert(treeToLevelOrder(t2) == std::vector<int>{0, 0, 0});
    // Clean up
    // (skipping full delete for brevity)

    // Test 3: Three levels
    // Original:     5
    //              / \
    //             3   2
    //            / \   \
    //           1   4   6
    // Depth 0: total=5 -> root=0
    // Depth 1: total=5 -> each child becomes total - parentChildSum(root)=5-5=0
    // Depth 2: total=11 -> for node 1: parent(3)'s children sum=1+4=5 -> new=11-5=6
    //          node 4: same parent=3 -> 6
    //          node 6: parent(2)'s children sum=6 -> 11-6=5
    TreeNode* t3 = buildTree({5, 3, 2, 1, 4, -1, 6});
    replaceValueInTree(t3);
    assert(treeToLevelOrder(t3) == std::vector<int>{0, 0, 0, 6, 6, 5});
    // Note: the level-order output after trimming will be {0,0,0,6,6,5}

    // Test 4: Unbalanced tree
    // Original:  1
    //            \
    //             2
    //              \
    //               3
    TreeNode* t4 = buildTree({1, -1, 2, -1, -1, -1, 3});
    replaceValueInTree(t4);
    // Depth 0: 1->0
    // Depth 1: total=2, parent(root) children sum=2 -> 2-2=0
    // Depth 2: total=3, parent(2) children sum=3 -> 3-3=0
    assert(treeToLevelOrder(t4) == std::vector<int>{0, 0, 0});

    // Test 5: Only right child at depth 1 and multiple leaves
    // Original:  10
    //            / \
    //           5   15
    //          /     \
    //         3       20
    TreeNode* t5 = buildTree({10, 5, 15, 3, -1, -1, 20});
    replaceValueInTree(t5);
    // Depth0: 10->0
    // Depth1: total=20, root children sum=20 -> each becomes 0
    // Depth2: total=23, parent(5) children sum=3, parent(15) children sum=20
    //  node3: 23-3=20
    //  node20: 23-20=3
    assert(treeToLevelOrder(t5) == std::vector<int>{0, 0, 0, 20, 3});

    return 0;
}
// The solution performs a level-order traversal (BFS) to process nodes depth by depth. For each depth, we collect all nodes at that level into a vector and record each child's parent. Then, for each parent, we calculate the sum of values of its direct children (left and right if they exist). After processing all parents at a given depth, we compute the total sum of all node values at that depth. Then, for each node at that depth, we set its value to the total sum minus the sum of its own parent's children (since siblings' children are all children of the same parent). The root is handled separately: its depth-0 total sum is just its own value, and the sum of its parent's children is 0 (no parent), so it becomes 0. This process repeats for each depth from top to bottom. Edge cases include a single-node tree (root becomes 0), a tree with only root and children, and missing left/right children (treat missing child's value as 0). Time complexity is O(n) where n is the number of nodes (each node visited once during BFS and once during replacement). Space complexity is O(n) for the queue and the per-depth vector.
