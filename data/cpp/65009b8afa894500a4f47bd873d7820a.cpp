// Write a C++ function that takes two singly linked lists of non-negative integers, where each node stores a single digit (0–9) and the lists represent two numbers in **least-significant-digit-first** order (i.e., the head is the units digit). The function must return a new singly linked list representing the sum of the two numbers, also in least-significant-digit-first order. The input lists must not be modified. Handle the case where the input lists have different lengths, and if the final addition produces an extra carry (e.g., 9+9+carry), append a new node with digit `1` at the end of the result list. You may assume each input list is non-empty and contains at least one digit. Use a node structure with `int data` and `node* next`, and the returned list must be dynamically allocated with `new` (not `malloc`). Provide a single free function `node* addTwoNumbers(const node* l1, const node* l2)` that returns the head of the new list.

#include <cassert>

// Helper to compare two linked lists for equality (by value).
bool compareLists(const node* a, const node* b) {
    while (a != nullptr && b != nullptr) {
        if (a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    return (a == nullptr && b == nullptr);
}

// Helper to delete a linked list.
void deleteList(node* head) {
    while (head != nullptr) {
        node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: 8->9->7 (789) + 9->2->3 (329) = 7->2->1->1 (1118)
    node* l1 = createNode(8);
    l1->next = createNode(9);
    l1->next->next = createNode(7);
    node* l2 = createNode(9);
    l2->next = createNode(2);
    l2->next->next = createNode(3);
    node* result = addTwoNumbers(l1, l2);
    assert(result->data == 7);
    assert(result->next->data == 2);
    assert(result->next->next->data == 1);
    assert(result->next->next->next->data == 1);
    assert(result->next->next->next->next == nullptr);
    deleteList(l1); deleteList(l2); deleteList(result);

    // Test 2: different lengths: 5->4 (45) + 1 (1) = 6->4 (46)
    l1 = createNode(5);
    l1->next = createNode(4);
    l2 = createNode(1);
    result = addTwoNumbers(l1, l2);
    assert(result->data == 6);
    assert(result->next->data == 4);
    assert(result->next->next == nullptr);
    deleteList(l1); deleteList(l2); deleteList(result);

    // Test 3: 0 (0) + 0 (0) = 0 (0)
    l1 = createNode(0);
    l2 = createNode(0);
    result = addTwoNumbers(l1, l2);
    assert(result->data == 0);
    assert(result->next == nullptr);
    deleteList(l1); deleteList(l2); deleteList(result);

    // Test 4: 9->9 (99) + 9 (9) = 8->0->1 (108)
    l1 = createNode(9);
    l1->next = createNode(9);
    l2 = createNode(9);
    result = addTwoNumbers(l1, l2);
    assert(result->data == 8);
    assert(result->next->data == 0);
    assert(result->next->next->data == 1);
    assert(result->next->next->next == nullptr);
    deleteList(l1); deleteList(l2); deleteList(result);

    return 0;
}

#include <iostream>

struct node {
    int data;
    node* next;
};

// Create a new node with given data.
node* createNode(int val) {
    node* newNode = new node;
    newNode->data = val;
    newNode->next = nullptr;
    return newNode;
}

// Add two numbers represented as singly linked lists (least-significant digit first).
// Returns a new list representing the sum. Input lists are not modified.
node* addTwoNumbers(const node* l1, const node* l2) {
    node* resultHead = nullptr;
    node* resultTail = nullptr;
    int carry = 0;

    const node* p1 = l1;
    const node* p2 = l2;

    while (p1 != nullptr || p2 != nullptr || carry != 0) {
        int digit1 = (p1 != nullptr) ? p1->data : 0;
        int digit2 = (p2 != nullptr) ? p2->data : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        int digit = sum % 10;

        node* newNode = createNode(digit);
        if (resultHead == nullptr) {
            resultHead = newNode;
            resultTail = newNode;
        } else {
            resultTail->next = newNode;
            resultTail = newNode;
        }

        if (p1 != nullptr) p1 = p1->next;
        if (p2 != nullptr) p2 = p2->next;
    }

    return resultHead;
}

// The algorithm traverses both linked lists simultaneously, maintaining a carry variable initialized to 0. At each step, it sums the current digits from both lists (if a list is exhausted, treat its digit as 0), adds the carry, computes the digit as `sum % 10`, and updates carry as `sum / 10`. It creates a new node for each computed digit and appends it to the result list. After the loop, if the carry is non-zero, append a final node with that carry value. Because the lists are least-significant-first, the traversal direction naturally processes digits from right to left. Edge cases include: lists of different lengths (the shorter list is exhausted early), a final carry after both lists are fully processed (e.g., 9+9=18 → result digits 8,1), and inputs like 0+0=0. The time complexity is O(max(n, m)), where n and m are the lengths of the two input lists, because each node is visited once. The auxiliary space is O(max(n, m)) for the result list, not counting the input lists (which are not modified). The solution uses `const` pointers for inputs to guarantee no mutation, and carefully handles pointer updates.
