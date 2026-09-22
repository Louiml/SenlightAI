/*
Write a C++ function `void buildSortedFromTree(int L[], int n)` that, given an array `L` of `n` integers, constructs a binary search tree (BST) from the array, then performs an in-order traversal of that tree to write the sorted values back into the same array `L`. The function must handle arbitrary `n` (including 0 and 1) and must not use any standard sorting library. The BST should be built iteratively (not recursively), and the in-order traversal can be recursive or iterative. After the call, `L` must contain the same multiset of integers as before, but sorted in non-decreasing order. You may assume the input array is valid and `n >= 0`. You are allowed to write helper functions (e.g., for node creation, insertion, and traversal) inside your solution, but the main exported function must be `void buildSortedFromTree(int L[], int n)`. For clarity: the array may contain duplicate values; duplicates must all appear in the sorted output. Also, to avoid relying on global random seeds, do not call `srand` or `rand` inside your function; only use values from `L`.
*/
#include <iostream>
#include <vector>

// Node structure for binary search tree
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Insert a value into the BST (iterative). Returns the (possibly new) root.
TreeNode* insertBST(TreeNode* root, int value) {
    TreeNode* newNode = new TreeNode(value);
    if (root == nullptr) return newNode;
    TreeNode* current = root;
    while (true) {
        if (value < current->val) {
            if (current->left == nullptr) {
                current->left = newNode;
                break;
            }
            current = current->left;
        } else {
            // value >= current->val, go right
            if (current->right == nullptr) {
                current->right = newNode;
                break;
            }
            current = current->right;
        }
    }
    return root;
}

// In-order traversal: fill the array with sorted values
void inorderFill(TreeNode* node, int L[], int& index) {
    if (node == nullptr) return;
    inorderFill(node->left, L, index);
    L[index++] = node->val;
    inorderFill(node->right, L, index);
}

// Free the tree memory to avoid leaks
void deleteTree(TreeNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Main function: builds BST from array, then sorts array via in-order traversal
void buildSortedFromTree(int L[], int n) {
    if (n <= 0) return; // nothing to sort

    TreeNode* root = nullptr;
    for (int i = 0; i < n; ++i) {
        root = insertBST(root, L[i]);
    }

    int index = 0;
    inorderFill(root, L, index);

    deleteTree(root); // cleanup
}
#include <cassert>
#include <iostream>

// Declaration of the function from the solution
void buildSortedFromTree(int L[], int n);

// Helper to check if array is sorted in non-decreasing order
bool isSorted(int L[], int n) {
    for (int i = 1; i < n; ++i) {
        if (L[i-1] > L[i]) return false;
    }
    return true;
}

int main() {
    // Test 1: empty array
    int arr0[] = {};
    buildSortedFromTree(arr0, 0);
    assert(isSorted(arr0, 0));

    // Test 2: single element
    int arr1[] = {5};
    buildSortedFromTree(arr1, 1);
    assert(isSorted(arr1, 1) && arr1[0] == 5);

    // Test 3: already sorted
    int arr2[] = {1, 2, 3, 4, 5};
    buildSortedFromTree(arr2, 5);
    assert(isSorted(arr2, 5));
    assert(arr2[0]==1 && arr2[4]==5);

    // Test 4: reverse sorted
    int arr3[] = {9, 7, 5, 3, 1};
    buildSortedFromTree(arr3, 5);
    assert(isSorted(arr3, 5));
    assert(arr3[0]==1 && arr3[4]==9);

    // Test 5: duplicates
    int arr4[] = {3, 1, 3, 2, 1, 3};
    buildSortedFromTree(arr4, 6);
    assert(isSorted(arr4, 6));
    // Count duplicates: should have 1 twice, 2 once, 3 thrice
    assert(arr4[0]==1 && arr4[1]==1 && arr4[2]==2 && arr4[3]==3 && arr4[4]==3 && arr4[5]==3);

    // Test 6: negative and positive mixed
    int arr5[] = {0, -5, 10, -2, 7, -5, 0};
    buildSortedFromTree(arr5, 7);
    assert(isSorted(arr5, 7));
    assert(arr5[0]==-5 && arr5[1]==-5 && arr5[2]==-2 && arr5[3]==0 && arr5[4]==0 && arr5[5]==7 && arr5[6]==10);

    // Test 7: worst-case degenerate (already sorted) — still correct
    int arr6[100];
    for (int i = 0; i < 100; ++i) arr6[i] = i;
    buildSortedFromTree(arr6, 100);
    assert(isSorted(arr6, 100));
    for (int i = 0; i < 100; ++i) assert(arr6[i] == i);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The core idea is to leverage the BST property: for any node, all values in its left subtree are strictly less than the node’s value, and all values in the right subtree are greater or equal (since duplicates are placed to the right). Building the tree iteratively from the array using insertion ensures the BST structure. Then, performing an in-order traversal (left subtree, node, right subtree) visits nodes in sorted order. During traversal, we write values back to the array sequentially. Edge cases: empty array (n=0) — just return immediately; single element (n=1) — tree has one node, traversal writes it back unchanged. Duplicates are handled because the insertion condition sends equal values to the right subtree, preserving their relative order, and in-order traversal includes all nodes. Time complexity: building the tree is O(n * h) where h is the tree height, worst-case O(n^2) for an already sorted input that creates a degenerate tree; in-order traversal is O(n). Space complexity: O(n) for the tree nodes. To mitigate worst-case time, one could use a balanced BST, but the task allows a simple BST. The proposed solution uses iterative insertion and a recursive in-order traversal (or iterative stack-based traversal) to write back.
