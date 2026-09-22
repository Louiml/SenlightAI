Write a C++ function `removeDuplicatesSorted(Node* head)` that takes the head of a singly linked list whose nodes are sorted in non-decreasing order by their integer `data` field, and removes all duplicate values in-place by deleting the redundant nodes. The function must handle an empty list and a list with all identical values. After removal, the remaining list must still be sorted and contain each distinct value exactly once. The function should not allocate any new nodes or use any temporary containers; it should only manipulate existing nodes and deallocate removed nodes using `delete`.
The algorithm uses a single pointer `current` that starts at the head. It repeatedly compares `current->data` with `current->next->data`. If they are equal, the next node is a duplicate: save its `next` pointer, delete the duplicate node, and relink `current->next` to the saved pointer. Crucially, `current` is only advanced when a deletion did **not** occur, because after deletion the new `current->next` could also be a duplicate of `current->data`. If the values differ, `current` moves to the next node. Edge cases include an empty list (return immediately) and a single node (the loop condition `current->next != nullptr` handles it, so nothing is changed). Time complexity is O(n) because each node is visited at most a constant number of times. Space complexity is O(1) since only a few pointers are used and no extra data structures are allocated.
#include <cstddef>

// Definition for singly-linked list.
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Remove all duplicate values from a sorted singly linked list in-place.
// The list is assumed to be sorted in non-decreasing order.
void removeDuplicatesSorted(Node* head) {
    // Empty list: nothing to do.
    if (head == nullptr) {
        return;
    }

    Node* current = head;
    while (current->next != nullptr) {
        if (current->data == current->next->data) {
            // Duplicate found: unlink and delete the next node.
            Node* duplicate = current->next;
            current->next = duplicate->next;
            delete duplicate;
            // Do NOT advance 'current' because the new next may also be a duplicate.
        } else {
            // No duplicate: move to the next node.
            current = current->next;
        }
    }
}
#include <cassert>
#include <iostream>

// Node struct and removeDuplicatesSorted function should be defined above (not repeated here).

// Helper to create a list from an initializer list (for testing).
Node* createList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* n = new Node(v);
        if (head == nullptr) {
            head = n;
            tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }
    return head;
}

// Helper to collect list values into a vector (for testing).
std::vector<int> listToVector(Node* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

// Helper to delete the entire list (for testing).
void deleteList(Node* head) {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: No duplicates.
    Node* list1 = createList({1, 2, 3, 4});
    removeDuplicatesSorted(list1);
    assert((listToVector(list1) == std::vector<int>{1, 2, 3, 4}));
    deleteList(list1);

    // Test 2: Consecutive duplicates in the middle.
    Node* list2 = createList({1, 2, 2, 3, 3, 3, 4});
    removeDuplicatesSorted(list2);
    assert((listToVector(list2) == std::vector<int>{1, 2, 3, 4}));
    deleteList(list2);

    // Test 3: All elements identical.
    Node* list3 = createList({5, 5, 5, 5});
    removeDuplicatesSorted(list3);
    assert((listToVector(list3) == std::vector<int>{5}));
    deleteList(list3);

    // Test 4: Single element.
    Node* list4 = createList({7});
    removeDuplicatesSorted(list4);
    assert((listToVector(list4) == std::vector<int>{7}));
    deleteList(list4);

    // Test 5: Empty list (head is nullptr).
    Node* list5 = nullptr;
    removeDuplicatesSorted(list5);
    assert(list5 == nullptr);

    // Test 6: Duplicates at the beginning and end.
    Node* list6 = createList({1, 1, 2, 3, 4, 4});
    removeDuplicatesSorted(list6);
    assert((listToVector(list6) == std::vector<int>{1, 2, 3, 4}));
    deleteList(list6);

    // Test 7: Two identical nodes only.
    Node* list7 = createList({9, 9});
    removeDuplicatesSorted(list7);
    assert((listToVector(list7) == std::vector<int>{9}));
    deleteList(list7);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
