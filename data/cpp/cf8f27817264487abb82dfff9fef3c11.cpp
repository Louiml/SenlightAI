Write a C++ function that takes the root of a binary search tree (BST) constructed from integer values and returns a vector of integers representing the left view of the tree, i.e., the first node seen at each level when traversing from left to right. The function should work for both empty and non-empty trees, and should use an iterative level-order traversal approach (queue-based) rather than recursion. The BST may contain duplicate values, and insertion order is arbitrary; the left view is determined by tree structure, not insertion sequence. The function must not modify the tree and should be const-correct.

// The left view of a binary tree is the set of nodes visible when the tree is viewed from the left side; that is, for each level (depth), the leftmost node at that level is included. The most straightforward iterative method uses a queue for level-order (BFS) traversal. At each level, we record the number of nodes currently in the queue (which equals the number of nodes at that level). We then process exactly that many nodes, popping each from the front. For the first node processed at each level (index 0), we add its data to the result. Then we enqueue its left child and right child (if they exist) for the next level. This process repeats until the queue is empty. Edge cases: an empty tree should return an empty vector; a single-node tree returns that node; skewed trees (all left or all right) work fine because the level size is 1. Time complexity is O(n) where n is the number of nodes, as each node is processed exactly once. Space complexity is O(w) where w is the maximum width of the tree, which can be up to O(n) in the worst case (e.g., a complete tree) for the queue. No recursion is needed, and no extra data structures beyond the result vector and queue are used.

#include <queue>
#include <vector>

// Node structure for a binary search tree
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

// Return the left view of the binary tree as a vector of node values.
// Uses iterative level-order traversal (BFS) with a queue.
std::vector<int> leftView(const Node* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result; // empty tree
    }

    std::queue<const Node*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        for (int i = 0; i < levelSize; ++i) {
            const Node* current = q.front();
            q.pop();

            // First node at this level is the leftmost
            if (i == 0) {
                result.push_back(current->data);
            }

            if (current->left != nullptr) {
                q.push(current->left);
            }
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }
    }

    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty tree
    {
        Node* root = nullptr;
        assert(leftView(root).empty());
    }

    // Test 2: Single node tree
    {
        Node* root = new Node(42);
        assert(leftView(root) == std::vector<int>{42});
        delete root;
    }

    // Test 3: Balanced tree (from the snippet)
    {
        Node* root = new Node(10);
        root->left = new Node(7);
        root->right = new Node(14);
        root->left->left = new Node(3);
        root->left->right = new Node(9);
        root->right->left = new Node(11);
        root->right->right = new Node(20);
        root->right->right->left = new Node(15);

        std::vector<int> expected = {10, 7, 3, 15};
        assert(leftView(root) == expected);

        // cleanup
        delete root->left->left;
        delete root->left->right;
        delete root->left;
        delete root->right->left;
        delete root->right->right->left;
        delete root->right->right;
        delete root->right;
        delete root;
    }

    // Test 4: Left-skewed tree
    {
        Node* root = new Node(5);
        root->left = new Node(4);
        root->left->left = new Node(3);
        assert(leftView(root) == std::vector<int>({5, 4, 3}));

        delete root->left->left;
        delete root->left;
        delete root;
    }

    // Test 5: Right-skewed tree
    {
        Node* root = new Node(1);
        root->right = new Node(2);
        root->right->right = new Node(3);
        assert(leftView(root) == std::vector<int>({1, 2, 3}));

        delete root->right->right;
        delete root->right;
        delete root;
    }

    // Test 6: Tree where left view is not just the leftmost branch
    {
        Node* root = new Node(1);
        root->left = new Node(2);
        root->right = new Node(3);
        root->left->right = new Node(4);
        root->right->left = new Node(5);
        root->right->right = new Node(6);
        // Levels: 1 | 2 (left of level 2), then at level3 leftmost is 4 (from node 2's right child), then at level4 leftmost is 5? Actually:
        // Level 0: {1} -> leftmost 1
        // Level 1: {2,3} -> leftmost 2
        // Level 2: {4,5,6} -> leftmost 4
        // So expected [1,2,4]
        assert(leftView(root) == std::vector<int>({1, 2, 4}));

        delete root->left->right;
        delete root->left;
        delete root->right->left;
        delete root->right->right;
        delete root->right;
        delete root;
    }

    return 0;
}
