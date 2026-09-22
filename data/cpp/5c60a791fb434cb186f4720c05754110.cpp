/*
Given a singly linked list where each node stores an integer, write a C++ function `removeEvenNodes` that takes the head pointer of the list as its parameter, removes all nodes whose `info` field is even, and returns the pointer to the new head of the modified list. The function must handle the case where the head itself is even (removing leading even nodes), and after removal, it should also update the `last` pointer of the list (which is a global variable of type `list*`) to point to the last node of the resulting list. The function must preserve the relative order of the remaining odd nodes, and if all nodes are even, the resulting list should be empty (head becomes `NULL`). The function will be used in a program that first constructs a list via user input (using a provided `make` function), prints it, then calls `removeEvenNodes`, and prints the result again.
*/
#include <cstddef>

struct list {
    int info;
    list* next;
};

// Global pointers as per the original specification
extern list* first;
extern list* last;

/**
 * Removes all nodes with even `info` values from a singly linked list.
 * @param head Pointer to the head of the list (may be NULL).
 * @return Pointer to the new head of the list after removal.
 */
list* removeEvenNodes(list* head) {
    if (head == NULL) {
        first = NULL;
        last = NULL;
        return NULL;
    }

    // Remove leading even nodes
    while (head != NULL && head->info % 2 == 0) {
        list* temp = head;
        head = head->next;
        delete temp;
    }

    // Update the global `first` pointer
    first = head;

    if (head == NULL) {
        // All nodes were even
        last = NULL;
        return NULL;
    }

    // Traverse the rest of the list
    list* prev = head;
    list* curr = head->next;

    while (curr != NULL) {
        if (curr->info % 2 == 0) {
            // Remove even node
            prev->next = curr->next;
            delete curr;
            curr = prev->next;
        } else {
            // Move to next
            prev = curr;
            curr = curr->next;
        }
    }

    // Update `last` to point to the final node
    last = prev;
    return head;
}
#include <cassert>

// Declare the global pointers and the solution function
list* first = NULL;
list* last = NULL;

// Forward declaration of the function
list* removeEvenNodes(list* head);

// Helper to build a list from an array (for testing)
list* buildList(const int* arr, int n) {
    if (n == 0) return NULL;
    list* head = new list{arr[0], NULL};
    list* tail = head;
    for (int i = 1; i < n; ++i) {
        tail->next = new list{arr[i], NULL};
        tail = tail->next;
    }
    first = head;
    last = tail;
    return head;
}

// Helper to convert list to vector for comparison
int* toArray(list* head, int& size) {
    size = 0;
    list* curr = head;
    while (curr) { size++; curr = curr->next; }
    int* arr = new int[size];
    curr = head;
    for (int i = 0; i < size; ++i) {
        arr[i] = curr->info;
        curr = curr->next;
    }
    return arr;
}

int main() {
    // Test 1: Normal mixed list
    int a1[] = {1, 2, 3, 4, 5};
    list* h1 = buildList(a1, 5);
    h1 = removeEvenNodes(h1);
    int sz1;
    int* r1 = toArray(h1, sz1);
    assert(sz1 == 3 && r1[0] == 1 && r1[1] == 3 && r1[2] == 5);
    assert(last != NULL && last->info == 5);
    delete[] r1;

    // Test 2: All even nodes
    int a2[] = {2, 4, 6};
    list* h2 = buildList(a2, 3);
    h2 = removeEvenNodes(h2);
    assert(h2 == NULL && last == NULL);

    // Test 3: All odd nodes
    int a3[] = {1, 3, 5};
    list* h3 = buildList(a3, 3);
    h3 = removeEvenNodes(h3);
    int sz3;
    int* r3 = toArray(h3, sz3);
    assert(sz3 == 3 && r3[0] == 1 && r3[1] == 3 && r3[2] == 5);
    assert(last != NULL && last->info == 5);
    delete[] r3;

    // Test 4: Head even, then odd, then even
    int a4[] = {2, 1, 4};
    list* h4 = buildList(a4, 3);
    h4 = removeEvenNodes(h4);
    int sz4;
    int* r4 = toArray(h4, sz4);
    assert(sz4 == 1 && r4[0] == 1);
    assert(last->info == 1);
    delete[] r4;

    // Test 5: Single even node
    int a5[] = {10};
    list* h5 = buildList(a5, 1);
    h5 = removeEvenNodes(h5);
    assert(h5 == NULL && last == NULL);

    // Test 6: Single odd node
    int a6[] = {7};
    list* h6 = buildList(a6, 1);
    h6 = removeEvenNodes(h6);
    int sz6;
    int* r6 = toArray(h6, sz6);
    assert(sz6 == 1 && r6[0] == 7);
    assert(last->info == 7);
    delete[] r6;

    // Test 7: Empty list
    list* h7 = removeEvenNodes(NULL);
    assert(h7 == NULL && last == NULL);

    // Clean up remaining lists (not strictly necessary for the test)
    return 0;
}
// The solution approach involves iterating through the linked list while maintaining pointers to the current node and the previous node (to allow deletion). First, we handle the special case where the head node is even: we advance the head pointer past all consecutive even nodes until we find an odd node or reach `NULL`. This updates the `first` global pointer to the new head. Then we traverse the remaining list with a `prev` pointer (initially the new head) and a `curr` pointer (initially the second node, if any). For each node, if `curr->info` is even, we unlink it by setting `prev->next = curr->next` and delete it, without advancing `prev`. If `curr->info` is odd, we advance both `prev` and `curr`. After the traversal, we update the `last` global pointer to point to the final node of the list (which is `prev` if the list is non‑empty, otherwise `NULL`). Edge cases include: an empty list (return `NULL` immediately), a list with only even nodes (head becomes `NULL` and `last` becomes `NULL`), and a list with only odd nodes (no changes). Time complexity is O(n) because we traverse each node exactly once. Space complexity is O(1) since we only use a few pointers and delete nodes in place.
