// Given a singly-linked list where each node contains an integer value, write a C++ function named `reverseLinkedList` that takes a pointer to the head node and returns a pointer to the head of the reversed list. The function must modify the original list by reversing the direction of all `next` pointers; do not allocate new nodes or copy node values. The input may be empty (`nullptr`), a single node, or a list with multiple nodes. Handle all cases correctly and return `nullptr` for an empty input. The function signature must be `ListNode* reverseLinkedList(ListNode* head)` and must not use extra storage beyond a constant number of pointer variables.

// The standard iterative reversal of a singly-linked list uses three pointers: `current` (the node being processed), `previous` (the node that will become `current->next`), and `forward` (to save the next node before breaking the link). Start with `current = head`, `previous = nullptr`. In each iteration, store `forward = current->next`, set `current->next = previous`, then move `previous` to `current` and `current` to `forward`. Continue until `current` becomes `nullptr`. At that point, `previous` points to the new head, which should be returned. Edge cases: if `head` is `nullptr`, the loop never executes and returning `previous` (which is `nullptr`) is correct; if there is only one node, the loop runs once and the node's `next` becomes `nullptr`, and `previous` points to that node. Time complexity is O(n) where n is the number of nodes. Space complexity is O(1) as only a constant number of pointers are used.

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Reverse a singly-linked list in-place and return the new head.
// Returns nullptr if the input list is empty.
ListNode* reverseLinkedList(ListNode* head) {
    ListNode* current = head;
    ListNode* previous = nullptr;
    ListNode* forward = nullptr;

    while (current != nullptr) {
        forward = current->next;   // Save the next node
        current->next = previous;  // Reverse the link
        previous = current;        // Move previous forward
        current = forward;         // Move current forward
    }

    return previous;  // New head of the reversed list
}

#include <cassert>

int main() {
    // Helper to build a linked list from an initializer list for testing
    ListNode* buildList(std::initializer_list<int> vals) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        for (int v : vals) {
            tail->next = new ListNode(v);
            tail = tail->next;
        }
        return dummy.next;
    }

    // Helper to convert a list to a vector for comparison
    std::vector<int> toVector(ListNode* head) {
        std::vector<int> result;
        while (head) {
            result.push_back(head->val);
            head = head->next;
        }
        return result;
    }

    // Helper to free memory
    void deleteList(ListNode* head) {
        while (head) {
            ListNode* next = head->next;
            delete head;
            head = next;
        }
    }

    // Test empty list
    ListNode* empty = nullptr;
    assert(reverseLinkedList(empty) == nullptr);

    // Test single node
    ListNode* single = buildList({42});
    ListNode* revSingle = reverseLinkedList(single);
    assert(toVector(revSingle) == std::vector<int>({42}));

    // Test two nodes
    ListNode* two = buildList({1, 2});
    ListNode* revTwo = reverseLinkedList(two);
    assert(toVector(revTwo) == std::vector<int>({2, 1}));

    // Test multiple nodes
    ListNode* many = buildList({10, 20, 30, 40, 50});
    ListNode* revMany = reverseLinkedList(many);
    assert(toVector(revMany) == std::vector<int>({50, 40, 30, 20, 10}));

    // Test all same values
    ListNode* same = buildList({5, 5, 5});
    ListNode* revSame = reverseLinkedList(same);
    assert(toVector(revSame) == std::vector<int>({5, 5, 5}));

    // Free memory
    deleteList(revSingle);
    deleteList(revTwo);
    deleteList(revMany);
    deleteList(revSame);

    return 0;
}
