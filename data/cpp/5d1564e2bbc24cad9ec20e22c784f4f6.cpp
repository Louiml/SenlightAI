Write a C++ function `int sumOfEvenNodes(node* root)` that takes a pointer to the root of a binary tree (where each node stores an integer `data` and pointers `left` and `right`) and returns the sum of all node values that are even. The function must handle an empty tree (return 0) and must not modify the tree. The tree is defined as `struct node { int data; node* left; node* right; };` and the function should work for any valid binary tree, including those with negative even numbers (e.g., -4 counts as even) and with only one node. Use recursion or iteration, but the solution must be straightforward and correct.
#include <cassert>

// The node structure is already defined above in the solution; we only need a main for tests.

// Helper to create a simple tree for testing (not part of the solution function)
node* createNode(int val) {
    return new node(val);
}

void destroyTree(node* root) {
    if (root) {
        destroyTree(root->left);
        destroyTree(root->right);
        delete root;
    }
}

int main() {
    // Test 1: Empty tree
    node* empty = nullptr;
    assert(sumOfEvenNodes(empty) == 0);

    // Test 2: Single even node
    node* singleEven = createNode(4);
    assert(sumOfEvenNodes(singleEven) == 4);
    destroyTree(singleEven);

    // Test 3: Single odd node
    node* singleOdd = createNode(5);
    assert(sumOfEvenNodes(singleOdd) == 0);
    destroyTree(singleOdd);

    // Test 4: Balanced tree with mixed values
    //       10
    //      /  \
    //     3    8
    //    / \    \
    //   2   6    7
    node* root = createNode(10);
    root->left = createNode(3);
    root->right = createNode(8);
    root->left->left = createNode(2);
    root->left->right = createNode(6);
    root->right->right = createNode(7);
    // Even nodes: 10, 8, 2, 6 → sum = 26
    assert(sumOfEvenNodes(root) == 26);
    destroyTree(root);

    // Test 5: Negative even numbers count
    node* negRoot = createNode(-4);
    negRoot->left = createNode(-3);
    negRoot->right = createNode(0);
    // Even: -4, 0 → sum = -4
    assert(sumOfEvenNodes(negRoot) == -4);
    destroyTree(negRoot);

    // Test 6: Skewed tree (left chain) with no evens
    node* skew = createNode(1);
    skew->left = createNode(3);
    skew->left->left = createNode(5);
    assert(sumOfEvenNodes(skew) == 0);
    destroyTree(skew);

    // Test 7: Skewed tree (right chain) with all evens
    node* skewEven = createNode(2);
    skewEven->right = createNode(4);
    skewEven->right->right = createNode(6);
    assert(sumOfEvenNodes(skewEven) == 12);
    destroyTree(skewEven);

    return 0;
}
#include <cstddef>

// Definition of the tree node (assumed to be provided externally, but included for completeness)
struct node {
    int data;
    node* left;
    node* right;
    node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Returns the sum of all even-valued nodes in the binary tree rooted at 'root'.
// Returns 0 for an empty tree. Does not modify the tree.
int sumOfEvenNodes(node* root) {
    if (root == nullptr) {
        return 0;
    }
    int currentSum = (root->data % 2 == 0) ? root->data : 0;
    return currentSum + sumOfEvenNodes(root->left) + sumOfEvenNodes(root->right);
}
// The solution uses a recursive depth-first traversal. The base case is a null pointer, which contributes 0 to the sum. For a non-null node, we check if its `data` is even (using `data % 2 == 0`). If yes, we add that value to the sum; otherwise we add 0. Then we recursively add the sums from the left and right subtrees. This naturally handles all edge cases: an empty tree returns 0; a single node with even data returns that node's value; a single node with odd data returns 0. Negative even numbers like -4 satisfy `-4 % 2 == 0` in C++ (since the remainder has the sign of the dividend, but for even numbers the remainder is always 0). The time complexity is O(n) where n is the number of nodes, because each node is visited exactly once. The space complexity is O(h) where h is the tree height, due to the recursive call stack (worst case O(n) for a skewed tree). The function does not modify the tree, so it can be declared `const`-safe if given a `const node*`, but the original snippet uses a non-const pointer, so we'll match that.
