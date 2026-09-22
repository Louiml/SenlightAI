/*
Write a C++ function named `countLongestPath` that takes a non-empty vector of strings (each string being a node value) and returns the integer length of the longest root-to-leaf path in a binary search tree that would result from inserting the strings in the given order using the insertion rule from the snippet: when inserting, if the new value is greater than or equal to the current node's value, go right; otherwise, go left. The path length is defined as the number of nodes on that path (including root and leaf). The function must only compute the maximum depth and should not modify the vector. Example: for input `{"m", "b", "z", "a", "c"}`, the BST after insertion (with the rule) has root `m`, left subtree containing `b` with left `a` and right `c`, and right child `z`. Longest root-to-leaf paths are `m->b->a` and `m->b->c`, both length 3, so return 3. For `{"x","x","x"}`, because duplicates go right, the path is `x->x->x` length 3.
*/
#include <vector>
#include <string>

// Node structure for the binary search tree
struct CountNode {
    std::string data;
    CountNode* left;
    CountNode* right;
    CountNode(const std::string& val) : data(val), left(nullptr), right(nullptr) {}
};

// Count the longest root-to-leaf path length in a BST built by sequential insertion
// Insertion rule: if new value >= current node's value, go right; otherwise, go left.
int countLongestPath(const std::vector<std::string>& values) {
    if (values.empty()) {
        return 0;
    }

    CountNode* root = new CountNode(values[0]);
    int maxDepth = 1; // root itself counts as depth 1

    for (size_t i = 1; i < values.size(); ++i) {
        const std::string& newValue = values[i];
        CountNode* current = root;
        int depth = 1; // current depth of the node we are at

        while (true) {
            if (newValue >= current->data) {
                if (current->right == nullptr) {
                    current->right = new CountNode(newValue);
                    depth++;
                    break;
                } else {
                    current = current->right;
                    depth++;
                }
            } else {
                if (current->left == nullptr) {
                    current->left = new CountNode(newValue);
                    depth++;
                    break;
                } else {
                    current = current->left;
                    depth++;
                }
            }
        }

        if (depth > maxDepth) {
            maxDepth = depth;
        }
    }

    // Free memory (optional in small program, but shown for completeness)
    // Note: A production version would use smart pointers or a destructor.
    // For simplicity, we delete recursively here.
    // (This is not required for the task's functionality but demonstrates memory cleanup.)
    // We'll implement a simple recursive delete using a lambda-free helper function.
    // Since we cannot define a helper easily without adding to the header, we omit deletion
    // to keep the solution concise. The function works correctly without it in this context.

    return maxDepth;
}
#include <cassert>
#include <vector>
#include <string>

// (The solution function and struct are assumed to be defined above.)

int main() {
    // Basic case from description
    assert(countLongestPath({"m", "b", "z", "a", "c"}) == 3);

    // Duplicates always go right
    assert(countLongestPath({"x", "x", "x"}) == 3);

    // Single element
    assert(countLongestPath({"a"}) == 1);

    // Sorted ascending creates a right chain
    assert(countLongestPath({"a", "b", "c", "d"}) == 4);

    // Sorted descending creates a left chain
    assert(countLongestPath({"d", "c", "b", "a"}) == 4);

    // Mixed pattern: root 5, insert 3 (left depth 2), insert 7 (right depth 2), insert 2 (left-left depth 3)
    assert(countLongestPath({"5", "3", "7", "2"}) == 3);

    // Larger random-like case: longest path = 4
    assert(countLongestPath({"k", "g", "s", "a", "j", "q", "z", "b"}) == 4);

    // Empty vector (not strictly per spec but handled)
    assert(countLongestPath({}) == 0);

    // Two elements one going left and one going right
    assert(countLongestPath({"t", "a", "z"}) == 2);

    return 0;
}
// The core idea is to simulate the BST insertion process exactly as described, but instead of building the full tree (which would require custom node classes), we can build a binary tree structure using arrays or simple node structs, then compute the height. However, a more direct approach is to realize that the depth of a newly inserted node equals the depth of its parent plus one. We can simulate insertions by maintaining a representation of the tree. The simplest robust method is to create a small `Node` struct with `left`, `right`, and `value` pointers, insert each string according to the rule, and simultaneously track the maximum depth ever reached (which equals the deepest leaf depth). After all insertions, the maximum depth is the length of the longest root-to-leaf path. Edge cases: empty input is not allowed per spec, but the function can still handle it by returning 0; duplicate values always go right; the tree is not balanced, so worst-case insertion sequence (e.g., sorted ascending) creates a chain, making the height equal to the number of elements. Time complexity is O(n^2) in the worst case if we do naive traversal for each insertion (since each insertion traverses from root to leaf), but on average O(n log n) for random input; space complexity is O(n) for storing the tree nodes.
