/*
Write a self-contained C++ function that constructs a binary search tree from a sequence of integer values provided in a `std::vector<int>`, and returns an `std::vector<int>` containing, in this exact order: (1) the number of nodes in the longest path from the root (i.e., the height of the tree, where height is defined as the number of nodes on the longest root-to-leaf path), (2) the minimum value in the tree, (3) the maximum value in the tree, (4) a boolean-like integer (1 if the target value is found, 0 otherwise), and (5) the values of the tree in ascending order (from smallest to largest). The function should handle duplicate insertions gracefully by ignoring duplicate values (i.e., not inserting a duplicate) to maintain a valid BST. Assume the input vector is non-empty and contains at least one integer. The function should be named `bst_operations` and take three parameters: a `const std::vector<int>&` for the insertion sequence, an integer `target` to search for, and a `bool&` output reference that will be set to `true` if the target is found and `false` otherwise. The function must return an `std::vector<int>` as described. Implement the BST using a `struct Node` with `int data`, `Node* left`, `Node* right`. Use recursion for tree traversal and standard iterative or recursive insertion. Ensure the function is const-correct where appropriate (the tree structure is mutable internally, but the input vector is const). The returned vector's size should be `4 + number_of_distinct_values` (since the first four elements are the metrics, followed by the sorted values).
*/

#include <vector>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

// Helper: insert value into BST, ignoring duplicates
void insertBST(Node*& root, int val) {
    if (root == nullptr) {
        root = new Node(val);
        return;
    }
    if (val < root->data) {
        insertBST(root->left, val);
    } else if (val > root->data) {
        insertBST(root->right, val);
    }
    // if equal, do nothing (duplicate ignored)
}

// Helper: compute height (number of nodes on longest path from root)
int height(const Node* root) {
    if (root == nullptr) return 0;
    return 1 + std::max(height(root->left), height(root->right));
}

// Helper: find minimum value
int findMin(const Node* root) {
    while (root->left != nullptr) root = root->left;
    return root->data;
}

// Helper: find maximum value
int findMax(const Node* root) {
    while (root->right != nullptr) root = root->right;
    return root->data;
}

// Helper: search for target
bool searchBST(const Node* root, int target) {
    while (root != nullptr) {
        if (root->data == target) return true;
        else if (target < root->data) root = root->left;
        else root = root->right;
    }
    return false;
}

// Helper: in-order traversal to collect sorted values
void inOrder(const Node* root, std::vector<int>& result) {
    if (root == nullptr) return;
    inOrder(root->left, result);
    result.push_back(root->data);
    inOrder(root->right, result);
}

// Main function: builds BST, returns metrics and sorted values
std::vector<int> bst_operations(const std::vector<int>& values, int target, bool& found) {
    Node* root = nullptr;
    for (int v : values) {
        insertBST(root, v);
    }
    
    std::vector<int> result;
    result.push_back(height(root));
    result.push_back(findMin(root));
    result.push_back(findMax(root));
    found = searchBST(root, target);
    result.push_back(found ? 1 : 0);
    
    inOrder(root, result);
    
    // Clean up memory (optional, but good practice)
    // function to delete tree would be needed; omitted for brevity
    return result;
}

#include <cassert>

