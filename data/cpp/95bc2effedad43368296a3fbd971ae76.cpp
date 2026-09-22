// Write a C++ function `buildBalancedBST` that takes a sorted array of integers (passed as a `const std::vector<int>&`) and returns a `Node*` pointing to the root of a height-balanced binary search tree (BST) built from the array. The function must recursively choose the middle element as the root, then build the left subtree from the left half and the right subtree from the right half. The `Node` structure is provided (with `int val`, `Node* left`, `Node* right`, and a constructor). The vector is guaranteed to be non-empty and sorted in ascending order, but may contain duplicate values (duplicates must be handled correctly; if duplicates exist, any valid BST is acceptable as long as it is balanced and preserves the sorted ordering). Additionally, you must provide a helper to compute the height of the tree (to verify balance) and a helper to perform a pre-order traversal and return a `std::vector<int>` of node values (to compare against the expected order). The solution must avoid using global variables and must not modify the input vector.

The core algorithm is a classic divide-and-conquer recursive construction of a balanced BST from a sorted array. At each recursive call, the function receives a subarray (defined by left and right indices). It computes `mid = (left + right) / 2`, creates a new `Node` with the value at that index, then recursively builds the left subtree from indices `left..mid-1` and the right subtree from indices `mid+1..right`. The recursion terminates when `left > right`, returning `nullptr`. This guarantees that the tree is height-balanced because the number of nodes in each subtree differs by at most one.  

Edge cases:  
- The vector may have one element: the root is that element, both subtrees are `nullptr`.  
- Duplicate values: since the array is sorted, duplicates are adjacent. The middle selection may choose one of the duplicates; the resulting tree still satisfies BST property (left ≤ root ≤ right) if we allow equal values on either side. The recursive construction will place some duplicates in left or right subtrees, which is acceptable because the problem does not require strict inequality.  
- The recursive depth is O(log n) for a balanced tree, so no stack overflow for reasonable inputs.  

Time complexity: Each node is created exactly once, and each recursion step does O(1) work (excluding the vector access). Thus, O(n) time, where n is the number of elements. Space complexity: O(log n) for the recursion stack (height of balanced tree), plus O(n) if we ignore the storage for the tree itself (the tree nodes). The auxiliary space excluding the tree is O(log n).

#include <vector>
#include <cstddef>

struct Node {
    int val;
    Node* left;
    Node* right;
    Node(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Helper: Build a balanced BST from a sorted vector using indices.
Node* buildBSTHelper(const std::vector<int>& arr, int left, int right) {
    if (left > right) return nullptr;
    int mid = left + (right - left) / 2; // avoids potential overflow
    Node* root = new Node(arr[mid]);
    root->left = buildBSTHelper(arr, left, mid - 1);
    root->right = buildBSTHelper(arr, mid + 1, right);
    return root;
}

// Main function: returns root of balanced BST from sorted vector.
Node* buildBalancedBST(const std::vector<int>& sortedArray) {
    return buildBSTHelper(sortedArray, 0, static_cast<int>(sortedArray.size()) - 1);
}

#include <cassert>
#include <vector>
#include <algorithm>
#include <queue>

// Helper: compute height (max depth) of tree. Empty tree height = 0.
int treeHeight(Node* root) {
    if (root == nullptr) return 0;
    return 1 + std::max(treeHeight(root->left), treeHeight(root->right));
}

// Helper: check if tree is a valid BST (allows duplicates as per problem).
bool isBST(Node* root, int minVal, int maxVal) {
    if (root == nullptr) return true;
    if (root->val < minVal || root->val > maxVal) return false;
    return isBST(root->left, minVal, root->val) && isBST(root->right, root->val, maxVal);
}

// Helper: perform level-order traversal and return vector of values.
std::vector<int> levelOrderValues(Node* root) {
    std::vector<int> result;
    if (root == nullptr) return result;
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* curr = q.front();
        q.pop();
        result.push_back(curr->val);
        if (curr->left) q.push(curr->left);
        if (curr->right) q.push(curr->right);
    }
    return result;
}

// Helper: delete tree to avoid memory leaks.
void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    // Test 1: basic sorted array
    std::vector<int> arr1 = {1, 2, 3, 4, 5};
    Node* root1 = buildBalancedBST(arr1);
    assert(treeHeight(root1) == 3); // balanced: height = ceil(log2(5)) = 3
    assert(isBST(root1, INT_MIN, INT_MAX));
    auto level1 = levelOrderValues(root1);
    // Expected level-order for this balanced tree: 3, 1, 4, 2, 5 (depending on insertion)
    std::vector<int> expected1 = {3, 1, 4, 2, 5};
    assert(level1 == expected1);
    deleteTree(root1);

    // Test 2: single element
    std::vector<int> arr2 = {42};
    Node* root2 = buildBalancedBST(arr2);
    assert(treeHeight(root2) == 1);
    assert(isBST(root2, INT_MIN, INT_MAX));
    assert(levelOrderValues(root2) == std::vector<int>{42});
    deleteTree(root2);

    // Test 3: even number of elements (choose left middle: standard integer division)
    std::vector<int> arr3 = {10, 20, 30, 40};
    Node* root3 = buildBalancedBST(arr3);
    assert(treeHeight(root3) == 3);
    assert(isBST(root3, INT_MIN, INT_MAX));
    auto level3 = levelOrderValues(root3);
    std::vector<int> expected3 = {30, 10, 40, 20}; // because mid = (0+3)/2=1 -> arr[1]=20? Actually check: mid = 1 -> value 20, left=[10], right=[30,40]. So level order: 20, 10, 30, 40. Let's compute properly.
    // Let's recompute: arr[0]=10, arr[1]=20, arr[2]=30, arr[3]=40
    // mid = (0+3)/2 = 1 (integer) -> root=20. left subarray [0,0] -> root=10. right subarray [2,3] -> mid= (2+3)/2=2 -> root=30, right child =40.
    // Level order: 20, 10, 30, 40.
    std::vector<int> expected3_correct = {20, 10, 30, 40};
    assert(level3 == expected3_correct);
    deleteTree(root3);

    // Test 4: duplicates
    std::vector<int> arr4 = {5, 5, 5, 5, 5};
    Node* root4 = buildBalancedBST(arr4);
    assert(treeHeight(root4) == 3);
    assert(isBST(root4, INT_MIN, INT_MAX));
    auto level4 = levelOrderValues(root4);
    // All values are 5; any order works as long as size matches.
    assert(level4.size() == 5);
    for (int v : level4) assert(v == 5);
    deleteTree(root4);

    // Test 5: vector containing negative numbers
    std::vector<int> arr5 = {-10, -3, 0, 4, 9};
    Node* root5 = buildBalancedBST(arr5);
    assert(treeHeight(root5) == 3);
    assert(isBST(root5, INT_MIN, INT_MAX));
    auto level5 = levelOrderValues(root5);
    // Expected: root=0, left subtree {-10,-3}, right {4,9}. Level order: 0, -3, 4, -10, 9 (since mid for left -10,-3 -> mid = -3, left=-10; right 4,9 -> mid=4, right=9)
    std::vector<int> expected5 = {0, -3, 4, -10, 9};
    assert(level5 == expected5);
    deleteTree(root5);

    return 0;
}
