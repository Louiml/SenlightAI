Write a C++ function named `buildLinkedListFromVector` that takes a non-empty `std::vector<int>&` (values are positive or negative integers, may contain duplicates) and returns a pointer to the head of a singly linked list built from the elements in the same order as they appear in the vector. Each node must be dynamically allocated using `new`. Your function must use the struct `Node` defined as `struct Node { int data; Node* next; Node(int val) : data(val), next(nullptr) {} };`. Do not modify the input vector. Ensure the function works correctly for any non-empty vector, including a single element.

The solution uses a dummy head node (a sentinel) to simplify the insertion logic. We iterate over the vector elements, for each value we allocate a new `Node` and link it to the current tail node (tracked via a temporary pointer). After the loop, the temporary pointer points to the last node, and the dummy node's `next` points to the actual first element. The function returns the dummy node's next pointer, which is the head of the list. Edge cases: vector with one element → dummy->next points to that node; duplicates are handled naturally because each value is inserted as a new node; negative values are also fine. Time complexity: O(n) where n is the vector size (single pass). Space complexity: O(n) for the newly allocated nodes (the dummy node is constant extra space). The input vector is not modified (we use a const reference to be safe, but the function signature given in the code snippet uses `vector<int>&`; for const correctness, we can use `const std::vector<int>&`).

// Node struct as required
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Build a singly linked list from a vector of integers.
// Returns pointer to the head of the new list.
// The list nodes are allocated with 'new' and must be freed by the caller.
Node* buildLinkedListFromVector(const std::vector<int>& arr) {
    // Dummy head simplifies linking
    Node* dummy = new Node(-1);
    Node* tail = dummy;
    for (int val : arr) {
        Node* newNode = new Node(val);
        tail->next = newNode;
        tail = newNode;
    }
    Node* head = dummy->next;
    delete dummy; // dummy is no longer needed
    return head;
}

#include <cassert>
#include <vector>
#include <iostream>

// Node struct (same as above)
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Function declaration (or include the solution above)
Node* buildLinkedListFromVector(const std::vector<int>& arr);

// Helper to convert list to vector for testing (to avoid memory leak, we also delete list)
bool checkList(Node* head, const std::vector<int>& expected) {
    Node* curr = head;
    size_t i = 0;
    while (curr != nullptr) {
        if (i >= expected.size() || curr->data != expected[i]) return false;
        curr = curr->next;
        ++i;
    }
    return i == expected.size();
}

int main() {
    // Test 1: Multiple elements, mixed signs
    std::vector<int> v1 = {1, -2, 3, -4};
    Node* head1 = buildLinkedListFromVector(v1);
    assert(checkList(head1, v1));
    // Cleanup
    while (head1) { Node* tmp = head1->next; delete head1; head1 = tmp; }

    // Test 2: Single element
    std::vector<int> v2 = {7};
    Node* head2 = buildLinkedListFromVector(v2);
    assert(checkList(head2, v2));
    while (head2) { Node* tmp = head2->next; delete head2; head2 = tmp; }

    // Test 3: Duplicates
    std::vector<int> v3 = {0, 0, 0};
    Node* head3 = buildLinkedListFromVector(v3);
    assert(checkList(head3, v3));
    while (head3) { Node* tmp = head3->next; delete head3; head3 = tmp; }

    // Test 4: Large vector (1000 elements, check first and last)
    std::vector<int> v4;
    for (int i = 0; i < 1000; ++i) v4.push_back(i % 5);
    Node* head4 = buildLinkedListFromVector(v4);
    assert(checkList(head4, v4));
    // Also verify first and last
    Node* last = head4;
    while (last->next) last = last->next;
    assert(head4->data == 0);
    assert(last->data == 4);
    while (head4) { Node* tmp = head4->next; delete head4; head4 = tmp; }

    // Test 5: Vector with only one negative value
    std::vector<int> v5 = {-100};
    Node* head5 = buildLinkedListFromVector(v5);
    assert(checkList(head5, v5));
    while (head5) { Node* tmp = head5->next; delete head5; head5 = tmp; }

    // Test 6: Original vector unchanged (pass const reference, so not needed)

    std::cout << "All tests passed.\n";
    return 0;
}
