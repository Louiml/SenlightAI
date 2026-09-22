Write a C++ function `std::vector<int> zigzagTraversal(const Node* root)` that performs a level-order traversal of a binary tree but alternates the direction of traversal at each level: left-to-right for the first level, right-to-left for the second, left-to-right for the third, and so on. Return the visited node values in that zigzag order as a `std::vector<int>`. The tree is represented by a struct `Node` with `int data`, `Node* left`, and `Node* right` (both pointers default to `nullptr`). Your function must handle an empty tree (return an empty vector), single-node trees, and trees where some nodes have only one child. Do not modify the tree.
// The standard approach uses two stacks to reverse the order of processing at each level. Start by pushing the root into the first stack `s1`. While either stack is non-empty, first process all nodes in `s1`: pop each node, append its data to the result, then push its left and right children (if they exist) into `s2`. This pushes left then right, so when `s2` is popped, nodes come out right-to-left for that level. Then, process all nodes in `s2`: pop each node, append its data, then push its right and left children (if they exist) into `s1` — note the order is reversed (right then left) so that the next level in `s1` is processed left-to-right. This alternation naturally produces the zigzag pattern. Edge cases: if `root` is `nullptr`, return an empty vector; if a node has only one child, only that child is pushed. The time complexity is O(n) where n is the number of nodes, since each node is pushed and popped exactly once. The space complexity is O(n) in the worst case (e.g., a full binary tree) because at any time all nodes of the current level are stored across the two stacks.
#include <vector>
#include <stack>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

// Return the values of the binary tree in zigzag (spiral) level order.
std::vector<int> zigzagTraversal(const Node* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::stack<const Node*> s1;
    std::stack<const Node*> s2;
    s1.push(root);

    while (!s1.empty() || !s2.empty()) {
        // Process current level from left to right, push children for next level (right-to-left)
        while (!s1.empty()) {
            const Node* current = s1.top();
            s1.pop();
            result.push_back(current->data);
            if (current->left != nullptr) {
                s2.push(current->left);
            }
            if (current->right != nullptr) {
                s2.push(current->right);
            }
        }
        // Process next level from right to left, push children for the following level (left-to-right)
        while (!s2.empty()) {
            const Node* current = s2.top();
            s2.pop();
            result.push_back(current->data);
            if (current->right != nullptr) {
                s1.push(current->right);
            }
            if (current->left != nullptr) {
                s1.push(current->left);
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty tree
    assert(zigzagTraversal(nullptr).empty());

    // Test 2: Single node
    Node* single = new Node(5);
    assert(zigzagTraversal(single) == std::vector<int>({5}));

    // Test 3: Simple balanced tree
    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    // Expected: level1 left->right: 1, level2 right->left: 3 2
    assert(zigzagTraversal(root) == std::vector<int>({1, 3, 2}));

    // Test 4: Deeper tree with missing children
    Node* root2 = new Node(10);
    root2->left = new Node(20);
    root2->right = new Node(30);
    root2->left->right = new Node(40);
    root2->right->left = new Node(50);
    root2->right->right = new Node(60);
    // Expected: level1: 10, level2: 30 20, level3: 40 50 60 (left->right)
    assert(zigzagTraversal(root2) == std::vector<int>({10, 30, 20, 40, 50, 60}));

    // Test 5: Left-skewed tree
    Node* root3 = new Node(1);
    root3->left = new Node(2);
    root3->left->left = new Node(3);
    // Expected: level1: 1, level2: 2, level3: 3
    assert(zigzagTraversal(root3) == std::vector<int>({1, 2, 3}));

    // Cleanup (not strictly necessary for asserts but for correctness)
    delete root3->left->left;
    delete root3->left;
    delete root3;
    delete root2->left->right;
    delete root2->right->left;
    delete root2->right->right;
    delete root2->left;
    delete root2->right;
    delete root2;
    delete single;

    return 0;
}
