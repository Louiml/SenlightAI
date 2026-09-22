/*
Implement a C++ function `buildSegmentTree` that, given two integers `start` and `end` representing the inclusive bounds of an interval, constructs and returns the root pointer to a full binary segment tree. Each node in the tree must be an instance of `SegmentTreeNode` (as defined below), storing its interval `[start, end]` and having left and right children covering the left half `[start, mid]` and right half `[mid+1, end]`, where `mid = (start + end) / 2`. If `start > end`, the function should return `nullptr`. The tree must be built recursively such that every leaf corresponds to a single point interval `[i, i]`. The function signature must be `SegmentTreeNode* buildSegmentTree(int start, int end);` and you must include the provided `SegmentTreeNode` class definition in your solution.
*/
#include <cstddef>

// Definition for a segment tree node.
class SegmentTreeNode {
public:
    int start, end;
    SegmentTreeNode *left, *right;
    SegmentTreeNode(int start, int end) {
        this->start = start;
        this->end = end;
        this->left = this->right = NULL;
    }
};

// Build a full segment tree over the inclusive interval [start, end].
// Returns nullptr if start > end.
SegmentTreeNode* buildSegmentTree(int start, int end) {
    if (start > end) {
        return nullptr;
    }
    SegmentTreeNode* root = new SegmentTreeNode(start, end);
    if (start == end) {
        return root;
    }
    int mid = start + (end - start) / 2;
    root->left = buildSegmentTree(start, mid);
    root->right = buildSegmentTree(mid + 1, end);
    return root;
}
#include <cassert>
#include <unordered_set>

// Helper to check tree structure: returns count of nodes and verifies interval correctness.
int validate(SegmentTreeNode* node, int expectedStart, int expectedEnd) {
    if (node == nullptr) {
        return 0;
    }
    assert(node->start == expectedStart);
    assert(node->end == expectedEnd);
    int leftCount = 0, rightCount = 0;
    if (node->start < node->end) {
        int mid = node->start + (node->end - node->start) / 2;
        leftCount = validate(node->left, node->start, mid);
        rightCount = validate(node->right, mid + 1, node->end);
    } else {
        assert(node->left == nullptr);
        assert(node->right == nullptr);
    }
    return 1 + leftCount + rightCount;
}

// Helper to count leaf nodes (start == end).
int countLeaves(SegmentTreeNode* node) {
    if (node == nullptr) {
        return 0;
    }
    if (node->start == node->end) {
        return 1;
    }
    return countLeaves(node->left) + countLeaves(node->right);
}

int main() {
    // Test 1: Invalid interval returns nullptr.
    assert(buildSegmentTree(5, 3) == nullptr);

    // Test 2: Single point tree.
    SegmentTreeNode* t1 = buildSegmentTree(3, 3);
    assert(t1 != nullptr);
    assert(validate(t1, 3, 3) == 1);
    assert(countLeaves(t1) == 1);
    assert(t1->left == nullptr && t1->right == nullptr);
    delete t1;

    // Test 3: Small interval [0, 3].
    SegmentTreeNode* t2 = buildSegmentTree(0, 3);
    assert(t2 != nullptr);
    assert(validate(t2, 0, 3) == 7); // Full binary tree with 4 leaves has 7 nodes.
    assert(countLeaves(t2) == 4);
    // Spot-check the root's children intervals.
    assert(t2->left->start == 0 && t2->left->end == 1);
    assert(t2->right->start == 2 && t2->right->end == 3);
    delete t2;

    // Test 4: Odd-length interval [2, 8].
    SegmentTreeNode* t3 = buildSegmentTree(2, 8);
    assert(validate(t3, 2, 8) == 15); // 7 intervals, full binary tree with 7 leaves has 15 nodes.
    assert(countLeaves(t3) == 7);
    delete t3;

    // Test 5: Negative and zero range [ -3, 1 ].
    SegmentTreeNode* t4 = buildSegmentTree(-3, 1);
    assert(validate(t4, -3, 1) == 9); // 5 leaves => 9 nodes.
    assert(countLeaves(t4) == 5);
    delete t4;

    // Test 6: Large interval to ensure recursion depth works.
    SegmentTreeNode* t5 = buildSegmentTree(0, 100000);
    // Node count should be 2 * 100001 - 1 = 200001.
    assert(validate(t5, 0, 100000) == 200001);
    assert(countLeaves(t5) == 100001);
    delete t5;

    return 0;
}
// The solution uses a recursive divide-and-conquer approach. Starting from the full interval `[start, end]`, if `start > end`, we return `nullptr` as an invalid empty tree. For a valid interval, we create a new `SegmentTreeNode` with the given start and end. If `start == end`, it is a leaf node, so we simply return it. Otherwise, we compute the midpoint `mid = (start + end) / 2` and recursively construct the left subtree for `[start, mid]` and the right subtree for `[mid+1, end]`. This yields a balanced binary tree where each internal node splits its interval as evenly as possible. The recursion terminates because each recursive call reduces the interval length by at least half (when the interval length is even) or to nearly half (when odd), eventually reaching length 1 intervals. Edge cases include `start > end` (invalid input returning `nullptr`), and `start == end` (leaf node). The time complexity is `O(n)` where `n = end - start + 1`, because each interval point corresponds to exactly one leaf node and the tree has roughly `2n - 1` nodes total, each created in constant time. The space complexity is `O(n)` for storing the tree nodes, plus `O(log n)` for the recursion call stack depth.
