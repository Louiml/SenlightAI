// Write a C++ function `int binaryLinkedListToDecimal(ListNode* head)` that takes a pointer to the head of a singly linked list where each node contains a single binary digit (`0` or `1`) in its `val` field, and returns the decimal (base-10) integer represented by that binary number. The linked list is given in the most significant bit first order (i.e., the first node is the leftmost binary digit). You may assume the list is non-empty and all node values are either `0` or `1`. The function should not modify the list and should work for arbitrarily long binary numbers (within the range of a 32-bit signed integer). Use the provided `ListNode` struct definition exactly as given.

The simplest and most direct approach is to traverse the linked list once, building the decimal value incrementally using the standard binary-to-decimal conversion: for each digit visited, multiply the current accumulated value by 2 and then add the digit's value (0 or 1). This works because the list is in most-significant-bit-first order; starting from the head, each new node represents a digit that is less significant than all previously encountered digits. For example, the list `1 → 0 → 1` yields: start with 0, then `(0*2)+1 = 1`, then `(1*2)+0 = 2`, then `(2*2)+1 = 5`, which is correct (binary 101 = decimal 5). Edge cases include a single node (e.g., `0` or `1`) and all zeros; both are handled naturally by the same loop. The algorithm runs in O(n) time where n is the number of nodes, and uses O(1) extra space because it only keeps a running sum and a pointer. No special handling for negative numbers is needed since binary digits are only 0/1 and the result fits in a 32-bit integer as guaranteed.

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Convert a binary number represented by a linked list (most significant bit first)
// into its decimal integer value.
int binaryLinkedListToDecimal(const ListNode* head) {
    int result = 0;
    const ListNode* current = head;
    while (current != nullptr) {
        result = (result * 2) + current->val;
        current = current->next;
    }
    return result;
}

#include <cassert>

int main() {
    // Test 1: 101 -> 5
    ListNode* n1 = new ListNode(1);
    ListNode* n2 = new ListNode(0);
    ListNode* n3 = new ListNode(1);
    n1->next = n2;
    n2->next = n3;
    assert(binaryLinkedListToDecimal(n1) == 5);

    // Test 2: 0 -> 0
    ListNode* zero = new ListNode(0);
    assert(binaryLinkedListToDecimal(zero) == 0);

    // Test 3: 1 -> 1
    ListNode* one = new ListNode(1);
    assert(binaryLinkedListToDecimal(one) == 1);

    // Test 4: 1000 -> 8
    ListNode* b1 = new ListNode(1);
    ListNode* b2 = new ListNode(0);
    ListNode* b3 = new ListNode(0);
    ListNode* b4 = new ListNode(0);
    b1->next = b2;
    b2->next = b3;
    b3->next = b4;
    assert(binaryLinkedListToDecimal(b1) == 8);

    // Test 5: 101010 -> 42
    ListNode* c1 = new ListNode(1);
    ListNode* c2 = new ListNode(0);
    ListNode* c3 = new ListNode(1);
    ListNode* c4 = new ListNode(0);
    ListNode* c5 = new ListNode(1);
    ListNode* c6 = new ListNode(0);
    c1->next = c2;
    c2->next = c3;
    c3->next = c4;
    c4->next = c5;
    c5->next = c6;
    assert(binaryLinkedListToDecimal(c1) == 42);

    // Test 6: 1111 -> 15
    ListNode* d1 = new ListNode(1);
    ListNode* d2 = new ListNode(1);
    ListNode* d3 = new ListNode(1);
    ListNode* d4 = new ListNode(1);
    d1->next = d2;
    d2->next = d3;
    d3->next = d4;
    assert(binaryLinkedListToDecimal(d1) == 15);

    // Cleanup (optional; not strictly required for assertions)
    // Delete all nodes to avoid memory leaks (omitted for brevity)
    return 0;
}
