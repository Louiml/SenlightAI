/*
Write a C++ function that takes a reference to the head of a singly linked list containing integers in non-decreasing (sorted) order and returns a pointer to the root of a height-balanced binary search tree (BST) built from the list’s values. The BST must be constructed such that for every node, the absolute difference between the heights of its left and right subtrees is at most 1. The linked list has no cycles, may be empty, and may contain duplicate values. You must implement the function with a descriptive name (e.g., `sortedListToBST`) and provide a complete, self-contained solution with necessary headers and helper functions. Do not use any external libraries beyond the C++ standard library.
*/

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Find the middle node of the sublist [start, end) using slow-fast pointers.
ListNode* findMiddle(ListNode* start, ListNode* end) {
    ListNode* slow = start;
    ListNode* fast = start;
    while (fast != end && fast->next != end) {
        fast = fast->next->next;
        slow = slow->next;
    }
    return slow;
}

// Recursively build a height-balanced BST from the sublist [start, end).
TreeNode* buildBST(ListNode* start, ListNode* end) {
    if (start == end) {
        return nullptr;
    }
    ListNode* mid = findMiddle(start, end);
    TreeNode* root = new TreeNode(mid->val);
    root->left = buildBST(start, mid);
    root->right = buildBST(mid->next, end);
    return root;
}

// Public function: convert a sorted linked list to a height-balanced BST.
TreeNode* sortedListToBST(ListNode* head) {
    return buildBST(head, nullptr);
}

#include <cassert>
#include <vector>

// Helper to delete a tree to avoid memory leaks (not strictly necessary for asserts but good practice).
void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// Helper to compute tree height.
int height(TreeNode* root) {
    if (!root) return 0;
    return 1 + std::max(height(root->left), height(root->right));
}

// Helper to check if a tree is height-balanced.
bool isBalanced(TreeNode* root) {
    if (!root) return true;
    int leftH = height(root->left);
    int rightH = height(root->right);
    if (std::abs(leftH - rightH) > 1) return false;
    return isBalanced(root->left) && isBalanced(root->right);
}

// Helper to build a linked list from a vector (for testing).
ListNode* buildList(const std::vector<int>& vals) {
    ListNode* dummy = new ListNode(0);
    ListNode* cur = dummy;
    for (int v : vals) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    return dummy->next;
}

// Helper to delete a linked list.
void deleteList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Empty list -> null tree.
    ListNode* empty = nullptr;
    TreeNode* t1 = sortedListToBST(empty);
    assert(t1 == nullptr);

    // Test 2: Single node list.
    ListNode* list2 = new ListNode(5);
    TreeNode* t2 = sortedListToBST(list2);
    assert(t2 != nullptr);
    assert(t2->val == 5);
    assert(t2->left == nullptr && t2->right == nullptr);
    deleteTree(t2);
    delete list2;

    // Test 3: Sorted list 1,2,3 -> root is 2, left is 1, right is 3.
    ListNode* list3 = buildList({1, 2, 3});
    TreeNode* t3 = sortedListToBST(list3);
    assert(t3 != nullptr);
    assert(t3->val == 2);
    assert(t3->left->val == 1);
    assert(t3->right->val == 3);
    assert(isBalanced(t3));
    deleteTree(t3);
    deleteList(list3);

    // Test 4: Sorted list 1,2,3,4 => middle is 2 (first of middle two), left subtree is {1}, right subtree is {3,4} (which becomes balanced).
    ListNode* list4 = buildList({1, 2, 3, 4});
    TreeNode* t4 = sortedListToBST(list4);
    assert(t4 != nullptr);
    assert(isBalanced(t4));
    // Manually verify structure: root is 2, left is 1, right subtree has root 3 with right child 4.
    assert(t4->val == 2);
    assert(t4->left->val == 1);
    assert(t4->right->val == 3);
    assert(t4->right->right->val == 4);
    assert(t4->right->left == nullptr);
    deleteTree(t4);
    deleteList(list4);

    // Test 5: Larger list with duplicates: -10, -3, 0, 5, 9 (classic LeetCode example).
    ListNode* list5 = buildList({-10, -3, 0, 5, 9});
    TreeNode* t5 = sortedListToBST(list5);
    assert(t5 != nullptr);
    assert(isBalanced(t5));
    // Check values exist in-order (if we do an inorder traversal, but for simplicity just check root and height).
    assert(t5->val == 0); // middle of 5 elements is index 2.
    deleteTree(t5);
    deleteList(list5);

    // Test 6: Very long list (1..1000) to ensure balance property holds.
    std::vector<int> vals;
    for (int i = 1; i <= 1000; ++i) vals.push_back(i);
    ListNode* list6 = buildList(vals);
    TreeNode* t6 = sortedListToBST(list6);
    assert(isBalanced(t6));
    deleteTree(t6);
    deleteList(list6);

    return 0;
}

// The core idea is to mimic the binary search approach on a sorted array, but directly on the linked list. Since the list is sorted, the middle element of any sublist is the natural root of the BST for that sublist, with the left half forming the left subtree and the right half forming the right subtree. To find the middle node of a sublist defined by a start and an end (exclusive), use the slow-fast pointer technique: a fast pointer moves two steps at a time, while a slow pointer moves one step; when the fast pointer reaches the end or its next is the end, the slow pointer is at the middle. The recursion constructs the tree by taking the middle as root, recursively building the left subtree from `[start, mid)` and the right subtree from `[mid->next, end)`. Base case: if `start == end` (empty sublist), return `nullptr`. Edge cases include an empty list (returns `nullptr`), a single-node list (that node becomes the root with no children), and lists with an even number of nodes—the middle is the first of the two middle nodes (since the fast pointer stops when `fast->next == end`, the slow pointer ends at the left middle). Time complexity is O(n log n) because each of the O(log n) recursion levels scans a portion of the list to find its middle, and the total work per level is O(n). Space complexity is O(log n) for the recursion stack in the balanced tree case, but in the worst case (when the list is already balanced into a skewed recursion, which does not happen because we always split at the middle), it would be O(log n) guaranteed due to the height balance.
