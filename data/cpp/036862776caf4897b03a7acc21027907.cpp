Write a C++ function `singlePassTreeTraversals` that takes a binary tree root pointer (`Node*`) as input, where `Node` is a struct with `int data`, `Node* left`, and `Node* right`. The function must return a `std::vector<std::vector<int>>` containing three vectors: the preorder, inorder, and postorder traversals of the tree, in that order. The function must perform all three traversals in a single pass over the tree using an explicit stack (no recursion, no helper functions), and must return an empty vector of vectors if the tree is empty. The input tree is non-cyclic (a proper binary tree). The function should be `const`-correct: it should not modify the tree, and should accept a `const Node*` parameter. The solution must be self-contained with all necessary headers.

The core idea is to use a stack of pairs, each storing a node pointer and an integer "state" (1, 2, or 3). Initially, push the root with state 1. The algorithm processes each pair:
- **State 1**: This is the first visit to the node. Record its data in the preorder vector. Change the state to 2 and push the pair back onto the stack. Then, if the node has a left child, push that child with state 1. This ensures that the left subtree is processed before returning to the node in state 2.
- **State 2**: This is the second visit (after the left subtree is done). Record the node's data in the inorder vector. Change the state to 3 and push the pair back. Then, if the node has a right child, push that child with state 1. This ensures the right subtree is processed before returning to the node in state 3.
- **State 3**: This is the third visit (after both subtrees are done). Record the node's data in the postorder vector. No further pushes occur.

This approach simulates the recursion stack manually, guaranteeing that each node is visited exactly three times in the correct order. **Edge cases**: An empty tree (root `nullptr`) returns an empty vector of vectors. A tree with only a root produces all three traversals as `{root}`. Nodes with only one child are handled naturally because we check for `left` and `right` existence before pushing. **Time complexity**: Each node is pushed onto the stack and popped exactly three times, so the total time is O(n), where n is the number of nodes. **Space complexity**: The stack may hold up to O(h) nodes in the worst case for a balanced tree, but in the worst case of a skewed tree it can hold O(n) nodes. The auxiliary space excluding the output vectors is O(n) in the worst case.

#include <vector>
#include <stack>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Perform preorder, inorder, and postorder traversals of a binary tree in a single pass.
// Returns a vector of three vectors: [preorder, inorder, postorder].
// If the tree is empty, returns an empty vector of vectors.
std::vector<std::vector<int>> singlePassTreeTraversals(const Node* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result; // empty
    }

    std::vector<int> pre, in, post;
    std::stack<std::pair<const Node*, int>> st; // node, state (1=pre, 2=in, 3=post)
    st.push({root, 1});

    while (!st.empty()) {
        auto [node, state] = st.top();
        st.pop();

        if (state == 1) {
            // First visit: record preorder
            pre.push_back(node->data);
            st.push({node, 2}); // revisit for inorder after left subtree
            if (node->left != nullptr) {
                st.push({node->left, 1});
            }
        } else if (state == 2) {
            // Second visit: record inorder (after left subtree)
            in.push_back(node->data);
            st.push({node, 3}); // revisit for postorder after right subtree
            if (node->right != nullptr) {
                st.push({node->right, 1});
            }
        } else { // state == 3
            // Third visit: record postorder (after both subtrees)
            post.push_back(node->data);
        }
    }

    result.push_back(pre);
    result.push_back(in);
    result.push_back(post);
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty tree
    {
        std::vector<std::vector<int>> res = singlePassTreeTraversals(nullptr);
        assert(res.empty());
    }

    // Test 2: Single node
    {
        Node* root = new Node(5);
        std::vector<std::vector<int>> res = singlePassTreeTraversals(root);
        assert(res.size() == 3);
        assert(res[0] == std::vector<int>({5})); // pre
        assert(res[1] == std::vector<int>({5})); // in
        assert(res[2] == std::vector<int>({5})); // post
        delete root;
    }

    // Test 3: Example from snippet (root=1, left=2, right=3, left-left=4, left-right=5)
    {
        Node* root = new Node(1);
        root->left = new Node(2);
        root->right = new Node(3);
        root->left->left = new Node(4);
        root->left->right = new Node(5);
        std::vector<std::vector<int>> res = singlePassTreeTraversals(root);
        assert(res[0] == std::vector<int>({1, 2, 4, 5, 3})); // pre
        assert(res[1] == std::vector<int>({4, 2, 5, 1, 3})); // in
        assert(res[2] == std::vector<int>({4, 5, 2, 3, 1})); // post
        // cleanup
        delete root->left->left;
        delete root->left->right;
        delete root->left;
        delete root->right;
        delete root;
    }

    // Test 4: Left-skewed tree (1->2->3)
    {
        Node* root = new Node(1);
        root->left = new Node(2);
        root->left->left = new Node(3);
        std::vector<std::vector<int>> res = singlePassTreeTraversals(root);
        assert(res[0] == std::vector<int>({1, 2, 3})); // pre
        assert(res[1] == std::vector<int>({3, 2, 1})); // in
        assert(res[2] == std::vector<int>({3, 2, 1})); // post
        delete root->left->left;
        delete root->left;
        delete root;
    }

    // Test 5: Right-skewed tree (1->2->3)
    {
        Node* root = new Node(1);
        root->right = new Node(2);
        root->right->right = new Node(3);
        std::vector<std::vector<int>> res = singlePassTreeTraversals(root);
        assert(res[0] == std::vector<int>({1, 2, 3})); // pre
        assert(res[1] == std::vector<int>({1, 2, 3})); // in
        assert(res[2] == std::vector<int>({3, 2, 1})); // post
        delete root->right->right;
        delete root->right;
        delete root;
    }

    // Test 6: Root with only right child
    {
        Node* root = new Node(10);
        root->right = new Node(20);
        std::vector<std::vector<int>> res = singlePassTreeTraversals(root);
        assert(res[0] == std::vector<int>({10, 20}));
        assert(res[1] == std::vector<int>({10, 20}));
        assert(res[2] == std::vector<int>({20, 10}));
        delete root->right;
        delete root;
    }

    // Test 7: Root with only left child
    {
        Node* root = new Node(10);
        root->left = new Node(5);
        std::vector<std::vector<int>> res = singlePassTreeTraversals(root);
        assert(res[0] == std::vector<int>({10, 5}));
        assert(res[1] == std::vector<int>({5, 10}));
        assert(res[2] == std::vector<int>({5, 10}));
        delete root->left;
        delete root;
    }

    // Test 8: More complex tree (balanced, 7 nodes)
    {
        Node* root = new Node(4);
        root->left = new Node(2);
        root->right = new Node(6);
        root->left->left = new Node(1);
        root->left->right = new Node(3);
        root->right->left = new Node(5);
        root->right->right = new Node(7);
        std::vector<std::vector<int>> res = singlePassTreeTraversals(root);
        assert(res[0] == std::vector<int>({4, 2, 1, 3, 6, 5, 7})); // pre
        assert(res[1] == std::vector<int>({1, 2, 3, 4, 5, 6, 7})); // in
        assert(res[2] == std::vector<int>({1, 3, 2, 5, 7, 6, 4})); // post
        // cleanup
        delete root->left->left;
        delete root->left->right;
        delete root->left;
        delete root->right->left;
        delete root->right->right;
        delete root->right;
        delete root;
    }

    return 0;
}
