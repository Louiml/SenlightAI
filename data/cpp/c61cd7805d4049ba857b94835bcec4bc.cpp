// Write a C++ function `std::vector<int> preorderTraversal(const Node* root)` that returns the preorder traversal (root, left subtree, right subtree) of a binary tree as a vector of integers. The function must use an iterative approach with an explicit stack (not recursion) and must handle empty trees by returning an empty vector. Each node contains an integer `data` and pointers `left` and `right`. The tree is not modified, and the function should be const-correct (accept a pointer to const node). Assume the node structure is already defined as: `struct Node { int data; Node* left; Node* right; };`.
// The solution uses a stack to simulate the recursive preorder traversal. The algorithm starts by pushing the root onto the stack. While the stack is not empty, we pop the top node, append its `data` to the result vector, then push its right child (if any) followed by its left child (if any) onto the stack. Pushing right first ensures that the left subtree is processed before the right subtree when popped, because stacks are LIFO. Edge cases include: an empty tree (root == nullptr) returns an empty vector; a tree with only one node returns that single value; nodes with only one child are handled naturally because only non-null children are pushed. Time complexity is O(n) because each node is visited exactly once. Space complexity is O(h) in the worst case for the stack, where h is the height of the tree (O(n) for a skewed tree, O(log n) for a balanced tree). The function does not modify the tree, so we use `const Node*` and return by value.
#include <vector>
#include <stack>

struct Node {
    int data;
    Node* left;
    Node* right;
};

// Perform iterative preorder traversal and return node values in order.
std::vector<int> preorderTraversal(const Node* root) {
    std::vector<int> result;
    if (root == nullptr) return result;

    std::stack<const Node*> nodeStack;
    nodeStack.push(root);

    while (!nodeStack.empty()) {
        const Node* current = nodeStack.top();
        nodeStack.pop();
        result.push_back(current->data);

        // Push right child first so that left is processed first.
        if (current->right != nullptr) {
            nodeStack.push(current->right);
        }
        if (current->left != nullptr) {
            nodeStack.push(current->left);
        }
    }

    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

// Node definition is assumed from the solution.

int main() {
    // Test 1: Empty tree
    Node* empty = nullptr;
    assert(preorderTraversal(empty) == std::vector<int>{});

    // Test 2: Single node
    Node single{5, nullptr, nullptr};
    assert(preorderTraversal(&single) == std::vector<int>{5});

    // Test 3: Full binary tree:      1
    //                                / \
    //                               2   3
    //                              / \   \
    //                             4   5   6
    Node n4{4, nullptr, nullptr};
    Node n5{5, nullptr, nullptr};
    Node n6{6, nullptr, nullptr};
    Node n2{2, &n4, &n5};
    Node n3{3, nullptr, &n6};
    Node n1{1, &n2, &n3};
    assert(preorderTraversal(&n1) == std::vector<int>({1, 2, 4, 5, 3, 6}));

    // Test 4: Left-skewed tree: 1 -> 2 -> 3
    Node n8{8, nullptr, nullptr};
    Node n7{7, &n8, nullptr};
    Node n9{9, &n7, nullptr};
    assert(preorderTraversal(&n9) == std::vector<int>({9, 7, 8}));

    // Test 5: Right-skewed tree: 10 -> 20 -> 30
    Node n30{30, nullptr, nullptr};
    Node n20{20, nullptr, &n30};
    Node n10{10, nullptr, &n20};
    assert(preorderTraversal(&n10) == std::vector<int>({10, 20, 30}));

    // Test 6: Tree with only left children
    Node l3{3, nullptr, nullptr};
    Node l2{2, &l3, nullptr};
    Node l1{1, &l2, nullptr};
    assert(preorderTraversal(&l1) == std::vector<int>({1, 2, 3}));

    std::cout << "All tests passed.\n";
    return 0;
}
