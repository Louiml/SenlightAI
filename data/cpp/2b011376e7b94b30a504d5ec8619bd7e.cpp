// Write a C++ function `removeAllOccurrences` that takes the head of a singly linked list and an integer value, and returns the head of a new list with all nodes whose `val` equals the given value removed. The function must handle empty lists, lists where all nodes are removed, consecutive duplicate removals, and lists with no matches. It should free the memory of every removed node using `delete` to avoid leaks. The input list is not modified beyond removing the target nodes; the remaining nodes must preserve their original relative order.
#include <cassert>

// Helper to create a list from an initializer list for testing.
ListNode* createList(std::initializer_list<int> values) {
    ListNode dummy;
    ListNode* tail = &dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to check if two lists are equal (and clean up both).
bool listsEqual(ListNode* a, ListNode* b) {
    while (a != nullptr && b != nullptr) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

// Helper to delete an entire list to avoid memory leaks in tests.
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Empty list
    assert(removeAllOccurrences(nullptr, 5) == nullptr);

    // All nodes removed
    ListNode* list1 = createList({3, 3, 3});
    ListNode* result1 = removeAllOccurrences(list1, 3);
    assert(result1 == nullptr);

    // No matches
    ListNode* list2 = createList({1, 2, 3});
    ListNode* result2 = removeAllOccurrences(list2, 4);
    assert(listsEqual(result2, createList({1, 2, 3})));
    deleteList(result2);

    // Remove head only
    ListNode* list3 = createList({5, 1, 2});
    ListNode* result3 = removeAllOccurrences(list3, 5);
    assert(listsEqual(result3, createList({1, 2})));
    deleteList(result3);

    // Remove tail only
    ListNode* list4 = createList({1, 2, 5});
    ListNode* result4 = removeAllOccurrences(list4, 5);
    assert(listsEqual(result4, createList({1, 2})));
    deleteList(result4);

    // Consecutive middle removals
    ListNode* list5 = createList({1, 5, 5, 2});
    ListNode* result5 = removeAllOccurrences(list5, 5);
    assert(listsEqual(result5, createList({1, 2})));
    deleteList(result5);

    // Alternating removals
    ListNode* list6 = createList({5, 1, 5, 2, 5});
    ListNode* result6 = removeAllOccurrences(list6, 5);
    assert(listsEqual(result6, createList({1, 2})));
    deleteList(result6);

    // Large list with no removals
    ListNode* list7 = createList({1, 2, 3, 4, 5});
    ListNode* result7 = removeAllOccurrences(list7, 6);
    assert(listsEqual(result7, createList({1, 2, 3, 4, 5})));
    deleteList(result7);

    return 0;
}
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// Removes all nodes with the given value from the linked list.
// Returns the head of the updated list. The caller retains ownership of the
// returned list and the function deletes all removed nodes.
ListNode* removeAllOccurrences(ListNode* head, int val) {
    // Remove any leading nodes that have the target value.
    while (head != nullptr && head->val == val) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }

    // Traverse the remaining list and remove subsequent matching nodes.
    ListNode* current = head;
    while (current != nullptr && current->next != nullptr) {
        if (current->next->val == val) {
            ListNode* temp = current->next;
            current->next = current->next->next;
            delete temp;
        } else {
            current = current->next;
        }
    }

    return head;
}
// The solution uses a two-phase approach. First, the head of the list is advanced past any leading nodes whose value equals the target, deleting each as it is skipped. This handles the case where the first node(s) need removal and ensures the returned head is either `nullptr` or a node with a value different from the target. Second, a traversal is performed starting from the new head. For each current node, if its next node contains the target value, that next node is deleted and the current node’s `next` pointer is updated to skip it. When the next node’s value is not the target, the traversal simply moves to the next node. This avoids skipping over newly exposed nodes after a deletion, since the current node is not advanced when a deletion occurs. Edge cases include an empty list (returns `nullptr` immediately), a list where all nodes are removed (the head becomes `nullptr` after the first phase, and the second loop is skipped), and a list with no matches (the loop simply traverses the entire list without deletions). The time complexity is O(n) where n is the number of nodes, as each node is visited at most twice. The space complexity is O(1) auxiliary, aside from the memory freed for removed nodes.
