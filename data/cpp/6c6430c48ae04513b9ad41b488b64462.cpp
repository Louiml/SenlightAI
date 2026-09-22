/*
Given a binary tree whose nodes are defined by `struct Node { int data; Node* left; Node* right; };`, write a C++ function `std::vector<int> reverseLevelOrderTraversal(const Node* root)` that returns the node values in reverse level-order: that is, traverse the tree level by level from top to bottom, but output the levels in reverse order (bottom-most level first, then the level above, etc.), and within each level, preserve the left-to-right order. For an empty tree (root is `nullptr`), return an empty vector. The tree may be skewed (all nodes on one side) or balanced, and levels may contain any number of nodes. The function must not modify the tree and must be `const`-correct.
*/
#include <vector>
#include <queue>
#include <stack>

struct Node {
    int data;
    Node* left;
    Node* right;
};

// Returns node values in reverse level-order (bottom level first, left-to-right within each level).
std::vector<int> reverseLevelOrderTraversal(const Node* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<const Node*> levelQueue;
    std::stack<std::vector<int>> levelStack;

    levelQueue.push(root);
    levelQueue.push(nullptr);  // Sentinel to mark the end of the first level

    std::vector<int> currentLevel;

    while (!levelQueue.empty()) {
        const Node* current = levelQueue.front();
        levelQueue.pop();

        if (current == nullptr) {
            // Finished a level
            levelStack.push(currentLevel);
            currentLevel.clear();
            if (!levelQueue.empty()) {
                levelQueue.push(nullptr);  // Mark end of next level
            }
        } else {
            currentLevel.push_back(current->data);
            if (current->left != nullptr) {
                levelQueue.push(current->left);
            }
            if (current->right != nullptr) {
                levelQueue.push(current->right);
            }
        }
    }

    // Pop levels from stack to reverse their order
    while (!levelStack.empty()) {
        const std::vector<int>& level = levelStack.top();
        result.insert(result.end(), level.begin(), level.end());
        levelStack.pop();
    }

    return result;
}
#include <cassert>
#include <vector>

// Reuse Node definition and reverseLevelOrderTraversal from solution
// (for brevity, they are assumed to be available above)

int main() {
    // Empty tree
    assert(reverseLevelOrderTraversal(nullptr).empty());

    // Single node
    Node n1{1, nullptr, nullptr};
    std::vector<int> r1 = reverseLevelOrderTraversal(&n1);
    assert((r1 == std::vector<int>{1}));

    // Complete tree of height 3 (levels: 1, 2-3, 4-5-6-7)
    Node n4{4, nullptr, nullptr};
    Node n5{5, nullptr, nullptr};
    Node n6{6, nullptr, nullptr};
    Node n7{7, nullptr, nullptr};
    Node n2{2, &n4, &n5};
    Node n3{3, &n6, &n7};
    Node n1root{1, &n2, &n3};
    std::vector<int> r2 = reverseLevelOrderTraversal(&n1root);
    assert((r2 == std::vector<int>{4,5,6,7,2,3,1}));

    // Left-skewed tree (only left children)
    Node a1{10, nullptr, nullptr};
    Node a2{20, &a1, nullptr};
    Node a3{30, &a2, nullptr};
    std::vector<int> r3 = reverseLevelOrderTraversal(&a3);
    assert((r3 == std::vector<int>{10,20,30}));

    // Right-skewed tree
    Node b1{5, nullptr, nullptr};
    Node b2{15, nullptr, &b1};
    Node b3{25, nullptr, &b2};
    std::vector<int> r4 = reverseLevelOrderTraversal(&b3);
    assert((r4 == std::vector<int>{5,15,25}));

    // Tree with only two levels (root with two children)
    Node c1{100, nullptr, nullptr};
    Node c2{200, nullptr, nullptr};
    Node croot{50, &c1, &c2};
    std::vector<int> r5 = reverseLevelOrderTraversal(&croot);
    assert((r5 == std::vector<int>{100,200,50}));

    // A more irregular tree: root=1, left=2 (with left=4), right=3 (with right=5)
    Node d4{4, nullptr, nullptr};
    Node d5{5, nullptr, nullptr};
    Node d2{2, &d4, nullptr};
    Node d3{3, nullptr, &d5};
    Node droot{1, &d2, &d3};
    std::vector<int> r6 = reverseLevelOrderTraversal(&droot);
    assert((r6 == std::vector<int>{4,5,2,3,1}));

    return 0;
}
// The solution uses a breadth-first traversal (level-order) to collect each level's node values separately, then reverses the list of levels. We use a queue to store nodes and a sentinel `nullptr` to mark the end of each level. Starting from the root, we push the root and a `nullptr`. While the queue is not empty, we pop the front. If it is `nullptr`, we have finished a level: we push that level’s collected vector into a stack (which will reverse the order of levels when popped), clear the temporary vector, and if the queue is not empty, push another `nullptr` sentinel. If the popped node is not `nullptr`, we append its data to the current level vector and enqueue its non-null children (left then right) to maintain left-to-right order. After the queue is empty, we pop vectors from the stack, appending their contents to the final result, giving bottom-up level order. Edge cases: empty tree (root `nullptr`) – the queue starts empty, so we return an empty vector without pushing any sentinel. A single node – one level with one value, stack holds it and returns it unchanged. Skewed trees – each level has exactly one node, still works. The time complexity is O(N) for visiting all nodes, and space complexity is O(N) in the worst case (for the queue, stack, and output) because we store all nodes’ values.
