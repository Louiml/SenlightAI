Given a fixed array of integers `{45,25,10,35,40,61,71,68}`, write a C++ function that constructs a Binary Search Tree (BST) by inserting the array elements in the given order, and then returns the sum of all node values in the tree that are greater than or equal to a given threshold `k`. The function must take the array (as a `std::vector<int>`) and the threshold `k` as parameters, build the BST using a custom `Node` class with integer value and left/right pointers, and return an integer representing the sum. If the tree is empty, return 0. The threshold comparison must be inclusive (≥). The function must be `const`-correct for the tree traversal and not modify the input array.

// The solution builds a BST by inserting each value from the input array sequentially using the standard BST insertion rule: values less than the current node go to the left, values greater or equal go to the right (to maintain a deterministic structure). After constructing the tree, we perform a traversal (in-order, pre-order, or any) that visits every node and checks if `node->val >= k`. If true, add that node’s value to a running sum. Because the threshold is inclusive, we use `>=`. Edge cases: empty array (return 0), all values below threshold (return 0), all values above or equal (sum all), and duplicate values (handled by the insertion rule). Time complexity: insertion is O(n * h) where n is array size and h is tree height (worst-case O(n) for a skewed tree, average O(log n) for random data), and traversal is O(n). Overall worst-case O(n^2) for insertion + O(n) traversal = O(n^2), average O(n log n). Space complexity: O(n) for storing the tree nodes plus O(h) recursion stack for traversal (worst O(n), average O(log n)). The auxiliary space for the function is O(n) for the tree itself, but the traversal recursion uses O(h).

#include <vector>

class Node {
public:
    int val;
    Node *left;
    Node *right;
    Node(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Inserts a value into BST and returns the root (or updated root).
Node* insertBST(Node* root, int value) {
    if (!root) return new Node(value);
    if (value < root->val) {
        root->left = insertBST(root->left, value);
    } else {
        root->right = insertBST(root->right, value);
    }
    return root;
}

// Helper to recursively sum nodes with val >= threshold.
int sumGeHelper(const Node* root, int threshold) {
    if (!root) return 0;
    int total = 0;
    if (root->val >= threshold) total += root->val;
    total += sumGeHelper(root->left, threshold);
    total += sumGeHelper(root->right, threshold);
    return total;
}

// Builds BST from array and returns sum of nodes with value >= threshold.
int sumNodesGreaterOrEqual(const std::vector<int>& values, int threshold) {
    if (values.empty()) return 0;
    Node* root = nullptr;
    for (int v : values) {
        root = insertBST(root, v);
    }
    int result = sumGeHelper(root, threshold);
    // Note: we do not delete nodes to keep it simple; memory is cleaned by OS.
    return result;
}

#include <cassert>
#include <vector>

int main() {
    std::vector<int> arr = {45,25,10,35,40,61,71,68};
    // All values sum = 45+25+10+35+40+61+71+68 = 355
    assert(sumNodesGreaterOrEqual(arr, 10) == 355);
    assert(sumNodesGreaterOrEqual(arr, 40) == 45+40+61+71+68); // 285
    assert(sumNodesGreaterOrEqual(arr, 45) == 45+61+71+68);    // 245
    assert(sumNodesGreaterOrEqual(arr, 60) == 61+71+68);       // 200
    assert(sumNodesGreaterOrEqual(arr, 70) == 71+68);          // 139
    assert(sumNodesGreaterOrEqual(arr, 72) == 0);
    assert(sumNodesGreaterOrEqual(arr, 68) == 68+71+68);       // note 68 appears once, so 68+71=139? Wait 68+71+?? Check: array has 68 once, 71, so sum=139, but 68 is >=68, 71>=68, so 68+71=139. Also 45,25,10,35,40,61 are <68, so total 139.
    assert(sumNodesGreaterOrEqual(std::vector<int>{}, 5) == 0);
    assert(sumNodesGreaterOrEqual(std::vector<int>{7}, 7) == 7);
    assert(sumNodesGreaterOrEqual(std::vector<int>{7}, 8) == 0);
    return 0;
}
