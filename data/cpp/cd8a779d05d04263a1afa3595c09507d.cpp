// Write a C++ function `void insertAtPosition(ListNode*& head, int value, int position)` that inserts a new node with the given `value` into a singly linked list at the 1-based `position` (e.g., position 1 means the new node becomes the new head, position 2 means it becomes the second node). The function should handle insertion at any valid position from 1 up to `list length + 1`. If the position is invalid (less than 1 or greater than length + 1), the function should do nothing (no insertion, no modification to the list). The linked list nodes are defined as `struct ListNode { int data; ListNode* next; }`. You may assume the caller manages memory and prints the list; your function only performs the insertion logic. Provide the function definition only (no `main`), with proper `const` correctness where applicable.
#include <cassert>

// Helper to build a list from initializer list for testing
ListNode* buildList(std::initializer_list<int> vals) {
    ListNode* head = nullptr;
    ListNode** ptr = &head;
    for (int v : vals) {
        *ptr = new ListNode(v);
        ptr = &((*ptr)->next);
    }
    return head;
}

// Helper to convert list to vector for easy comparison
std::vector<int> listToVector(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Clean up list memory
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Insert at head of empty list
    ListNode* head1 = nullptr;
    insertAtPosition(head1, 10, 1);
    assert(listToVector(head1) == std::vector<int>({10}));
    deleteList(head1);

    // Test 2: Insert at head of non-empty list
    ListNode* head2 = buildList({2, 3});
    insertAtPosition(head2, 1, 1);
    assert(listToVector(head2) == std::vector<int>({1, 2, 3}));
    deleteList(head2);

    // Test 3: Insert in middle
    ListNode* head3 = buildList({1, 3, 4});
    insertAtPosition(head3, 2, 2);
    assert(listToVector(head3) == std::vector<int>({1, 2, 3, 4}));
    deleteList(head3);

    // Test 4: Insert at tail
    ListNode* head4 = buildList({1, 2});
    insertAtPosition(head4, 3, 3);
    assert(listToVector(head4) == std::vector<int>({1, 2, 3}));
    deleteList(head4);

    // Test 5: Invalid position 0
    ListNode* head5 = buildList({1, 2});
    insertAtPosition(head5, 99, 0);
    assert(listToVector(head5) == std::vector<int>({1, 2}));
    deleteList(head5);

    // Test 6: Invalid position greater than length+1
    ListNode* head6 = buildList({1});
    insertAtPosition(head6, 99, 3);
    assert(listToVector(head6) == std::vector<int>({1}));
    deleteList(head6);

    // Test 7: Insert at position equal to length+1 (tail) on longer list
    ListNode* head7 = buildList({5, 10, 15});
    insertAtPosition(head7, 20, 4);
    assert(listToVector(head7) == std::vector<int>({5, 10, 15, 20}));
    deleteList(head7);

    return 0;
}
#include <cstddef> // for nullptr

struct ListNode {
    int data;
    ListNode* next;
    ListNode(int val) : data(val), next(nullptr) {}
};

// Insert a new node with given value at 1-based position.
// If position is invalid (out of range), do nothing.
void insertAtPosition(ListNode*& head, int value, int position) {
    if (position < 1) {
        return;
    }
    
    ListNode* newNode = new ListNode(value);
    
    // Insert at head
    if (position == 1) {
        newNode->next = head;
        head = newNode;
        return;
    }
    
    // Traverse to find the (position-1)-th node
    ListNode* current = head;
    for (int i = 0; i < position - 2; ++i) {
        if (current == nullptr) {
            delete newNode; // invalid position, clean up
            return;
        }
        current = current->next;
    }
    
    // If current is null, then position is invalid (list too short)
    if (current == nullptr) {
        delete newNode;
        return;
    }
    
    // Link new node after current
    newNode->next = current->next;
    current->next = newNode;
}
// The core idea is to traverse the linked list to find the node just before the insertion point. If the position is 1, the new node is inserted at the head: set the new node's `next` to the current head, then update the head pointer to the new node. For positions greater than 1, we need to move to the (position-1)-th node. Start with a pointer `current` equal to `head`. Since we already know position is valid (checked upfront), we move `current` forward `position-2` steps (because we start at the first node). If during traversal we reach `nullptr` before reaching the desired predecessor, that means the position is invalid (e.g., position greater than length+1); in that case do nothing. After reaching the correct predecessor, set the new node's `next` to `current->next`, then set `current->next` to the new node. Important edge cases: (1) inserting at head (position 1) when list is empty or non-empty; (2) inserting at the tail (position = length+1) where the predecessor's `next` is `nullptr`; (3) invalid positions like 0, negative, or too large — these should be silently ignored. Time complexity is O(n) in the worst case because we may traverse up to the end of the list. Space complexity is O(1) for the new node allocation (we do not count the returned list as extra space). The function must use a reference to the head pointer (`ListNode*&`) so it can modify the head when inserting at position 1.
