// Write a C++ function that takes the head of a singly linked list and swaps every two adjacent nodes, returning the new head of the modified list. For example, given `1 -> 2 -> 3 -> 4`, the function should return `2 -> 1 -> 4 -> 3`. If the list has an odd number of nodes, the last node remains in place. The function must handle empty lists and single-node lists gracefully. Nodes contain an integer value and a pointer to the next node; you may define the `ListNode` struct as part of your solution. The function should not allocate new nodes except a temporary dummy head if needed, and must modify the list in-place.

The algorithm uses a dummy node placed before the head to simplify pointer updates, especially for the first pair. We maintain a `prev` pointer that points to the node immediately before the current pair, and a `curr` pointer to the first node of the pair. In each iteration, we perform the swap by rewiring the next pointers: `prev->next` points to the second node of the pair, `curr->next` points to the node after the pair (previously the second node's next), and the second node's next points to `curr`. Then we advance `prev` to `curr` (the second node of the swapped pair becomes the new "previous" for the next pair) and `curr` to `curr->next` (the first node of the next pair). The loop continues while both `curr` and `curr->next` are non-null. Edge cases include an empty list, a single node, and an odd length where the last node is left untouched. Time complexity is O(n) with one pass, and space complexity is O(1) beyond the dummy node.

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Swap every two adjacent nodes in the list and return the new head.
ListNode* swapAdjacentPairs(ListNode* head) {
    if (!head || !head->next) return head;

    ListNode dummy(0);
    ListNode* prev = &dummy;
    ListNode* curr = head;

    while (curr && curr->next) {
        // Save the second node of the pair
        ListNode* second = curr->next;
        // Connect prev to the second node
        prev->next = second;
        // Connect curr to the node after the pair
        curr->next = second->next;
        // Connect second to curr (swap)
        second->next = curr;

        // Move prev and curr forward
        prev = curr;
        curr = curr->next;
    }
    return dummy.next;
}

#include <cassert>

int main() {
    // Helper to create a list from a vector-like initializer list is not needed; build manually.
    // Test empty list
    ListNode* empty = nullptr;
    assert(swapAdjacentPairs(empty) == nullptr);

    // Test single node
    ListNode* single = new ListNode(1);
    ListNode* result = swapAdjacentPairs(single);
    assert(result == single);
    assert(result->val == 1);

    // Test two nodes: 1->2
    ListNode* a = new ListNode(1);
    ListNode* b = new ListNode(2);
    a->next = b;
    result = swapAdjacentPairs(a);
    assert(result == b);
    assert(result->val == 2);
    assert(result->next == a);
    assert(result->next->val == 1);
    assert(result->next->next == nullptr);

    // Test four nodes: 1->2->3->4
    ListNode* n1 = new ListNode(1);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(3);
    ListNode* n4 = new ListNode(4);
    n1->next = n2; n2->next = n3; n3->next = n4;
    result = swapAdjacentPairs(n1);
    assert(result == n2);
    assert(result->val == 2);
    assert(result->next == n1);
    assert(result->next->val == 1);
    assert(result->next->next == n4);
    assert(result->next->next->val == 4);
    assert(result->next->next->next == n3);
    assert(result->next->next->next->val == 3);
    assert(result->next->next->next->next == nullptr);

    // Test odd length: 1->2->3
    ListNode* m1 = new ListNode(1);
    ListNode* m2 = new ListNode(2);
    ListNode* m3 = new ListNode(3);
    m1->next = m2; m2->next = m3;
    result = swapAdjacentPairs(m1);
    assert(result == m2);
    assert(result->val == 2);
    assert(result->next == m1);
    assert(result->next->val == 1);
    assert(result->next->next == m3);
    assert(result->next->next->val == 3);
    assert(result->next->next->next == nullptr);

    // Test list with duplicate values: 5->5->5->5
    ListNode* d1 = new ListNode(5);
    ListNode* d2 = new ListNode(5);
    ListNode* d3 = new ListNode(5);
    ListNode* d4 = new ListNode(5);
    d1->next = d2; d2->next = d3; d3->next = d4;
    result = swapAdjacentPairs(d1);
    assert(result == d2);
    assert(result->val == 5);
    assert(result->next == d1);
    assert(result->next->next == d4);
    assert(result->next->next->next == d3);
    assert(result->next->next->next->next == nullptr);

    return 0;
}
