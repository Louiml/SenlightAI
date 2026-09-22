Write a standalone C++ function named `analyzeBST` that takes a pointer to the root of a binary search tree (BST) constructed from integer keys, and returns a `std::string` containing the following eight values in exactly this order, separated by single spaces: the height of the tree (with height of an empty tree defined as 0), the smallest key (return `-1` if the tree is empty), the largest key (return `-1` if empty), the total node count, the leaf node count (nodes with no children), the non-leaf node count (nodes with at least one child), the inorder traversal of the tree as a space-separated sequence of integers (empty string if tree is empty), and the preorder traversal as a space-separated sequence (empty string if empty). The function must handle an empty tree, duplicate keys (ignored during insertion, so each key appears once), and should not modify the tree. You may assume that the `node` structure is defined as in the snippet: `struct node { int key; struct node *left, *right; };`. The function must be `const`‑correct, meaning it takes a `const node*` and does not modify the input. You are allowed to use helper recursive functions internally, but all logic must be encapsulated within `analyzeBST`.
The solution uses recursive helper functions to compute each metric: height (longest path from root to leaf, empty tree height 0), smallest/largest (follow leftmost/rightmost pointers), node count (1 + left + right), leaf count (if both children null return 1 else sum), non-leaf count (if null or leaf return 0 else 1 + sum), and the inorder/preorder traversals (recursively build string streams). Edge cases include an empty tree (all counters zero, smallest/largest -1, traversals empty), a single-node tree (height 1, smallest==largest, leaf=1, non-leaf=0), and skewed trees where recursion depth equals number of nodes. The main algorithm performs one pass over the tree, visiting each node a constant number of times across all metrics, so time complexity is O(n) where n is the number of nodes. Auxiliary space is O(h) for recursion stack and O(n) for the concatenated string results; in practice, the traversal strings dominate. The function is const‑correct by taking `const node*` and not altering the tree.
#include <string>
#include <sstream>

struct node {
    int key;
    struct node *left, *right;
};

// Helper functions (internal, not exposed)
static int treeHeight(const node* root) {
    if (!root) return 0;
    int leftH = treeHeight(root->left);
    int rightH = treeHeight(root->right);
    return (leftH > rightH ? leftH : rightH) + 1;
}

static int findMin(const node* root) {
    if (!root) return -1;
    while (root->left) root = root->left;
    return root->key;
}

static int findMax(const node* root) {
    if (!root) return -1;
    while (root->right) root = root->right;
    return root->key;
}

static int countAll(const node* root) {
    if (!root) return 0;
    return 1 + countAll(root->left) + countAll(root->right);
}

static int countLeaves(const node* root) {
    if (!root) return 0;
    if (!root->left && !root->right) return 1;
    return countLeaves(root->left) + countLeaves(root->right);
}

static int countNonLeaves(const node* root) {
    if (!root || (!root->left && !root->right)) return 0;
    return 1 + countNonLeaves(root->left) + countNonLeaves(root->right);
}

static void inorderCollect(const node* root, std::ostringstream& out) {
    if (!root) return;
    inorderCollect(root->left, out);
    out << root->key << " ";
    inorderCollect(root->right, out);
}

static void preorderCollect(const node* root, std::ostringstream& out) {
    if (!root) return;
    out << root->key << " ";
    preorderCollect(root->left, out);
    preorderCollect(root->right, out);
}

// Public function: returns a string with eight metrics separated by spaces.
std::string analyzeBST(const node* root) {
    std::ostringstream result;
    result << treeHeight(root) << " "
           << findMin(root) << " "
           << findMax(root) << " "
           << countAll(root) << " "
           << countLeaves(root) << " "
           << countNonLeaves(root) << " ";

    std::ostringstream in, pre;
    inorderCollect(root, in);
    preorderCollect(root, pre);
    std::string inStr = in.str();
    std::string preStr = pre.str();
    // Remove trailing space if not empty
    if (!inStr.empty()) inStr.pop_back();
    if (!preStr.empty()) preStr.pop_back();
    result << inStr << " " << preStr;
    return result.str();
}
#include <cassert>
#include <string>

// Assume the solution code is already included above.

// Helper to build a BST (duplicates ignored)
void bstInsert(node*& root, int key) {
    if (!root) {
        root = new node{key, nullptr, nullptr};
    } else if (key < root->key) {
        bstInsert(root->left, key);
    } else if (key > root->key) {
        bstInsert(root->right, key);
    }
}

int main() {
    // Test 1: Empty tree
    node* root = nullptr;
    assert(analyzeBST(root) == "0 -1 -1 0 0 0  ");
    delete root;

    // Test 2: Single node
    root = nullptr;
    bstInsert(root, 5);
    assert(analyzeBST(root) == "1 5 5 1 1 0 5 5");

    // Test 3: The example tree from the snippet
    root = nullptr;
    bstInsert(root, 50);
    bstInsert(root, 30);
    bstInsert(root, 20);
    bstInsert(root, 40);
    bstInsert(root, 70);
    bstInsert(root, 60);
    bstInsert(root, 80);
    // Height is 3 (50->70->80 or 50->30->20)
    // smallest=20, largest=80, nodes=7, leaves=4 (20,40,60,80), non-leaves=3
    // inorder: 20 30 40 50 60 70 80
    // preorder: 50 30 20 40 70 60 80
    assert(analyzeBST(root) == "3 20 80 7 4 3 20 30 40 50 60 70 80 50 30 20 40 70 60 80");

    // Test 4: Duplicate key ignored
    root = nullptr;
    bstInsert(root, 10);
    bstInsert(root, 10);
    assert(analyzeBST(root) == "1 10 10 1 1 0 10 10");

    // Test 5: Skewed right tree
    root = nullptr;
    bstInsert(root, 1);
    bstInsert(root, 2);
    bstInsert(root, 3);
    // height=3, smallest=1, largest=3, nodes=3, leaves=1 (the 3), non-leaves=2
    // inorder: 1 2 3, preorder: 1 2 3
    assert(analyzeBST(root) == "3 1 3 3 1 2 1 2 3 1 2 3");

    // Test 6: Skewed left tree
    root = nullptr;
    bstInsert(root, 7);
    bstInsert(root, 5);
    bstInsert(root, 3);
    // height=3, smallest=3, largest=7, nodes=3, leaves=1 (3), non-leaves=2
    // inorder: 3 5 7, preorder: 7 5 3
    assert(analyzeBST(root) == "3 3 7 3 1 2 3 5 7 7 5 3");

    // Test 7: Balanced tree with 10 nodes
    root = nullptr;
    int values[] = {50, 30, 70, 20, 40, 60, 80, 15, 35, 75};
    for (int v : values) bstInsert(root, v);
    // height=4, smallest=15, largest=80, nodes=10, leaves=4 (15,35,60,75), non-leaves=6
    // inorder: 15 20 30 35 40 50 60 70 75 80
    // preorder: 50 30 20 15 40 35 70 60 80 75
    assert(analyzeBST(root) == "4 15 80 10 4 6 15 20 30 35 40 50 60 70 75 80 50 30 20 15 40 35 70 60 80 75");

    // Cleanup: delete trees
    // (Not necessary for the test, but for completeness)
    // We could write a helper to delete tree, but not required for assert checks.

    return 0;
}
