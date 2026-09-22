Write a C++ function `bool isCompleteBinaryTree(const std::vector<int>& leftChild, const std::vector<int>& rightChild, int numNodes)` that returns `true` if the binary tree described by the two arrays (where `leftChild[i]` and `rightChild[i]` are the node indices of the left and right children of node `i`, with `0` meaning no child, and node indices are 1-based) is a **complete binary tree**, and `false` otherwise. A complete binary tree is one where all levels are completely filled except possibly the last level, and the last level has all nodes as far left as possible. The input is guaranteed to represent a valid binary tree (exactly one root, no cycles, each node has at most one parent). The function should use **level-order traversal (BFS)** and return `false` if it encounters a missing node before all `numNodes` nodes have been visited.

#include <cassert>
#include <vector>

// The function is declared above, but for the test we include it here (or assume it's in the same file).
bool isCompleteBinaryTree(const std::vector<int>& leftChild, const std::vector<int>& rightChild, int numNodes);

int main() {
    // Test 1: Perfect binary tree with 7 nodes (complete)
    std::vector<int> left1 = {0, 2, 4, 6, 0, 0, 0};
    std::vector<int> right1 = {0, 3, 5, 7, 0, 0, 0};
    assert(isCompleteBinaryTree(left1, right1, 7) == true);

    // Test 2: Tree with 4 nodes, complete (last level partially filled from left)
    // Nodes: 1->(2,3), 2->(4,0), 3->(0,0), 4->(0,0)
    std::vector<int> left2 = {0, 2, 4, 0, 0};
    std::vector<int> right2 = {0, 3, 0, 0, 0};
    assert(isCompleteBinaryTree(left2, right2, 4) == true);

    // Test 3: Tree with 4 nodes, NOT complete (node 3 has a left child but node 2 doesn't have any? Actually let's make node 3 have a child and node 2 not)
    // Nodes: 1->(2,3), 2->(0,0), 3->(4,0), 4->(0,0) => right side filled, not complete
    std::vector<int> left3 = {0, 2, 0, 4, 0};
    std::vector<int> right3 = {0, 3, 0, 0, 0};
    assert(isCompleteBinaryTree(left3, right3, 4) == false);

    // Test 4: Single node tree
    std::vector<int> left4 = {0, 0};
    std::vector<int> right4 = {0, 0};
    assert(isCompleteBinaryTree(left4, right4, 1) == true);

    // Test 5: Tree where a node has only right child (not complete)
    // Nodes: 1->(0,2), 2->(0,0) => only right child
    std::vector<int> left5 = {0, 0, 0};
    std::vector<int> right5 = {0, 2, 0};
    assert(isCompleteBinaryTree(left5, right5, 2) == false);

    // Test 6: Tree with 3 nodes, complete (root has left and right, left has left)
    // Nodes: 1->(2,3), 2->(4,0), 3->(0,0), 4->(0,0) but numNodes=3? Actually if numNodes=3, then 2->(4) is invalid. Let's use 3 nodes: 1->(2,3), 2->(0,0), 3->(0,0) complete.
    std::vector<int> left6 = {0, 2, 0, 0};
    std::vector<int> right6 = {0, 3, 0, 0};
    assert(isCompleteBinaryTree(left6, right6, 3) == true);

    // Test 7: Tree with 6 nodes, complete (last level has 3 nodes: left, middle, but not right)
    // Nodes: 1->(2,3), 2->(4,5), 3->(6,0), 4->0,5->0,6->0 => complete
    std::vector<int> left7 = {0, 2, 4, 6, 0, 0, 0};
    std::vector<int> right7 = {0, 3, 5, 0, 0, 0, 0};
    assert(isCompleteBinaryTree(left7, right7, 6) == true);

    // Test 8: Tree with 6 nodes, NOT complete (node 3 has right child instead of left)
    // Nodes: 1->(2,3), 2->(4,5), 3->(0,6) => not complete
    std::vector<int> left8 = {0, 2, 4, 0, 0, 0, 0};
    std::vector<int> right8 = {0, 3, 5, 6, 0, 0, 0};
    assert(isCompleteBinaryTree(left8, right8, 6) == false);

    // Test 9: Tree with 2 nodes where root has only left child (complete)
    std::vector<int> left9 = {0, 2, 0};
    std::vector<int> right9 = {0, 0, 0};
    assert(isCompleteBinaryTree(left9, right9, 2) == true);

    // Test 10: Tree with 2 nodes where root has only right child (not complete)
    std::vector<int> left10 = {0, 0, 0};
    std::vector<int> right10 = {0, 2, 0};
    assert(isCompleteBinaryTree(left10, right10, 2) == false);

    return 0;
}

#include <vector>
#include <queue>

// Returns true if the binary tree defined by leftChild and rightChild (1-based indices, 0 = no child) is a complete binary tree.
bool isCompleteBinaryTree(const std::vector<int>& leftChild, const std::vector<int>& rightChild, int numNodes) {
    // Find the root: the node that is never a child of any other node
    std::vector<bool> hasParent(numNodes + 1, false);
    for (int i = 1; i <= numNodes; ++i) {
        if (leftChild[i] != 0) hasParent[leftChild[i]] = true;
        if (rightChild[i] != 0) hasParent[rightChild[i]] = true;
    }
    int root = -1;
    for (int i = 1; i <= numNodes; ++i) {
        if (!hasParent[i]) {
            root = i;
            break;
        }
    }
    if (root == -1) return false; // Invalid tree (no root), but per problem statement this shouldn't happen

    // Level-order traversal using a queue
    std::queue<int> q;
    q.push(root);
    int visited = 0;
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        if (node == 0) return false; // Found a missing node before all nodes are visited
        ++visited;
        // Enqueue children (possibly 0)
        q.push(leftChild[node]);
        q.push(rightChild[node]);
        if (visited == numNodes) {
            // All nodes visited; if there are still non-zero nodes in the queue, it's not complete.
            // But by construction, after visiting all numNodes, any remaining non-zero would have been enqueued earlier.
            // For a complete tree, all remaining queue entries must be 0.
            while (!q.empty()) {
                if (q.front() != 0) return false;
                q.pop();
            }
            return true;
        }
    }
    return false; // Should never reach here if numNodes > 0
}

// The solution uses a queue for level-order traversal. First, determine the root by finding the node index that never appears as a child of any other node (since no parent pointers are given, we can mark all children as "has parent" and the one unmarked index is the root). Then enqueue the root and initialize a counter. While the counter is less than or equal to `numNodes` (or while the queue is non-empty), dequeue the front node. If the front node is `0` (meaning a missing child), return `false` immediately because the tree cannot be complete — a `0` would only be allowed after all nodes have been visited, meaning the last level is not fully filled from the left. Otherwise, enqueue both children (possibly `0`) and increment the counter. If the loop finishes without encountering a `0` before visiting all nodes, return `true`. Important edge cases: (1) a tree with only a root (numNodes=1) returns `true`; (2) if a node's child index is out of range (given valid input, this won't happen); (3) when the tree is a perfect binary tree, all nodes are visited with no `0` encountered. Time complexity is O(n) because each node is enqueued and dequeued exactly once. Space complexity is O(n) for the queue (worst-case: a full level holds about n/2 nodes) and O(n) for the mark array.
