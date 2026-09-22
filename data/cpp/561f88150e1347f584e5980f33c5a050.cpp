Given a binary tree whose nodes store integers, write a C++ function `std::vector<int> verticalOrderTraversal(const Node* root)` that returns the node values in vertical order from left to right, and for the same horizontal distance, from top to bottom (level order within each column). The input tree is non-empty, and each node's `data` is an integer that may be negative or duplicate. You must not modify the tree; only traverse it. The function should return a vector of integers in the required order.
The solution uses a map where the key is the horizontal distance (hd) from the root, and the value is a vector of node values encountered at that distance. We perform a level-order traversal using a queue that stores pairs of (node pointer, hd). The root starts at hd = 0; moving left subtracts 1, moving right adds 1. Because we traverse level by level (BFS), nodes with the same hd are naturally visited in top-to-bottom order, so appending to the vector for that hd preserves the correct vertical order. After the BFS completes, we iterate over the map keys in ascending order (which is automatic for `std::map`) and concatenate all the vectors into a single output vector. Edge cases: The tree is guaranteed non-empty, but the function should safely handle a null root by returning an empty vector (though not required by the task). Duplicate values and negative numbers are handled naturally. Time complexity is O(n log n) due to map insertion; space complexity is O(n) for the map and queue.
#include <vector>
#include <map>
#include <queue>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Return node values in vertical order (left to right, top to bottom per column).
std::vector<int> verticalOrderTraversal(const Node* root) {
    std::vector<int> result;
    if (root == nullptr) return result;

    std::map<int, std::vector<int>> columns;
    std::queue<std::pair<const Node*, int>> q;
    q.push({root, 0});

    while (!q.empty()) {
        auto [node, hd] = q.front();
        q.pop();
        columns[hd].push_back(node->data);
        if (node->left) q.push({node->left, hd - 1});
        if (node->right) q.push({node->right, hd + 1});
    }

    for (const auto& entry : columns) {
        for (int val : entry.second) {
            result.push_back(val);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// Node struct is taken from the solution.
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

std::vector<int> verticalOrderTraversal(const Node* root);

int main() {
    // Example tree from the problem snippet
    Node* root1 = new Node(20);
    root1->left = new Node(40);
    root1->right = new Node(50);
    root1->left->left = new Node(60);
    root1->left->right = new Node(50);
    root1->right->right = new Node(60);
    root1->right->left = new Node(40);
    std::vector<int> expected1 = {60, 40, 20, 50, 50, 40, 60};
    assert(verticalOrderTraversal(root1) == expected1);

    // Single node tree
    Node* root2 = new Node(7);
    assert(verticalOrderTraversal(root2) == std::vector<int>({7}));

    // Left-skewed tree (only left children)
    Node* root3 = new Node(1);
    root3->left = new Node(2);
    root3->left->left = new Node(3);
    std::vector<int> expected3 = {3, 2, 1};
    assert(verticalOrderTraversal(root3) == expected3);

    // Right-skewed tree (only right children)
    Node* root4 = new Node(1);
    root4->right = new Node(2);
    root4->right->right = new Node(3);
    std::vector<int> expected4 = {1, 2, 3};
    assert(verticalOrderTraversal(root4) == expected4);

    // Balanced tree with negative and duplicate values
    Node* root5 = new Node(0);
    root5->left = new Node(-5);
    root5->right = new Node(5);
    root5->left->right = new Node(-2);
    root5->right->left = new Node(2);
    root5->right->right = new Node(5);
    std::vector<int> expected5 = {-5, -2, 0, 2, 5, 5};
    assert(verticalOrderTraversal(root5) == expected5);

    return 0;
}
