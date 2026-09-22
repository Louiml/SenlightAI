Write a C++ function `addOneToLinkedList` that accepts a singly linked list of non-negative digits (each node holds a single digit 0-9, with the most significant digit at the head) and returns a pointer to the head of a new linked list representing the number obtained by adding 1 to the original number. The function must not modify the input list; it should create a new list. Handle the case where the input number is all 9's (e.g., 9 → 10, 99 → 100) by adding a new node at the front. The input list is non-empty and contains at least one digit. The function signature is exactly: `Node* addOneToLinkedList(const Node* head);` where `Node` is already defined as `struct Node { int data; Node* next; };`. You may assume the input list is well-formed and contains only valid digits.

// The core idea is to simulate the addition of 1 to the decimal number, but we must avoid modifying the original list. Since addition propagates from the least significant digit (the tail) toward the head, we first reverse the list to process digits from least to most significant. We create a reversed copy (or reverse in place if allowed, but since we cannot modify input, we copy). Walk through the reversed list starting with carry=1. For each node, add the carry to its value; if the result becomes 10, set the node's value to 0 and keep carry=1; otherwise set carry=0 and continue. After processing all digits, if carry remains 1, we need a new most significant digit. In the reversed list (where the most significant digit is at the tail), we append a new node with value 1 at the end (since reversed order). Then reverse the modified list back to original order and return its head. Edge cases: all digits 9 (e.g., 9 → 10, 999 → 1000), single digit, zeros. Time complexity O(n) because we traverse the list a constant number of times (copy+reverse, process, reverse). Space complexity O(n) because we create a new list of the same length, plus possibly one extra node.

#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Helper: reverse a linked list and return new head.
static Node* reverseList(Node* head) {
    Node* prev = nullptr;
    Node* curr = head;
    while (curr) {
        Node* nextTemp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextTemp;
    }
    return prev;
}

// Helper: deep copy a linked list (preserving order) and return new head.
static Node* copyList(const Node* head) {
    Node* newHead = nullptr;
    Node* tail = nullptr;
    while (head) {
        Node* newNode = new Node(head->data);
        if (!newHead) {
            newHead = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        head = head->next;
    }
    return newHead;
}

// Add 1 to a number represented by a linked list of digits (most significant at head).
// Returns a new list; input list is not modified.
Node* addOneToLinkedList(const Node* head) {
    // Step 1: Create a copy of the input list.
    Node* copyHead = copyList(head);
    
    // Step 2: Reverse the copy to process from least significant digit.
    Node* reversed = reverseList(copyHead);
    
    // Step 3: Add 1 with carry propagation.
    int carry = 1;
    Node* curr = reversed;
    while (curr) {
        int sum = curr->data + carry;
        curr->data = sum % 10;
        carry = sum / 10;
        curr = curr->next;
    }
    
    // Step 4: If carry remains, we need a new most significant digit (1).
    // In reversed order, the old most significant digit is at the tail.
    if (carry) {
        // Traverse to the end of the reversed list.
        Node* tail = reversed;
        while (tail->next) {
            tail = tail->next;
        }
        tail->next = new Node(1); // This is the new most significant digit.
    }
    
    // Step 5: Reverse back to original order (most significant at head).
    Node* result = reverseList(reversed);
    
    return result;
}

#include <cassert>

// Helper to build a list from an initializer list-like vector style.
Node* buildList(std::initializer_list<int> vals) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : vals) {
        Node* n = new Node(v);
        if (!head) { head = n; tail = n; }
        else { tail->next = n; tail = n; }
    }
    return head;
}

// Helper to compare two lists by value.
bool listsEqual(const Node* a, const Node* b) {
    while (a && b) {
        if (a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

// Helper to free list.
void freeList(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: 123 + 1 = 124
    Node* l1 = buildList({1,2,3});
    Node* r1 = addOneToLinkedList(l1);
    assert(listsEqual(r1, buildList({1,2,4})));
    freeList(l1); freeList(r1);

    // Test 2: 9 + 1 = 10
    Node* l2 = buildList({9});
    Node* r2 = addOneToLinkedList(l2);
    assert(listsEqual(r2, buildList({1,0})));
    freeList(l2); freeList(r2);

    // Test 3: 99 + 1 = 100
    Node* l3 = buildList({9,9});
    Node* r3 = addOneToLinkedList(l3);
    assert(listsEqual(r3, buildList({1,0,0})));
    freeList(l3); freeList(r3);

    // Test 4: 0 + 1 = 1
    Node* l4 = buildList({0});
    Node* r4 = addOneToLinkedList(l4);
    assert(listsEqual(r4, buildList({1})));
    freeList(l4); freeList(r4);

    // Test 5: 199 + 1 = 200
    Node* l5 = buildList({1,9,9});
    Node* r5 = addOneToLinkedList(l5);
    assert(listsEqual(r5, buildList({2,0,0})));
    freeList(l5); freeList(r5);

    // Test 6: 8 + 1 = 9
    Node* l6 = buildList({8});
    Node* r6 = addOneToLinkedList(l6);
    assert(listsEqual(r6, buildList({9})));
    freeList(l6); freeList(r6);

    // Test 7: 10 + 1 = 11
    Node* l7 = buildList({1,0});
    Node* r7 = addOneToLinkedList(l7);
    assert(listsEqual(r7, buildList({1,1})));
    freeList(l7); freeList(r7);

    // Test 8: 999 + 1 = 1000
    Node* l8 = buildList({9,9,9});
    Node* r8 = addOneToLinkedList(l8);
    assert(listsEqual(r8, buildList({1,0,0,0})));
    freeList(l8); freeList(r8);

    return 0;
}
