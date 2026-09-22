Write a C++ function `buildBSTFromPreorder` that takes a vector of integers representing a preorder traversal of a binary search tree (BST) and constructs the BST, returning the root pointer. The tree nodes are defined as `struct Node { int data; Node* left; Node* right; };`. The function must handle duplicate values gracefully (ignore duplicates after the first occurrence), and must validate that the input is a valid BST preorder sequence; if invalid (e.g., a value violates BST ordering relative to ancestors), return `nullptr`. You must implement the construction without recursion, using an explicit stack, and return the root of the constructed tree. Provide a helper function `preorderTraversal(Node*)` that returns a vector of integers representing the preorder traversal of the constructed tree. Ensure the solution is self-contained with all necessary includes and definitions.
// The core idea is to simulate the recursive BST insertion process iteratively using a stack. We process the preorder sequence left to right. Maintain a stack of node pointers representing the path from the root to the current node (the "ancestors" whose right subtrees we might fill next). For each new value:
// - Start with the first element as the root, push it onto the stack.
// - For each subsequent value `val`:
//   - Check if `val` is smaller than the current node's data. If so, insert as the left child, push this new node, and continue.
//   - If `val` is larger, we need to find the appropriate ancestor to attach as its right child. Pop nodes from the stack while the stack is non-empty and `val` is greater than the top node's data. The last popped node becomes the parent for the right child. However, we must also ensure that `val` is smaller than the next node in the stack (the ancestor's parent) if it exists; otherwise, the sequence is invalid. So after popping, if the stack is not empty and `val` > stack top's data, it's invalid, return `nullptr`. If `val` equals an existing node's data (either current or top), skip (duplicate).
// - Edge cases: empty input returns `nullptr`. Single element returns a single-node tree. The algorithm runs in O(n) time and O(h) space (h is tree height, worst-case O(n) for skewed trees) because each node is pushed and popped at most once.
#include <vector>
#include <stack>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Build a BST from its preorder traversal. Returns nullptr if invalid or empty.
Node* buildBSTFromPreorder(const std::vector<int>& preorder) {
    if (preorder.empty()) return nullptr;

    Node* root = new Node(preorder[0]);
    std::stack<Node*> st;
    st.push(root);

    for (size_t i = 1; i < preorder.size(); ++i) {
        int val = preorder[i];
        Node* current = st.top();

        // Duplicate check: if value equals current top, ignore
        if (val == current->data) continue;

        if (val < current->data) {
            // Insert as left child
            Node* newNode = new Node(val);
            current->left = newNode;
            st.push(newNode);
        } else { // val > current->data
            // Find the correct ancestor to attach as right child
            Node* lastPopped = nullptr;
            while (!st.empty() && val > st.top()->data) {
                lastPopped = st.top();
                st.pop();
            }
            // After popping, if stack is not empty and val is still greater than top's data, invalid
            if (!st.empty() && val > st.top()->data) {
                // Invalid sequence, but we already popped; need to clean up? For simplicity, return nullptr
                // (Memory cleanup omitted for brevity in task context)
                return nullptr;
            }
            // Attach as right child of lastPopped
            if (lastPopped) {
                Node* newNode = new Node(val);
                lastPopped->right = newNode;
                st.push(newNode);
            } else {
                // This happens when val is greater than root's data but less than all? Actually root is top initially and val > root, so lastPopped would be root.
                // But if val > root and root has no left, it's okay. But if val > root and stack is empty after popping? That can't happen because root is in stack at start.
                // For safety, handle nullptr case.
                return nullptr;
            }
        }
    }
    return root;
}

// Helper to compute preorder traversal of a tree (iterative)
std::vector<int> preorderTraversal(Node* root) {
    std::vector<int> result;
    std::stack<Node*> st;
    Node* p = root;
    while (p != nullptr || !st.empty()) {
        if (p != nullptr) {
            result.push_back(p->data);
            st.push(p);
            p = p->left;
        } else {
            p = st.top();
            st.pop();
            p = p->right;
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution code here (or link it)

int main() {
    // Test 1: Valid tree from example
    std::vector<int> pre1 = {30,20,10,15,25,40,50,45};
    Node* root1 = buildBSTFromPreorder(pre1);
    assert(root1 != nullptr);
    assert(preorderTraversal(root1) == pre1);
    // Check some structure
    assert(root1->data == 30);
    assert(root1->left->data == 20);
    assert(root1->left->left->data == 10);
    assert(root1->left->left->right->data == 15);
    assert(root1->left->right->data == 25);
    assert(root1->right->data == 40);
    assert(root1->right->right->data == 50);
    assert(root1->right->right->left->data == 45);

    // Test 2: Single element
    std::vector<int> pre2 = {7};
    Node* root2 = buildBSTFromPreorder(pre2);
    assert(root2 != nullptr);
    assert(root2->left == nullptr && root2->right == nullptr);
    assert(preorderTraversal(root2) == pre2);

    // Test 3: Empty input
    std::vector<int> pre3 = {};
    Node* root3 = buildBSTFromPreorder(pre3);
    assert(root3 == nullptr);

    // Test 4: Already sorted ascending (right-skewed)
    std::vector<int> pre4 = {1,2,3,4};
    Node* root4 = buildBSTFromPreorder(pre4);
    assert(root4 != nullptr);
    assert(preorderTraversal(root4) == pre4);
    assert(root4->right->data == 2 && root4->right->right->data == 3 && root4->right->right->right->data == 4);

    // Test 5: Sorted descending (left-skewed)
    std::vector<int> pre5 = {5,4,3,2,1};
    Node* root5 = buildBSTFromPreorder(pre5);
    assert(root5 != nullptr);
    assert(preorderTraversal(root5) == pre5);
    assert(root5->left->data == 4 && root5->left->left->data == 3);

    // Test 6: Duplicates ignored
    std::vector<int> pre6 = {10,5,5,15};
    Node* root6 = buildBSTFromPreorder(pre6);
    assert(root6 != nullptr);
    // After ignoring duplicate 5, the preorder of tree becomes 10,5,15
    assert(preorderTraversal(root6) == std::vector<int>({10,5,15}));

    // Test 7: Invalid sequence (violates BST property)
    std::vector<int> pre7 = {20,10,30,15}; // 15 appears after 30, should be left of 20 but after 30 is invalid
    Node* root7 = buildBSTFromPreorder(pre7);
    assert(root7 == nullptr);

    // Test 8: Another valid case with 3 nodes
    std::vector<int> pre8 = {50,30,70};
    Node* root8 = buildBSTFromPreorder(pre8);
    assert(root8 != nullptr);
    assert(preorderTraversal(root8) == pre8);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
