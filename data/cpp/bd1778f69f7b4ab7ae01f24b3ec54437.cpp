Write a C++ function named `averageOfLevels` that takes a binary tree root (as a `TreeNode*` with integer values) and returns a `std::vector<double>` where each element corresponds to the average value of all nodes at that depth level, in order from root to deepest level. The function must handle an empty tree (return empty vector) and trees with up to 10,000 nodes, with node values ranging from -2^31 to 2^31-1, so you must avoid integer overflow when summing level values. Implement the function as a free function (not a class method) that accepts `const TreeNode* root` to demonstrate const-correctness.
The solution uses a breadth-first search (BFS) approach with a queue to process nodes level by level. For each level, we record the number of nodes in the queue at that moment (this represents the level size). We then pop all nodes of that level, summing their values, and enqueue their non-null children for the next level. After processing the entire level, we compute the average as `(double)sum / size` and append it to the result vector. Edge cases include: an empty tree (return empty vector immediately), a single-node tree (returns {value}), and values that may sum to large magnitudes—we use `long` for the sum to avoid overflow. Time complexity is O(n) where n is the number of nodes, since each node is visited exactly once. Space complexity is O(m) where m is the maximum width of the tree (the maximum number of nodes at any level), which is the maximum size of the queue.
#include <vector>
#include <queue>

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Returns the average value of nodes at each level of the binary tree.
std::vector<double> averageOfLevels(const TreeNode* root) {
    std::vector<double> result;
    if (root == nullptr) {
        return result;
    }
    
    std::queue<const TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        int levelSize = q.size();
        long long sum = 0;  // Use long long to avoid overflow for large levels
        for (int i = 0; i < levelSize; ++i) {
            const TreeNode* current = q.front();
            q.pop();
            sum += current->val;
            if (current->left != nullptr) {
                q.push(current->left);
            }
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }
        result.push_back(static_cast<double>(sum) / levelSize);
    }
    return result;
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Empty tree
    {
        TreeNode* root = nullptr;
        std::vector<double> res = averageOfLevels(root);
        assert(res.empty());
    }
    
    // Test 2: Single node
    {
        TreeNode* root = new TreeNode(5);
        std::vector<double> res = averageOfLevels(root);
        assert(res.size() == 1 && std::abs(res[0] - 5.0) < 1e-9);
        delete root;
    }
    
    // Test 3: Balanced tree: root=3, left=9, right=20, right->left=15, right->right=7
    {
        TreeNode* root = new TreeNode(3);
        root->left = new TreeNode(9);
        root->right = new TreeNode(20);
        root->right->left = new TreeNode(15);
        root->right->right = new TreeNode(7);
        std::vector<double> res = averageOfLevels(root);
        assert(res.size() == 3);
        assert(std::abs(res[0] - 3.0) < 1e-9);
        assert(std::abs(res[1] - 14.5) < 1e-9);
        assert(std::abs(res[2] - 11.0) < 1e-9);
        // Cleanup
        delete root->right->left;
        delete root->right->right;
        delete root->right;
        delete root->left;
        delete root;
    }
    
    // Test 4: Skewed left tree: 1-2-3
    {
        TreeNode* root = new TreeNode(1);
        root->left = new TreeNode(2);
        root->left->left = new TreeNode(3);
        std::vector<double> res = averageOfLevels(root);
        assert(res.size() == 3);
        assert(std::abs(res[0] - 1.0) < 1e-9);
        assert(std::abs(res[1] - 2.0) < 1e-9);
        assert(std::abs(res[2] - 3.0) < 1e-9);
        // Cleanup
        delete root->left->left;
        delete root->left;
        delete root;
    }
    
    // Test 5: Large values to check overflow handling
    {
        TreeNode* root = new TreeNode(2147483647);
        root->left = new TreeNode(2147483647);
        root->right = new TreeNode(2147483647);
        std::vector<double> res = averageOfLevels(root);
        assert(res.size() == 2);
        assert(std::abs(res[0] - 2147483647.0) < 1e-9);
        assert(std::abs(res[1] - 2147483647.0) < 1e-9);
        // Cleanup
        delete root->left;
        delete root->right;
        delete root;
    }
    
    // Test 6: Negative values
    {
        TreeNode* root = new TreeNode(-10);
        root->left = new TreeNode(-20);
        root->right = new TreeNode(-30);
        std::vector<double> res = averageOfLevels(root);
        assert(res.size() == 2);
        assert(std::abs(res[0] - (-10.0)) < 1e-9);
        assert(std::abs(res[1] - (-25.0)) < 1e-9);
        // Cleanup
        delete root->left;
        delete root->right;
        delete root;
    }
    
    return 0;
}
