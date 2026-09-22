Write a C++ function `int convertToSortedArray(const rb_tree_node* root, const rb_tree_node* nil, int* array, int arraySize)` that performs an in-order traversal of a red-black tree and copies the keys into the provided array. The function must return the number of keys copied. The tree is represented using the provided `rb_tree_node` structure with fields `key`, `left`, `right`, `p`, and `color`. The tree uses a sentinel node (`T_nil`) that represents both null children and the parent of the root; all leaf children and the root's parent point to this sentinel. The sentinel itself is not part of the data and its `left`/`right`/`p` point to itself, but its `key` is irrelevant. The traversal must be strictly in-order (left, node, right), skipping the sentinel. The function must handle an empty tree (root is the sentinel) by copying nothing and returning 0. It must also handle a tree with duplicate keys by copying them all without modification. The function should not modify the tree, so the root and nil pointers should be treated as `const`. The caller guarantees that `array` has at least as many elements as the tree size; if `arraySize` is smaller than the number of keys, the function must stop copying and return the number of keys actually copied (which will be `arraySize`). Assume the `rb_tree_node` struct is already defined in a header (do not include it in your solution), but for standalone compilation, you may include a minimal definition in your answer if needed.
// The core algorithm is a recursive in-order traversal. Starting from the root, we recursively traverse the left subtree, then copy the current node's key, then recursively traverse the right subtree. The recursion terminates when the current node equals the sentinel `nil` (which represents a null child). Because the tree uses a single sentinel for all null pointers, we pass the sentinel explicitly and compare nodes with `==` rather than checking for `nullptr`. The tricky part is passing the array index by reference across recursive calls so that keys are placed consecutively. We maintain an index counter that is shared across recursion, and we must stop when the index reaches `arraySize`. To handle stopping early, we can either return the count from the recursive helper or pass the index by reference and check it before copying. The function returns the number of keys actually copied. Edge cases include: (a) an empty tree (root == nil), which immediately returns 0; (b) a tree with one or more nodes where the traversal must not visit the sentinel; (c) `arraySize` being zero, in which case we copy nothing and return 0; (d) `arraySize` being smaller than the tree size, so we copy exactly `arraySize` keys and return that value. Time complexity is O(n) for a tree with n nodes, because each node is visited once. Space complexity is O(h) for the recursion stack, where h is the tree height (which could be O(n) in the worst case for an unbalanced tree, but for a red-black tree it is O(log n) in practice, though we cannot guarantee it here).
#include <cstddef>

// Assume the following struct is available from a header.
// If not, uncomment the definition below for standalone use.
/*
struct rb_tree_node {
    int key;
    bool color; // RB_RED or RB_BLACK
    rb_tree_node* p;
    rb_tree_node* left;
    rb_tree_node* right;
};
*/

namespace {

// Helper: recursively traverse the tree in-order and fill the array.
// 'index' is passed by reference to track the position in the array.
// 'arraySize' is the capacity of the array; we stop when full.
void inOrderFill(const rb_tree_node* node, const rb_tree_node* nil, int* array, int arraySize, int& index) {
    if (node == nil) {
        return;
    }
    if (index >= arraySize) {
        return; // Array full; stop further traversal.
    }
    // Traverse left subtree first.
    inOrderFill(node->left, nil, array, arraySize, index);
    // Copy current node's key if there is space.
    if (index < arraySize) {
        array[index++] = node->key;
    }
    // Traverse right subtree.
    inOrderFill(node->right, nil, array, arraySize, index);
}

} // namespace

// Convert the red-black tree into a sorted array via in-order traversal.
// Returns the number of keys copied (at most arraySize).
// The tree is not modified; root and nil are const pointers.
int convertToSortedArray(const rb_tree_node* root, const rb_tree_node* nil, int* array, int arraySize) {
    int index = 0;
    inOrderFill(root, nil, array, arraySize, index);
    return index;
}
#include <cassert>
#include <cstddef>

struct rb_tree_node {
    int key;
    bool color; // true = RED, false = BLACK
    rb_tree_node* p;
    rb_tree_node* left;
    rb_tree_node* right;
};

// Helper to create a sentinel node.
rb_tree_node* makeNil() {
    rb_tree_node* nil = new rb_tree_node();
    nil->color = false; // BLACK
    nil->p = nil;
    nil->left = nil;
    nil->right = nil;
    nil->key = 0; // unused
    return nil;
}

// Helper to create a simple node (leaf children are set to nil, parent set to nil).
rb_tree_node* makeNode(int key, rb_tree_node* nil) {
    rb_tree_node* n = new rb_tree_node();
    n->key = key;
    n->color = true; // default RED
    n->p = nil;
    n->left = nil;
    n->right = nil;
    return n;
}

// Include the solution function (from above).
int convertToSortedArray(const rb_tree_node* root, const rb_tree_node* nil, int* array, int arraySize);
// The namespace helper is in the solution; for this test we declare it manually.
namespace {
void inOrderFill(const rb_tree_node* node, const rb_tree_node* nil, int* array, int arraySize, int& index);
}

int main() {
    // Build a simple tree: root=2, left child=1, right child=3.
    rb_tree_node* nil = makeNil();
    rb_tree_node* root = makeNode(2, nil);
    rb_tree_node* left = makeNode(1, nil);
    rb_tree_node* right = makeNode(3, nil);
    root->left = left;
    left->p = root;
    root->right = right;
    right->p = root;

    // Test case 1: full array enough space.
    int arr1[3] = {0,0,0};
    int result1 = convertToSortedArray(root, nil, arr1, 3);
    assert(result1 == 3);
    assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 3);

    // Test case 2: array too small (capacity 2).
    int arr2[2] = {0,0};
    int result2 = convertToSortedArray(root, nil, arr2, 2);
    assert(result2 == 2);
    assert(arr2[0] == 1 && arr2[1] == 2);

    // Test case 3: empty tree (root == nil).
    int arr3[5] = {9,9,9,9,9};
    int result3 = convertToSortedArray(nil, nil, arr3, 5);
    assert(result3 == 0);
    // Array unchanged.
    for (int i = 0; i < 5; ++i) assert(arr3[i] == 9);

    // Test case 4: array size zero.
    int result4 = convertToSortedArray(root, nil, nullptr, 0);
    assert(result4 == 0);

    // Test case 5: single node tree.
    rb_tree_node* single = makeNode(42, nil);
    int arr5[1] = {0};
    int result5 = convertToSortedArray(single, nil, arr5, 1);
    assert(result5 == 1 && arr5[0] == 42);

    // Test case 6: duplicate keys.
    rb_tree_node* dupRoot = makeNode(5, nil);
    rb_tree_node* dupLeft = makeNode(5, nil);
    dupRoot->left = dupLeft;
    dupLeft->p = dupRoot;
    int arr6[2] = {0,0};
    int result6 = convertToSortedArray(dupRoot, nil, arr6, 2);
    assert(result6 == 2);
    assert(arr6[0] == 5 && arr6[1] == 5);

    // Cleanup: not strictly needed for assert-style test, but omit deletes for brevity.
    // (In a real program you'd delete nodes recursively; here we skip.)

    return 0;
}
