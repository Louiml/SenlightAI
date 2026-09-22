/*
Write a C++ function `reverseList` that takes a singly linked list built from nodes containing an integer `data` and a `next` pointer, and returns a new head pointer for the same list with the order of nodes reversed. The function should not allocate any new nodes; it must rearrange the existing nodes by changing their `next` links. The function should handle an empty list (returning `nullptr`) and a single-node list (returning the same node). Use the `Node` struct provided below, and ensure the function is `const`-correct where applicable.
```cpp
struct Node {
    int data;
    Node* next;
    Node(int x) : data(x), next(nullptr) {}
};
```
*/

#include <cstddef>  // for nullptr

struct Node {
    int data;
    Node* next;
    Node(int x) : data(x), next(nullptr) {}
};

// Reverses the singly linked list starting at head and returns the new head.
Node* reverseList(Node* head) {
    Node* prev = nullptr;
    Node* curr = head;
    Node* next = nullptr;

    while (curr != nullptr) {
        next = curr->next;   // Save the next node
        curr->next = prev;   // Reverse the link
        prev = curr;         // Move prev forward
        curr = next;         // Move curr forward
    }

    return prev;  // New head of the reversed list
}

#include <cassert>

int main() {
    // Helper to create a list from a vector for testing (not part of solution).
    auto createList = [](std::initializer_list<int> values) {
        Node* head = nullptr;
        Node* tail = nullptr;
        for (int v : values) {
            Node* n = new Node(v);
            if (!head) head = tail = n;
            else { tail->next = n; tail = n; }
        }
        return head;
    };

    // Helper to compare list contents.
    auto listEqual = [](Node* head, std::initializer_list<int> values) {
        Node* curr = head;
        for (int v : values) {
            if (!curr || curr->data != v) return false;
            curr = curr->next;
        }
        return curr == nullptr;
    };

    // Test 1: Empty list
    assert(reverseList(nullptr) == nullptr);

    // Test 2: Single node
    Node* single = new Node(42);
    assert(reverseList(single) == single);
    assert(single->data == 42 && single->next == nullptr);

    // Test 3: Multiple nodes
    Node* list = createList({1, 2, 3, 4, 5});
    Node* reversed = reverseList(list);
    assert(listEqual(reversed, {5, 4, 3, 2, 1}));

    // Test 4: Two nodes
    Node* two = createList({10, 20});
    Node* revTwo = reverseList(two);
    assert(listEqual(revTwo, {20, 10}));

    // Clean up (not strictly needed for assert test, but good practice)
    // (Deleting nodes is omitted for brevity in this test snippet.)
}

// The solution uses an iterative three-pointer approach. Initialize three pointers: `prev` as `nullptr`, `curr` as `head`, and `next` as `nullptr`. While `curr` is not `nullptr`, save the next node (`next = curr->next`), then reverse the link (`curr->next = prev`), move `prev` to `curr`, and move `curr` to `next`. After the loop, `prev` points to the new head of the reversed list. Edge cases include an empty list (the loop doesn't execute, returns `nullptr`) and a single-node list (after the first iteration, `prev` becomes that node and `curr` becomes `nullptr`, so it returns the same single node). Time complexity is O(n) where n is the number of nodes, and space complexity is O(1) since only a constant number of pointers are used.
