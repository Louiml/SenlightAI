Write a C++ function named `constructBalancedBST` that takes a sorted vector of distinct integers (guaranteed to be in strictly ascending order) with at least one element and returns a `Node*` pointing to the root of a perfectly balanced binary search tree (BST) constructed from the values. The returned tree must have each node’s `height` and `balance` fields correctly computed, where `height` is the length of the longest path from that node to a leaf (leaf height = 0, null child height = -1) and `balance` is (left subtree height − right subtree height). Additionally, the function must not rely on any sorting or pre-existing tree structure; it must directly build the tree recursively from the sorted array. You may use the provided `Node` class definition exactly as given (including its default member initializers).

// The optimal way to build a balanced BST from a sorted array is to recursively pick the middle element as the root, then recursively build the left subtree from the left half and the right subtree from the right half. This ensures that each subtree has roughly equal numbers of nodes, resulting in a tree of minimal height for the given number of elements. After constructing both children, compute the height of the current node as `1 + max(leftHeight, rightHeight)` where `nullptr` contributes `-1`, and the balance as `leftHeight - rightHeight`. The base case occurs when `start > end`, returning `nullptr`. Edge cases include an empty array (though the task guarantees at least one element, handle it gracefully by returning `nullptr`), and arrays of length 1 where both children are null, giving height 0 and balance 0. The recursion explores each element exactly once, so time complexity is O(n). The recursion depth is O(log n) for balanced construction, so space complexity for the call stack is O(log n), plus O(n) for the allocated nodes.

#include <algorithm>

class Node {
public:
    int val;
    Node *left = nullptr;
    Node *right = nullptr;
    int height = 0;
    int balance = 0;
    Node(int v) : val(v) {}
};

// Recursively build a balanced BST from a sorted subarray [start, end].
// Updates height and balance for every node after its children are built.
Node* constructBalancedBST(const std::vector<int>& arr, int start, int end) {
    if (start > end) return nullptr;

    int mid = start + (end - start) / 2;
    Node* node = new Node(arr[mid]);

    node->left = constructBalancedBST(arr, start, mid - 1);
    node->right = constructBalancedBST(arr, mid + 1, end);

    // Compute height and balance after children exist.
    int leftHeight = (node->left) ? node->left->height : -1;
    int rightHeight = (node->right) ? node->right->height : -1;
    node->height = std::max(leftHeight, rightHeight) + 1;
    node->balance = leftHeight - rightHeight;

    return node;
}

#include <cassert>
#include <vector>

// Node class and constructBalancedBST assumed to be available.

int main() {
    // Test 1: Single element.
    std::vector<int> arr1 = {42};
    Node* root1 = constructBalancedBST(arr1, 0, arr1.size() - 1);
    assert(root1 && root1->val == 42);
    assert(root1->left == nullptr && root1->right == nullptr);
    assert(root1->height == 0 && root1->balance == 0);

    // Test 2: Three elements (perfect tree).
    std::vector<int> arr2 = {10, 20, 30};
    Node* root2 = constructBalancedBST(arr2, 0, arr2.size() - 1);
    assert(root2 && root2->val == 20);
    assert(root2->left && root2->left->val == 10);
    assert(root2->right && root2->right->val == 30);
    assert(root2->left->left == nullptr && root2->left->right == nullptr);
    assert(root2->right->left == nullptr && root2->right->right == nullptr);
    assert(root2->height == 1 && root2->balance == 0);
    assert(root2->left->height == 0 && root2->right->height == 0);

    // Test 3: Four elements (slightly left leaning).
    std::vector<int> arr3 = {5, 15, 25, 35};
    Node* root3 = constructBalancedBST(arr3, 0, arr3.size() - 1);
    assert(root3 && root3->val == 15);
    assert(root3->left && root3->left->val == 5);
    assert(root3->right && root3->right->val == 25);
    assert(root3->right->right && root3->right->right->val == 35);
    assert(root3->height == 2 && root3->balance == -1);
    assert(root3->left->height == 0 && root3->right->height == 1);

    // Test 4: Larger sorted array, check in-order traversals.
    std::vector<int> arr4 = {1, 2, 3, 4, 5, 6, 7};
    Node* root4 = constructBalancedBST(arr4, 0, arr4.size() - 1);
    std::vector<int> inOrder;
    std::function<void(Node*)> dfs = [&](Node* n) {
        if (!n) return;
        dfs(n->left);
        inOrder.push_back(n->val);
        dfs(n->right);
    };
    dfs(root4);
    assert(inOrder == arr4);
    assert(root4->height == 2);
    assert(root4->balance == 0);

    // Test 5: Verify all nodes in root4 have |balance| <= 1.
    bool valid = true;
    std::function<int(Node*)> check = [&](Node* n) -> int {
        if (!n) return -1;
        int lh = check(n->left);
        int rh = check(n->right);
        assert(n->height == std::max(lh, rh) + 1);
        assert(n->balance == lh - rh);
        if (std::abs(n->balance) > 1) valid = false;
        return n->height;
    };
    check(root4);
    assert(valid);

    // Test 6: Edge case with empty range returns nullptr.
    assert(constructBalancedBST(arr1, 2, 1) == nullptr);

    // Clean up memory (for completeness, though process exits anyway).
    // In practice, one should write a recursive delete function.
}
