// Write a C++ function `int getNthFromEnd(const Node* head, int n)` that takes a singly linked list (defined by the given `Node` class with `int data` and `Node* next`) and a positive integer `n`, and returns the data value of the `n`-th node from the end of the list. The list may be empty (head is `nullptr`), and `n` may be larger than the number of nodes. If the list is empty or `n` is greater than the list size, return `-1`. The function must not modify the list and must handle all cases correctly.

#include <cassert>

int main() {
    // Helper to create a list from an initializer list (for testing)
    Node* buildList(std::initializer_list<int> values) {
        Node* head = nullptr;
        Node* tail = nullptr;
        for (int v : values) {
            if (head == nullptr) {
                head = tail = new Node(v, nullptr);
            } else {
                tail->next = new Node(v, nullptr);
                tail = tail->next;
            }
        }
        return head;
    }

    // Test 1: Normal list, n=2
    Node* list1 = buildList({10, 20, 30, 40, 50});
    assert(getNthFromEnd(list1, 2) == 40);

    // Test 2: n=1 returns last
    assert(getNthFromEnd(list1, 1) == 50);

    // Test 3: n=size returns first
    assert(getNthFromEnd(list1, 5) == 10);

    // Test 4: n larger than size
    assert(getNthFromEnd(list1, 6) == -1);

    // Test 5: Empty list
    Node* empty = nullptr;
    assert(getNthFromEnd(empty, 3) == -1);

    // Test 6: Single node list
    Node* single = new Node(42, nullptr);
    assert(getNthFromEnd(single, 1) == 42);
    assert(getNthFromEnd(single, 2) == -1);

    // Test 7: n=0 (invalid) returns -1
    assert(getNthFromEnd(list1, 0) == -1);

    // Clean up (simplified; not required for tests but good practice)
    // (In a full solution, proper deletion would be added.)
    return 0;
}

#include <cstddef>

// Node structure as given in the snippet
struct Node {
    int data;
    Node* next;
    Node() : data(0), next(nullptr) {}
    Node(int d, Node* n) : data(d), next(n) {}
};

// Returns the data value of the n-th node from the end.
// Returns -1 if the list is empty or n > list size.
int getNthFromEnd(const Node* head, int n) {
    if (head == nullptr || n <= 0) {
        return -1;
    }

    const Node* first = head;
    const Node* second = head;

    // Advance 'first' by n steps
    for (int i = 0; i < n; ++i) {
        if (first == nullptr) {
            return -1; // n exceeds list size
        }
        first = first->next;
    }

    // Move both pointers until 'first' reaches the end
    while (first != nullptr) {
        first = first->next;
        second = second->next;
    }

    return second->data;
}

// The solution uses a two-pointer (tortoise and hare) technique to find the `n`-th node from the end in a single pass. Initialize two pointers, `first` and `second`, both starting at `head`. Advance `first` by `n` steps first. If during this advancement we reach the end of the list (i.e., `first` becomes `nullptr`) before completing `n` steps, then `n` is larger than the list size—return `-1`. If the list is empty (`head == nullptr`), also return `-1`. After `first` has been advanced `n` steps, move both `first` and `second` simultaneously until `first` reaches the end. At that point, `second` points to the `n`-th node from the end, and we return its `data`. Edge cases include `n == 1` (which returns the last node), `n` equal to the list size (which returns the first node), and `n` greater than the size. Time complexity is O(L) where L is the list length because each node is visited at most twice. Space complexity is O(1) as only two pointers are used.
