// Write a C++ function `removeElements` that takes a pointer to the head of a singly linked list and an integer value `val`, and returns a new linked list containing only the nodes whose values are **not** equal to `val`. The original list must not be modified; all retained values must be copied into newly allocated nodes in their original relative order. The function should handle empty lists, lists where all nodes match `val`, and lists where no nodes match. Use the provided `ListNode` struct with `val` and `next` members. Memory leaks are not a concern for this exercise, but the returned list must be properly terminated with `nullptr`.
// The solution iterates over the original list using a temporary pointer. For each node whose value does **not** equal `val`, we allocate a new `ListNode` with that value and append it to the tail of the result list. To append efficiently, we keep a `prev` pointer that tracks the last node of the result list. When the first retained node is created, we set it as the new head; for subsequent retained nodes, we link them via `prev->next`. Nodes whose value equals `val` are simply skipped, and the original list remains untouched. Edge cases include an empty input list (returns `nullptr`), all nodes matching `val` (returns `nullptr`), and a single retained node (head and `prev` point to it). Time complexity is \(O(n)\) where \(n\) is the number of nodes in the input list, since each node is visited once. Space complexity is \(O(m)\) where \(m\) is the number of retained nodes, because new nodes are allocated for each kept value; no extra data structures are used.
#include <cstddef>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Return a new linked list containing only nodes whose value != val.
// The original list is not modified.
ListNode* removeElements(ListNode* head, int val) {
    ListNode* current = head;
    ListNode* newHead = nullptr;
    ListNode* tail = nullptr;

    while (current != nullptr) {
        if (current->val != val) {
            ListNode* newNode = new ListNode(current->val);
            if (tail != nullptr) {
                tail->next = newNode;
            } else {
                newHead = newNode;
            }
            tail = newNode;
        }
        current = current->next;
    }
    return newHead;
}
#include <cassert>

int main() {
    // Helper to build a list from an initializer list and free it
    auto build = [](std::initializer_list<int> values) -> ListNode* {
        ListNode* head = nullptr;
        ListNode* tail = nullptr;
        for (int v : values) {
            ListNode* n = new ListNode(v);
            if (tail) tail->next = n; else head = n;
            tail = n;
        }
        return head;
    };

    auto destroy = [](ListNode* head) {
        while (head) { ListNode* next = head->next; delete head; head = next; }
    };

    auto equal = [](ListNode* a, ListNode* b) -> bool {
        while (a && b) {
            if (a->val != b->val) return false;
            a = a->next; b = b->next;
        }
        return a == nullptr && b == nullptr;
    };

    // Test 1: typical case with some removals
    ListNode* list1 = build({1, 2, 6, 3, 4, 5, 6});
    ListNode* result1 = removeElements(list1, 6);
    assert(equal(result1, build({1, 2, 3, 4, 5})));
    destroy(result1); destroy(list1);

    // Test 2: remove head and tail
    ListNode* list2 = build({7, 1, 2, 3, 7});
    ListNode* result2 = removeElements(list2, 7);
    assert(equal(result2, build({1, 2, 3})));
    destroy(result2); destroy(list2);

    // Test 3: all nodes match
    ListNode* list3 = build({5, 5, 5});
    ListNode* result3 = removeElements(list3, 5);
    assert(result3 == nullptr);
    destroy(list3);

    // Test 4: no nodes match
    ListNode* list4 = build({1, 2, 3});
    ListNode* result4 = removeElements(list4, 9);
    assert(equal(result4, list4));
    destroy(result4); destroy(list4);

    // Test 5: empty list
    ListNode* result5 = removeElements(nullptr, 1);
    assert(result5 == nullptr);

    // Test 6: single node matches
    ListNode* list6 = new ListNode(4);
    ListNode* result6 = removeElements(list6, 4);
    assert(result6 == nullptr);
    delete list6;

    // Test 7: single node does not match
    ListNode* list7 = new ListNode(4);
    ListNode* result7 = removeElements(list7, 3);
    assert(equal(result7, list7));
    destroy(result7); delete list7;

    return 0;
}
