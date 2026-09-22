/*
Given a singly linked list of integers and a target value `value`, write a C++ function `Node* insertBeforeValue(Node* head, Node*& tail, int value, int newData)` that inserts a new node containing `newData` immediately before the first occurrence of `value` in the list. If `value` is not found, append the new node at the tail. If the list is empty, the new node becomes the head. The function must update the head (if insertion occurs before the first node) and `tail` (if insertion occurs at the end). The function returns the new head of the list. You may assume `value` appears at most once in the list. The original list order must be preserved for all other nodes. The solution must be implemented without using any standard containers (e.g., `std::list`); only raw linked-list nodes and pointer manipulation are allowed.
*/
#include <iostream>

// Node structure for singly linked list
struct Node {
    int data;
    Node* next;

    explicit Node(int val) : data(val), next(nullptr) {}
};

/*
 * Insert a new node with newData before the first occurrence of value.
 * If value not found, append at the tail.
 * Updates head and tail as necessary. Returns new head.
 */
Node* insertBeforeValue(Node* head, Node*& tail, int value, int newData) {
    Node* newNode = new Node(newData);

    // Case 1: Empty list
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
        return head;
    }

    // Case 2: Insert before the first node (head)
    if (head->data == value) {
        newNode->next = head;
        head = newNode;
        return head;
    }

    // Case 3: Traverse to find value or end of list
    Node* prev = head;
    Node* curr = head->next;

    while (curr != nullptr && curr->data != value) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != nullptr) {
        // Found value: insert between prev and curr
        newNode->next = curr;
        prev->next = newNode;
    } else {
        // Value not found: append at tail
        prev->next = newNode;
        tail = newNode;
    }

    return head;
}
#include <cassert>

// Helper to build list from initializer list (for testing only)
Node* buildList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* n = new Node(v);
        if (!head) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }
    return head;
}

// Helper to convert list to vector-like array for comparison
std::vector<int> listToVector(const Node* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

int main() {
    // Test 1: Insert in middle
    Node* head = buildList({1, 2, 4, 5});
    Node* tail = head->next->next; // last node (5)
    head = insertBeforeValue(head, tail, 4, 3);
    assert((listToVector(head) == std::vector<int>{1, 2, 3, 4, 5}));

    // Test 2: Insert before head
    head = buildList({2, 3});
    tail = head->next;
    head = insertBeforeValue(head, tail, 2, 1);
    assert((listToVector(head) == std::vector<int>{1, 2, 3}));

    // Test 3: Value not found, append at tail
    head = buildList({1, 2, 3});
    tail = head->next->next;
    head = insertBeforeValue(head, tail, 99, 4);
    assert((listToVector(head) == std::vector<int>{1, 2, 3, 4}));
    assert(tail->data == 4); // tail updated

    // Test 4: Empty list
    head = nullptr;
    tail = nullptr;
    head = insertBeforeValue(head, tail, 10, 5);
    assert(head != nullptr && tail != nullptr);
    assert(head->data == 5 && tail->data == 5);
    assert(head->next == nullptr);

    // Test 5: Single node, value not found (append)
    head = buildList({42});
    tail = head;
    head = insertBeforeValue(head, tail, 1, 0);
    assert((listToVector(head) == std::vector<int>{42, 0}));
    assert(tail->data == 0);

    // Cleanup (optional for test environment)
    // Note: In a real program, you'd delete all nodes to avoid leaks.
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The core idea is to traverse the list while maintaining a pointer to the previous node, looking for the first node whose `data` equals `value`. We handle three distinct cases:
// 1. **Empty list**: Create the new node, set it as both head and tail, and return it.
// 2. **Head insertion**: If the first node already contains `value`, call a helper (or inline logic) to insert at head, updating the head reference and leaving tail unchanged.
// 3. **Middle or tail insertion**: We iterate using a `curr` pointer starting from `head->next` and a `prev` pointer starting from `head`. If we find a node with `data == value`, we insert the new node between `prev` and `curr`, updating `curr`'s predecessor. If we reach the end (i.e., `curr == nullptr`) without finding `value`, we append the new node at the tail, updating the `tail` pointer accordingly.
//
// Edge cases include: inserting before the first node (head update), value not found (tail insertion, tail update), and a single-node list where the node does not match (append at tail, tail update). The algorithm runs in **O(n)** time, where n is the number of nodes, because we traverse each node at most once. Space usage is **O(1)** additional, as we only create one new node regardless of list size.
