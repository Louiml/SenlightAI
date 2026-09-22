/*
Write a C++ function named `rotateListRight` that takes the head of a singly linked list (where each node has an integer `val` and a pointer to the next node) and a non-negative integer `k`, and returns the head of the list after rotating it to the right by `k` positions. Rotation means that the last `k % length` nodes are moved to the front in their original order, and the remaining prefix is appended after them. The function must handle an empty list, a single-node list, and cases where `k` is zero or a multiple of the list length (in which case the list is unchanged). The input list may contain up to 500 nodes, values between -100 and 100, and `k` can be as large as 2×10⁹. Your implementation should not allocate new nodes; it should only rearrange existing pointers. Provide a clean, self-contained function with proper `const` correctness where applicable (note: since we modify the list, the head parameter is non-const, but you can mark it as a pointer to non-const `ListNode`). The function must be standalone, include necessary headers, and not rely on any external library beyond the standard `List` node definition you provide.
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

// Rotate the linked list to the right by k positions.
ListNode* rotateListRight(ListNode* head, int k) {
    if (head == nullptr || k == 0) {
        return head;
    }

    // Compute the length of the list.
    ListNode* current = head;
    int length = 0;
    while (current != nullptr) {
        ++length;
        current = current->next;
    }

    // If only one node or rotation is a multiple of length, no change.
    if (length == 1) {
        return head;
    }

    int effective_k = k % length;
    if (effective_k == 0) {
        return head;
    }

    // Find the node at position (length - effective_k).
    // This node's next will become the new head.
    int steps_to_tail = length - effective_k;
    ListNode* break_node = head;
    for (int i = 1; i < steps_to_tail; ++i) {
        break_node = break_node->next;
    }

    // Splice the list: new head is break_node->next.
    ListNode* new_head = break_node->next;
    break_node->next = nullptr;

    // Find the old tail and link it to the original head.
    ListNode* tail = new_head;
    while (tail->next != nullptr) {
        tail = tail->next;
    }
    tail->next = head;

    return new_head;
}

#include <cassert>
#include <cstddef>

// Assume the ListNode definition and rotateListRight from above are included here.

int main() {
    // Helper to create a list from an initializer list.
    auto makeList = [](std::initializer_list<int> vals) {
        ListNode* head = nullptr;
        ListNode* tail = nullptr;
        for (int v : vals) {
            ListNode* node = new ListNode(v);
            if (head == nullptr) {
                head = node;
            } else {
                tail->next = node;
            }
            tail = node;
        }
        return head;
    };

    // Helper to compare a list to an expected sequence.
    auto assertList = [](ListNode* head, std::initializer_list<int> expected) {
        ListNode* cur = head;
        for (int v : expected) {
            assert(cur != nullptr);
            assert(cur->val == v);
            cur = cur->next;
        }
        assert(cur == nullptr);
    };

    // Test 1: Example 1
    ListNode* list1 = makeList({1, 2, 3, 4, 5});
    ListNode* rotated1 = rotateListRight(list1, 2);
    assertList(rotated1, {4, 5, 1, 2, 3});

    // Test 2: Example 2
    ListNode* list2 = makeList({0, 1, 2});
    ListNode* rotated2 = rotateListRight(list2, 4);
    assertList(rotated2, {2, 0, 1});

    // Test 3: k = 0
    ListNode* list3 = makeList({1, 2, 3});
    ListNode* rotated3 = rotateListRight(list3, 0);
    assertList(rotated3, {1, 2, 3});

    // Test 4: k is a multiple of length
    ListNode* list4 = makeList({1, 2, 3, 4});
    ListNode* rotated4 = rotateListRight(list4, 8);
    assertList(rotated4, {1, 2, 3, 4});

    // Test 5: Single node
    ListNode* list5 = makeList({7});
    ListNode* rotated5 = rotateListRight(list5, 100);
    assertList(rotated5, {7});

    // Test 6: Empty list
    ListNode* list6 = nullptr;
    ListNode* rotated6 = rotateListRight(list6, 5);
    assert(rotated6 == nullptr);

    // Test 7: Two nodes, k=1
    ListNode* list7 = makeList({1, 2});
    ListNode* rotated7 = rotateListRight(list7, 1);
    assertList(rotated7, {2, 1});

    // Test 8: Two nodes, k=3 (mod 2 = 1)
    ListNode* list8 = makeList({1, 2});
    ListNode* rotated8 = rotateListRight(list8, 3);
    assertList(rotated8, {2, 1});

    // Test 9: Negative values
    ListNode* list9 = makeList({-10, 20, -30});
    ListNode* rotated9 = rotateListRight(list9, 1);
    assertList(rotated9, {-30, -10, 20});

    // Test 10: Large k value within constraints
    ListNode* list10 = makeList({1, 2, 3, 4, 5});
    ListNode* rotated10 = rotateListRight(list10, 2000000000);
    // 2e9 % 5 = 0, so unchanged
    assertList(rotated10, {1, 2, 3, 4, 5});

    // Clean up (optional, but for completeness).
    // In real code, we would delete all nodes, but this is a test snippet.
}

// The core idea is to first compute the length of the list, `n`, by traversing it once. Since `k` can be huge, we reduce it modulo `n` to get the effective number of rotations, `r = k % n`. If the list is empty, has one node, or `r == 0`, the list is already rotated, so we return the original head. Otherwise, we need to move the last `r` nodes to the front. To do this, we find the node at position `n - r` (the last node of the prefix that will become the new tail). We traverse the list with a pointer `p` starting at the head, moving `n - r - 1` steps (so `p` ends at the node before the new head). The new head is `p->next`. We set `p->next = nullptr` to break the list. Then we traverse from the new head to the last node (which is the original tail) and set its next pointer to the original head, forming the rotated list. Edge cases include when `n - r` equals `n` (i.e., `r == 0`), which we handle early, and when the list length is 0 or 1, also handled early. Time complexity is O(n) for the length calculation and O(n) for the traversal to find the break point, so overall O(n). Space complexity is O(1) since we only use a few pointers.
