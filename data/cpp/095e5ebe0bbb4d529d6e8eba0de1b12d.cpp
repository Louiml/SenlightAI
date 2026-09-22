// Write a standalone C++ function named `maximumSumNonAdjacent` that accepts a binary tree root pointer (defined by a provided `Node` struct with `int data`, `Node* left`, `Node* right`) and returns the maximum possible sum of node values chosen from the tree under the constraint that no two chosen nodes are directly adjacent (i.e., no parent-child pair can both be selected). The function must handle empty trees (return 0), single nodes, skewed trees, and balanced trees, and must be implemented efficiently. You are not allowed to use any global or static variables; the function must be self-contained and should not modify the tree structure.

The problem is a classic tree dynamic programming (DP) problem often called "House Robber III" or "Maximum Sum of Non-Adjacent Nodes." For each node, we compute two values:  
- `dp0[node]`: the maximum sum from the subtree rooted at this node if this node is **included** in the sum. Then we cannot include its children, so `dp0[node] = node->data + dp1[left] + dp1[right]` where `dp1` represents the best sum when the child is **excluded**.  
- `dp1[node]`: the maximum sum from the subtree if this node is **excluded**. Then we are free to include or exclude each child, so `dp1[node] = max(dp0[left], dp1[left]) + max(dp0[right], dp1[right])`.  

We traverse the tree in post-order (left, right, root) because we need children results before computing the parent. For a `NULL` node, both `dp0` and `dp1` are 0. The final answer for the root is `max(dp0[root], dp1[root])`.  

Edge cases:  
- Empty tree: returns 0 (covered by base case).  
- Single node: `dp0 = data`, `dp1 = 0`, answer is `data`.  
- Skewed tree: recursion depth is O(n), which could cause stack overflow for extremely deep trees, but that is acceptable for typical input sizes.  

Time complexity: O(n) since we visit each node exactly once.  
Space complexity: O(n) for the recursion stack in the worst case (skewed tree) plus O(n) for any maps used; a simpler implementation can avoid maps by returning a pair of values from a recursive helper, reducing auxiliary space to O(n) stack depth.

#include <algorithm>

struct Node {
    int data;
    Node* left;
    Node* right;
};

// Helper function to compute the best sums for a subtree.
// Returns a pair: (best if current node is included, best if excluded)
static std::pair<int, int> solve(Node* root) {
    if (root == nullptr) {
        return {0, 0};
    }
    auto left = solve(root->left);
    auto right = solve(root->right);

    int include = root->data + left.second + right.second;
    int exclude = std::max(left.first, left.second) + std::max(right.first, right.second);
    return {include, exclude};
}

// Public function: returns maximum sum of non-adjacent nodes in the tree.
// Returns 0 for an empty tree (null root).
int maximumSumNonAdjacent(Node* root) {
    auto result = solve(root);
    return std::max(result.first, result.second);
}

#include <cassert>
#include <vector>

// Mock helper to build a tree from a vector<int> in level-order (use -1 for null)
Node* buildTreeFromVector(const std::vector<int>& values) {
    if (values.empty() || values[0] == -1) return nullptr;
    Node* root = new Node{values[0], nullptr, nullptr};
    std::vector<Node*> nodes{root};
    int idx = 1;
    for (size_t i = 0; i < nodes.size() && idx < values.size(); ++i) {
        if (nodes[i] == nullptr) continue;
        if (idx < values.size() && values[idx] != -1) {
            nodes[i]->left = new Node{values[idx], nullptr, nullptr};
            nodes.push_back(nodes[i]->left);
        }
        idx++;
        if (idx < values.size() && values[idx] != -1) {
            nodes[i]->right = new Node{values[idx], nullptr, nullptr};
            nodes.push_back(nodes[i]->right);
        }
        idx++;
    }
    return root;
}

// Helper to delete tree
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    // Test 1: Empty tree
    assert(maximumSumNonAdjacent(nullptr) == 0);

    // Test 2: Single node
    Node* t = new Node{5, nullptr, nullptr};
    assert(maximumSumNonAdjacent(t) == 5);
    deleteTree(t);

    // Test 3: Simple balanced tree: [3,2,3,1,-1, -1, 1] => max sum = 3+3 = 6 (choose root and right child's right? Actually choose root (3) and both children? No, root's children cannot be chosen. Common answer 7? Let's compute properly.)
    // Tree: root=3, left=2, right=3, left-left=1, right-right=1.
    t = buildTreeFromVector({3,2,3,1,-1,-1,1});
    // Best: choose root (3) + left-left (1) + right-right (1) = 5? Or choose left (2) + right (3) + right-right? no. Let's compute: 
    // Option include root: 3 + (best exclude left) + (best exclude right) = 3 + (max include left=2+1=3? Actually left: include=2+0+1=3, exclude=max(1,0)+0=1 => best exclude left=1) + (right: include=3+0+1=4, exclude=0+max(0,0)=0 => best exclude right=0) => 3+1+0=4.
    // Option exclude root: max(3,1)+max(4,0)=3+4=7. So answer=7.
    assert(maximumSumNonAdjacent(t) == 7);
    deleteTree(t);

    // Test 4: Skewed left chain: [1,2,-1,3,-1,-1,-1] => path 1-2-3 => max sum = 1+3=4 (choose 1 and 3)
    t = buildTreeFromVector({1,2,-1,3,-1,-1,-1});
    assert(maximumSumNonAdjacent(t) == 4);
    deleteTree(t);

    // Test 5: All same values: [2,2,2,2,2,2,2] => best choose one level of 2s: choose root (2)+ grandchildren (2+2)=6? Actually include root: 2 + exclude children (children best when excluded: each child exclude gives max of include child=2+grandchildren, exclude=0 => children best exclude: for each child, include=2+2=4, exclude=0 => max=4? Wait, if root excluded, we can choose children: root exclude => max(left)+max(right) where left include=2+ (grandchildren) = 2+2=4, exclude=0 => max=4, same for right => 8. So answer 8.
    t = buildTreeFromVector({2,2,2,2,2,2,2});
    assert(maximumSumNonAdjacent(t) == 8);
    deleteTree(t);

    // Test 6: Negative values: [-1,2,3] => best choose children? choose 2 (left) and 3 (right) =5, root not chosen.
    t = buildTreeFromVector({-1,2,3});
    assert(maximumSumNonAdjacent(t) == 5);
    deleteTree(t);

    return 0;
}
