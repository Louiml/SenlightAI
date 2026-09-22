Write a C++ function `void quickSortLinkedList(Node** headRef)` that sorts a singly linked list of integers in ascending order using the quicksort algorithm. The function should take a pointer to the head pointer of the list, sort the list in place (without creating a new list), and leave the head pointer pointing to the first node of the sorted list. The `Node` structure is defined as `struct Node { int data; struct Node* next; };`. The function must handle empty lists, single-element lists, lists with duplicate values, and lists already sorted in either ascending or descending order. The linked list nodes are dynamically allocated and must not be freed inside the sort function (memory management is the caller's responsibility). The function must not use any extra arrays or vectors; only pointer manipulation is allowed. The time complexity for average cases should be O(n log n), with worst-case O(n²), and the auxiliary space complexity should be O(log n) due to recursion stack.
The solution implements the classic quicksort for linked lists using pointer manipulation. The main idea is to choose the last node as the pivot, then partition the list around it: nodes with data less than the pivot are moved to the front (maintaining original relative order), while nodes greater than or equal to the pivot are moved after the pivot. We track the new head and new tail of the list during partitioning. After partitioning, the pivot is in its correct final position, so we recursively sort the left sublist (nodes before the pivot) and the right sublist (nodes after the pivot) and then link them together. Important edge cases include: an empty list or a list with one node (base case, return immediately), duplicate pivot values (they go to the right sublist to avoid infinite recursion), and the case where the pivot is already the smallest element (so the left sublist is empty, and we skip the left recursion). The `getTail` helper finds the last node of the list. The time complexity is O(n log n) on average because each partition step takes O(n) time and the recursion depth averages O(log n). The worst-case (e.g., already sorted list) is O(n²) because each partition splits off only the pivot. Space complexity is O(log n) for the recursion stack in average cases, but O(n) in worst case. The implementation modifies the list in place, and the head pointer is updated to the new head after sorting.
#include <cstddef>

struct Node {
    int data;
    struct Node* next;
};

// Helper: return a pointer to the last node of the list starting at 'cur'.
static Node* getTail(Node* cur) {
    while (cur != nullptr && cur->next != nullptr)
        cur = cur->next;
    return cur;
}

// Partition the list segment from 'head' to 'end' (inclusive). The pivot is 'end'.
// On return, 'newHead' points to the first node of the partitioned segment,
// 'newEnd' points to the last node, and the function returns the pivot node.
static Node* partition(Node* head, Node* end, Node** newHead, Node** newEnd) {
    Node* pivot = end;
    Node* prev = nullptr;
    Node* cur = head;
    Node* tail = pivot;

    while (cur != pivot) {
        if (cur->data < pivot->data) {
            // Node stays in the "less than pivot" portion.
            if (*newHead == nullptr)
                *newHead = cur;
            prev = cur;
            cur = cur->next;
        } else {
            // Move cur to after the pivot.
            if (prev)
                prev->next = cur->next;
            Node* tmp = cur->next;
            cur->next = nullptr;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }

    // If all nodes are >= pivot, pivot becomes the new head.
    if (*newHead == nullptr)
        *newHead = pivot;

    *newEnd = tail;
    return pivot;
}

// Recursive quicksort for the segment from 'head' to 'end' (inclusive).
static Node* quickSortRecur(Node* head, Node* end) {
    if (!head || head == end)
        return head;

    Node* newHead = nullptr;
    Node* newEnd = nullptr;

    Node* pivot = partition(head, end, &newHead, &newEnd);

    // Sort the left part (nodes before the pivot), if any.
    if (newHead != pivot) {
        Node* tmp = newHead;
        while (tmp->next != pivot)
            tmp = tmp->next;
        tmp->next = nullptr;  // detach left part from pivot

        newHead = quickSortRecur(newHead, tmp);

        // Reattach pivot at the end of the sorted left part.
        tmp = getTail(newHead);
        tmp->next = pivot;
    }

    // Sort the right part (nodes after the pivot).
    pivot->next = quickSortRecur(pivot->next, newEnd);

    return newHead;
}

// Publicly accessible function: sort the linked list pointed to by 'headRef' in ascending order.
void quickSortLinkedList(Node** headRef) {
    if (headRef == nullptr || *headRef == nullptr)
        return;
    (*headRef) = quickSortRecur(*headRef, getTail(*headRef));
}
#include <cassert>
#include <cstddef>

// Node structure and function prototype (assume included from solution)
struct Node {
    int data;
    struct Node* next;
};
void quickSortLinkedList(Node** headRef);

// Helper to create a linked list from an array (caller must free)
static Node* createList(const int* arr, int n) {
    Node* head = nullptr;
    for (int i = n - 1; i >= 0; --i) {
        Node* node = new Node;
        node->data = arr[i];
        node->next = head;
        head = node;
    }
    return head;
}

// Helper to check if a linked list is sorted ascending and matches array
static bool checkSorted(Node* head, const int* expected, int n) {
    Node* cur = head;
    for (int i = 0; i < n; ++i) {
        if (cur == nullptr || cur->data != expected[i])
            return false;
        cur = cur->next;
    }
    return cur == nullptr;
}

// Helper to free list
static void freeList(Node* head) {
    while (head) {
        Node* tmp = head->next;
        delete head;
        head = tmp;
    }
}

int main() {
    // Test 1: Empty list
    Node* head1 = nullptr;
    quickSortLinkedList(&head1);
    assert(head1 == nullptr);

    // Test 2: Single element
    int arr2[] = {42};
    Node* head2 = createList(arr2, 1);
    quickSortLinkedList(&head2);
    assert(checkSorted(head2, arr2, 1));
    freeList(head2);

    // Test 3: Already sorted ascending
    int arr3[] = {1, 2, 3, 4, 5};
    Node* head3 = createList(arr3, 5);
    quickSortLinkedList(&head3);
    assert(checkSorted(head3, arr3, 5));
    freeList(head3);

    // Test 4: Reverse sorted
    int arr4_in[] = {5, 4, 3, 2, 1};
    int arr4_out[] = {1, 2, 3, 4, 5};
    Node* head4 = createList(arr4_in, 5);
    quickSortLinkedList(&head4);
    assert(checkSorted(head4, arr4_out, 5));
    freeList(head4);

    // Test 5: Unsorted with duplicates
    int arr5_in[] = {3, 1, 2, 1, 3, 2};
    int arr5_out[] = {1, 1, 2, 2, 3, 3};
    Node* head5 = createList(arr5_in, 6);
    quickSortLinkedList(&head5);
    assert(checkSorted(head5, arr5_out, 6));
    freeList(head5);

    // Test 6: All same values
    int arr6[] = {7, 7, 7, 7};
    Node* head6 = createList(arr6, 4);
    quickSortLinkedList(&head6);
    assert(checkSorted(head6, arr6, 4));
    freeList(head6);

    // Test 7: Large random-like list (simple alternating pattern)
    int arr7_in[] = {9, -1, 5, -8, 3, 0, -2, 7};
    int arr7_out[] = {-8, -2, -1, 0, 3, 5, 7, 9};
    Node* head7 = createList(arr7_in, 8);
    quickSortLinkedList(&head7);
    assert(checkSorted(head7, arr7_out, 8));
    freeList(head7);

    // Test 8: Two elements descending
    int arr8_in[] = {2, 1};
    int arr8_out[] = {1, 2};
    Node* head8 = createList(arr8_in, 2);
    quickSortLinkedList(&head8);
    assert(checkSorted(head8, arr8_out, 2));
    freeList(head8);

    return 0;
}
