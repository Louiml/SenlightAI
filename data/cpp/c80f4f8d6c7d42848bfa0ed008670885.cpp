/*
Write a C++ function `reverseSublist(ListNode* head, int left, int right)` that reverses the nodes of a singly linked list only between positions `left` and `right` (1-indexed), leaving the rest unchanged. The function must handle arbitrary valid positions (1 ≤ left ≤ right ≤ list length), including the case where the sublist is the entire list or a single node. You are given the standard `ListNode` structure with `int val` and `ListNode* next`. The function should return the new head of the list (which may change if `left == 1`). Your solution must not allocate new nodes (only temporary pointers), and must preserve the relative order of unaffected nodes.
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

// Reverse the sublist from position 'left' to 'right' (1-indexed) in place.
// Returns the new head of the list.
ListNode* reverseSublist(ListNode* head, int left, int right) {
    if (head == nullptr || left == right) {
        return head;
    }

    ListNode dummy(0, head);
    ListNode* prevNode = &dummy;

    // Move prevNode to the node just before 'left'.
    for (int i = 1; i < left; ++i) {
        prevNode = prevNode->next;
    }

    ListNode* currNode = prevNode->next;

    // Reverse the sublist by repeatedly moving the node after currNode
    // to the position right after prevNode.
    for (int i = 0; i < right - left; ++i) {
        ListNode* temp = prevNode->next;         // the node to move (currently after prevNode)
        prevNode->next = currNode->next;         // skip the moved node in the original chain
        currNode->next = currNode->next->next;   // remove the moved node from after currNode
        prevNode->next->next = temp;             // place the moved node right after prevNode
    }

    return dummy.next;
}

#include <cassert>

// Helper to create a list from an initializer list-like array.
ListNode* createList(std::initializer_list<int> values) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// Helper to check list contents against an array.
bool checkList(ListNode* head, std::initializer_list<int> expected) {
    ListNode* curr = head;
    for (int v : expected) {
        if (curr == nullptr || curr->val != v) return false;
        curr = curr->next;
    }
    return curr == nullptr;
}

// Helper to free memory.
void freeList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Reverse middle portion
    ListNode* l1 = createList({1,2,3,4,5});
    l1 = reverseSublist(l1, 2, 4);
    assert(checkList(l1, {1,4,3,2,5}));
    freeList(l1);

    // Test 2: Reverse entire list
    l1 = createList({1,2,3});
    l1 = reverseSublist(l1, 1, 3);
    assert(checkList(l1, {3,2,1}));
    freeList(l1);

    // Test 3: Reverse first two nodes
    l1 = createList({1,2,3,4});
    l1 = reverseSublist(l1, 1, 2);
    assert(checkList(l1, {2,1,3,4}));
    freeList(l1);

    // Test 4: Reverse last two nodes
    l1 = createList({1,2,3,4});
    l1 = reverseSublist(l1, 3, 4);
    assert(checkList(l1, {1,2,4,3}));
    freeList(l1);

    // Test 5: left == right (no change)
    l1 = createList({1,2,3});
    l1 = reverseSublist(l1, 2, 2);
    assert(checkList(l1, {1,2,3}));
    freeList(l1);

    // Test 6: Single node list
    l1 = createList({7});
    l1 = reverseSublist(l1, 1, 1);
    assert(checkList(l1, {7}));
    freeList(l1);

    // Test 7: Two-node list fully reversed
    l1 = createList({1,2});
    l1 = reverseSublist(l1, 1, 2);
    assert(checkList(l1, {2,1}));
    freeList(l1);

    // Test 8: Null head
    assert(reverseSublist(nullptr, 1, 1) == nullptr);

    // Test 9: Complex case (reverse positions 3 to 5 in a 6-node list)
    l1 = createList({1,2,3,4,5,6});
    l1 = reverseSublist(l1, 3, 5);
    assert(checkList(l1, {1,2,5,4,3,6}));
    freeList(l1);

    // Test 10: left>1 and right == length
    l1 = createList({10,20,30,40});
    l1 = reverseSublist(l1, 2, 4);
    assert(checkList(l1, {10,40,30,20}));
    freeList(l1);

    return 0;
}

// The core algorithm uses a dummy node pointing to the original head, which simplifies handling of the case where reversal starts at the first node (since `dummy->next` becomes the new head). First, we advance a `prevNode` pointer to the node just before position `left`. Then `currNode` is set to the node at position `left` (i.e., `prevNode->next`). We perform `right - left` iterations. In each iteration, we extract the node immediately after `currNode` and move it to the front of the sublist (right after `prevNode`) by adjusting three pointers: `temp` saves the node to move, `prevNode->next` becomes `currNode->next` (skipping the moved node), `currNode->next` is updated to skip the moved node as well, and finally the moved node’s next is set to the old `prevNode->next`. This effectively reverses the sublist in-place with a single pass. Edge cases: when `left == right`, no iterations occur and the list is unchanged; when `left == 1`, the dummy node ensures correct handling; when the sublist covers the whole list, the dummy’s next becomes the new head after reversal. Time complexity is O(n) where n is the number of nodes (we only traverse from head to position `right`), and space complexity is O(1) because we only use a few pointers. The input list is not `const` because we modify it in place.
