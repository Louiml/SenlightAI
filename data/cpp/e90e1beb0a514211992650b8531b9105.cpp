// Write a C++ function that, given a vector of integers representing values to insert into a Binary Search Tree (BST), returns the height of the resulting tree. The height is defined as the number of nodes on the longest path from the root to a leaf (the root contributes 1). The BST insertion rule is: if a value is less than or equal to the current node's value, it goes to the left; otherwise, it goes to the right. You must implement the BST construction and height computation from scratch, without using any standard library containers for the tree itself. The function should be efficient for up to \(10^5\) nodes and handle duplicates gracefully (duplicates are inserted as left children, never breaking the tree).
The core algorithm is recursive tree building: start with an empty root (nullptr). For each integer in the input vector, recursively insert it following the BST property (<= left, > right). After all insertions, compute the height recursively: for a null node, height is 0; for a non-null node, height is 1 + max(height(left), height(right)). Important edge cases: an empty input vector should return 0 (height of an empty tree); a single node returns 1; all equal values produce a degenerate left-skewed tree of height equal to the number of nodes (since equal values go left). Time complexity: insertion per element is \(O(h)\) where \(h\) is current height, so worst-case \(O(n^2)\) for sorted input; height computation is \(O(n)\) after building. Space complexity: \(O(n)\) for the tree nodes plus \(O(h)\) recursion stack for both insertion and height.
#include <vector>
#include <algorithm>

struct Node {
    int val;
    Node* left;
    Node* right;
    Node(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Helper: insert a value into BST rooted at 'root' (may be nullptr)
Node* insertNode(Node* root, int val) {
    if (root == nullptr) {
        return new Node(val);
    }
    if (val <= root->val) {
        root->left = insertNode(root->left, val);
    } else {
        root->right = insertNode(root->right, val);
    }
    return root;
}

// Helper: compute height of BST (0 for empty)
int findHeight(const Node* root) {
    if (root == nullptr) return 0;
    return 1 + std::max(findHeight(root->left), findHeight(root->right));
}

// Free function: build BST from values and return its height
int bstHeight(const std::vector<int>& values) {
    Node* root = nullptr;
    for (int v : values) {
        root = insertNode(root, v);
    }
    int h = findHeight(root);
    // Clean up: simple recursive deletion to avoid memory leak (optional but proper)
    // (not shown in code block? but for a self-contained function, we can omit deletion
    //  since the problem is about the algorithm; but to be safe, we can include a helper)
    // For simplicity and to match the task, we just return; memory is not required to be freed in the function.
    return h;
}
#include <cassert>
#include <vector>

// Declaration of the solution function (already defined above)
int bstHeight(const std::vector<int>& values);

int main() {
    assert(bstHeight({}) == 0);
    assert(bstHeight({5}) == 1);
    assert(bstHeight({5, 3, 7}) == 2);
    assert(bstHeight({5, 3, 7, 2, 4, 6, 8}) == 3);
    assert(bstHeight({1, 2, 3, 4}) == 4); // sorted ascending -> right-skewed
    assert(bstHeight({4, 3, 2, 1}) == 4); // sorted descending -> left-skewed
    assert(bstHeight({2, 2, 2}) == 3);    // duplicates go left
    assert(bstHeight({10, 5, 15, 3, 7, 12, 18, 1}) == 4);
    assert(bstHeight({1, 1, 1, 1}) == 4); // all duplicates
    return 0;
}
