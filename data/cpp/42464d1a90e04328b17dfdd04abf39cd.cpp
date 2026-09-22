/*
Write a C++ function `std::vector<uint32_t> reconstruct_preorder(const std::vector<uint32_t>& postorder, const std::vector<uint32_t>& inorder)` that, given the postorder and inorder traversal sequences of a binary tree with unique `uint32_t` node values, returns the preorder traversal sequence of that tree. You may assume the inputs are valid (i.e., they represent some binary tree with the same set of unique values) and non-empty. The function should not modify the input vectors and should work efficiently for trees of size up to 10^5 nodes. The returned vector must contain all node values in preorder (root, left subtree, right subtree).
*/

#include <cstdint>
#include <vector>
#include <unordered_map>

// Given postorder and inorder traversals (unique node values), return preorder traversal.
std::vector<uint32_t> reconstruct_preorder(const std::vector<uint32_t>& postorder, const std::vector<uint32_t>& inorder) {
    const std::size_t n = postorder.size();
    std::unordered_map<uint32_t, std::size_t> in_index;
    for (std::size_t i = 0; i < n; ++i) {
        in_index[inorder[i]] = i;
    }

    std::vector<uint32_t> result;
    result.reserve(n);

    // Recursive helper: post segment [postLeft, postRight] corresponds to inorder segment [inLeft, inRight].
    // The helper uses a lambda to access result by reference.
    std::function<void(std::size_t, std::size_t, std::size_t, std::size_t)> build =
        [&](std::size_t postLeft, std::size_t postRight, std::size_t inLeft, std::size_t inRight) {
            if (inLeft > inRight) {
                return;
            }
            const uint32_t root_val = postorder[postRight];
            result.push_back(root_val);

            const std::size_t inRoot = in_index[root_val];
            const std::size_t left_size = inRoot - inLeft;

            // Left subtree: inorder [inLeft, inRoot-1], postorder [postLeft, postLeft + left_size - 1]
            if (left_size > 0) {
                build(postLeft, postLeft + left_size - 1, inLeft, inRoot - 1);
            }
            // Right subtree: inorder [inRoot+1, inRight], postorder [postLeft + left_size, postRight - 1]
            if (inRoot < inRight) {
                build(postLeft + left_size, postRight - 1, inRoot + 1, inRight);
            }
        };

    build(0, n - 1, 0, n - 1);
    return result;
}

#include <cassert>
#include <cstdint>
#include <vector>

// Include the solution here or assume it is already included.

int main() {
    // Test 1: Simple balanced tree
    std::vector<uint32_t> post1 = {4, 5, 2, 6, 7, 3, 1};
    std::vector<uint32_t> in1   = {4, 2, 5, 1, 6, 3, 7};
    std::vector<uint32_t> pre1  = {1, 2, 4, 5, 3, 6, 7};
    assert(reconstruct_preorder(post1, in1) == pre1);

    // Test 2: Left-skewed tree (each node has only left child)
    std::vector<uint32_t> post2 = {5, 4, 3, 2, 1};
    std::vector<uint32_t> in2   = {5, 4, 3, 2, 1};
    std::vector<uint32_t> pre2  = {1, 2, 3, 4, 5};
    assert(reconstruct_preorder(post2, in2) == pre2);

    // Test 3: Right-skewed tree (each node has only right child)
    std::vector<uint32_t> post3 = {1, 2, 3, 4, 5};
    std::vector<uint32_t> in3   = {1, 2, 3, 4, 5};
    std::vector<uint32_t> pre3  = {1, 2, 3, 4, 5};
    assert(reconstruct_preorder(post3, in3) == pre3);

    // Test 4: Single node
    std::vector<uint32_t> post4 = {42};
    std::vector<uint32_t> in4   = {42};
    std::vector<uint32_t> pre4  = {42};
    assert(reconstruct_preorder(post4, in4) == pre4);

    // Test 5: Two nodes, left child
    std::vector<uint32_t> post5 = {2, 1};
    std::vector<uint32_t> in5   = {2, 1};
    std::vector<uint32_t> pre5  = {1, 2};
    assert(reconstruct_preorder(post5, in5) == pre5);

    // Test 6: Two nodes, right child
    std::vector<uint32_t> post6 = {1, 2};
    std::vector<uint32_t> in6   = {1, 2};
    std::vector<uint32_t> pre6  = {1, 2};
    assert(reconstruct_preorder(post6, in6) == pre6);

    // Test 7: Random larger tree (values 1..7)
    std::vector<uint32_t> post7 = {7, 3, 4, 2, 6, 5, 1};
    std::vector<uint32_t> in7   = {3, 7, 4, 1, 6, 5, 2};
    std::vector<uint32_t> pre7  = {1, 4, 3, 7, 5, 6, 2};
    assert(reconstruct_preorder(post7, in7) == pre7);

    // Test 8: Empty? Not allowed per spec, but check that empty input is handled gracefully if called.
    // (We skip because the spec says non-empty.)

    return 0;
}

// The key insight is that in a postorder traversal, the last element is always the root of the (sub)tree. In the inorder traversal, the root splits the sequence into the left subtree's inorder (all elements to the left of the root) and the right subtree's inorder (all elements to the right). We can recursively reconstruct the tree and collect preorder output: first visit the root, then recursively reconstruct and visit the left subtree, then the right subtree. To avoid repeatedly searching for the root's position in inorder, we can precompute a hash map from value to its index in the inorder vector (since values are unique). The recursive helper function takes indices: `postLeft`, `postRight`, `inLeft`, `inRight` (all inclusive). The root is `postorder[postRight]`. Its position `inRoot` in inorder is obtained from the map. The left subtree has `inLeft` to `inRoot-1` for inorder, and the corresponding postorder segment is `postLeft` to `postLeft + (inRoot - inLeft - 1)`. The right subtree has inorder `inRoot+1` to `inRight`, and postorder segment `postLeft + (inRoot - inLeft)` to `postRight - 1`. Recursively process left and right subtrees after pushing the root to the result. Edge cases include empty subtrees (when `inLeft > inRight`), in which case we return immediately. The algorithm runs in O(n) time because each node is processed once, and the map lookup is O(1) on average. The space complexity is O(n) for the map and the recursion stack (worst-case O(n) for skewed tree).
