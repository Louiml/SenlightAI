// Write a C++ function that, given a non-empty sorted singly linked list and an integer `data`, inserts a new node containing `data` into the correct position so that the list remains sorted in non-decreasing order. The list is represented by a `Node` struct with `int data` and `Node* next`. The function should return the head of the modified list. Assume that the input list is always sorted (possibly with duplicates). Handle edge cases where the new data is smaller than the current head, larger than all existing nodes, or duplicates of existing values (in which case, you may insert before or after equal elements as long as the list stays sorted—your chosen position should be consistent, e.g., after all equal elements). The function signature is `Node* sortedInsert(Node* head, int data)`. The list is non-empty; you do not need to handle an empty list, but your solution should be robust if it were called with a null head. Do not include a `main` function in your solution submission, only the function and necessary includes/struct.
The algorithm uses a two-pointer traversal to locate the correct insertion point while maintaining the sorted order. First, create a new node with the given data. If the data is less than or equal to the head's data (or if head is null), insert at the front by setting the new node's next to the current head and returning the new node as the new head. Otherwise, initialize `prev` to head and `curr` to head->next, then advance both pointers while `curr` exists and `curr->data` is strictly less than the given data (to preserve stability by inserting after existing duplicates). Once the loop stops, insert the new node between `prev` and `curr` by updating `prev->next = newnode` and `newnode->next = curr`. Finally, return the original head. Edge cases include: new data is smaller than head (handled by front insertion), new data is larger than all existing nodes (loop ends with `curr == nullptr`, insertion at tail), and duplicates (the condition uses strict `<` so we stop at the first node with data >= given value, placing the new node after all equal-value nodes). Time complexity is O(n) in the worst case due to traversal; space complexity is O(1) auxiliary.
#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Insert a new node with given data into a sorted (non-decreasing) linked list.
// Returns the head of the modified list. The input list is assumed sorted and non-empty.
Node* sortedInsert(Node* head, int data) {
    Node* new_node = new Node(data);

    // Handle insertion at the beginning (also covers null head for robustness)
    if (head == nullptr || data <= head->data) {
        new_node->next = head;
        return new_node;
    }

    Node* prev = head;
    Node* curr = head->next;

    // Advance while we haven't reached the end and current node's data is less than new data
    while (curr != nullptr && curr->data < data) {
        prev = curr;
        curr = curr->next;
    }

    // Insert between prev and curr
    prev->next = new_node;
    new_node->next = curr;

    return head;
}
#include <cassert>

// Helper to build a list from a vector-like initializer list for testing
Node* buildList(std::initializer_list<int> vals) {
    Node* head = nullptr;
    Node** tail = &head;
    for (int v : vals) {
        *tail = new Node(v);
        tail = &((*tail)->next);
    }
    return head;
}

// Helper to free list memory
void freeList(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// Helper to convert list to vector-like values for comparison
std::vector<int> toVector(Node* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

int main() {
    // Test 1: Insert in the middle
    Node* list1 = buildList({1, 3, 5});
    list1 = sortedInsert(list1, 4);
    assert(toVector(list1) == std::vector<int>({1, 3, 4, 5}));
    freeList(list1);

    // Test 2: Insert at the beginning
    Node* list2 = buildList({2, 3});
    list2 = sortedInsert(list2, 1);
    assert(toVector(list2) == std::vector<int>({1, 2, 3}));
    freeList(list2);

    // Test 3: Insert at the end
    Node* list3 = buildList({1, 2});
    list3 = sortedInsert(list3, 5);
    assert(toVector(list3) == std::vector<int>({1, 2, 5}));
    freeList(list3);

    // Test 4: Insert duplicate value (should go after existing equal values)
    Node* list4 = buildList({2, 4, 4, 6});
    list4 = sortedInsert(list4, 4);
    assert(toVector(list4) == std::vector<int>({2, 4, 4, 4, 6}));
    freeList(list4);

    // Test 5: Insert into single-element list (at begin)
    Node* list5 = buildList({5});
    list5 = sortedInsert(list5, 3);
    assert(toVector(list5) == std::vector<int>({3, 5}));
    freeList(list5);

    // Test 6: Insert into single-element list (at end)
    Node* list6 = buildList({5});
    list6 = sortedInsert(list6, 7);
    assert(toVector(list6) == std::vector<int>({5, 7}));
    freeList(list6);

    // Test 7: Insert equal to head
    Node* list7 = buildList({3, 5});
    list7 = sortedInsert(list7, 3);
    assert(toVector(list7) == std::vector<int>({3, 3, 5}));
    freeList(list7);

    // Test 8: Insert equal to last element (goes after it)
    Node* list8 = buildList({1, 2, 2});
    list8 = sortedInsert(list8, 2);
    assert(toVector(list8) == std::vector<int>({1, 2, 2, 2}));
    freeList(list8);

    return 0;
}
