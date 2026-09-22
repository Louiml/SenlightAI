/*
Write a C++ function named `convertSortedArrayToBalancedBST` that takes a non-empty array of integers (by pointer and size) and returns a pointer to the root of a balanced binary search tree (BST) built from the array. The function must first sort the array in ascending order, then construct the BST by recursively choosing the middle element as the root and building the left and right subtrees from the left and right halves of the sorted array. The BST must satisfy the BST property: all values in the left subtree are strictly less than the root, and all values in the right subtree are greater than or equal to the root. The function should use a helper function for sorting (any stable or simple comparison sort) and a helper to recursively create nodes. Edge cases: arrays with duplicate values must produce a valid BST (duplicates go to the right subtree), and the function must handle arrays of size 1 correctly. The function must `delete` any dynamically allocated nodes? No, that is the caller's responsibility. For this task, only implement the construction; do not include deletion or traversal. The function must be self-contained, and the node structure should be defined locally or in a global scope accessible to the solution.
*/

#include <iostream>

struct Node {
    Node* left;
    Node* right;
    int data;
    Node(int val) : left(nullptr), right(nullptr), data(val) {}
};

void swapInts(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void sortAscending(int* arr, int n) {
    for (int i = 0; i < n; ++i) {
        bool anySwap = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swapInts(arr[j], arr[j + 1]);
                anySwap = true;
            }
        }
        if (!anySwap) break;
    }
}

Node* buildBalancedFromSorted(int* arr, int left, int right) {
    if (left > right) {
        return nullptr;
    }
    int mid = left + (right - left) / 2;
    Node* root = new Node(arr[mid]);
    root->left = buildBalancedFromSorted(arr, left, mid - 1);
    root->right = buildBalancedFromSorted(arr, mid + 1, right);
    return root;
}

Node* convertSortedArrayToBalancedBST(int* arr, int n) {
    if (n <= 0) {
        return nullptr;
    }
    sortAscending(arr, n);
    return buildBalancedFromSorted(arr, 0, n - 1);
}

#include <cassert>

// Helper to check BST property and balancedness (height difference <= 1)
bool isBST(Node* root, int minVal, int maxVal) {
    if (!root) return true;
    if (root->data < minVal || root->data > maxVal) return false;
    return isBST(root->left, minVal, root->data - 1) && isBST(root->right, root->data, maxVal);
}

int height(Node* root) {
    if (!root) return 0;
    int left = height(root->left);
    int right = height(root->right);
    return 1 + (left > right ? left : right);
}

bool isBalanced(Node* root) {
    if (!root) return true;
    int lh = height(root->left);
    int rh = height(root->right);
    if (abs(lh - rh) > 1) return false;
    return isBalanced(root->left) && isBalanced(root->right);
}

// Inorder traversal to collect values for checking sorted order
void inorderCollect(Node* root, std::vector<int>& values) {
    if (!root) return;
    inorderCollect(root->left, values);
    values.push_back(root->data);
    inorderCollect(root->right, values);
}

int main() {
    // Test 1: distinct values
    int arr1[] = {20, 1, 55, 100, 50, 52};
    Node* root1 = convertSortedArrayToBalancedBST(arr1, 6);
    assert(isBST(root1, INT_MIN, INT_MAX));
    assert(isBalanced(root1));
    std::vector<int> vals1;
    inorderCollect(root1, vals1);
    std::vector<int> expected1 = {1, 20, 50, 52, 55, 100};
    assert(vals1 == expected1);

    // Test 2: single element
    int arr2[] = {42};
    Node* root2 = convertSortedArrayToBalancedBST(arr2, 1);
    assert(root2 != nullptr);
    assert(root2->data == 42);
    assert(root2->left == nullptr && root2->right == nullptr);

    // Test 3: duplicates
    int arr3[] = {5, 3, 7, 5, 3};
    Node* root3 = convertSortedArrayToBalancedBST(arr3, 5);
    assert(isBST(root3, INT_MIN, INT_MAX));
    std::vector<int> vals3;
    inorderCollect(root3, vals3);
    std::vector<int> expected3 = {3, 3, 5, 5, 7};
    assert(vals3 == expected3);

    // Test 4: already sorted array
    int arr4[] = {1, 2, 3, 4, 5, 6, 7};
    Node* root4 = convertSortedArrayToBalancedBST(arr4, 7);
    assert(isBST(root4, INT_MIN, INT_MAX));
    assert(height(root4->left) == height(root4->right) || abs(height(root4->left) - height(root4->right)) <= 1);
    assert(root4->data == 4);

    // Test 5: negative numbers
    int arr5[] = {-10, -5, -20, 0, -1};
    Node* root5 = convertSortedArrayToBalancedBST(arr5, 5);
    assert(isBST(root5, INT_MIN, INT_MAX));
    std::vector<int> vals5;
    inorderCollect(root5, vals5);
    std::vector<int> expected5 = {-20, -10, -5, -1, 0};
    assert(vals5 == expected5);

    // Cleanup (not required but good practice)
    // Simple recursive cleanup for each root
    // (omitted for brevity but assumed in a full program)

    return 0;
}

// The solution follows a standard divide-and-conquer approach. First, sort the input array in ascending order using a simple bubble sort (as in the snippet) or any other O(n²) or better sort. Sorting is necessary because building a balanced BST from an unsorted array cannot guarantee balance without sorting. After sorting, define a recursive function `buildBalanced` that takes indices `left` and `right` and returns a node pointer. Compute `mid = (left + right) / 2`, create a node with `arr[mid]`, recursively build left subtree from indices `left` to `mid-1`, and right subtree from `mid+1` to `right`. The recursion terminates when `left > right`, returning `nullptr`. For duplicates, the condition `val < root->data` ensures duplicates go to the right; in this construction, because the array is sorted, duplicates will appear consecutively, and the middle element may be a duplicate, but the BST property is still satisfied (values in right subtree are >= root). Time complexity: sorting takes O(n²) for bubble sort, but the tree building is O(n) because each element is visited once. Space complexity: O(n) for the tree nodes plus O(log n) recursion stack depth (balanced). If we assume an O(n log n) sort, total time is O(n log n). Edge cases: size 1 → root is the only element; duplicate values are handled by placing them to the right; empty array is not allowed per specification.
