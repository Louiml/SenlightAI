/*
Write a C++ function that takes a binary tree built from a user-driven recursive constructor (where -1 indicates a null child) and returns two vectors: one containing the tree’s preorder traversal and one containing its inorder traversal. The function must not modify the tree, must handle an empty tree, and must produce the correct order for arbitrary tree shapes. The constructor is provided separately; your solution only needs to implement the traversal extraction.
*/
#include <vector>
#include <stack>

// TreeNode structure (assumed available from the provided snippet)
class Tnode {
public:
    int data;
    Tnode* left;
    Tnode* right;
    Tnode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Return {preorder, inorder} for a binary tree without modifying it.
// Uses iterative traversals to avoid recursion depth issues.
std::pair<std::vector<int>, std::vector<int>> getTraversals(const Tnode* root) {
    std::vector<int> pre;
    std::vector<int> in;

    // Preorder (root, left, right) - iterative with a stack
    if (root) {
        std::stack<const Tnode*> stack;
        stack.push(root);
        while (!stack.empty()) {
            const Tnode* current = stack.top();
            stack.pop();
            pre.push_back(current->data);
            // Push right first so left is processed first
            if (current->right) stack.push(current->right);
            if (current->left) stack.push(current->left);
        }
    }

    // Inorder (left, root, right) - iterative with a stack
    const Tnode* current = root;
    std::stack<const Tnode*> stack;
    while (current || !stack.empty()) {
        // Go as far left as possible
        while (current) {
            stack.push(current);
            current = current->left;
        }
        current = stack.top();
        stack.pop();
        in.push_back(current->data);
        current = current->right;
    }

    return {pre, in};
}
#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (or link to it)

// Helper to build a tree from a vector (using -1 as null) for testing
Tnode* buildTree(const std::vector<int>& arr, int& idx) {
    if (idx >= arr.size() || arr[idx] == -1) {
        idx++;
        return nullptr;
    }
    Tnode* node = new Tnode(arr[idx]);
    idx++;
    node->left = buildTree(arr, idx);
    node->right = buildTree(arr, idx);
    return node;
}

int main() {
    // Test 1: Empty tree
    const Tnode* empty = nullptr;
    auto result = getTraversals(empty);
    assert(result.first.empty() && result.second.empty());

    // Test 2: Single node
    Tnode single(5);
    result = getTraversals(&single);
    assert(result.first == std::vector<int>({5}));
    assert(result.second == std::vector<int>({5}));

    // Test 3: More complex tree
    // Build tree from preorder sequence: 1 2 -1 -1 3 4 -1 -1 5 -1 -1
    std::vector<int> arr = {1, 2, -1, -1, 3, 4, -1, -1, 5, -1, -1};
    int idx = 0;
    Tnode* tree = buildTree(arr, idx);
    result = getTraversals(tree);
    assert(result.first == std::vector<int>({1, 2, 3, 4, 5}));
    assert(result.second == std::vector<int>({2, 1, 4, 3, 5}));

    // Test 4: Left-skewed tree
    arr = {10, 20, 30, -1, -1, -1, -1};
    idx = 0;
    Tnode* leftSkew = buildTree(arr, idx);
    result = getTraversals(leftSkew);
    assert(result.first == std::vector<int>({10, 20, 30}));
    assert(result.second == std::vector<int>({30, 20, 10}));

    // Test 5: Right-skewed tree
    arr = {10, -1, 20, -1, 30, -1, -1};
    idx = 0;
    Tnode* rightSkew = buildTree(arr, idx);
    result = getTraversals(rightSkew);
    assert(result.first == std::vector<int>({10, 20, 30}));
    assert(result.second == std::vector<int>({10, 20, 30}));

    // Cleanup (not necessary for tests but good practice)
    // In a real program, you'd delete tree nodes here

    return 0;
}
// The solution uses two classic depth-first traversals. For preorder (root, left, right), we can implement either recursively or iteratively using a stack; the iterative version is preferred for clarity and to avoid recursion depth issues. For inorder (left, root, right), a stack-based algorithm that goes all the way left, then visits the node, then moves right, is standard. Edge cases: an empty tree (root == nullptr) should return two empty vectors. A single-node tree returns the same value in both traversals. Skewed trees (all left or all right) must not cause stack overflow if iterative; recursion would be O(n) depth but acceptable for small n. Time complexity is O(n) for each traversal, where n is the number of nodes, and space complexity is O(n) in the worst case for the stack (for a skewed tree) and O(n) for the vectors themselves. We must use `const Tnode*` to ensure the tree is not modified and apply `const` correctly to pointers and references.
