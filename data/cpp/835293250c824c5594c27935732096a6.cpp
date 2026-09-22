/*
Write a C++ function that takes the head of a singly linked list and an integer value, and removes all nodes whose data equals that value. The function should return the head of the updated list. The linked list may be empty, contain consecutive or scattered occurrences of the target value, and the head itself may need to be removed. You must not modify the node structure (assume it has `int val` and `ListNode* next`), and you must handle memory correctly (though deallocation is not required for this exercise). The function must be named `removeAll` and accept `ListNode* head, int val`.
*/

#include <cstddef>

// Definition for singly-linked list (provided by the environment)
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Remove all nodes with value equal to val, return new head.
ListNode* removeAll(ListNode* head, int val) {
    // Advance head past any leading occurrences of val.
    while (head != nullptr && head->val == val) {
        head = head->next;
    }
    
    // Traverse the remaining list and remove occurrences.
    ListNode* curr = head;
    while (curr != nullptr && curr->next != nullptr) {
        if (curr->next->val == val) {
            curr->next = curr->next->next;  // skip the matching node
        } else {
            curr = curr->next;              // move to next node
        }
    }
    return head;
}

#include <cassert>

int main() {
    // Helper to build a list from a vector-like initializer list (simplified).
    auto build = [](std::initializer_list<int> vals) -> ListNode* {
        ListNode* dummy = new ListNode(0);
        ListNode* tail = dummy;
        for (int v : vals) {
            tail->next = new ListNode(v);
            tail = tail->next;
        }
        ListNode* result = dummy->next;
        delete dummy;
        return result;
    };

    // Helper to convert list to vector for comparison.
    auto toVector = [](ListNode* head) -> std::vector<int> {
        std::vector<int> result;
        while (head) {
            result.push_back(head->val);
            head = head->next;
        }
        return result;
    };

    // Test 1: Remove head element.
    ListNode* l1 = build({1,2,3});
    ListNode* r1 = removeAll(l1, 1);
    assert((toVector(r1) == std::vector<int>{2,3}));

    // Test 2: Remove internal and tail elements.
    ListNode* l2 = build({1,2,1,3,1});
    ListNode* r2 = removeAll(l2, 1);
    assert((toVector(r2) == std::vector<int>{2,3}));

    // Test 3: Remove all elements.
    ListNode* l3 = build({5,5,5});
    ListNode* r3 = removeAll(l3, 5);
    assert(r3 == nullptr);

    // Test 4: Empty list.
    ListNode* l4 = nullptr;
    ListNode* r4 = removeAll(l4, 10);
    assert(r4 == nullptr);

    // Test 5: Remove nothing.
    ListNode* l5 = build({1,2,3});
    ListNode* r5 = removeAll(l5, 9);
    assert((toVector(r5) == std::vector<int>{1,2,3}));

    // Test 6: Consecutive occurrences in the middle.
    ListNode* l6 = build({1,2,2,2,3});
    ListNode* r6 = removeAll(l6, 2);
    assert((toVector(r6) == std::vector<int>{1,3}));

    return 0;
}

// The solution uses two pointers. First, advance a new head pointer past any leading nodes that contain the target value, because those must be removed from the front. Then, traverse the remaining list with a current pointer (`curr`) that initially points to the new head. At each step, check if `curr->next` exists and if its value equals the target; if so, skip that node by setting `curr->next = curr->next->next` (without advancing `curr`), otherwise advance `curr` to the next node. This ensures that consecutive occurrences are handled correctly because after removing a node, we re-check the same `curr->next` position. Edge cases include an empty list (return `nullptr`), a list where all nodes match (return `nullptr`), and a list where only internal or tail nodes match. Time complexity is O(n) because each node is visited at most once. Space complexity is O(1) auxiliary, ignoring the list itself.
