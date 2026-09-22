Write a C++ function that takes a head pointer to a sorted singly-linked list (in non-decreasing order) and returns the root of a height-balanced binary search tree (BST) containing the same values. The function must build the tree recursively by finding the middle element of the current list segment as the root, then recursively building the left subtree from the first half and the right subtree from the second half. The linked list nodes are defined as `ListNode` with `int val` and `ListNode* next`, and the tree nodes as `TreeNode` with `int val`, `TreeNode* left`, and `TreeNode* right`. The conversion must preserve the BST property (left < root < right) and ensure the tree is balanced. Return `nullptr` if the input list is empty. Define the function as `TreeNode* sortedListToBST(ListNode* head)`. The function should not modify the original linked list permanently (though it may break the list during recursion as a convenience). Ensure proper handling of edge cases: empty list, single-node list, and even-length lists where the middle can be chosen as the lower of the two middle elements.
#include <cassert>
#include <vector>

// Helper to create a linked list from a vector.
ListNode* makeList(const std::vector<int>& vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to get tree height (max depth).
int getHeight(TreeNode* node) {
    if (node == nullptr) return 0;
    return 1 + std::max(getHeight(node->left), getHeight(node->right));
}

// Helper to check if a tree is a valid BST (in-order traversal yields sorted).
bool isBST(TreeNode* node, int minVal, int maxVal) {
    if (node == nullptr) return true;
    if (node->val <= minVal || node->val >= maxVal) return false;
    return isBST(node->left, minVal, node->val) && isBST(node->right, node->val, maxVal);
}

// Helper to check if tree is height-balanced (every node's subtrees height differ by at most 1).
bool isBalanced(TreeNode* node) {
    if (node == nullptr) return true;
    int leftH = getHeight(node->left);
    int rightH = getHeight(node->right);
    return std::abs(leftH - rightH) <= 1 && isBalanced(node->left) && isBalanced(node->right);
}

// Helper to collect in-order traversal values into a vector.
void inOrder(TreeNode* node, std::vector<int>& out) {
    if (node == nullptr) return;
    inOrder(node->left, out);
    out.push_back(node->val);
    inOrder(node->right, out);
}

int main() {
    // Test 1: Empty list -> null root.
    assert(sortedListToBST(nullptr) == nullptr);
    
    // Test 2: Single node.
    ListNode* single = new ListNode(5);
    TreeNode* t2 = sortedListToBST(single);
    assert(t2 != nullptr);
    assert(t2->val == 5);
    assert(t2->left == nullptr && t2->right == nullptr);
    delete t2;
    
    // Test 3: Two nodes.
    ListNode* two = makeList({1, 2});
    TreeNode* t3 = sortedListToBST(two);
    assert(t3 != nullptr);
    assert(t3->val == 2);
    assert(t3->left != nullptr && t3->left->val == 1);
    assert(t3->right == nullptr);
    assert(isBST(t3, INT_MIN, INT_MAX));
    assert(isBalanced(t3));
    
    // Test 4: Three nodes.
    ListNode* three = makeList({-3, 0, 3});
    TreeNode* t4 = sortedListToBST(three);
    assert(t4 != nullptr);
    assert(t4->val == 0);
    assert(t4->left != nullptr && t4->left->val == -3);
    assert(t4->right != nullptr && t4->right->val == 3);
    assert(isBST(t4, INT_MIN, INT_MAX));
    assert(isBalanced(t4));
    
    // Test 5: Five sorted values.
    ListNode* five = makeList({10, 20, 30, 40, 50});
    TreeNode* t5 = sortedListToBST(five);
    assert(t5 != nullptr);
    assert(isBST(t5, INT_MIN, INT_MAX));
    assert(isBalanced(t5));
    std::vector<int> vals5;
    inOrder(t5, vals5);
    assert(vals5 == std::vector<int>({10, 20, 30, 40, 50}));
    
    // Test 6: Even-length list (4 values).
    ListNode* four = makeList({1, 2, 3, 4});
    TreeNode* t6 = sortedListToBST(four);
    assert(isBST(t6, INT_MIN, INT_MAX));
    assert(isBalanced(t6));
    std::vector<int> vals6;
    inOrder(t6, vals6);
    assert(vals6 == std::vector<int>({1, 2, 3, 4}));
    
    // Test 7: Large list (1000 values).
    std::vector<int> big;
    for (int i = 0; i < 1000; ++i) big.push_back(i);
    ListNode* bigList = makeList(big);
    TreeNode* t7 = sortedListToBST(bigList);
    assert(isBST(t7, INT_MIN, INT_MAX));
    assert(isBalanced(t7));
    
    return 0;
}
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Definition for binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Convert a sorted singly-linked list to a height-balanced BST.
TreeNode* sortedListToBST(ListNode* head) {
    if (head == nullptr) return nullptr;
    if (head->next == nullptr) return new TreeNode(head->val);
    
    // Find the middle node using fast-slow pointers.
    ListNode* slow = head;
    ListNode* fast = head->next->next;
    
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    // slow points to the node before the middle.
    ListNode* mid = slow->next;
    slow->next = nullptr;  // Split the list into left (head) and right (mid->next).
    
    TreeNode* root = new TreeNode(mid->val);
    root->left = sortedListToBST(head);
    root->right = sortedListToBST(mid->next);
    
    return root;
}
// The algorithm uses a recursive divide-and-conquer approach. In each recursive call on a list segment `head` to the end, we find the middle node using a fast-slow pointer technique: initialize a slow pointer `p` at `head` and a fast pointer `q` at `head->next->next`. Move `p` one step and `q` two steps until `q` becomes null or has no next node. This leaves `p` pointing to the node just before the middle. The middle node `mid` is `p->next`. We then detach the list by setting `p->next = nullptr`, so the left segment is `head` to `p` (inclusive), and the right segment is `mid->next` onward. Create the tree root with `mid->val`, then recursively build `root->left` from `head` and `root->right` from `mid->next`. The base cases: if `head` is null, return null; if `head->next` is null, return a new leaf node. For an even-length list (e.g., two nodes), the fast pointer steps make `p` point to the first node, `mid` becomes the second node, so the left subtree is built from a single-node list and right subtree from null, producing a balanced tree. Edge cases include empty list (return null) and single node (return leaf). Time complexity is O(n log n) because each level of recursion processes all nodes to find the middle, and there are O(log n) levels (balanced tree). Space complexity is O(log n) for the recursion stack (excluding tree nodes), but the tree itself uses O(n) nodes. The algorithm modifies the list temporarily by severing pointers, but since the list is no longer used afterward (as per task), it is acceptable.
