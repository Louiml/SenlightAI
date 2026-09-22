Write a C++ function named `binaryTreeMaxDepth` that takes a vector of integers representing a level-order traversal of a binary tree (where `-1` denotes a null node) and returns the maximum depth (number of nodes along the longest root-to-leaf path). The tree is complete in the sense that all non-null nodes are given in level order, and a missing child is represented by `-1`. For example, the input `{1,2,3,4,-1,-1,5}` corresponds to a tree with root 1, left child 2, right child 3, left child of 2 is 4, and right child of 3 is 5, so the maximum depth is 3. The tree may be empty (vector empty) in which case return 0. The vector is guaranteed to be a valid representation of a binary tree with no missing intermediate nodes.
// We can solve this by simulating a level-order traversal using a queue that stores indices into the vector. Start by enqueueing index 0 (root) and a depth value of 1. While the queue is not empty, dequeue an index and its associated depth. Update the maximum depth encountered so far. Then, for the left child (index `2*i+1`) and right child (index `2*i+2`), check if the index is within the vector bounds and if the value at that index is not `-1` (null). If valid, enqueue that index with depth+1. Continue until all nodes are processed. Edge cases: empty vector returns 0; a single root node with no children returns depth 1; a node may have only one child (the other is `-1`). Since we only traverse each node once, time complexity is O(n) where n is the number of nodes in the vector (including null markers), and space complexity is O(n) in the worst case for the queue.
#include <vector>
#include <queue>
#include <utility>

// Given a level-order representation of a binary tree where -1 denotes a null node,
// return the maximum depth (number of nodes on the longest root-to-leaf path).
// An empty tree (empty vector) has depth 0.
int binaryTreeMaxDepth(const std::vector<int>& levelOrder) {
    if (levelOrder.empty()) {
        return 0;
    }

    std::queue<std::pair<int, int>> queue; // (index, depth)
    queue.push({0, 1});
    int maxDepth = 1;

    while (!queue.empty()) {
        int index = queue.front().first;
        int depth = queue.front().second;
        queue.pop();

        if (depth > maxDepth) {
            maxDepth = depth;
        }

        int leftIndex = 2 * index + 1;
        int rightIndex = 2 * index + 2;

        if (leftIndex < static_cast<int>(levelOrder.size()) && levelOrder[leftIndex] != -1) {
            queue.push({leftIndex, depth + 1});
        }
        if (rightIndex < static_cast<int>(levelOrder.size()) && levelOrder[rightIndex] != -1) {
            queue.push({rightIndex, depth + 1});
        }
    }

    return maxDepth;
}
#include <cassert>
#include <vector>

// The solution function is declared above; include the declaration or define it here.
// For the test, we assume the function is available.

int main() {
    // Empty tree
    assert(binaryTreeMaxDepth({}) == 0);

    // Single root
    assert(binaryTreeMaxDepth({5}) == 1);

    // Root with left child only
    assert(binaryTreeMaxDepth({1, 2, -1}) == 2);

    // Root with right child only
    assert(binaryTreeMaxDepth({1, -1, 3}) == 2);

    // Full binary tree of height 3 (nodes 1-7)
    assert(binaryTreeMaxDepth({1,2,3,4,5,6,7}) == 3);

    // Example from task: {1,2,3,4,-1,-1,5} -> depth 3
    assert(binaryTreeMaxDepth({1,2,3,4,-1,-1,5}) == 3);

    // Deeper tree with missing nodes but non-null leaf at depth 4
    // Structure: 1->2->4->8 (left chain), others null
    // Level order: 1,2,-1,4,-1,-1,-1,8
    assert(binaryTreeMaxDepth({1,2,-1,4,-1,-1,-1,8}) == 4);

    // Tree with no leaves beyond root but many null markers
    // Level order: 1,-1,-1 (nulls for children)
    assert(binaryTreeMaxDepth({1,-1,-1}) == 1);

    // Large complete tree of height 4 (15 nodes)
    std::vector<int> fullTree;
    for (int i = 1; i <= 15; ++i) fullTree.push_back(i);
    assert(binaryTreeMaxDepth(fullTree) == 4);

    return 0;
}
