Write a C++ function `bool hasCycle(const Node* head)` that detects whether a singly linked list, whose last node points back to itself in a circular manner (i.e., the list is a circular linked list where traversal never ends), contains a cycle. The function must handle a circular list properly and must not dereference null pointers. The input is guaranteed to be either a non‑circular linked list ending in a `NULL` pointer or a perfectly circular list where the last node's `next` points back to the first node (the head). The function must return `true` if the list is circular and `false` otherwise. Edge cases include an empty list (`head == nullptr`) and a list with a single node whose `next` points to itself (a cycle of length 1). Use constant auxiliary space (O(1)) and linear time (O(n)) where `n` is the number of nodes.
The standard solution uses Floyd’s cycle‑detection algorithm, often called the tortoise and hare approach. Two pointers, `slow` and `fast`, both start at the head. `slow` advances one node per iteration, while `fast` advances two nodes per iteration. If the list has a cycle, the two pointers will eventually meet inside the cycle (because the relative speed between them is one node per step, so the distance between them shrinks by one each iteration and will become zero after at most `cycle_length` steps). If the list is linear, `fast` will eventually reach a `NULL` pointer, indicating no cycle. However, since the problem may give a perfectly circular list where the last node points back to the head, the `fast` pointer will never become `NULL`; instead, it will keep looping. Thus, the while‑loop condition must be written carefully. A common approach is to check `while (fast != nullptr && fast->next != nullptr)` before advancing `fast = fast->next->next`. For a circular list, this condition is always true, so the loop will never exit via the null check; but eventually `slow` and `fast` will meet, and the function returns `true`. For a linear list, the null check will eventually break the loop and return `false`. Important edge cases: an empty list (`head == nullptr`) should return `false` because there is no cycle. A single‑node circular list (where `head->next == head`) will cause `slow` and `fast` to both be `head` on the first iteration, so they meet immediately and return `true`. For a single‑node linear list (`head->next == nullptr`), the loop condition fails and returns `false`. The algorithm runs in O(n) time because the `fast` pointer moves twice per iteration and will either hit `NULL` (linear, after at most `n/2` steps) or meet `slow` (circular, after at most `n` steps). Space is O(1) as only two pointers are used. The function must be `const`‑correct: since it does not modify the list, the parameter should be `const Node*` to allow calling with a `const` pointer. However, the `Node` struct itself has a `next` pointer that is non‑const; using `const Node*` means the pointer cannot be modified to point elsewhere, but `next` is still accessible. Since we only traverse, this is fine. To be fully const‑correct, we could also use `const Node*` for the pointer variables, but that is optional.
#include <cstddef>  // for nullptr

struct Node {
    int data;
    Node* next;
};

// Detects whether a linked list contains a cycle (i.e., is circular).
// Returns true if the list has a cycle, false otherwise (including empty list).
bool hasCycle(const Node* head) {
    if (head == nullptr) {
        return false;
    }

    const Node* slow = head;
    const Node* fast = head;

    // Move slow by 1 step and fast by 2 steps each iteration.
    // If fast reaches nullptr, there is no cycle.
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return true;  // They met, so a cycle exists.
        }
    }

    return false;  // fast hit nullptr, so the list is linear.
}
#include <cassert>

// Helper to build a circular linked list from a vector of values.
// The last node points back to the first node (head).
Node* buildCircle(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    Node* head = new Node{values[0], nullptr};
    Node* tail = head;
    for (size_t i = 1; i < values.size(); ++i) {
        tail->next = new Node{values[i], nullptr};
        tail = tail->next;
    }
    tail->next = head;  // make it circular
    return head;
}

// Helper to build a linear linked list (last node's next = nullptr).
Node* buildLinear(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    Node* head = new Node{values[0], nullptr};
    Node* tail = head;
    for (size_t i = 1; i < values.size(); ++i) {
        tail->next = new Node{values[i], nullptr};
        tail = tail->next;
    }
    // tail->next is already nullptr
    return head;
}

int main() {
    // Empty list: no cycle
    assert(hasCycle(nullptr) == false);

    // Single node, linear
    Node* singleLinear = new Node{5, nullptr};
    assert(hasCycle(singleLinear) == false);
    delete singleLinear;

    // Single node, circular (points to itself)
    Node* singleCircular = new Node{7, nullptr};
    singleCircular->next = singleCircular;
    assert(hasCycle(singleCircular) == true);
    // Avoid deleting because it is self‑referential; we leak in test for simplicity.

    // Linear list of length 3
    Node* linear3 = buildLinear({1, 2, 3});
    assert(hasCycle(linear3) == false);
    // Clean up linear3
    Node* cur = linear3;
    while (cur) {
        Node* temp = cur;
        cur = cur->next;
        delete temp;
    }

    // Circular list of length 3
    Node* circle3 = buildCircle({10, 20, 30});
    assert(hasCycle(circle3) == true);
    // Clean up: since it's circular, we must handle carefully.
    // For test simplicity, we delete the three nodes manually.
    Node* c = circle3->next->next;
    delete circle3->next->next;
    delete circle3->next;
    delete circle3;

    // Circular list of length 4
    Node* circle4 = buildCircle({1, 2, 3, 4});
    assert(hasCycle(circle4) == true);
    // Clean up: delete nodes 1,2,3,4
    Node* d = circle4;
    for (int i = 0; i < 4; ++i) {
        Node* next = d->next;
        delete d;
        d = next;
    }

    return 0;
}
