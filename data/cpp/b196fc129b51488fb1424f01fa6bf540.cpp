Write a C++ function that takes a vector of integers representing a complete binary tree stored in level-order (with the root at index 0, and for any node at index `i`, its left child is at `2*i+1` and its right child at `2*i+2`), builds the corresponding binary tree, and then performs a single iterative traversal that simultaneously collects the preorder, inorder, and postorder sequences. The function should return a `std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>` containing these three traversals in the order (preorder, inorder, postorder). Assume the input vector is non-empty and may contain duplicate values. Handle missing children naturally when indices go out of bounds. The solution must use only a single stack and no recursion.

// The core idea is to simulate a depth-first traversal using a stack where each stack entry keeps track of a node and a state number (1, 2, or 3) representing which visit we are currently processing. Initially, push the root with state 1. When we pop an entry with state 1, we record the node's data into the preorder result, increment its state to 2, push it back, and then push its left child (if any) with state 1—this ensures the left subtree is processed before returning to the parent. When we pop an entry with state 2, we record into inorder, increment state to 3, push back, then push the right child (if any) with state 1. When state 3 is popped, we record into postorder and do not push anything else. This guarantees that each node is visited exactly three times, producing all three traversals in one pass. Edge cases include null children (we simply skip pushing them), a tree with only a root (all traversals have one element), and skewed trees where the stack depth equals the number of nodes. The iterative approach avoids recursion stack overflow for very deep trees. Time complexity is O(n) because each node is pushed and popped exactly three times. Space complexity is O(h) for the stack, where h is the tree height, plus O(n) for the three result vectors.

#include <vector>
#include <stack>
#include <tuple>

// Tree node definition
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    explicit TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Build a binary tree from a level-order vector (index 0 = root, left = 2*i+1, right = 2*i+2)
TreeNode* buildTreeFromVector(const std::vector<int>& data, int index) {
    if (index >= static_cast<int>(data.size())) {
        return nullptr;
    }
    TreeNode* node = new TreeNode(data[index]);
    node->left = buildTreeFromVector(data, 2 * index + 1);
    node->right = buildTreeFromVector(data, 2 * index + 2);
    return node;
}

// Perform single iterative traversal producing preorder, inorder, postorder
std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> getTraversals(const std::vector<int>& treeValues) {
    std::vector<int> preorder, inorder, postorder;
    if (treeValues.empty()) {
        return std::make_tuple(preorder, inorder, postorder);
    }

    TreeNode* root = buildTreeFromVector(treeValues, 0);
    std::stack<std::pair<TreeNode*, int>> st; // state 1 = pre, 2 = in, 3 = post
    st.push({root, 1});

    while (!st.empty()) {
        auto [node, state] = st.top();
        st.pop();

        if (state == 1) {
            preorder.push_back(node->val);
            state = 2;
            st.push({node, state});
            if (node->left != nullptr) {
                st.push({node->left, 1});
            }
        } else if (state == 2) {
            inorder.push_back(node->val);
            state = 3;
            st.push({node, state});
            if (node->right != nullptr) {
                st.push({node->right, 1});
            }
        } else {
            postorder.push_back(node->val);
        }
    }

    return std::make_tuple(preorder, inorder, postorder);
}

#include <cassert>
#include <tuple>
#include <vector>

// Include the solution code here (TreeNode, buildTreeFromVector, getTraversals)

int main() {
    // Test 1: Complete tree {1,2,3,4,5,6,7}
    std::vector<int> v1 = {1,2,3,4,5,6,7};
    auto [pre1, in1, post1] = getTraversals(v1);
    assert((pre1 == std::vector<int>{1,2,4,5,3,6,7}));
    assert((in1 == std::vector<int>{4,2,5,1,6,3,7}));
    assert((post1 == std::vector<int>{4,5,2,6,7,3,1}));

    // Test 2: Single node
    std::vector<int> v2 = {42};
    auto [pre2, in2, post2] = getTraversals(v2);
    assert((pre2 == std::vector<int>{42}));
    assert((in2 == std::vector<int>{42}));
    assert((post2 == std::vector<int>{42}));

    // Test 3: Skewed left tree {1,2,0,3}
    // Level-order: 1(root), 2(left), 0(no right), 3(left child of 2)
    // Actual tree: 1 -> left 2 -> left 3
    std::vector<int> v3 = {1,2,0,3};
    auto [pre3, in3, post3] = getTraversals(v3);
    assert((pre3 == std::vector<int>{1,2,3}));
    assert((in3 == std::vector<int>{3,2,1}));
    assert((post3 == std::vector<int>{3,2,1}));

    // Test 4: Duplicate values
    std::vector<int> v4 = {5,5,5};
    auto [pre4, in4, post4] = getTraversals(v4);
    assert((pre4 == std::vector<int>{5,5,5}));
    assert((in4 == std::vector<int>{5,5,5}));
    assert((post4 == std::vector<int>{5,5,5}));

    // Test 5: Empty tree (should return three empty vectors)
    std::vector<int> v5 = {};
    auto [pre5, in5, post5] = getTraversals(v5);
    assert(pre5.empty() && in5.empty() && post5.empty());

    // Test 6: Tree with only left children but missing right
    // Vector {1,2,0,4} represents: 1, left=2, right absent, 2's left=4
    std::vector<int> v6 = {1,2,0,4};
    auto [pre6, in6, post6] = getTraversals(v6);
    assert((pre6 == std::vector<int>{1,2,4}));
    assert((in6 == std::vector<int>{4,2,1}));
    assert((post6 == std::vector<int>{4,2,1}));

    return 0;
}
