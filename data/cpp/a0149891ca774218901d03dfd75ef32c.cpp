// Given an unsorted singly-linked list of integers, write a C++ function `void removeDuplicates(Node* head)` that removes all duplicate values from the list, keeping only the first occurrence of each value. The function should modify the list in-place and free the memory of removed nodes. You may assume the list is not circular and that no sentinel or dummy head is used.

The solution traverses the linked list once with an outer loop that represents the current node whose value we are checking for duplicates. For each such node, we run an inner loop starting from the current node's next pointer, scanning all subsequent nodes. Whenever we find a node whose value equals the current node's value, we "unlink" that duplicate node by updating the previous node's next pointer to skip it, then delete the duplicate node. We must be careful to preserve the pointer to the previous node while scanning, and we must not advance the current node after deleting a duplicate (since the next node may still be a duplicate). The outer loop advances only after all duplicates of the current value have been removed. Edge cases: an empty list or a single-node list requires no action. Also, if duplicates appear consecutively, the inner loop must handle multiple deletions in a row correctly. Time complexity is O(n²) in the worst case (e.g., all elements identical), and O(1) auxiliary space beyond the list's existing nodes. The function operates in-place and uses `delete` to free memory.

#include <cstddef>

// Node structure for a doubly-linked list (as given in the snippet)
struct Node {
    int value;
    Node* next;
    Node* prev;
    Node(int v) : value(v), next(nullptr), prev(nullptr) {}
};

// Remove duplicate values from a sorted or unsorted linked list.
// Keeps only the first occurrence of each value. Modifies the list in-place.
void removeDuplicates(Node* head) {
    if (head == nullptr) return;

    Node* outer = head;
    while (outer != nullptr) {
        Node* prev = outer;
        Node* inner = outer->next;
        while (inner != nullptr) {
            if (inner->value == outer->value) {
                // Unlink and delete the duplicate node
                prev->next = inner->next;
                if (inner->next != nullptr) {
                    inner->next->prev = prev;
                }
                Node* temp = inner;
                inner = inner->next; // Move to the next candidate
                delete temp;
            } else {
                prev = inner;
                inner = inner->next;
            }
        }
        outer = outer->next;
    }
}

#include <cassert>
#include <iostream>

// Node structure (same as in solution)
struct Node {
    int value;
    Node* next;
    Node* prev;
    Node(int v) : value(v), next(nullptr), prev(nullptr) {}
};

// Helper to build a list from an initializer list
Node* buildList(std::initializer_list<int> vals) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : vals) {
        Node* n = new Node(v);
        if (head == nullptr) {
            head = n;
            tail = n;
        } else {
            tail->next = n;
            n->prev = tail;
            tail = n;
        }
    }
    return head;
}

// Helper to get list as a vector for comparison
std::vector<int> toVector(Node* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->value);
        head = head->next;
    }
    return result;
}

// Helper to free the list
void freeList(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: General case with duplicates
    Node* list1 = buildList({3, 1, 3, 2, 1, 4, 3});
    removeDuplicates(list1);
    assert((toVector(list1) == std::vector<int>{3, 1, 2, 4}));
    freeList(list1);

    // Test 2: All duplicates
    Node* list2 = buildList({5, 5, 5, 5});
    removeDuplicates(list2);
    assert((toVector(list2) == std::vector<int>{5}));
    freeList(list2);

    // Test 3: No duplicates
    Node* list3 = buildList({1, 2, 3, 4});
    removeDuplicates(list3);
    assert((toVector(list3) == std::vector<int>{1, 2, 3, 4}));
    freeList(list3);

    // Test 4: Single element
    Node* list4 = buildList({9});
    removeDuplicates(list4);
    assert((toVector(list4) == std::vector<int>{9}));
    freeList(list4);

    // Test 5: Empty list
    Node* list5 = nullptr;
    removeDuplicates(list5);
    assert(list5 == nullptr);

    // Test 6: Duplicates at the end
    Node* list6 = buildList({2, 2, 3, 3, 3, 7});
    removeDuplicates(list6);
    assert((toVector(list6) == std::vector<int>{2, 3, 7}));
    freeList(list6);

    // Test 7: Duplicates at the beginning and middle
    Node* list7 = buildList({8, 8, 1, 8, 2, 1});
    removeDuplicates(list7);
    assert((toVector(list7) == std::vector<int>{8, 1, 2}));
    freeList(list7);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
