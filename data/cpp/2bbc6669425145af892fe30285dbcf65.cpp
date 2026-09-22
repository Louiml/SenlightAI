// Write a C++ function named `postorderUsingTwoStacks` that takes a binary tree constructed from a level-order vector representation (where `-1` denotes `NULL`), and returns a `std::vector<int>` containing the postorder traversal of the tree using exactly two stacks (no recursion, no additional container other than the two stacks and the output vector). The function should accept the root `TreeNode*` and return the traversal. The input tree nodes contain integer `data` fields, and the tree nodes are defined with `struct TreeNode { int data; TreeNode *left, *right; };`. Assume the tree is non-empty and correctly built. The function must not modify the tree, and must be `const`-correct where possible (though `TreeNode` itself is not const, the function should not alter node data or pointers). Handle the edge case of a tree with only a single node.
#include <cassert>
#include <vector>
#include <stack>

// TreeNode definition (same as in solution for standalone testing)
struct TreeNode {
    int data;
    TreeNode *left, *right;
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper to build tree from level-order vector (with -1 for NULL)
TreeNode* buildTree(const std::vector<int>& values) {
    if (values.empty() || values[0] == -1) return nullptr;
    TreeNode* root = new TreeNode(values[0]);
    std::queue<TreeNode*> q;
    q.push(root);
    int i = 1;
    while (i < values.size() && !q.empty()) {
        TreeNode* node = q.front();
        q.pop();
        if (values[i] != -1) {
            node->left = new TreeNode(values[i]);
            q.push(node->left);
        }
        i++;
        if (i < values.size() && values[i] != -1) {
            node->right = new TreeNode(values[i]);
            q.push(node->right);
        }
        i++;
    }
    return root;
}

// Declare the function to test (inline here for simplicity)
std::vector<int> postorderUsingTwoStacks(TreeNode* root);

int main() {
    // Test 1: Empty tree
    assert(postorderUsingTwoStacks(nullptr).empty());

    // Test 2: Single node
    TreeNode* single = new TreeNode(5);
    assert(postorderUsingTwoStacks(single) == std::vector<int>({5}));
    delete single;

    // Test 3: Balanced tree: 1 2 3 4 5 6 7 -> postorder: 4 5 2 6 7 3 1
    TreeNode* root1 = buildTree({1,2,3,4,5,6,7});
    std::vector<int> expected1 = {4,5,2,6,7,3,1};
    assert(postorderUsingTwoStacks(root1) == expected1);

    // Test 4: Left-skewed: 1 2 -1 3 -1 -1 -1 -> tree: 1->2->3, postorder: 3 2 1
    TreeNode* root2 = buildTree({1,2,-1,3});
    std::vector<int> expected2 = {3,2,1};
    assert(postorderUsingTwoStacks(root2) == expected2);

    // Test 5: Right-skewed: 1 -1 2 -1 3 -> tree: 1->2->3, postorder: 3 2 1
    TreeNode* root3 = buildTree({1,-1,2,-1,3});
    assert(postorderUsingTwoStacks(root3) == expected2);

    // Test 6: Tree with only right children: 10 -1 20 -1 30 -> postorder: 30 20 10
    TreeNode* root4 = buildTree({10,-1,20,-1,30});
    assert(postorderUsingTwoStacks(root4) == std::vector<int>({30,20,10}));

    // Test 7: Complex tree with -1s: 1 2 3 -1 4 -1 5 -> postorder: 4 2 5 3 1
    TreeNode* root5 = buildTree({1,2,3,-1,4,-1,5});
    assert(postorderUsingTwoStacks(root5) == std::vector<int>({4,2,5,3,1}));

    // Cleanup (simple recursive deletion for test purposes)
    std::function<void(TreeNode*)> deleteTree = [&](TreeNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    };
    deleteTree(root1);
    deleteTree(root2);
    deleteTree(root3);
    deleteTree(root4);
    deleteTree(root5);

    return 0;
}
#include <vector>
#include <stack>

struct TreeNode {
    int data;
    TreeNode *left, *right;
};

// Perform postorder traversal using two stacks and return node values.
std::vector<int> postorderUsingTwoStacks(TreeNode* root) {
    std::vector<int> result;
    if (!root) return result;

    std::stack<TreeNode*> stack1, stack2;
    stack1.push(root);

    while (!stack1.empty()) {
        TreeNode* current = stack1.top();
        stack1.pop();
        stack2.push(current);

        // Push left then right so that right is popped first from stack1.
        if (current->left) stack1.push(current->left);
        if (current->right) stack1.push(current->right);
    }

    while (!stack2.empty()) {
        result.push_back(stack2.top()->data);
        stack2.pop();
    }
    return result;
}
// The solution uses two stacks to simulate a postorder traversal without recursion. The algorithm works as follows: push the root onto stack1. Then repeatedly pop the top node from stack1, push it onto stack2, and then push its left and right children (if they exist) onto stack1. This ordering (pushing left then right onto stack1) ensures that when nodes are popped from stack1, they are processed in root-right-left order, and thus stack2, after all nodes are transferred, will contain them in left-right-root order, which is the postorder sequence. After the loop, pop all nodes from stack2 and collect their `data` into a result vector. Important edge cases: an empty tree (return empty vector), a single node (just push root and pop it), nodes with only one child (check for `NULL` before pushing). Time complexity is O(n) because each node is pushed onto and popped from each stack exactly once. Space complexity is O(n) in the worst case (skewed tree) for the two stacks combined, plus O(n) for the output vector.
