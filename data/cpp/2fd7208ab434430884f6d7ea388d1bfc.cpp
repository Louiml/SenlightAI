Write a C++ function that takes the root of a perfect binary tree (where every internal node has exactly two children and all leaves are at the same level) and populates each node's `next` pointer so that it points to the node immediately to its right on the same level. If there is no node to the right, the `next` pointer should be set to `nullptr`. The function should modify the tree in place and return the root. Assume the tree nodes are defined as a `Node` struct with integer `val`, and pointers to `left`, `right`, and `next` children (defaulting to `nullptr`). You must not use any extra space beyond the call stack (i.e., no level-order traversal with a queue). The function signature is `Node* connectPerfectTree(Node* root)`. The input tree is guaranteed to be perfect.

The solution uses a recursive depth-first traversal that exploits the perfect tree property. The key insight is that we can connect nodes in two distinct categories: (1) sibling nodes sharing the same parent (e.g., left child → right child), and (2) neighboring nodes on the same level but with different parents (e.g., the right child of node A to the left child of node A's next sibling). 

We define a helper that takes two nodes that are adjacent on the same level and connects them via `next`. Then we recursively process three pairs: the left subtree's two children, the right subtree's two children, and the cross-pair connecting the right child of the left subtree to the left child of the right subtree. The recursion naturally visits each node exactly once, and because the tree is perfect, we never need to check whether a `next` pointer already exists. The base case is when either node is null, which only happens at the leaves (since perfect trees have full depth). 

Alternatively, a more compact recursive solution processes each node directly: if a node has a left child, connect it to the right child; if a node has both a right child and a `next` pointer, connect the right child to the `next` node's left child. Then recurse on left and right children. This avoids passing pairs explicitly but has the same complexity.

Time complexity: Each node is visited once, so O(n) where n is the number of nodes. Space complexity: O(log n) due to the recursion stack depth for a perfect tree (height = log₂(n+1) - 1).

Edge cases: Empty tree (root is null) — return null immediately. A tree with only a root (height 0) — no connections needed, return the root unchanged. The algorithm assumes the tree is perfect; if not, it would still work for correctly structured subtrees but may skip some connections between cousins when a sibling is missing.

#include <cstddef>

struct Node {
    int val;
    Node* left;
    Node* right;
    Node* next;
    Node() : val(0), left(nullptr), right(nullptr), next(nullptr) {}
    Node(int _val) : val(_val), left(nullptr), right(nullptr), next(nullptr) {}
    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};

// Connects right sibling pointers for a perfect binary tree.
// Recursively links same-level nodes in-place. Returns the root.
Node* connectPerfectTree(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }

    // Helper that recursively connects two adjacent subtrees.
    // left and right are nodes that should be linked via next.
    auto connectSiblings = [&](auto&& self, Node* left, Node* right) -> void {
        if (left == nullptr || right == nullptr) {
            return;
        }
        left->next = right;
        // Connect children within the same parent
        self(self, left->left, left->right);
        self(self, right->left, right->right);
        // Connect the cross pair between the two different parents
        self(self, left->right, right->left);
    };

    connectSiblings(connectSiblings, root->left, root->right);
    return root;
}

#include <cassert>

int main() {
    // Helper to build a perfect tree of height 2 (nodes 1..7)
    //    1
    //   / \
    //  2   3
    // / \ / \
    //4  5 6  7
    Node* n4 = new Node(4);
    Node* n5 = new Node(5);
    Node* n6 = new Node(6);
    Node* n7 = new Node(7);
    Node* n2 = new Node(2, n4, n5, nullptr);
    Node* n3 = new Node(3, n6, n7, nullptr);
    Node* root = new Node(1, n2, n3, nullptr);

    connectPerfectTree(root);

    // Check connections level by level
    // Level 1 (just root): next should be nullptr
    assert(root->next == nullptr);

    // Level 2: 2->3, 3->nullptr
    assert(n2->next == n3);
    assert(n3->next == nullptr);

    // Level 3: 4->5, 5->6, 6->7, 7->nullptr
    assert(n4->next == n5);
    assert(n5->next == n6);
    assert(n6->next == n7);
    assert(n7->next == nullptr);

    // Test empty tree
    Node* empty = nullptr;
    assert(connectPerfectTree(empty) == nullptr);

    // Test single node tree
    Node* single = new Node(10);
    connectPerfectTree(single);
    assert(single->next == nullptr);

    // Clean up (not strictly necessary for test, but good practice)
    delete single;
    delete n7;
    delete n6;
    delete n5;
    delete n4;
    delete n3;
    delete n2;
    delete root;

    return 0;
}
