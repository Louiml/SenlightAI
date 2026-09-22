// Write a C++ function that takes the head of a singly linked list and a non-negative integer `k`, and rotates the list to the right by `k` positions. Rotation to the right by `k` means that the last `k` nodes are moved to the front, preserving their relative order. For example, a list `1 → 2 → 3 → 4 → 5` rotated by `2` becomes `4 → 5 → 1 → 2 → 3`. If `k` equals the length of the list or is a multiple of the length, the list remains unchanged. The function should return the new head of the rotated list and must not allocate new nodes; it should only modify the original nodes' `next` pointers. Assume the list is non-empty and consists only of unique integer values (for simplicity in testing). You are provided with the `Node` structure: `struct Node { int data; Node* next; };` Implement the function with signature `Node* rotateRight(Node* head, int k)`.
#include <cassert>

// Helper to create a list from a vector-like initializer list
Node* createList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int val : values) {
        Node* node = new Node(val);
        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

// Helper to convert list to vector-like string for comparison
bool listEquals(Node* head, std::initializer_list<int> expected) {
    Node* current = head;
    for (int val : expected) {
        if (current == nullptr || current->data != val) return false;
        current = current->next;
    }
    return current == nullptr;
}

// Helper to delete list memory (not strictly needed for assertions but good practice)
void deleteList(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Basic rotation by 2 on a 5-node list
    Node* list1 = createList({1, 2, 3, 4, 5});
    Node* rotated1 = rotateRight(list1, 2);
    assert(listEquals(rotated1, {4, 5, 1, 2, 3}));
    deleteList(rotated1);

    // Test 2: Rotation by 0 returns original list
    Node* list2 = createList({10, 20, 30});
    Node* rotated2 = rotateRight(list2, 0);
    assert(listEquals(rotated2, {10, 20, 30}));
    deleteList(rotated2);

    // Test 3: Rotation by length returns original list
    Node* list3 = createList({7, 8, 9});
    Node* rotated3 = rotateRight(list3, 3);
    assert(listEquals(rotated3, {7, 8, 9}));
    deleteList(rotated3);

    // Test 4: Rotation by multiple of length (e.g., 6) returns original
    Node* list4 = createList({1, 2, 3, 4});
    Node* rotated4 = rotateRight(list4, 8);
    assert(listEquals(rotated4, {1, 2, 3, 4}));
    deleteList(rotated4);

    // Test 5: Single node list with k=5 should still be itself
    Node* list5 = new Node(42);
    Node* rotated5 = rotateRight(list5, 5);
    assert(listEquals(rotated5, {42}));
    deleteList(rotated5);

    // Test 6: Rotation by 1 on 2-node list swaps order
    Node* list6 = createList({100, 200});
    Node* rotated6 = rotateRight(list6, 1);
    assert(listEquals(rotated6, {200, 100}));
    deleteList(rotated6);

    // Test 7: Large k (e.g., 12) with 5-node list, effective k=2
    Node* list7 = createList({1, 2, 3, 4, 5});
    Node* rotated7 = rotateRight(list7, 12);
    assert(listEquals(rotated7, {4, 5, 1, 2, 3}));
    deleteList(rotated7);

    return 0;
}
#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Rotate the singly linked list to the right by k positions.
// Returns the new head of the rotated list.
Node* rotateRight(Node* head, int k) {
    if (head == nullptr || head->next == nullptr || k == 0) {
        return head;
    }

    // Compute length of the list
    Node* current = head;
    int length = 0;
    while (current != nullptr) {
        ++length;
        current = current->next;
    }

    // Normalize rotation count
    int effectiveK = k % length;
    if (effectiveK == 0) {
        return head;
    }

    // Find the split point: new tail is at position (length - effectiveK - 1)
    int stepsToNewTail = length - effectiveK - 1;
    Node* newTail = head;
    for (int i = 0; i < stepsToNewTail; ++i) {
        newTail = newTail->next;
    }

    Node* newHead = newTail->next;

    // Find the original tail to connect it to the old head
    Node* oldTail = newHead;
    while (oldTail->next != nullptr) {
        oldTail = oldTail->next;
    }

    // Perform rotation
    oldTail->next = head;
    newTail->next = nullptr;

    return newHead;
}
// The core idea is to first compute the total length `n` of the linked list by traversing it once. Normalize the rotation count: since rotating by `n` returns the original list, the effective rotation is `k % n`. If this value is `0`, return the original head. Otherwise, we need to find the node at position `n - (k % n) - 1` (0-indexed from head), which will become the new tail after rotation; the node right after it becomes the new head. We traverse the list again, tracking the last node, the node at the split position, and the node after the split. Then, we set the old tail's `next` to the original head, set the split node's `next` to `nullptr`, and return the node after the split. Edge cases: list with a single node (rotation does nothing), `k = 0` (return head), and `k` being a multiple of `n` (return head). Time complexity is `O(n)` because we traverse the list up to two times, and space complexity is `O(1)` beyond the input list.
