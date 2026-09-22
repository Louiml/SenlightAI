/*
Write a C++ function named `iterativePreorder` that takes a pointer to the root of a binary tree (where each `Node` has an `int data`, and left/right child pointers) and returns a `std::vector<int>` containing the tree's preorder traversal (node, left subtree, right subtree) using an iterative approach with an explicit stack. The function must not use recursion and must not use any global or static variables. You may assume the tree is non-empty and may contain duplicate values.
*/
#include <vector>
#include <stack>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int x) : data(x), left(nullptr), right(nullptr) {}
};

// Returns the preorder traversal of the binary tree iteratively.
// Preorder: root, left subtree, right subtree.
std::vector<int> iterativePreorder(const Node* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }
    
    std::stack<const Node*> nodeStack;
    nodeStack.push(root);
    
    while (!nodeStack.empty()) {
        const Node* current = nodeStack.top();
        nodeStack.pop();
        result.push_back(current->data);
        
        // Push right first so that left is processed first.
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

// Node struct and iterativePreorder are assumed to be defined above.

int main() {
    // Test 1: Single node
    Node* n1 = new Node(5);
    std::vector<int> res1 = iterativePreorder(n1);
    assert(res1 == std::vector<int>({5}));
    delete n1;

    // Test 2: Left-skewed tree (1->2->3)
    Node* n2 = new Node(1);
    n2->left = new Node(2);
    n2->left->left = new Node(3);
    std::vector<int> res2 = iterativePreorder(n2);
    assert(res2 == std::vector<int>({1, 2, 3}));
    delete n2->left->left;
    delete n2->left;
    delete n2;

    // Test 3: Right-skewed tree (1->2->3)
    Node* n3 = new Node(1);
    n3->right = new Node(2);
    n3->right->right = new Node(3);
    std::vector<int> res3 = iterativePreorder(n3);
    assert(res3 == std::vector<int>({1, 2, 3}));
    delete n3->right->right;
    delete n3->right;
    delete n3;

    // Test 4: Balanced-ish tree from snippet (1,2,3,4,5,6)
    Node* n4 = new Node(1);
    n4->left = new Node(2);
    n4->right = new Node(3);
    n4->left->left = new Node(4);
    n4->left->right = new Node(5);
    n4->right->right = new Node(6);
    std::vector<int> res4 = iterativePreorder(n4);
    assert(res4 == std::vector<int>({1, 2, 4, 5, 3, 6}));
    delete n4->left->left;
    delete n4->left->right;
    delete n4->right->right;
    delete n4->left;
    delete n4->right;
    delete n4;

    // Test 5: Null root (empty tree) should return empty vector
    Node* n5 = nullptr;
    std::vector<int> res5 = iterativePreorder(n5);
    assert(res5.empty());

    // Test 6: Duplicate values
    Node* n6 = new Node(2);
    n6->left = new Node(2);
    n6->right = new Node(2);
    std::vector<int> res6 = iterativePreorder(n6);
    assert(res6 == std::vector<int>({2, 2, 2}));
    delete n6->left;
    delete n6->right;
    delete n6;

    return 0;
}
// The standard iterative preorder traversal typically uses a single stack of nodes, pushing the right child then the left child after visiting a node. However, the given code snippet uses a *state-machine* approach with a stack of pairs `(Node*, int state)`, where state 1 means "visit node and prepare to push left child", state 2 means "push right child", and state 3 means "pop". For the task, we simplify to the classic two-stack or single-stack method for clarity and efficiency. The main algorithm: push the root onto the stack. While the stack is not empty, pop the top node, add its data to the result vector, then push its right child (if exists), then push its left child (if exists). Because the stack is LIFO, pushing right first ensures left is processed next, yielding preorder. Edge cases: empty tree (though task states non-empty, we handle gracefully by returning empty vector), single node, left-skewed, right-skewed trees, and duplicate values. Time complexity is O(n) because each node is visited exactly once. Space complexity is O(h) for the stack in the worst case (skewed tree), where h is the height; for a balanced tree it's O(log n).
