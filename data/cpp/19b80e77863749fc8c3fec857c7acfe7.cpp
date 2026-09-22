// Write a C++ function named `removeAllOccurrences` that takes a singly linked list (represented by a `ListNode` struct with `int val` and `ListNode* next`) and an integer `target`, and returns the head of the list after removing **all** nodes whose value equals `target`. The function must handle an empty list, a list where all nodes match, and a list with consecutive or scattered matches. The original list order must be preserved for remaining nodes. Do not modify the input values; only unlink matching nodes and properly manage memory by deleting removed nodes.

The core approach is a two‑phase algorithm. First, skip any matching nodes at the beginning of the list so that the new head is either `nullptr` (if all nodes match) or the first non‑matching node. This handles the edge case where the original head needs to change. Second, iterate through the list starting from this new head, and for each node, check whether its next node’s value equals `target`. If so, temporarily store the matching next node, bypass it by setting `curr->next = curr->next->next`, and delete the stored node to free memory. Otherwise, advance `curr`. This works because once the head is non‑matching, we never need to re‑check the head itself; we only look ahead. Edge cases: empty list (return `nullptr` immediately), all nodes match (after the first loop, `curr` becomes `nullptr`, and the second loop is skipped), and the target appears at the tail (the `curr->next` check naturally stops before `curr` becomes `nullptr`). Time complexity is \(O(n)\) with a single pass for the head‑skipping phase and another for the removal loop, but combined it’s still linear. Space complexity is \(O(1)\) auxiliary, not counting the list nodes themselves, since we only use a few pointers.

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Remove all nodes with the given target value and return the new head.
ListNode* removeAllOccurrences(ListNode* head, int target) {
    // Remove matching nodes from the front of the list.
    while (head != nullptr && head->val == target) {
        ListNode* toDelete = head;
        head = head->next;
        delete toDelete;
    }
    
    // If the list is empty (all nodes were removed), return nullptr.
    if (head == nullptr) {
        return nullptr;
    }
    
    // Iterate through the list, removing any matching nodes after the current node.
    ListNode* curr = head;
    while (curr->next != nullptr) {
        if (curr->next->val == target) {
            ListNode* toDelete = curr->next;
            curr->next = curr->next->next;
            delete toDelete;
            // Do not advance curr; the new curr->next may also match.
        } else {
            curr = curr->next;
        }
    }
    
    return head;
}

#include <cassert>

// Helper to create a linked list from a vector (for testing).
ListNode* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    ListNode* head = new ListNode(values[0]);
    ListNode* curr = head;
    for (size_t i = 1; i < values.size(); ++i) {
        curr->next = new ListNode(values[i]);
        curr = curr->next;
    }
    return head;
}

// Helper to compare a list to a vector.
bool listEquals(const ListNode* head, const std::vector<int>& expected) {
    const ListNode* curr = head;
    size_t i = 0;
    while (curr != nullptr && i < expected.size()) {
        if (curr->val != expected[i]) return false;
        curr = curr->next;
        ++i;
    }
    return curr == nullptr && i == expected.size();
}

int main() {
    // Test 1: Remove from middle
    ListNode* list1 = createList({1, 2, 6, 3, 4, 5, 6});
    list1 = removeAllOccurrences(list1, 6);
    assert(listEquals(list1, {1, 2, 3, 4, 5}));
    
    // Test 2: Remove all elements
    ListNode* list2 = createList({7, 7, 7, 7});
    list2 = removeAllOccurrences(list2, 7);
    assert(list2 == nullptr);
    
    // Test 3: Empty list
    ListNode* list3 = nullptr;
    list3 = removeAllOccurrences(list3, 5);
    assert(list3 == nullptr);
    
    // Test 4: Remove from head
    ListNode* list4 = createList({1, 1, 2, 3});
    list4 = removeAllOccurrences(list4, 1);
    assert(listEquals(list4, {2, 3}));
    
    // Test 5: Remove from tail
    ListNode* list5 = createList({1, 2, 3, 4});
    list5 = removeAllOccurrences(list5, 4);
    assert(listEquals(list5, {1, 2, 3}));
    
    // Test 6: No matches
    ListNode* list6 = createList({1, 2, 3});
    list6 = removeAllOccurrences(list6, 9);
    assert(listEquals(list6, {1, 2, 3}));
    
    // Test 7: Single node matching
    ListNode* list7 = createList({10});
    list7 = removeAllOccurrences(list7, 10);
    assert(list7 == nullptr);
    
    // Test 8: Single node not matching
    ListNode* list8 = createList({10});
    list8 = removeAllOccurrences(list8, 5);
    assert(listEquals(list8, {10}));
    
    // Test 9: Consecutive matches in middle
    ListNode* list9 = createList({1, 2, 2, 2, 3});
    list9 = removeAllOccurrences(list9, 2);
    assert(listEquals(list9, {1, 3}));
    
    // Test 10: All same value repeated
    ListNode* list10 = createList({5, 5, 5});
    list10 = removeAllOccurrences(list10, 5);
    assert(list10 == nullptr);
    
    return 0;
}
