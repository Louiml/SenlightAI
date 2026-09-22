// Write a C++ function named `insertSorted` that takes a reference to the head pointer of a singly linked list that is already sorted in non-decreasing order, plus an integer `value`. The function should insert a new node with the given value into the correct position so that the list remains sorted after insertion. The function must handle all possible insertion cases: inserting into an empty list, inserting at the beginning (when the new value is smaller than or equal to the current head), inserting in the middle, and inserting at the end. The function should not allocate more nodes than necessary and must work correctly with duplicate values. You may assume the existing list is always sorted. The function should modify the list in-place and return nothing (void). Do not write a `main` function in your solution; just provide the function and any necessary auxiliary functions/classes.
#include <cassert>
#include <vector>

// Node and function declarations (assume they are provided from solution)
struct Node {
    int data;
    Node* next;
    explicit Node(int val) : data(val), next(nullptr) {}
};
void insertSorted(Node*& head, int value);

// Helper to convert list to vector for comparison
std::vector<int> toVector(const Node* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Helper to clear list memory
void deleteList(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Insert into empty list
    {
        Node* head = nullptr;
        insertSorted(head, 5);
        assert(toVector(head) == std::vector<int>({5}));
        deleteList(head);
    }

    // Test 2: Insert at beginning (value smaller than head)
    {
        Node* head = new Node(10);
        head->next = new Node(20);
        insertSorted(head, 5);
        assert(toVector(head) == std::vector<int>({5, 10, 20}));
        deleteList(head);
    }

    // Test 3: Insert at beginning (value equal to head)
    {
        Node* head = new Node(10);
        head->next = new Node(20);
        insertSorted(head, 10);
        assert(toVector(head) == std::vector<int>({10, 10, 20}));
        deleteList(head);
    }

    // Test 4: Insert in middle
    {
        Node* head = new Node(10);
        head->next = new Node(30);
        insertSorted(head, 20);
        assert(toVector(head) == std::vector<int>({10, 20, 30}));
        deleteList(head);
    }

    // Test 5: Insert at end
    {
        Node* head = new Node(10);
        head->next = new Node(20);
        insertSorted(head, 30);
        assert(toVector(head) == std::vector<int>({10, 20, 30}));
        deleteList(head);
    }

    // Test 6: Insert duplicate in middle
    {
        Node* head = new Node(10);
        head->next = new Node(20);
        head->next->next = new Node(30);
        insertSorted(head, 20);
        assert(toVector(head) == std::vector<int>({10, 20, 20, 30}));
        deleteList(head);
    }

    // Test 7: Multiple sequential inserts maintain sorted order
    {
        Node* head = nullptr;
        insertSorted(head, 40);
        insertSorted(head, 10);
        insertSorted(head, 30);
        insertSorted(head, 20);
        insertSorted(head, 10);
        assert(toVector(head) == std::vector<int>({10, 10, 20, 30, 40}));
        deleteList(head);
    }

    // Test 8: Negative numbers and large positive values
    {
        Node* head = new Node(-5);
        head->next = new Node(0);
        insertSorted(head, -10);
        insertSorted(head, 100);
        insertSorted(head, -5);
        assert(toVector(head) == std::vector<int>({-10, -5, -5, 0, 100}));
        deleteList(head);
    }

    return 0;
}
#include <cstddef>

// Node structure for singly linked list
struct Node {
    int data;
    Node* next;
    explicit Node(int val) : data(val), next(nullptr) {}
};

// Insert a new value into a sorted singly linked list (non-decreasing order)
void insertSorted(Node*& head, int value) {
    Node* newNode = new Node(value);

    // Case 1: empty list
    if (head == nullptr) {
        head = newNode;
        return;
    }

    // Case 2: insert at beginning if new value is less than or equal to head
    if (value <= head->data) {
        newNode->next = head;
        head = newNode;
        return;
    }

    // Case 3: find proper position in middle or end
    Node* current = head;
    while (current->next != nullptr && current->next->data < value) {
        current = current->next;
    }

    // Insert after current (works for both middle and end)
    newNode->next = current->next;
    current->next = newNode;
}
// The solution traverses the sorted linked list to find the first node whose next node has a value greater than the new value, or reaches the end of the list. Insert the new node before that next node (or at the tail). Edge cases: (1) empty list → set head to new node. (2) new value ≤ head’s data → insert at front. (3) otherwise, walk with a pointer `current`, comparing `current->next->data` with the new value. If `current->next` is null, append at end. If `current->next->data >= value`, insert after `current`. Duplicate values are handled naturally: if the new value equals an existing value, inserting after that equal value (or before, but we choose after to keep stability) does not break sorted order. Time complexity is O(n) in the worst case (inserting at the end or middle), O(1) average for insertion at front. Space complexity is O(1) extra (only one new node allocated).
