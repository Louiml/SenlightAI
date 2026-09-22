// Given a binary tree represented by a level-order input sequence (where `-1` denotes a null child and the first value is the root), write a C++ function `int treeHeightFromLevelOrder(const std::vector<int>& levelOrder)` that reconstructs the tree implicitly (without building nodes) and returns its height, defined as the number of nodes on the longest root-to-leaf path. The function should handle empty input (return 0) and single-node trees (return 1). The input vector contains the complete level-order sequence including all `-1` placeholders for missing children, as produced by a standard queue-based level-order traversal. You may assume the input is valid and well-formed (i.e., for any non-null node at index `i`, its left child (if present) is at `2*i+1` and right child at `2*i+2`, and the array size matches the full binary tree representation).
#include <cassert>
#include <vector>

// The solution function is defined above; we only test it here.

int main() {
    // Example from the original snippet: 1 2 3 4 5 6 7 -1 -1 -1 -1 8 9 ...
    std::vector<int> tree1 = {1, 2, 3, 4, 5, 6, 7, -1, -1, -1, -1, 8, 9, -1, -1, -1, -1, -1, -1};
    assert(treeHeightFromLevelOrder(tree1) == 4); // Path: 1-3-6-8 or 1-3-6-9

    // Empty tree
    std::vector<int> emptyTree;
    assert(treeHeightFromLevelOrder(emptyTree) == 0);

    // Single node
    std::vector<int> singleNode = {42};
    assert(treeHeightFromLevelOrder(singleNode) == 1);

    // Left-skewed tree: 1-2-3-4
    std::vector<int> leftSkewed = {1, 2, -1, 3, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1, -1};
    assert(treeHeightFromLevelOrder(leftSkewed) == 4);

    // Right-skewed tree: 1-2-3-4 (right children only)
    std::vector<int> rightSkewed = {1, -1, 2, -1, -1, 3, -1, -1, -1, -1, -1, 4, -1, -1, -1};
    assert(treeHeightFromLevelOrder(rightSkewed) == 4);

    // Balanced tree of height 3: root and two full levels
    std::vector<int> balanced = {1, 2, 3, 4, 5, 6, 7};
    assert(treeHeightFromLevelOrder(balanced) == 3);

    // Root with only left child and that child has only left child
    std::vector<int> smallLeft = {1, 2, -1, 3, -1, -1, -1};
    assert(treeHeightFromLevelOrder(smallLeft) == 3);

    // Root with only right child and that child has only right child
    std::vector<int> smallRight = {1, -1, 2, -1, -1, -1, 3};
    assert(treeHeightFromLevelOrder(smallRight) == 3);

    // Tree with null placeholders but valid shape
    std::vector<int> withNulls = {1, 2, 3, -1, 4, -1, -1};
    assert(treeHeightFromLevelOrder(withNulls) == 3); // Path: 1-2-4

    return 0;
}
#include <vector>
#include <algorithm>

// Compute the height of a binary tree given its level-order array representation.
// The array uses -1 for null child placeholders. Returns 0 for empty input.
int treeHeightFromLevelOrder(const std::vector<int>& levelOrder) {
    if (levelOrder.empty() || levelOrder[0] == -1) {
        return 0;
    }
    
    // Recursive helper: compute height of subtree rooted at index i.
    // Returns 0 if index is out of bounds or node is null (-1).
    auto heightAt = [&](int i) -> int {
        if (i >= static_cast<int>(levelOrder.size()) || levelOrder[i] == -1) {
            return 0;
        }
        return 1 + std::max(heightAt(2 * i + 1), heightAt(2 * i + 2));
    };
    
    return heightAt(0);
}
// The key insight is that in a level-order representation stored in an array with `-1` for nulls, each node at index `i` has its left child at index `2*i+1` and right child at `2*i+2` (0-based indices), **if** those indices are within the array bounds and the child value is not `-1`. The height can be computed recursively without building the tree: define a helper that, given an index `i`, returns 0 if `i` is out of bounds or `levelOrder[i] == -1`; otherwise it returns 1 + max(helper(`2*i+1`), helper(`2*i+2`)). The main function simply calls this helper with index 0. Edge cases: empty vector → return 0; root is `-1` (not possible per problem statement but handle gracefully) → return 0; single node → returns 1. The recursion depth equals the tree height, which for a skewed tree could be O(n), but for a typical balanced tree is O(log n). Time complexity is O(n) because every index is visited at most once (each node’s children are checked exactly once). Space complexity is O(h) for the call stack (h = height), plus O(1) auxiliary ignoring input storage.
