// Write a C++ function that converts a Binary Search Tree (BST) into a Greater Tree, where every node's original value is replaced with the sum of its original value plus the values of all nodes greater than it in the entire tree. The function should take the root of the BST as input and return the modified root. The BST may be empty, and node values can be any integers (positive, negative, or zero). The original tree must be modified in-place; creating a new tree is not allowed. The function should handle trees of any size, including skewed trees (all nodes on one side).

// The core observation is that in a BST, an in-order traversal yields nodes in ascending order. To accumulate the sum of all greater-valued nodes for each node, we need to process nodes in descending order. This is achieved by performing a reverse in-order traversal: visit the right subtree first, then the current node, then the left subtree. During this traversal, we maintain a running sum that starts at 0. For each visited node, we add its original value to the running sum, then assign the updated running sum as the node's new value. This works because when we visit a node in reverse in-order, we have already processed all nodes with greater values (the entire right subtree has already been processed, contributing their original values to the running sum). Edge cases include an empty tree (return nullptr immediately), a tree with a single node (that node becomes its own original value since no greater nodes exist), and negative values (the running sum can decrease at first, but the algorithm remains correct). Time complexity is O(n) where n is the number of nodes, as each node is visited exactly once. Space complexity is O(h) for the recursion stack, where h is the tree height (O(n) for skewed trees, O(log n) for balanced trees).

#include <functional>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Convert BST to a Greater Tree by replacing each node's value with the sum
// of its original value plus all values of nodes greater than it in the tree.
// Returns the modified root (same as input root). Assumes valid BST.
TreeNode* bstToGreaterTree(TreeNode* root) {
    if (root == nullptr) return nullptr;
    
    int runningSum = 0;
    std::function<void(TreeNode*)> traverse = [&](TreeNode* node) {
        if (node == nullptr) return;
        // Visit right subtree first (larger values)
        traverse(node->right);
        // Update current node: add its original value to running sum
        runningSum += node->val;
        node->val = runningSum;
        // Then visit left subtree (smaller values)
        traverse(node->left);
    };
    
    traverse(root);
    return root;
}

#include <cassert>

// Helper to check BST property
bool isBST(TreeNode* root, int minVal, int maxVal) {
    if (root == nullptr) return true;
    if (root->val < minVal || root->val > maxVal) return false;
    return isBST(root->left, minVal, root->val) && 
           isBST(root->right, root->val, maxVal);
}

// Helper to compute expected Greater Tree value for a node (sum of all nodes >= its value)
int expectedValue(TreeNode* root, int target) {
    if (root == nullptr) return 0;
    int sum = 0;
    if (root->val >= target) sum += root->val;
    sum += expectedValue(root->left, target);
    sum += expectedValue(root->right, target);
    return sum;
}

// Helper to verify the Greater Tree property
bool verifyGreaterTree(TreeNode* root, const std::vector<int>& inorder) {
    if (root == nullptr) return true;
    // Collect current inorder values
    std::vector<int> current;
    std::function<void(TreeNode*)> collect = [&](TreeNode* n) {
        if (!n) return;
        collect(n->left);
        current.push_back(n->val);
        collect(n->right);
    };
    collect(root);
    // For each position, check against expected computed from original inorder
    for (size_t i = 0; i < current.size(); ++i) {
        int orig = inorder[i];
        int expected = 0;
        for (size_t j = i; j < inorder.size(); ++j) {
            expected += inorder[j]; // Since original inorder is ascending, all >= are from i onward
        }
        if (current[i] != expected) return false;
    }
    return true;
}

int main() {
    // Test 1: Empty tree
    TreeNode* empty = nullptr;
    assert(bstToGreaterTree(empty) == nullptr);

    // Test 2: Single node
    TreeNode* single = new TreeNode(5);
    bstToGreaterTree(single);
    assert(single->val == 5);
    delete single;

    // Test 3: Basic tree (example from problem: [4,1,6,0,2,5,7,null,null,null,3,null,null,null,8] -> [30,36,21,36,35,26,15,null,null,null,33,null,null,null,8])
    // Construct simpler: [2,1,3] -> expected [5,6,3]
    TreeNode* t1 = new TreeNode(2);
    t1->left = new TreeNode(1);
    t1->right = new TreeNode(3);
    bstToGreaterTree(t1);
    assert(t1->val == 5);
    assert(t1->left->val == 6);
    assert(t1->right->val == 3);
    delete t1->left;
    delete t1->right;
    delete t1;

    // Test 4: Tree with negative values: [-3, -1, 0] -> original inorder [-3,-1,0], expected [-1? Let's compute: for -3, sum of >= -3 is -3-1+0=-4; for -1, -1+0=-1; for 0, 0]
    // Construct: root=-1, left=-3, right=0
    TreeNode* t2 = new TreeNode(-1);
    t2->left = new TreeNode(-3);
    t2->right = new TreeNode(0);
    bstToGreaterTree(t2);
    assert(t2->val == -1); // -1 + 0 = -1
    assert(t2->left->val == -4); // -3 + (-1) + 0 = -4
    assert(t2->right->val == 0); // 0
    delete t2->left;
    delete t2->right;
    delete t2;

    // Test 5: Larger tree [4,1,6,0,2,5,7,null,null,null,3,null,null,null,8] (classic example)
    // Build tree manually
    TreeNode* root = new TreeNode(4);
    root->left = new TreeNode(1);
    root->right = new TreeNode(6);
    root->left->left = new TreeNode(0);
    root->left->right = new TreeNode(2);
    root->right->left = new TreeNode(5);
    root->right->right = new TreeNode(7);
    root->left->right->right = new TreeNode(3);
    root->right->right->right = new TreeNode(8);
    
    // Save original inorder for verification
    std::vector<int> original;
    std::function<void(TreeNode*)> getInorder = [&](TreeNode* n) {
        if (!n) return;
        getInorder(n->left);
        original.push_back(n->val);
        getInorder(n->right);
    };
    getInorder(root);
    
    // Apply transformation and verify
    bstToGreaterTree(root);
    assert(verifyGreaterTree(root, original));
    assert(root->val == 30); // 4+5+6+7+8 = 30
    assert(root->left->val == 36); // 1+2+3+4+5+6+7+8 = 36
    assert(root->right->val == 21); // 6+7+8 = 21
    assert(root->left->left->val == 36); // 0+1+...+8 = 36
    assert(root->left->right->val == 35); // 2+3+4+5+6+7+8 = 35
    assert(root->right->left->val == 26); // 5+6+7+8 = 26
    assert(root->right->right->val == 15); // 7+8 = 15
    assert(root->left->right->right->val == 33); // 3+4+5+6+7+8 = 33
    assert(root->right->right->right->val == 8); // 8

    // Test 6: Skewed left tree (all left children): [3,2,1] -> original inorder [1,2,3] -> expected [6,5,3]
    TreeNode* skewed = new TreeNode(3);
    skewed->left = new TreeNode(2);
    skewed->left->left = new TreeNode(1);
    bstToGreaterTree(skewed);
    assert(skewed->val == 6);
    assert(skewed->left->val == 5);
    assert(skewed->left->left->val == 3);
    delete skewed->left->left;
    delete skewed->left;
    delete skewed;

    // Cleanup for larger tree
    // (deleting manually; not critical for test performance)
    
    return 0;
}
