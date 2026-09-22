/*
Write a C++ function that takes a non-null pointer to the root of a binary tree (where each node stores an integer and has left/right child pointers possibly null) and returns the maximum value stored in the tree. If the tree contains only one node, that node’s data is the maximum. Assume the tree is not empty, but nodes may have only one child or none. The function must not modify the tree and should be marked as `const`-safe (i.e., accept a `const Node*` parameter). The tree is not necessarily a binary search tree, so you must traverse all nodes.
*/

#include <queue>

// Node structure for the binary tree.
struct Node {
    int data;
    Node* left;
    Node* right;
    
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Returns the maximum data value in the binary tree.
// Assumes root is non-null and tree is non-empty.
int findMax(const Node* root) {
    std::queue<const Node*> q;
    q.push(root);
    int maxVal = root->data;
    
    while (!q.empty()) {
        const Node* current = q.front();
        q.pop();
        
        if (current->data > maxVal) {
            maxVal = current->data;
        }
        
        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }
    return maxVal;
}

#include <cassert>

int main() {
    // Test 1: Single node tree
    Node* root1 = new Node(5);
    assert(findMax(root1) == 5);
    delete root1;

    // Test 2: Full tree with distinct values
    Node* root2 = new Node(10);
    root2->left = new Node(3);
    root2->right = new Node(7);
    root2->left->left = new Node(15);
    root2->right->right = new Node(2);
    assert(findMax(root2) == 15);
    // Cleanup
    delete root2->left->left;
    delete root2->left;
    delete root2->right->right;
    delete root2->right;
    delete root2;

    // Test 3: Tree with negative numbers, max is positive at root
    Node* root3 = new Node(-1);
    root3->left = new Node(-5);
    root3->right = new Node(-2);
    assert(findMax(root3) == -1);
    delete root3->left;
    delete root3->right;
    delete root3;

    // Test 4: All values negative, max is closest to zero
    Node* root4 = new Node(-8);
    root4->left = new Node(-3);
    root4->right = new Node(-7);
    assert(findMax(root4) == -3);
    delete root4->left;
    delete root4->right;
    delete root4;

    // Test 5: Skewed tree (only left children)
    Node* root5 = new Node(4);
    root5->left = new Node(9);
    root5->left->left = new Node(1);
    assert(findMax(root5) == 9);
    delete root5->left->left;
    delete root5->left;
    delete root5;

    // Test 6: Skewed tree (only right children) with all equal values
    Node* root6 = new Node(2);
    root6->right = new Node(2);
    root6->right->right = new Node(2);
    assert(findMax(root6) == 2);
    delete root6->right->right;
    delete root6->right;
    delete root6;

    return 0;
}

// The solution uses a breadth-first traversal (level-order) with a queue to visit every node exactly once. Start by initializing `max` with the root’s data. Then push the root into the queue. While the queue is not empty, pop the front node, compare its data with the current `max`, and update if larger. Then push its left child (if not null) and right child (if not null) into the queue. The loop ends when all nodes are visited. Edge cases: a single-node tree returns that node’s data; a tree where all values are equal returns that value; a tree with only left or only right children still works because the traversal checks both children independently. Time complexity is O(n) where n is the number of nodes, since each node is processed once. Space complexity is O(w) where w is the maximum width of the tree (queue size), which in worst case (a complete tree) is O(n/2) ≈ O(n), and in best case (a skewed tree) is O(1).
