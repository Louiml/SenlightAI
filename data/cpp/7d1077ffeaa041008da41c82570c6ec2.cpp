/*
Write a C++ function that, given a vector of integers representing the values to be inserted into a binary search tree, returns a string that contains the in-order and level-order traversal results on separate lines, with the values in each traversal separated by a single space. The function must build the BST from the given array, perform an in-order traversal (left, root, right) and a level-order traversal (breadth-first) after all insertions, and format the output exactly as: "Inorder: v1 v2 ... vn\nLevelorder: v1 v2 ... vn\n". Handle duplicate keys by ignoring them (only the first occurrence is inserted). The input vector may be empty (then output just the headers with no numbers). Do not use recursion beyond normal depth – iterative level-order is required; in-order may be recursive or iterative, but recursion depth is fine for typical constraints. The function signature must be `std::string traversalString(const std::vector<int>& values)`.
*/
#include <string>
#include <vector>
#include <queue>
#include <sstream>

// BST node structure
struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Insert a key into the BST; duplicates are ignored.
Node* insert(Node* root, int key) {
    if (root == nullptr) {
        return new Node(key);
    }
    if (key < root->data) {
        root->left = insert(root->left, key);
    } else if (key > root->data) {
        root->right = insert(root->right, key);
    }
    // If equal, do nothing.
    return root;
}

// Collect in-order traversal values.
void inorderCollect(Node* root, std::vector<int>& result) {
    if (root == nullptr) {
        return;
    }
    inorderCollect(root->left, result);
    result.push_back(root->data);
    inorderCollect(root->right, result);
}

// Collect level-order traversal values using a queue.
void levelOrderCollect(Node* root, std::vector<int>& result) {
    if (root == nullptr) {
        return;
    }
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        result.push_back(current->data);
        if (current->left) {
            q.push(current->left);
        }
        if (current->right) {
            q.push(current->right);
        }
    }
}

// Build a BST from the given values and return formatted traversal string.
std::string traversalString(const std::vector<int>& values) {
    Node* root = nullptr;
    for (int val : values) {
        root = insert(root, val);
    }
    
    std::vector<int> inResult, levelResult;
    inorderCollect(root, inResult);
    levelOrderCollect(root, levelResult);
    
    std::ostringstream oss;
    oss << "Inorder:";
    for (int v : inResult) {
        oss << " " << v;
    }
    oss << "\nLevelorder:";
    for (int v : levelResult) {
        oss << " " << v;
    }
    oss << "\n";
    return oss.str();
}
#include <cassert>
#include <vector>
#include <string>

// Declaration of the function under test
std::string traversalString(const std::vector<int>& values);

int main() {
    // Test 1: Basic BST with multiple values
    std::vector<int> v1 = {5, 3, 8, 1, 4, 7, 9};
    std::string res1 = traversalString(v1);
    assert(res1 == "Inorder: 1 3 4 5 7 8 9\nLevelorder: 5 3 8 1 4 7 9\n");
    
    // Test 2: Empty input
    std::vector<int> v2;
    std::string res2 = traversalString(v2);
    assert(res2 == "Inorder:\nLevelorder:\n");
    
    // Test 3: Single element
    std::vector<int> v3 = {42};
    std::string res3 = traversalString(v3);
    assert(res3 == "Inorder: 42\nLevelorder: 42\n");
    
    // Test 4: Duplicates are ignored
    std::vector<int> v4 = {10, 5, 10, 5, 20};
    std::string res4 = traversalString(v4);
    assert(res4 == "Inorder: 5 10 20\nLevelorder: 10 5 20\n");
    
    // Test 5: Skewed tree (all descending)
    std::vector<int> v5 = {9, 8, 7, 6};
    std::string res5 = traversalString(v5);
    assert(res5 == "Inorder: 6 7 8 9\nLevelorder: 9 8 7 6\n");
    
    // Test 6: Negative and zero values
    std::vector<int> v6 = {0, -3, 5, -1, 2};
    std::string res6 = traversalString(v6);
    assert(res6 == "Inorder: -3 -1 0 2 5\nLevelorder: 0 -3 5 -1 2\n");
    
    // Test 7: Larger balanced tree
    std::vector<int> v7 = {15, 10, 20, 8, 12, 17, 25, 6, 11, 16, 27};
    std::string res7 = traversalString(v7);
    assert(res7 == "Inorder: 6 8 10 11 12 15 16 17 20 25 27\nLevelorder: 15 10 20 8 12 17 25 6 11 16 27\n");
    
    return 0;
}
// The solution involves building a standard binary search tree node structure, then inserting each integer from the vector one by one: if the value is less than the current node’s data, recurse left; if greater, recurse right; if equal, do nothing (to ignore duplicates). After all insertions, we perform an in-order traversal using recursion (or an explicit stack) to collect the sorted sequence, and a level-order traversal using a queue that processes nodes level by level, pushing left and right children for each visited node. The final string is built by joining both sequences with spaces and separating the two lines with a newline. Edge cases: empty input (return just headers with no numbers), duplicate values (ignore), only one node (both traversals return that single value), and skewed trees (level-order still works correctly). Time complexity is O(n) for building the tree (if it stays balanced, but worst-case O(n^2) for a skewed tree) and O(n) for each traversal, so total O(n^2) worst-case. Space complexity: O(n) for the tree plus O(n) for the queue and recursion stack in the worst case.
