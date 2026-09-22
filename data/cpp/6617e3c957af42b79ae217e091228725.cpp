Write a C++ function `std::vector<int> collectLevelOrder(const std::vector<int>& input)` that takes a non-empty vector of integers representing values to be inserted into a complete binary tree using level-order insertion (i.e., each new node is placed at the first available position from top to bottom, left to right). The function should return a new vector containing the values of that binary tree traversed in level order (breadth-first search), where each level's nodes are output from left to right. If the input is empty, return an empty vector. The function must preserve the original insertion order for the level-order output — for example, inserting `[1,2,3,4]` creates a tree with root 1, left child 2, right child 3, and left child of 2 as 4, producing output `[1,2,3,4]`. Edge cases include single element, duplicates, and very large inputs — ensure the solution is time-efficient.

The straightforward approach is to build a binary tree using level-order insertion, then perform a BFS traversal to collect values. To build the tree, maintain a queue of nodes, starting with the root. For each new value, pop the front node; if its left child is null, attach the new node there; otherwise, if its right child is null, attach there; otherwise, push both children and repeat. This ensures each insertion finds the correct parent. After building, BFS using a queue: push root, then while queue not empty, pop front, record its value, push its left and right children if they exist. This yields the desired level-order output, which is exactly the input order because level-order insertion ensures the tree is "complete" and level-order traversal of a complete tree yields the same order as the original insertion sequence. Edge cases: empty input returns empty vector; single element returns same; duplicates are handled naturally. Time complexity: O(n) for insertion (each node visited once) and O(n) for BFS, so O(n). Space: O(n) for the tree and queues.

#include <vector>
#include <queue>
#include <memory>

// Node structure for the binary tree.
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Build a complete binary tree using level-order insertion and return its level-order traversal.
std::vector<int> collectLevelOrder(const std::vector<int>& input) {
    if (input.empty()) return {};
    
    // Create root from first value.
    TreeNode* root = new TreeNode(input[0]);
    std::queue<TreeNode*> buildQueue;
    buildQueue.push(root);
    
    // Insert remaining values level-order.
    for (size_t i = 1; i < input.size(); ++i) {
        TreeNode* parent = buildQueue.front();
        TreeNode* newNode = new TreeNode(input[i]);
        
        if (parent->left == nullptr) {
            parent->left = newNode;
        } else if (parent->right == nullptr) {
            parent->right = newNode;
        } else {
            // Both children exist, pop parent and push children for future.
            buildQueue.pop();
            buildQueue.push(parent->left);
            buildQueue.push(parent->right);
            parent = buildQueue.front();
            if (parent->left == nullptr) {
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }
        }
        buildQueue.push(newNode);
    }
    
    // Perform BFS to collect values in level order.
    std::vector<int> result;
    std::queue<TreeNode*> bfsQueue;
    bfsQueue.push(root);
    while (!bfsQueue.empty()) {
        TreeNode* curr = bfsQueue.front();
        bfsQueue.pop();
        result.push_back(curr->val);
        if (curr->left) bfsQueue.push(curr->left);
        if (curr->right) bfsQueue.push(curr->right);
    }
    
    // Clean up memory (not strictly necessary for small tests, but good practice).
    // Since we don't have a destructor, we'll do a simple BFS delete.
    std::queue<TreeNode*> cleanup;
    cleanup.push(root);
    while (!cleanup.empty()) {
        TreeNode* t = cleanup.front();
        cleanup.pop();
        if (t->left) cleanup.push(t->left);
        if (t->right) cleanup.push(t->right);
        delete t;
    }
    
    return result;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Basic cases
    assert(collectLevelOrder({1, 2, 3, 4}) == std::vector<int>({1, 2, 3, 4}));
    assert(collectLevelOrder({5}) == std::vector<int>({5}));
    assert(collectLevelOrder({}) == std::vector<int>());
    
    // Duplicate values
    assert(collectLevelOrder({7, 7, 7}) == std::vector<int>({7, 7, 7}));
    
    // Larger complete tree
    std::vector<int> vals = {10, 20, 30, 40, 50, 60, 70};
    assert(collectLevelOrder(vals) == vals);
    
    // Non-power-of-two levels
    std::vector<int> vals2 = {1, 2, 3, 4, 5};
    assert(collectLevelOrder(vals2) == vals2);
    
    // Negative and zero values
    assert(collectLevelOrder({-1, 0, -3, 4}) == std::vector<int>({-1, 0, -3, 4}));
    
    // Large input to ensure no crash
    std::vector<int> big(1000);
    for (int i = 0; i < 1000; ++i) big[i] = i;
    assert(collectLevelOrder(big) == big);
    
    return 0;
}
