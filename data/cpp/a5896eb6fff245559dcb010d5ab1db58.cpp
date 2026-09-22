Write a standalone C++ function `collectLevelOrder(const Node* root)` that takes the root of a binary tree (where each `Node` contains an `int data` and pointers `left` and `right`) and returns a `std::vector<std::vector<int>>` containing the values of the tree in level-order (breadth-first) traversal, grouped by level. The function must be `const`-correct (i.e., it must not modify the tree, and the root parameter should be a pointer to const). The returned vector should have one inner vector per level, with values in left-to-right order at that level. If the tree is empty (root is `nullptr`), return an empty `std::vector<std::vector<int>>`. You may assume the node structure is provided exactly as in the snippet. Do **not** write a `main` function in the solution; only the free function and necessary helper (if any) are required.

#include <cassert>
#include <vector>

// Node definition must be included for the test file (provided in Solution)

int main() {
    // Test 1: Empty tree
    std::vector<std::vector<int>> res1 = collectLevelOrder(nullptr);
    assert(res1.empty());

    // Test 2: Single node
    Node* single = new Node(7);
    std::vector<std::vector<int>> res2 = collectLevelOrder(single);
    assert(res2.size() == 1);
    assert(res2[0] == std::vector<int>({7}));
    delete single;

    // Test 3: Balanced tree from snippet
    //      1
    //    /   \
    //   2     3
    //  / \     \
    // 4   5     6
    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->right = new Node(6);

    std::vector<std::vector<int>> expected = {
        {1},
        {2, 3},
        {4, 5, 6}
    };
    assert(collectLevelOrder(root) == expected);
    delete root->left->left;
    delete root->left->right;
    delete root->left;
    delete root->right->right;
    delete root->right;
    delete root;

    // Test 4: Left-skewed tree
    Node* leftSkew = new Node(10);
    leftSkew->left = new Node(20);
    leftSkew->left->left = new Node(30);
    std::vector<std::vector<int>> res4 = collectLevelOrder(leftSkew);
    assert(res4 == std::vector<std::vector<int>>({{10}, {20}, {30}}));
    delete leftSkew->left->left;
    delete leftSkew->left;
    delete leftSkew;

    // Test 5: Right-skewed tree
    Node* rightSkew = new Node(1);
    rightSkew->right = new Node(2);
    rightSkew->right->right = new Node(3);
    std::vector<std::vector<int>> res5 = collectLevelOrder(rightSkew);
    assert(res5 == std::vector<std::vector<int>>({{1}, {2}, {3}}));
    delete rightSkew->right->right;
    delete rightSkew->right;
    delete rightSkew;

    // Test 6: Tree with only left children in one branch, right in another
    Node* mixed = new Node(5);
    mixed->left = new Node(6);
    mixed->right = new Node(7);
    mixed->left->left = new Node(8);
    mixed->right->right = new Node(9);
    std::vector<std::vector<int>> res6 = collectLevelOrder(mixed);
    assert(res6 == std::vector<std::vector<int>>({{5}, {6, 7}, {8, 9}}));
    delete mixed->left->left;
    delete mixed->right->right;
    delete mixed->left;
    delete mixed->right;
    delete mixed;

    return 0;
}

#include <vector>
#include <queue>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Returns level-order traversal of the binary tree grouped by level.
std::vector<std::vector<int>> collectLevelOrder(const Node* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<const Node*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        std::vector<int> currentLevel;
        currentLevel.reserve(levelSize);

        for (int i = 0; i < levelSize; ++i) {
            const Node* current = q.front();
            q.pop();
            currentLevel.push_back(current->data);

            if (current->left != nullptr) {
                q.push(current->left);
            }
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }

        result.push_back(std::move(currentLevel));
    }

    return result;
}

// The core algorithm is a standard level-order traversal using a queue. Start by pushing the root (if not null) into a `std::queue<const Node*>`. Then, while the queue is not empty, record the current queue size `sz` (number of nodes at the current level). Pop `sz` nodes one by one, pushing their left and right children (if they exist) into the queue for the next level, and collecting their `data` into a temporary vector. After processing all `sz` nodes, push that temporary vector into the result vector. This ensures that each inner vector corresponds to exactly one level and preserves left-to-right order. Edge cases: (1) Empty tree: immediately return an empty vector. (2) Single node: the loop processes one level with one value, result is `{{root->data}}`. (3) Unbalanced trees: the queue handles arbitrary shapes correctly; null children are simply not pushed. Time complexity is `O(n)` where `n` is the number of nodes, because each node is visited exactly once. Space complexity is `O(w)` for the queue, where `w` is the maximum width of the tree (at most `n` in the worst case for a complete tree at the deepest level), plus `O(n)` for the returned result vector itself.
