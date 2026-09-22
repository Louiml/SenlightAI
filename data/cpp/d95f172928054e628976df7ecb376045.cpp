/*
Write a C++ function that takes the root of a perfect binary tree where every node has a `next` pointer initially set to `nullptr`, and populates each node's `next` pointer to point to its next right node in the same level. If there is no next right node (i.e., the rightmost node in a level), set `next` to `nullptr`. The function should return the root of the modified tree. You are given the `Node` structure with `val`, `left`, `right`, and `next` pointers; the tree is perfect (all internal nodes have both children, and all leaves are at the same depth). The function should work in-place without using extra space proportional to the tree size beyond a constant amount (though using a queue/level-order traversal is acceptable for correctness; you may also implement an O(1) space solution if desired). Handle the empty tree case by returning `nullptr`.
*/

#include <deque>

// Definition for a Node.
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

// Populate each next pointer to point to its next right node.
// If there is no next right node, the next pointer should be set to nullptr.
// Returns the root of the modified tree.
Node* connect(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }
    std::deque<Node*> queue;
    queue.push_back(root);
    while (!queue.empty()) {
        int level_size = static_cast<int>(queue.size());
        for (int i = 0; i < level_size; ++i) {
            Node* current = queue.front();
            queue.pop_front();
            // If not the last node in this level, link to the next in queue.
            current->next = (i < level_size - 1) ? queue.front() : nullptr;
            // Add children for the next level (perfect tree guarantees both exist).
            if (current->left != nullptr) {
                queue.push_back(current->left);
                queue.push_back(current->right);
            }
        }
    }
    return root;
}

#include <cassert>

// Helper to create a perfect tree for testing (small size).
Node* createPerfectTree(int depth) {
    if (depth == 0) return nullptr;
    Node* root = new Node(1);
    std::deque<Node*> nodes;
    nodes.push_back(root);
    int val = 2;
    for (int d = 1; d < depth; ++d) {
        int size = static_cast<int>(nodes.size());
        for (int i = 0; i < size; ++i) {
            Node* n = nodes.front();
            nodes.pop_front();
            n->left = new Node(val++);
            n->right = new Node(val++);
            nodes.push_back(n->left);
            nodes.push_back(n->right);
        }
    }
    return root;
}

// Helper to verify that all next pointers in a level are correctly linked.
bool verifyNextPointers(Node* root) {
    if (root == nullptr) return true;
    std::deque<Node*> current_level;
    current_level.push_back(root);
    while (!current_level.empty()) {
        int size = static_cast<int>(current_level.size());
        for (int i = 0; i < size; ++i) {
            Node* n = current_level.front();
            current_level.pop_front();
            // Check next pointer: for last node in level, should be nullptr.
            if (i < size - 1) {
                if (n->next != current_level.front()) return false;
            } else {
                if (n->next != nullptr) return false;
            }
            // Add children for next level.
            if (n->left != nullptr) {
                current_level.push_back(n->left);
                current_level.push_back(n->right);
            }
        }
    }
    return true;
}

int main() {
    // Test empty tree
    assert(connect(nullptr) == nullptr);

    // Test single node tree
    Node* single = new Node(5);
    connect(single);
    assert(single->next == nullptr);
    delete single;

    // Test perfect tree of depth 2 (root + 2 children)
    Node* root2 = createPerfectTree(2);
    connect(root2);
    assert(verifyNextPointers(root2));
    // Check specific links: root->next is null; left child->next = right child
    assert(root2->next == nullptr);
    assert(root2->left->next == root2->right);
    assert(root2->right->next == nullptr);
    // Clean up (not fully implemented for brevity; but test logic is the focus)

    // Test perfect tree of depth 3 (7 nodes)
    Node* root3 = createPerfectTree(3);
    connect(root3);
    assert(verifyNextPointers(root3));
    // Check specific links
    assert(root3->next == nullptr);
    assert(root3->left->next == root3->right);
    assert(root3->right->next == nullptr);
    assert(root3->left->left->next == root3->left->right);
    assert(root3->left->right->next == root3->right->left);
    assert(root3->right->left->next == root3->right->right);
    assert(root3->right->right->next == nullptr);

    return 0;
}

// The solution uses a level-order (breadth-first) traversal with a queue (here implemented as a `deque`). For each level, we process nodes in order from left to right: we record the number of nodes `n` in the current level, then pop each node, set its `next` to the next node in the queue if it is not the last node of the level, otherwise set `next` to `nullptr`. After processing each node, we push its left and right children (if they exist) into the queue for the next level. Since the tree is perfect, we do not need to check for missing children. Edge cases: if the root is `nullptr`, return `nullptr` immediately; for a single-node tree, its `next` becomes `nullptr`. The algorithm runs in O(n) time because every node is visited exactly once, and O(n) space in the worst case due to the queue holding at most the width of the tree (which for a perfect tree is O(n) at the last level). The implementation ensures the order of processing is correct by using a `deque` and carefully handling the level boundary with a counter.
