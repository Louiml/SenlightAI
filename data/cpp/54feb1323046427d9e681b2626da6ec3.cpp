Write a C++ function `BinaryTree<int>* convertToBST(BinaryTree<int>* root)` that takes the root of a binary tree (not necessarily a BST) where each node stores an integer, and transforms the tree in place so that it becomes a Binary Search Tree (BST) while preserving the original tree's shape (i.e., the same parent-child links). The values in the tree should be replaced by the sorted unique values obtained from the original tree, assigned according to an inorder traversal: the first (smallest) value goes to the leftmost node, the next to the next node in inorder, and so on. The function should return the root pointer. Assume all node values are distinct (no duplicates). You may use helper functions internally. The function must work for an empty tree (root == NULL) by returning NULL without error.
The core idea is to extract all node values from the original tree via an inorder traversal, place them into a sorted container (e.g., a `std::set` because it automatically sorts and also gets rid of duplicates—though the problem guarantees distinct values, a set still works), and then perform a second inorder traversal to overwrite each node's data with the values from the sorted container in order. This is possible because an inorder traversal of a BST yields nodes in ascending order; by assigning sorted values in the same order, the tree's shape is retained and its values become BST-consistent. Edge cases: an empty tree (`root == NULL`) should simply return `NULL`; a single-node tree remains unchanged; and nodes with only left or only right children are handled uniformly. Time complexity is O(n log n) due to inserting n values into a set, and O(n) extra space for the set and recursion stack. The two traversals each take O(n).
#include <set>
#include <cstddef>

template<typename T>
class BinaryTree {
public:
    T data;
    BinaryTree<T>* left;
    BinaryTree<T>* right;
    BinaryTree(T val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper to collect unique values in inorder order into a set
template<typename T>
void collectInorder(BinaryTree<T>* node, std::set<T>& values) {
    if (!node) return;
    collectInorder(node->left, values);
    values.insert(node->data);
    collectInorder(node->right, values);
}

// Helper to overwrite values in inorder order using set iterator
template<typename T>
void assignInorder(BinaryTree<T>* node, typename std::set<T>::iterator& it) {
    if (!node) return;
    assignInorder(node->left, it);
    node->data = *it;
    ++it;
    assignInorder(node->right, it);
}

// Convert a binary tree to a BST by replacing values with sorted unique values.
template<typename T>
BinaryTree<T>* convertToBST(BinaryTree<T>* root) {
    if (!root) return nullptr;
    std::set<T> values;
    collectInorder(root, values);
    typename std::set<T>::iterator it = values.begin();
    assignInorder(root, it);
    return root;
}
#include <cassert>
#include <iostream>

// Assume the template class and functions from Solution are included above.

int main() {
    // Test 1: Empty tree
    BinaryTree<int>* empty = nullptr;
    assert(convertToBST(empty) == nullptr);

    // Test 2: Single node
    BinaryTree<int>* single = new BinaryTree<int>(5);
    convertToBST(single);
    assert(single->data == 5);

    // Test 3: Basic tree from example: 1-2-3-4-5-6-7 (left to right, then right subtree)
    // Original shape: root=1, left=2, right=3, 2->left=4, 2->right=5, 3->left=6, 3->right=7
    BinaryTree<int>* root = new BinaryTree<int>(1);
    root->left = new BinaryTree<int>(2);
    root->right = new BinaryTree<int>(3);
    root->left->left = new BinaryTree<int>(4);
    root->left->right = new BinaryTree<int>(5);
    root->right->left = new BinaryTree<int>(6);
    root->right->right = new BinaryTree<int>(7);
    convertToBST(root);
    // Inorder of resulting tree should be {1,2,3,4,5,6,7} in sorted order, but since shape is fixed, the inorder traversal now yields 1,2,3,4,5,6,7.
    // Check values by performing inorder manually:
    assert(root->data == 4); // root becomes 4 (since inorder positions: 4,2,5,1,6,3,7 -> sorted: 1,2,3,4,5,6,7 assigned inorder)
    // Instead, let's just verify BST property by checking inorder sequence.
    // We'll collect inorder values.
    std::vector<int> inorder;
    std::function<void(BinaryTree<int>*)> trav = [&](BinaryTree<int>* n) {
        if (!n) return;
        trav(n->left);
        inorder.push_back(n->data);
        trav(n->right);
    };
    trav(root);
    assert((inorder == std::vector<int>{1,2,3,4,5,6,7}));

    // Test 4: Unbalanced tree with large values
    // shape: root=100, left only->50, left->left->25, left->left->left->10
    BinaryTree<int>* skewed = new BinaryTree<int>(100);
    skewed->left = new BinaryTree<int>(50);
    skewed->left->left = new BinaryTree<int>(25);
    skewed->left->left->left = new BinaryTree<int>(10);
    convertToBST(skewed);
    // Inorder ordering of nodes: 10,25,50,100 -> sorted values: 10,25,50,100. Since list is already same, data unchanged.
    assert(skewed->data == 100);
    assert(skewed->left->data == 50);
    assert(skewed->left->left->data == 25);
    assert(skewed->left->left->left->data == 10);

    // Test 5: Tree with negative and positive values, not sorted
    // shape: root=0, left=-10, right=10, left->left=-20, left->right=-5
    BinaryTree<int>* mixed = new BinaryTree<int>(0);
    mixed->left = new BinaryTree<int>(-10);
    mixed->right = new BinaryTree<int>(10);
    mixed->left->left = new BinaryTree<int>(-20);
    mixed->left->right = new BinaryTree<int>(-5);
    convertToBST(mixed);
    // Original inorder nodes (by position): -20,-10,-5,0,10 -> sorted values assigned: -20,-10,-5,0,10.
    // So root (position 3 in inorder) gets -5, left (-10) gets -10, etc.
    std::vector<int> in2;
    std::function<void(BinaryTree<int>*)> trav2 = [&](BinaryTree<int>* n) {
        if (!n) return;
        trav2(n->left);
        in2.push_back(n->data);
        trav2(n->right);
    };
    trav2(mixed);
    assert((in2 == std::vector<int>{-20,-10,-5,0,10}));

    // Test 6: Duplicate values (though problem says distinct, test robustness)
    BinaryTree<int>* dup = new BinaryTree<int>(5);
    dup->left = new BinaryTree<int>(5);
    dup->right = new BinaryTree<int>(5);
    convertToBST(dup);
    // Set will have only one value {5}, so all nodes become 5.
    assert(dup->data == 5 && dup->left->data == 5 && dup->right->data == 5);

    std::cout << "All tests passed.\n";
    return 0;
}
