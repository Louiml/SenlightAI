/*
Write a C++ function named `reverseLinkedList` that accepts a singly linked list of integers (represented by a `Node` class with fields `data` and `next`) and reverses the list in-place using a recursive approach. The function should modify the original list so that the head points to the former tail, and all node pointers are reversed accordingly. The function must handle edge cases such as an empty list and a list with a single node. You may design the function to return the new head pointer or modify the head reference directly, but ensure the signature is clear. The implementation should not allocate new nodes or use additional data structures; only constant extra space is allowed apart from the recursion stack.
*/

#include <cstddef> // for nullptr

// Node structure for singly linked list
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Recursively reverse a singly linked list in-place.
// Returns the new head of the reversed list.
// Handles empty list and single-node list.
Node* reverseLinkedList(Node* head) {
    // Base case: empty list or only one node
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
    // Recursively reverse the rest of the list
    Node* newHead = reverseLinkedList(head->next);
    // Make the next node's next point back to current node
    head->next->next = head;
    // Terminate the reversed tail
    head->next = nullptr;
    return newHead;
}

#include <cassert>

// Helper to create a list from an initializer list (for tests)
Node* createList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* newNode = new Node(v);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// Helper to free list memory
void freeList(Node* head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// Helper to check if list matches expected values
bool listMatches(Node* head, std::initializer_list<int> expected) {
    Node* curr = head;
    for (int v : expected) {
        if (curr == nullptr || curr->data != v) return false;
        curr = curr->next;
    }
    return curr == nullptr;
}

int main() {
    // Test 1: Empty list -> returns nullptr
    Node* empty = nullptr;
    assert(reverseLinkedList(empty) == nullptr);

    // Test 2: Single node
    Node* single = new Node(5);
    single = reverseLinkedList(single);
    assert(single->data == 5);
    assert(single->next == nullptr);
    delete single;

    // Test 3: Two nodes
    Node* two = createList({1, 2});
    two = reverseLinkedList(two);
    assert(listMatches(two, {2, 1}));
    freeList(two);

    // Test 4: Three nodes
    Node* three = createList({1, 2, 3});
    three = reverseLinkedList(three);
    assert(listMatches(three, {3, 2, 1}));
    freeList(three);

    // Test 5: Larger list with duplicates
    Node* list = createList({10, 20, 30, 40, 50});
    list = reverseLinkedList(list);
    assert(listMatches(list, {50, 40, 30, 20, 10}));
    freeList(list);

    // Test 6: Already reversed list - becomes original order
    Node* rev = createList({4, 3, 2, 1});
    rev = reverseLinkedList(rev);
    assert(listMatches(rev, {1, 2, 3, 4}));
    freeList(rev);

    // Test 7: Negative numbers
    Node* neg = createList({-1, -2, -3});
    neg = reverseLinkedList(neg);
    assert(listMatches(neg, {-3, -2, -1}));
    freeList(neg);

    // Test 8: All same values
    Node* same = createList({7, 7, 7});
    same = reverseLinkedList(same);
    assert(listMatches(same, {7, 7, 7}));
    freeList(same);

    return 0;
}

// The solution uses recursion to reverse the linked list. Two common recursive strategies exist: a two-pointer recursive approach (forward recursion with prev and curr pointers) and a simpler "easy" approach that reverses from the tail back to the head. The **easy recursive approach** works as follows: if the head is null or the next pointer is null, return head (base case). Otherwise, recursively call the function on `head->next`, which returns the new head of the reversed sublist. After the recursive call, we set `head->next->next = head` (making the next node point back to head) and then set `head->next = NULL` to terminate the reversed list. We then return the new head from the recursive call, which propagates all the way up. This is elegant and avoids needing extra parameters.
//
// Edge cases: 
// - Empty list (`head == nullptr`): returns nullptr.
// - Single node: base case triggers, returns the same node.
//
// Time complexity: O(n), where n is the number of nodes, because each node is visited once.  
// Space complexity: O(n) due to recursion stack depth, which equals the length of the list. This is still considered linear extra space, but acceptable for a recursive solution. The task explicitly allows recursion, so the stack space is expected.
//
// Alternative approaches (iterative) use O(1) space, but since the task asks for a recursive solution, we implement that. The function signature should be `Node* reverseLinkedList(Node* head)` returning the new head, or alternatively accept a reference to head pointer. For the test, we'll use the return-value version for simplicity.
