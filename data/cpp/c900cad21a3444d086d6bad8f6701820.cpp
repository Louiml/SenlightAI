Write a C++ function `void reconstructPreorder(const int* postorder, const int* inorder, int n)` that prints the preorder traversal of a binary tree given its postorder and inorder traversals. The tree has distinct integer node values, with both input arrays of size `n` (1 ≤ n ≤ 100). The function should output the preorder sequence separated by spaces, with a trailing space after the last element for simplicity. The postorder and inorder arrays are guaranteed to represent a valid binary tree with no duplicate values.

The key observation is that in a postorder traversal, the last element is always the root of the current subtree. In the inorder traversal, the root divides the sequence into the left subtree (elements before the root) and the right subtree (elements after the root). The algorithm recursively processes the postorder array by identifying the root as `postorder[n-1]`, finding its index in the inorder array, and then recursing on the left part (using the first part of postorder and the first part of inorder) and the right part (using the postorder segment after the left subtree and the inorder segment after the root). Since the number of nodes in the left subtree is exactly the index of the root in inorder, the postorder left segment has that many elements, and the right segment starts after those. Edge cases include when `n` is zero (do nothing) or when a subtree is empty (the recursion naturally handles it because the index will be 0 or `n-1`). The recursion depth is at most `O(n)` in the worst case (skewed tree), and each call does a linear search for the root, leading to a time complexity of `O(n^2)` for the worst case (skewed tree), and `O(n log n)` for balanced trees. Space complexity is `O(n)` for the recursion stack in the worst case.

#include <iostream>
#include <vector>
#include <algorithm>

// Prints the preorder traversal of a binary tree given its postorder and inorder traversals.
// postorder and inorder are arrays of size n (n >= 1) containing distinct values.
// The function outputs the preorder sequence values separated by spaces, with a trailing space.
void reconstructPreorder(const int* postorder, const int* inorder, int n) {
    if (n <= 0) {
        return;
    }

    const int rootValue = postorder[n - 1]; // Root of this subtree (last in postorder)
    std::cout << rootValue << " ";

    // Find the root's index in inorder to split into left and right subtrees.
    int rootIndex = 0;
    while (rootIndex < n && inorder[rootIndex] != rootValue) {
        ++rootIndex;
    }

    const int leftSize = rootIndex;
    const int rightSize = n - leftSize - 1;

    // Left subtree: first leftSize elements of both arrays.
    reconstructPreorder(postorder, inorder, leftSize);

    // Right subtree: postorder segment after left subtree, inorder segment after root.
    reconstructPreorder(postorder + leftSize, inorder + leftSize + 1, rightSize);
}

#include <cassert>
#include <sstream>
#include <string>

// Helper to capture output of reconstructPreorder and return as string (used in tests).
std::string callAndCapture(const std::vector<int>& post, const std::vector<int>& in) {
    std::streambuf* old = std::cout.rdbuf();
    std::ostringstream ss;
    std::cout.rdbuf(ss.rdbuf());
    reconstructPreorder(post.data(), in.data(), static_cast<int>(post.size()));
    std::cout.rdbuf(old);
    return ss.str();
}

int main() {
    // Basic case: balanced tree (postorder 4 5 2 6 7 3 1, inorder 4 2 5 1 6 3 7)
    {
        std::vector<int> post = {4, 5, 2, 6, 7, 3, 1};
        std::vector<int> in   = {4, 2, 5, 1, 6, 3, 7};
        assert(callAndCapture(post, in) == "1 2 4 5 3 6 7 ");
    }
    // Left-skewed tree (postorder 5 4 3 2 1, inorder 1 2 3 4 5)
    {
        std::vector<int> post = {5, 4, 3, 2, 1};
        std::vector<int> in   = {1, 2, 3, 4, 5};
        assert(callAndCapture(post, in) == "1 2 3 4 5 ");
    }
    // Right-skewed tree (postorder 1 2 3 4 5, inorder 1 2 3 4 5)
    {
        std::vector<int> post = {1, 2, 3, 4, 5};
        std::vector<int> in   = {1, 2, 3, 4, 5};
        assert(callAndCapture(post, in) == "1 2 3 4 5 ");
    }
    // Single node
    {
        std::vector<int> post = {42};
        std::vector<int> in   = {42};
        assert(callAndCapture(post, in) == "42 ");
    }
    // Two nodes (root left) (postorder 2 1, inorder 1 2)
    {
        std::vector<int> post = {2, 1};
        std::vector<int> in   = {1, 2};
        assert(callAndCapture(post, in) == "1 2 ");
    }
    // Two nodes (root right) (postorder 1 2, inorder 1 2) - actually same as skewed
    {
        std::vector<int> post = {1, 2};
        std::vector<int> in   = {1, 2};
        assert(callAndCapture(post, in) == "2 1 ");
    }
    // Larger random-ish tree with known preorder
    {
        std::vector<int> post = {8, 4, 5, 2, 6, 7, 3, 1};
        std::vector<int> in   = {8, 4, 2, 5, 1, 6, 3, 7};
        assert(callAndCapture(post, in) == "1 2 4 8 5 3 6 7 ");
    }
    return 0;
}
