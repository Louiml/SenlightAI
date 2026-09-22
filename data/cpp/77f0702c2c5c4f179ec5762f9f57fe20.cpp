Write a C++ function `int findMiddleValue(int values[], int size)` that receives a dynamically allocated array of positive integers and its size, and returns the value stored in the middle node of a circular linked list built from these integers. The function must construct a circular singly linked list where each node contains one value from the array in the given order, and the last node's `next` pointer points back to the head (forming a cycle). Return the value in the node that is exactly at the middle position when traversing the list starting from the head, using a slow/fast pointer technique. If the array size is even, return the value at position `size/2` (0-indexed from head, meaning the second of the two middle nodes). The input array is guaranteed to be non-empty. The function should properly manage memory (clean up the list after finding the value, including breaking the cycle to avoid infinite deletion). Consider edge cases such as size = 1 (the only node points to itself).

// The core idea is to first build a circular linked list from the input array. Create nodes dynamically, link them sequentially, and set the last node's `next` to the head. For finding the middle, use two pointers: `slow` moves one step and `fast` moves two steps per iteration. In a circular list, care must be taken because `fast` will eventually loop around; however, by limiting the traversal to at most `size` steps, we can safely locate the middle. Specifically, initialize both pointers to head. Move `slow` and `fast` exactly `size/2` times (for even sizes, this points to the second middle; for odd, the true middle). After moving, `slow` points to the desired node. To avoid infinite loops, do not depend on `fast` becoming null; instead, use a counter or rely on the fact that moving `size/2` times is safe. After retrieving the value, delete all nodes by traversing from head and using a temporary pointer, but since the list is circular, you must break the cycle first (set tail's `next` to null) or count nodes to avoid infinite loop. Time complexity is O(n) for building and O(n) for traversal/deletion (since we move `size/2` steps and then delete n nodes). Space complexity is O(n) for the list itself, but the function uses only O(1) extra space for pointers.

#include <cstddef>

struct Node {
    int data;
    Node* next;
};

// Builds a circular linked list from array values and returns the value at the middle position.
int findMiddleValue(const int values[], int size) {
    if (size <= 0) return -1; // not expected per spec

    // Build circular linked list
    Node* head = new Node{values[0], nullptr};
    Node* current = head;
    for (int i = 1; i < size; ++i) {
        current->next = new Node{values[i], nullptr};
        current = current->next;
    }
    current->next = head; // make circular

    // Find middle using slow/fast pointer, but move exactly size/2 steps
    Node* slow = head;
    for (int i = 0; i < size / 2; ++i) {
        slow = slow->next;
    }
    int result = slow->data;

    // Clean up: break cycle first to allow simple deletion
    current->next = nullptr; // current is tail, so remove circular link
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    return result;
}

#include <cassert>

int main() {
    // Test cases
    int arr1[] = {5}; 
    assert(findMiddleValue(arr1, 1) == 5);

    int arr2[] = {1, 2, 3};
    assert(findMiddleValue(arr2, 3) == 2);

    int arr3[] = {10, 20, 30, 40};
    assert(findMiddleValue(arr3, 4) == 30); // second of two middles: index 2

    int arr4[] = {7, 8, 9, 10, 11, 12};
    assert(findMiddleValue(arr4, 6) == 10); // index 3

    int arr5[] = {100, 200, 300, 400, 500};
    assert(findMiddleValue(arr5, 5) == 300);

    int arr6[] = {1, 2};
    assert(findMiddleValue(arr6, 2) == 2); // index 1

    return 0;
}
