/*
Write a C++ function `int binaryLinkedListToInteger(const ListNode* head)` that takes a singly-linked list where each node contains a single binary digit (0 or 1) and returns the integer value represented by that binary sequence, treating the head as the most significant bit. The linked list is guaranteed to be non-empty and contain only valid binary digits. The function must not modify the linked list and must handle the full range of binary values fitting in a 32-bit signed integer (i.e., up to 31 bits). You may assume the input linked list is correctly constructed with no cycles.
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

// Convert a binary linked list (head = most significant bit) to an integer.
// The function does not modify the list and assumes non-empty valid binary digits.
int binaryLinkedListToInteger(const ListNode* head) {
    int result = 0;
    const ListNode* current = head;
    while (current != nullptr) {
        result = result * 2 + current->val;
        current = current->next;
    }
    return result;
}

#include <cassert>

int main() {
    // Test case 1: Single node zero
    ListNode* n1 = new ListNode(0);
    assert(binaryLinkedListToInteger(n1) == 0);
    delete n1;

    // Test case 2: Single node one
    ListNode* n2 = new ListNode(1);
    assert(binaryLinkedListToInteger(n2) == 1);
    delete n2;

    // Test case 3: 10 (binary) = 2
    ListNode* n3a = new ListNode(1);
    ListNode* n3b = new ListNode(0);
    n3a->next = n3b;
    assert(binaryLinkedListToInteger(n3a) == 2);
    delete n3a; // deletes n3b as well due to chain? No, must delete n3b separately
    delete n3b;

    // Test case 4: 101 (binary) = 5
    ListNode* n4a = new ListNode(1);
    ListNode* n4b = new ListNode(0);
    ListNode* n4c = new ListNode(1);
    n4a->next = n4b;
    n4b->next = n4c;
    assert(binaryLinkedListToInteger(n4a) == 5);
    delete n4a; delete n4b; delete n4c;

    // Test case 5: 1111 (binary) = 15
    ListNode* n5a = new ListNode(1);
    ListNode* n5b = new ListNode(1);
    ListNode* n5c = new ListNode(1);
    ListNode* n5d = new ListNode(1);
    n5a->next = n5b; n5b->next = n5c; n5c->next = n5d;
    assert(binaryLinkedListToInteger(n5a) == 15);
    delete n5a; delete n5b; delete n5c; delete n5d;

    // Test case 6: 100000 (binary) = 32
    ListNode* n6a = new ListNode(1);
    ListNode* cur = n6a;
    for (int i = 0; i < 5; ++i) {
        cur->next = new ListNode(0);
        cur = cur->next;
    }
    assert(binaryLinkedListToInteger(n6a) == 32);
    // cleanup
    cur = n6a;
    while (cur) {
        ListNode* temp = cur;
        cur = cur->next;
        delete temp;
    }

    // Test case 7: all zeros length 3
    ListNode* n7a = new ListNode(0);
    ListNode* n7b = new ListNode(0);
    ListNode* n7c = new ListNode(0);
    n7a->next = n7b; n7b->next = n7c;
    assert(binaryLinkedListToInteger(n7a) == 0);
    delete n7a; delete n7b; delete n7c;

    // Test case 8: 1101 (binary) = 13
    ListNode* n8a = new ListNode(1);
    ListNode* n8b = new ListNode(1);
    ListNode* n8c = new ListNode(0);
    ListNode* n8d = new ListNode(1);
    n8a->next = n8b; n8b->next = n8c; n8c->next = n8d;
    assert(binaryLinkedListToInteger(n8a) == 13);
    delete n8a; delete n8b; delete n8c; delete n8d;

    return 0;
}

// The solution processes the linked list from the head to the tail, constructing the integer bit by bit. For each node, the current result is shifted left by one position (equivalent to multiplying by 2) and then the node's value is added (either 0 or 1). This is the standard binary-to-decimal conversion: `result = result * 2 + node->val`. Since the head is the most significant bit, this works correctly because each subsequent node represents the next lower-order bit. Edge cases: a single-node list with value 0 returns 0; a single-node list with value 1 returns 1; a list of all 1's of length up to 31 gives the maximum positive integer. The algorithm must not modify the list, so we use a `const` pointer and a local iterator. Time complexity is O(n) where n is the number of nodes, and space complexity is O(1) auxiliary space (excluding the input list).
