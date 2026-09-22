Write a C++ function `std::string treeOrderToString(const Node* root, int order)` that takes a pointer to the root of a binary tree (where each node stores an integer `data` and has `left` and `right` child pointers) and an integer `order` (where `1` = inorder, `2` = preorder, `3` = postorder). The function must return a string containing the traversal values separated by single spaces (no trailing space). The tree is populated externally (not in your function), and the input is guaranteed to be a valid binary tree using `-1` as a sentinel for NULL children, but you do **not** need to parse or build the tree—you only implement the traversal logic. Handle empty trees (root == nullptr) by returning an empty string. The function must not modify the tree and must be `const`-correct.

The solution uses classic recursive depth-first traversal. For each order:
- **Inorder** (left, root, right): recurse left, append current node’s data, recurse right.
- **Preorder** (root, left, right): append current node’s data, recurse left, recurse right.
- **Postorder** (left, right, root): recurse left, recurse right, append current node’s data.

Because the function returns a string, we accumulate results in a `std::vector<int>` (or directly in a string with separators). The cleanest approach is to pass a vector by reference to a helper that fills it, then join the vector elements with spaces. Edge cases: nullptr root returns empty string; a tree with only one node returns that node’s value as a string; any node with `data == -1` is treated as a NULL marker in the original creation code, but in the traversal we assume the tree is already well-formed (no node with data == -1 exists as a real node). The time complexity is O(n) where n is the number of nodes, and space complexity is O(h) for recursion stack plus O(n) for the returned string.

#include <string>
#include <vector>
#include <sstream>

class Node {
public:
    int data;
    Node *left, *right;
};

// Helper: recursive traversal that stores values in `out`
static void traverse(const Node* root, int order, std::vector<int>& out) {
    if (!root) return;
    if (order == 2) out.push_back(root->data); // preorder
    traverse(root->left, order, out);
    if (order == 1) out.push_back(root->data); // inorder
    traverse(root->right, order, out);
    if (order == 3) out.push_back(root->data); // postorder
}

// Returns the requested traversal as a space-separated string.
// order: 1=inorder, 2=preorder, 3=postorder
std::string treeOrderToString(const Node* root, int order) {
    std::vector<int> vals;
    traverse(root, order, vals);
    std::ostringstream oss;
    for (size_t i = 0; i < vals.size(); ++i) {
        if (i > 0) oss << ' ';
        oss << vals[i];
    }
    return oss.str();
}

#include <cassert>
#include <string>

// Node and treeOrderToString from above would be included here

int main() {
    // Build a simple tree:       1
    //                          /   \
    //                         2     3
    //                        / \     \
    //                       4   5     6
    Node* n4 = new Node{4, nullptr, nullptr};
    Node* n5 = new Node{5, nullptr, nullptr};
    Node* n6 = new Node{6, nullptr, nullptr};
    Node* n2 = new Node{2, n4, n5};
    Node* n3 = new Node{3, nullptr, n6};
    Node* root = new Node{1, n2, n3};

    assert(treeOrderToString(root, 1) == "4 2 5 1 3 6");
    assert(treeOrderToString(root, 2) == "1 2 4 5 3 6");
    assert(treeOrderToString(root, 3) == "4 5 2 6 3 1");

    // Single node tree
    Node* single = new Node{42, nullptr, nullptr};
    assert(treeOrderToString(single, 1) == "42");
    assert(treeOrderToString(single, 2) == "42");
    assert(treeOrderToString(single, 3) == "42");

    // Empty tree
    assert(treeOrderToString(nullptr, 1) == "");
    assert(treeOrderToString(nullptr, 2) == "");
    assert(treeOrderToString(nullptr, 3) == "");

    // Left-skewed tree: 1 -> 2 -> 3
    Node* n3b = new Node{3, nullptr, nullptr};
    Node* n2b = new Node{2, n3b, nullptr};
    Node* rootb = new Node{1, n2b, nullptr};
    assert(treeOrderToString(rootb, 1) == "3 2 1");
    assert(treeOrderToString(rootb, 2) == "1 2 3");
    assert(treeOrderToString(rootb, 3) == "3 2 1");

    // Cleanup not required for test, but in real code would delete nodes.
}
