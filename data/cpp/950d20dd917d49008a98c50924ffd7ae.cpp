Write a C++ function that builds a binary search tree (BST) from a given vector of integers and performs a level-order traversal (breadth-first traversal), returning a vector of integers in the order they are visited. The BST should be built according to the standard insertion rule: for each node, the left subtree contains only values less than the node's data, and the right subtree contains only values greater than or equal to the node's data. If the input vector is empty, the function should return an empty vector. The function must handle duplicate values correctly (they will go to the right subtree). The signature should be `std::vector<int> levelOrderTraversal(const std::vector<int>& input)`. The function should not print anything; it must return the result as a vector. Assume all integers are within the range of `int`.
// The solution builds the BST by iterating over the input vector and inserting each element using a standard iterative BST insert algorithm, similar to the provided snippet. The insertion logic: start from the root; if the root is null, the new node becomes the root; otherwise, traverse down the tree, going left if the new value is less than the current node's data, and right otherwise (including when equal), until reaching a null child pointer, then attach the new node there. After building the tree, perform a level-order traversal using a queue: start with the root, dequeue a node, add its data to the result vector, then enqueue its left and right children (if not null), continuing until the queue is empty. Edge cases include an empty input vector (return empty result) and duplicate values (they will be placed in the right subtree, so both duplicates will be visited but the traversal order will reflect the tree structure). Time complexity: O(n) for building (each insertion is O(h) where h is tree height, but on average O(log n) for random data; worst-case O(n^2) for sorted data) and O(n) for level-order traversal (each node visited once). Space complexity: O(n) for the tree and O(n) for the queue used in traversal.
#include <vector>
#include <queue>
#include <memory>

// TreeNode structure for the binary search tree.
struct TreeNode {
    int data;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;
    explicit TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Build a BST from input vector and return level-order traversal as a vector.
std::vector<int> levelOrderTraversal(const std::vector<int>& input) {
    std::vector<int> result;
    if (input.empty()) {
        return result;
    }

    // Build BST
    std::shared_ptr<TreeNode> root = nullptr;
    for (int val : input) {
        auto newNode = std::make_shared<TreeNode>(val);
        if (!root) {
            root = newNode;
        } else {
            std::shared_ptr<TreeNode> current = root;
            while (true) {
                if (val < current->data) {
                    if (current->left) {
                        current = current->left;
                    } else {
                        current->left = newNode;
                        break;
                    }
                } else {
                    if (current->right) {
                        current = current->right;
                    } else {
                        current->right = newNode;
                        break;
                    }
                }
            }
        }
    }

    // Level-order traversal
    std::queue<std::shared_ptr<TreeNode>> q;
    q.push(root);
    while (!q.empty()) {
        auto node = q.front();
        q.pop();
        result.push_back(node->data);
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }

    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic insertion and traversal
    std::vector<int> input1 = {10, 5, 15, 3, 7, 12, 18};
    std::vector<int> expected1 = {10, 5, 15, 3, 7, 12, 18};
    assert(levelOrderTraversal(input1) == expected1);

    // Test 2: Duplicate values (go to right)
    std::vector<int> input2 = {5, 5, 5};
    std::vector<int> expected2 = {5, 5, 5};
    assert(levelOrderTraversal(input2) == expected2);

    // Test 3: Single element
    std::vector<int> input3 = {42};
    std::vector<int> expected3 = {42};
    assert(levelOrderTraversal(input3) == expected3);

    // Test 4: Empty input
    std::vector<int> input4 = {};
    assert(levelOrderTraversal(input4).empty());

    // Test 5: Already sorted ascending (right-skewed)
    std::vector<int> input5 = {1, 2, 3, 4};
    std::vector<int> expected5 = {1, 2, 3, 4};
    assert(levelOrderTraversal(input5) == expected5);

    // Test 6: Sorted descending (left-skewed)
    std::vector<int> input6 = {4, 3, 2, 1};
    std::vector<int> expected6 = {4, 3, 2, 1};
    assert(levelOrderTraversal(input6) == expected6);

    // Test 7: Mixed with negative numbers
    std::vector<int> input7 = {-3, 7, -10, 0, 5};
    std::vector<int> expected7 = {-3, -10, 7, 0, 5};
    assert(levelOrderTraversal(input7) == expected7);

    // Test 8: Larger random tree
    std::vector<int> input8 = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    std::vector<int> expected8 = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    assert(levelOrderTraversal(input8) == expected8);

    return 0;
}
