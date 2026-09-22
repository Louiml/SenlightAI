// Write a C++ function `ListNode* removeKthFromEnd(ListNode* head, int k)` that removes the k-th node from the end of a singly linked list and returns the head of the modified list. The list is non-empty, k is guaranteed to be between 1 and the list length inclusive. The function must handle deletion of the head node correctly, use only constant extra space (no arrays or vectors), and must not call `delete` on nodes (just re-link pointers). The list nodes are defined as: `struct ListNode { int val; ListNode *next; ListNode(int x) : val(x), next(nullptr) {} };` The function should work for lists of length 1 up to arbitrarily large sizes, and should preserve the order of remaining nodes.

The simplest approach is a two-pass method: first traverse the entire list to compute its length `L`. Then the target node to remove is at position `L - k + 1` (1-indexed from the head). If the target is the head (i.e., `L - k + 1 == 1`), we just return `head->next`. Otherwise, we traverse to the node just before the target (i.e., `L - k` steps from the head) and re-link its `next` pointer to skip the target node. The main edge case is removing the head, which requires no pointer re-linking but returning the second node as the new head. Another edge case is when the list has only one node and k == 1, which returns `nullptr`. Time complexity is O(L) because we traverse the list twice (or once to compute length and once to find the predecessor), and space complexity is O(1) since we only use a few temporary pointers and an integer. We must be careful not to dereference `nullptr` when the target is the last node; however, the predecessor’s `next` will be set to `nullptr` after removal, which is fine.

#include <cstdlib>  // for nullptr

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Removes the k-th node from the end of the list and returns the new head.
ListNode* removeKthFromEnd(ListNode* head, int k) {
    // First pass: compute the length of the list.
    int length = 0;
    ListNode* current = head;
    while (current != nullptr) {
        ++length;
        current = current->next;
    }

    // The position (1-indexed) of the node to remove from the head.
    int targetPosition = length - k + 1;

    // If removing the head, just return the next node.
    if (targetPosition == 1) {
        return head->next;
    }

    // Otherwise, traverse to the node just before the target.
    ListNode* previous = head;
    for (int i = 1; i < targetPosition - 1; ++i) {
        previous = previous->next;
    }

    // Skip the target node.
    previous->next = previous->next->next;

    return head;
}

#include <cassert>

// Helper to create a list from a vector for testing (not part of solution).
ListNode* createList(const std::initializer_list<int>& values) {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    for (int v : values) {
        ListNode* node = new ListNode(v);
        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

// Helper to convert list to a vector for comparison.
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

int main() {
    // Test 1: Remove middle node from a 5-node list.
    ListNode* list1 = createList({1, 2, 3, 4, 5});
    ListNode* result1 = removeKthFromEnd(list1, 2); // remove node with value 4
    assert(listToVector(result1) == std::vector<int>({1, 2, 3, 5}));

    // Test 2: Remove head node.
    ListNode* list2 = createList({1, 2, 3});
    ListNode* result2 = removeKthFromEnd(list2, 3); // remove head (value 1)
    assert(listToVector(result2) == std::vector<int>({2, 3}));

    // Test 3: Remove last node.
    ListNode* list3 = createList({7, 8, 9});
    ListNode* result3 = removeKthFromEnd(list3, 1); // remove value 9
    assert(listToVector(result3) == std::vector<int>({7, 8}));

    // Test 4: Single-node list, remove it.
    ListNode* list4 = createList({42});
    ListNode* result4 = removeKthFromEnd(list4, 1);
    assert(result4 == nullptr);

    // Test 5: Two-node list, remove first from end (head).
    ListNode* list5 = createList({10, 20});
    ListNode* result5 = removeKthFromEnd(list5, 2);
    assert(listToVector(result5) == std::vector<int>({20}));

    // Test 6: Remove from end in a larger list.
    ListNode* list6 = createList({1, 2, 3, 4, 5, 6});
    ListNode* result6 = removeKthFromEnd(list6, 5); // remove value 2
    assert(listToVector(result6) == std::vector<int>({1, 3, 4, 5, 6}));

    return 0;
}
