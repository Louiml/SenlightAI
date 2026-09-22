Write a C++ function that takes the head of a singly linked list (where each node stores an integer and a `next` pointer) and returns a boolean indicating whether the list contains a cycle. A cycle exists if some node's `next` pointer can be followed back to an earlier node, making traversal infinite. The function must use Floyd’s cycle-detection algorithm (also known as the hare-and-tortoise approach) with two pointers moving at different speeds, and it must not modify the list. The function should handle edge cases such as an empty list, a single-node list without a cycle, and a list where the cycle connects to the head itself. Do not use extra data structures like hash sets. The function signature is: `bool hasCycle(const Node* head)`. You may assume the `Node` structure is defined as having an integer `data` and a `Node* next` pointer.
#include <cassert>
#include <cstddef>

// Node structure is defined for testing purposes.
struct Node {
    int data;
    Node* next;
    explicit Node(int value) : data(value), next(nullptr) {}
};

// Function being tested.
bool hasCycle(const Node* head);

int main() {
    // Test 1: Empty list has no cycle.
    Node* head1 = nullptr;
    assert(hasCycle(head1) == false);

    // Test 2: Single node without a cycle.
    Node* head2 = new Node(1);
    assert(hasCycle(head2) == false);
    delete head2;

    // Test 3: Two nodes without a cycle.
    Node* head3 = new Node(1);
    head3->next = new Node(2);
    assert(hasCycle(head3) == false);
    delete head3->next;
    delete head3;

    // Test 4: Cycle where tail points to head.
    Node* head4 = new Node(1);
    head4->next = new Node(2);
    head4->next->next = head4; // cycle
    assert(hasCycle(head4) == true);
    // Cleanup: avoid deleting in cycle; manually break and delete.
    head4->next->next = nullptr;
    delete head4->next;
    delete head4;

    // Test 5: Cycle where tail points to second node.
    Node* head5 = new Node(1);
    head5->next = new Node(2);
    head5->next->next = new Node(3);
    head5->next->next->next = head5->next; // cycle to 2
    assert(hasCycle(head5) == true);
    // Cleanup: break cycle.
    head5->next->next->next = nullptr;
    delete head5->next->next;
    delete head5->next;
    delete head5;

    // Test 6: Long list without a cycle (5 nodes).
    Node* head6 = new Node(1);
    Node* temp = head6;
    for (int i = 2; i <= 5; ++i) {
        temp->next = new Node(i);
        temp = temp->next;
    }
    assert(hasCycle(head6) == false);
    while (head6 != nullptr) {
        Node* toDelete = head6;
        head6 = head6->next;
        delete toDelete;
    }

    // Test 7: Cycle of length 1 (node points to itself).
    Node* head7 = new Node(1);
    head7->next = head7;
    assert(hasCycle(head7) == true);
    head7->next = nullptr;
    delete head7;

    return 0;
}
#include <cstddef>

// Node structure for a singly linked list.
struct Node {
    int data;
    Node* next;
    explicit Node(int value) : data(value), next(nullptr) {}
};

// Detect whether the linked list contains a cycle using Floyd's algorithm.
// Returns true if a cycle is detected, false otherwise.
bool hasCycle(const Node* head) {
    if (head == nullptr) {
        return false; // Empty list has no cycle.
    }

    const Node* slow = head;
    const Node* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return true; // Meet point indicates a cycle.
        }
    }

    return false; // Fast pointer reached the end, so no cycle.
}
// The solution uses two pointers, `slow` and `fast`, both starting at the head. In each iteration, `slow` moves one step and `fast` moves two steps. If the list has no cycle, `fast` will eventually reach `nullptr` (or `fast->next` becomes `nullptr`), and we return `false`. If there is a cycle, the two pointers will eventually meet inside the cycle because `fast` gains one step per iteration relative to `slow`. This works regardless of the cycle’s length or entry point. Important edge cases: an empty list or a list with only one node and no cycle should return `false` immediately because the loop condition `fast != nullptr && fast->next != nullptr` will be false. A cycle that includes the head is also detected, as both pointers start at the head, but they will still meet after some steps because `slow` and `fast` move at different relative speeds. The algorithm runs in O(n) time, where n is the number of nodes, because the pointers traverse at most a constant number of times around the cycle before meeting; without a cycle, the fast pointer reaches the end in O(n) steps. Space complexity is O(1), as only two pointers are used.
