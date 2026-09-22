// Write a C++ function named `levelOrderValues` that takes a pointer to the root of a binary tree (where each node contains an integer value and pointers to its left and right children) and returns a `std::vector<int>` containing the node values in level order (i.e., top-to-bottom, left-to-right across each level). The function must handle an empty tree (return an empty vector) and must not modify the tree. The tree may be unbalanced, and nodes can contain any integer values, including duplicates and negative numbers. Assume the `TreeNode` struct is predefined as shown in the snippet.
#include <cassert>
#include <vector>

// Assuming the TreeNode struct and the levelOrderValues function are defined above.

int main() {
    // Test 1: Empty tree (null root)
    assert(levelOrderValues(nullptr).empty());

    // Test 2: Single node tree
    TreeNode n1(5);
    std::vector<int> expect2 = {5};
    assert(levelOrderValues(&n1) == expect2);

    // Test 3: Complete binary tree:     1
    //                                  / \
    //                                 2   3
    //                                / \ / \
    //                               4  5 6  7
    TreeNode t3_1(1), t3_2(2), t3_3(3), t3_4(4), t3_5(5), t3_6(6), t3_7(7);
    t3_1.left = &t3_2; t3_1.right = &t3_3;
    t3_2.left = &t3_4; t3_2.right = &t3_5;
    t3_3.left = &t3_6; t3_3.right = &t3_7;
    std::vector<int> expect3 = {1, 2, 3, 4, 5, 6, 7};
    assert(levelOrderValues(&t3_1) == expect3);

    // Test 4: Left-skewed tree: 10 -> 20 -> 30
    TreeNode t4_1(10), t4_2(20), t4_3(30);
    t4_1.left = &t4_2;
    t4_2.left = &t4_3;
    std::vector<int> expect4 = {10, 20, 30};
    assert(levelOrderValues(&t4_1) == expect4);

    // Test 5: Right-skewed tree with negative values: -1 -> -2 -> -3
    TreeNode t5_1(-1), t5_2(-2), t5_3(-3);
    t5_1.right = &t5_2;
    t5_2.right = &t5_3;
    std::vector<int> expect5 = {-1, -2, -3};
    assert(levelOrderValues(&t5_1) == expect5);

    // Test 6: Tree with only left children at different levels: 1 -> 2, and 1 -> 3 is absent, 2 has left 4
    TreeNode t6_1(1), t6_2(2), t6_4(4);
    t6_1.left = &t6_2;
    t6_2.left = &t6_4;
    std::vector<int> expect6 = {1, 2, 4};
    assert(levelOrderValues(&t6_1) == expect6);

    // Test 7: Tree with duplicates and missing children:      7
    //                                                       /   \
    //                                                      7     7
    //                                                             \
    //                                                              7
    TreeNode t7_1(7), t7_2(7), t7_3(7), t7_4(7);
    t7_1.left = &t7_2;
    t7_1.right = &t7_3;
    t7_3.right = &t7_4;
    std::vector<int> expect7 = {7, 7, 7, 7};
    assert(levelOrderValues(&t7_1) == expect7);

    return 0;
}
#include <vector>
#include <queue>

// TreeNode definition as per the problem statement
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Perform level order traversal, returning node values top-to-bottom, left-to-right.
std::vector<int> levelOrderValues(const TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<const TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        const TreeNode* current = q.front();
        q.pop();
        result.push_back(current->val);

        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }

    return result;
}
// The core of the solution is breadth-first search (BFS) using a queue. Start by checking if the root is `nullptr`; if so, return an empty vector. Otherwise, push the root into a queue. While the queue is not empty, pop the front node, record its value in the result vector, and then push its left and right children (if they exist) into the queue. Because the queue processes nodes in FIFO order, nodes are visited level by level, from left to right. Duplicate values are handled naturally since each node is visited exactly once. The algorithm works for any tree shape, including skewed or complete trees, with no special case needed except the null root check. Time complexity is O(n) where n is the number of nodes, as each node is enqueued and dequeued once. Space complexity is O(w) where w is the maximum width of the tree (the largest number of nodes at any level), but in the worst case (a completely unbalanced tree), it is O(n) because the queue can hold at most one node at a time; for a balanced tree, the queue holds at most the number of leaf nodes at the deepest level, which is O(n/2) = O(n). The function does not mutate the tree, so we can pass the root as a `const TreeNode*` for correct const-correctness.