int main() {
    bool found;
    
    // Test 1: Basic sequence with distinct values
    std::vector<int> v1 = {5, 1, 2, 6};
    std::vector<int> r1 = bst_operations(v1, 5, found);
    assert(r1.size() == 4 + 4); // 4 metrics + 4 distinct values
    assert(r1[0] == 3); // height: 5-1-2 or 5-6, longest path has 3 nodes
    assert(r1[1] == 1); // min
    assert(r1[2] == 6); // max
    assert(r1[3] == 1); // target 5 found
    assert(found == true);
    // Sorted values should be 1,2,5,6
    assert(r1[4] == 1 && r1[5] == 2 && r1[6] == 5 && r1[7] == 6);
    
    // Test 2: Duplicates are ignored, target not found
    std::vector<int> v2 = {10, 10, 7, 7, 15, 12};
    std::vector<int> r2 = bst_operations(v2, 20, found);
    assert(r2.size() == 4 + 4); // distinct: 7,10,12,15
    assert(r2[0] == 3); // height: 10-7 or 10-15-12, longest 3
    assert(r2[1] == 7);
    assert(r2[2] == 15);
    assert(r2[3] == 0);
    assert(found == false);
    assert(r2[4] == 7 && r2[5] == 10 && r2[6] == 12 && r2[7] == 15);
    
    // Test 3: Single element
    std::vector<int> v3 = {42};
    std::vector<int> r3 = bst_operations(v3, 42, found);
    assert(r3.size() == 4 + 1);
    assert(r3[0] == 1); // height of single node
    assert(r3[1] == 42 && r3[2] == 42);
    assert(r3[3] == 1 && found == true);
    assert(r3[4] == 42);
    
    // Test 4: Sorted input causes skewed tree (worst case)
    std::vector<int> v4 = {1, 2, 3, 4, 5};
    std::vector<int> r4 = bst_operations(v4, 3, found);
    assert(r4.size() == 4 + 5);
    assert(r4[0] == 5); // height = number of nodes (skewed)
    assert(r4[1] == 1 && r4[2] == 5);
    assert(r4[3] == 1 && found == true);
    assert(r4[4] == 1 && r4[5] == 2 && r4[6] == 3 && r4[7] == 4 && r4[8] == 5);
    
    // Test 5: Negative numbers and zeros
    std::vector<int> v5 = {-5, 0, 5, -3, 3};
    std::vector<int> r5 = bst_operations(v5, -3, found);
    assert(r5.size() == 4 + 5);
    assert(r5[0] == 3); // height
    assert(r5[1] == -5 && r5[2] == 5);
    assert(r5[3] == 1 && found == true);
    assert(r5[4] == -5 && r5[5] == -3 && r5[6] == 0 && r5[7] == 3 && r5[8] == 5);
    
    // Test 6: All duplicates of same value
    std::vector<int> v6 = {7, 7, 7};
    std::vector<int> r6 = bst_operations(v6, 7, found);
    assert(r6.size() == 4 + 1);
    assert(r6[0] == 1);
    assert(r6[1] == 7 && r6[2] == 7);
    assert(r6[3] == 1 && found == true);
    assert(r6[4] == 7);
    
    // Test 7: Right-skewed tree with negative target not found
    std::vector<int> v7 = {1, 2, 3};
    std::vector<int> r7 = bst_operations(v7, 0, found);
    assert(r7.size() == 4 + 3);
    assert(r7[0] == 3);
    assert(r7[1] == 1 && r7[2] == 3);
    assert(r7[3] == 0 && found == false);
    assert(r7[4] == 1 && r7[5] == 2 && r7[6] == 3);
    
    return 0;
}

// The solution involves building a binary search tree (BST) from the given vector of integers. We define a `Node` structure with integer data and left/right child pointers. Insertion follows the BST property: values less than the current node go left, greater go right, and duplicates are ignored (if the value equals the current node, we simply do nothing and return). After building the tree, we compute: (1) the height using a recursive function that returns 0 for a null node and otherwise 1 + max(height(left), height(right)); (2) the minimum by traversing left children repeatedly until null; (3) the maximum by traversing right children repeatedly until null; (4) search for the target using standard BST search (while loop or recursion, returning true if found, false if null reached); (5) in-order traversal to collect sorted values—we use a recursive function that visits left subtree, then root, then right subtree, appending each value to a result vector. Important edge cases: empty input is not allowed for the task (since it says non-empty), but we could handle it defensively by returning zeros and empty vector; duplicate values must be ignored to avoid violating BST properties; the height of a single-node tree is 1; the minimum and maximum are the root when the tree has only one node. Time complexity: Inserting n values takes O(n * h) where h is the tree height (average O(n log n) if random, worst O(n^2) if sorted); computing height, min, max, search, and in-order traversal each take O(n) in the worst case (linear for traversal, O(h) for min/max/search). Space complexity: O(n) for storing nodes and O(h) recursion stack for tree operations (worst O(n) for skewed tree).
