/*
Write a C++ function that takes two sorted singly-linked lists of integers and returns a new sorted linked list containing the intersection of the two input lists (i.e., all values that appear in both lists, preserving order and including duplicates). The input lists are sorted in non-decreasing order. You may assume both lists are non-empty and contain valid integer values. Use the provided `Node` structure with `data` and `next` fields. The function should return the head pointer of the resulting linked list; if there is no intersection, return `nullptr`. Do not modify the input lists.
*/

#include <cstddef>

struct Node {
    int data;
    Node *next;
    Node(int val) : data(val), next(nullptr) {}
};

// Returns a new sorted linked list containing the intersection of two sorted lists.
// The input lists are not modified. If the intersection is empty, returns nullptr.
Node* findIntersection(Node* head1, Node* head2) {
    Node* resultHead = nullptr;
    Node* resultTail = nullptr;

    Node* p1 = head1;
    Node* p2 = head2;

    while (p1 != nullptr && p2 != nullptr) {
        if (p1->data == p2->data) {
            // Add the common value to the result list
            Node* newNode = new Node(p1->data);
            if (resultHead == nullptr) {
                resultHead = newNode;
                resultTail = newNode;
            } else {
                resultTail->next = newNode;
                resultTail = newNode;
            }
            p1 = p1->next;
            p2 = p2->next;
        } else if (p1->data < p2->data) {
            p1 = p1->next;
        } else {
            p2 = p2->next;
        }
    }

    return resultHead;
}

#include <cassert>
#include <iostream>

// Helper to create a list from an initializer list
Node* makeList(std::initializer_list<int> values) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : values) {
        Node* newNode = new Node(v);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// Helper to compare two lists (values only, ignoring memory addresses)
bool listsEqual(Node* a, Node* b) {
    while (a != nullptr && b != nullptr) {
        if (a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    return (a == nullptr && b == nullptr);
}

// Helper to free a list
void freeList(Node* head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Test 1: Basic intersection
    Node* list1 = makeList({1, 2, 3, 4, 5});
    Node* list2 = makeList({2, 4, 6});
    Node* result = findIntersection(list1, list2);
    assert(listsEqual(result, makeList({2, 4})));
    freeList(result);
    freeList(list1);
    freeList(list2);

    // Test 2: No intersection (empty result should be nullptr)
    list1 = makeList({1, 3, 5});
    list2 = makeList({2, 4, 6});
    result = findIntersection(list1, list2);
    assert(result == nullptr);
    freeList(list1);
    freeList(list2);

    // Test 3: Identical lists (all elements common)
    list1 = makeList({1, 2, 3});
    list2 = makeList({1, 2, 3});
    result = findIntersection(list1, list2);
    assert(listsEqual(result, makeList({1, 2, 3})));
    freeList(result);
    freeList(list1);
    freeList(list2);

    // Test 4: Duplicates in both lists
    list1 = makeList({1, 1, 2, 3});
    list2 = makeList({1, 1, 2, 2});
    result = findIntersection(list1, list2);
    assert(listsEqual(result, makeList({1, 1, 2})));
    freeList(result);
    freeList(list1);
    freeList(list2);

    // Test 5: First list shorter
    list1 = makeList({5});
    list2 = makeList({1, 5, 9});
    result = findIntersection(list1, list2);
    assert(listsEqual(result, makeList({5})));
    freeList(result);
    freeList(list1);
    freeList(list2);

    // Test 6: Second list shorter
    list1 = makeList({1, 2, 3, 4});
    list2 = makeList({3});
    result = findIntersection(list1, list2);
    assert(listsEqual(result, makeList({3})));
    freeList(result);
    freeList(list1);
    freeList(list2);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// The main idea is to use a two-pointer technique on the two sorted lists. Since both lists are sorted, we can compare the current nodes of each list:
// - If the data values are equal, that value is part of the intersection. We create a new node for the result list and advance both pointers.
// - If the value in the first list is smaller, advance the first pointer only, because the value cannot appear later in the second list (which is sorted).
// - If the value in the second list is smaller, advance the second pointer only.
// Continue until either pointer reaches the end. Edge cases include: duplicates (which are handled naturally because equal values are processed once per occurrence in both lists), an empty intersection (return `nullptr`), and lists of different lengths. The time complexity is O(n + m) where n and m are the lengths of the input lists, since each pointer traverses its list at most once. The space complexity is O(k) where k is the size of the intersection, because we only allocate nodes for the result.
