// Given a binary tree whose nodes contain integer values, write a C++ function that performs a **post-order traversal iteratively** (i.e., without recursion) using an explicit stack and returns a `std::vector<int>` containing the node values in post-order sequence (left subtree, right subtree, root). The function must accept a `TreeNode*` root pointer. Assume `TreeNode` is defined as in the snippet with `int data`, `TreeNode* left`, and `TreeNode* right`. Your solution must not modify the tree or use recursion; it should use only a stack and a couple of auxiliary variables. The traversal must handle an empty tree (return empty vector) and trees with only one node.
// The iterative post-order traversal is the most complex of the three depth-first traversals because the root must be visited last, after both subtrees. A common approach uses two stacks: push the root onto the first stack, then repeatedly pop a node, push it onto the second stack (which reverses order), and push its left and right children onto the first stack. Since we push left then right, the second stack will accumulate nodes in root‑right‑left order, and popping the second stack yields left‑right‑root (post‑order). This is simple and correct. Edge cases: an empty tree (root == nullptr) immediately returns an empty vector; a single‑node tree returns that node’s value. Time complexity is O(n) because each node is pushed and popped exactly once from each stack, and space complexity is O(n) in the worst case (skewed tree) for the stacks. The solution is non‑recursive, uses `const TreeNode*` for safety, and returns the result by value.
#include <vector>
#include <stack>

// TreeNode definition (assumed provided externally, but included for completeness)
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Perform iterative post-order traversal of a binary tree.
// Returns a vector of node values in post-order (left, right, root).
std::vector<int> postOrderIterative(const TreeNode* root) {
    std::vector<int> result;
    if (root == nullptr) return result;

    std::stack<const TreeNode*> s1, s2;
    s1.push(root);

    while (!s1.empty()) {
        const TreeNode* current = s1.top();
        s1.pop();
        s2.push(current);

        // Push left first then right, so that right is processed first in s1,
        // and the order in s2 becomes root->right->left.
        if (current->left != nullptr) {
            s1.push(current->left);
        }
        if (current->right != nullptr) {
            s1.push(current->right);
        }
    }

    // s2 contains nodes in root, right, left order; pop to get left, right, root.
    while (!s2.empty()) {
        result.push_back(s2.top()->data);
        s2.pop();
    }
    return result;
}
#include <cassert>
#include <vector>

// TreeNode definition (already provided in solution, but repeated for test compile)
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : data(val), left(nullptr), right(nullptr) {}
};

std::vector<int> postOrderIterative(const TreeNode* root);

int main() {
    // Test 1: Empty tree
    assert(postOrderIterative(nullptr).empty());

    // Test 2: Single node
    TreeNode* single = new TreeNode(5);
    assert(postOrderIterative(single) == std::vector<int>{5});
    delete single;

    // Test 3: Tree from snippet (1-2-3-4-5-6-7)
    TreeNode* n1 = new TreeNode(1);
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n3 = new TreeNode(3);
    TreeNode* n4 = new TreeNode(4);
    TreeNode* n5 = new TreeNode(5);
    TreeNode* n6 = new TreeNode(6);
    TreeNode* n7 = new TreeNode(7);
    n1->left = n2; n1->right = n3;
    n2->left = n4; n2->right = n5;
    n3->left = n6; n3->right = n7;
    std::vector<int> expected = {4,5,2,6,7,3,1};
    assert(postOrderIterative(n1) == expected);

    // Test 4: Left-skewed tree: 1->2->3->4
    TreeNode* a = new TreeNode(1);
    a->left = new TreeNode(2);
    a->left->left = new TreeNode(3);
    a->left->left->left = new TreeNode(4);
    std::vector<int> leftSkew = {4,3,2,1};
    assert(postOrderIterative(a) == leftSkew);

    // Test 5: Right-skewed tree: 1->2->3->4
    TreeNode* b = new TreeNode(1);
    b->right = new TreeNode(2);
    b->right->right = new TreeNode(3);
    b->right->right->right = new TreeNode(4);
    std::vector<int> rightSkew = {4,3,2,1};
    assert(postOrderIterative(b) == rightSkew);

    // Test 6: Full tree with only two levels (root, two children)
    TreeNode* c = new TreeNode(10);
    c->left = new TreeNode(20);
    c->right = new TreeNode(30);
    std::vector<int> twoLevel = {20,30,10};
    assert(postOrderIterative(c) == twoLevel);

    // Clean up (not exhaustive for brevity)
    delete single;
    delete n4; delete n5; delete n2; delete n6; delete n7; delete n3; delete n1;
    // Free left-skew: 
    TreeNode* tmp = a;
    while (tmp) { TreeNode* next = tmp->left; delete tmp; tmp = next; }
    // Free right-skew:
    tmp = b;
    while (tmp) { TreeNode* next = tmp->right; delete tmp; tmp = next; }
    // Free two-level:
    delete c->left; delete c->right; delete c;

    return 0;
}
