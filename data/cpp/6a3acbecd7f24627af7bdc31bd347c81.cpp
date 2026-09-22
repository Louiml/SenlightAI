Write a C++ function `mergeSortedLists` that takes two heads of sorted singly linked lists (each in non-decreasing order, possibly containing duplicate values) and returns the head of a single merged sorted linked list. The function must not allocate new nodes; it must reuse the existing nodes by rearranging their `next` pointers. The input lists may be empty (nullptr heads), and either list may be longer than the other. The returned list must contain all nodes from both inputs in non-decreasing order. You may assume the nodes have an integer `data` field and a `next` pointer. Define a `SinglyLinkedListNode` struct with a constructor for convenience.
The core approach is an iterative in-place merge that avoids extra memory. Handle the trivial cases first: if both heads are null, return null; if one head is null, return the other. Then, ensure `head1` points to the smaller first element (swap heads if needed) so the returned head is correct. Maintain a `tail` pointer that tracks the last node of the merged list built so far. In a loop while both current pointers are non-null, advance `head1` while its data is less than or equal to `head2->data`, updating `tail` along the way. When `head1` becomes null or its data exceeds `head2`'s data, link `tail->next` to `head2`, then swap `head1` and `head2` to continue with the other list. This works because each iteration picks the smallest available node from the front of the two remaining portions. Edge cases: duplicate values are handled by using `<=` so equal elements keep stability (though not required), and when one list runs out, the remaining list is attached as-is. Time complexity is O(n+m) because each node is visited once. Space complexity is O(1) auxiliary, excluding recursion (we use iteration) and the output list itself.
#include <cstddef>

struct SinglyLinkedListNode {
    int data;
    SinglyLinkedListNode* next;
    SinglyLinkedListNode(int val) : data(val), next(nullptr) {}
};

// Merge two sorted singly linked lists in place and return the merged head.
SinglyLinkedListNode* mergeSortedLists(SinglyLinkedListNode* head1, SinglyLinkedListNode* head2) {
    if (head1 == nullptr) return head2;
    if (head2 == nullptr) return head1;

    // Ensure head1 points to the smaller first element.
    if (head1->data > head2->data) {
        SinglyLinkedListNode* temp = head1;
        head1 = head2;
        head2 = temp;
    }

    SinglyLinkedListNode* mergedHead = head1;
    SinglyLinkedListNode* tail = nullptr;

    while (head1 != nullptr && head2 != nullptr) {
        // Advance head1 while it is <= head2, updating tail.
        while (head1 != nullptr && head1->data <= head2->data) {
            tail = head1;
            head1 = head1->next;
        }
        // Link the tail to the current head2.
        tail->next = head2;
        // Swap head1 and head2 to continue processing the other list.
        SinglyLinkedListNode* temp = head1;
        head1 = head2;
        head2 = temp;
    }

    return mergedHead;
}
#include <cassert>

// Helper to build a list from an initializer list (for testing).
SinglyLinkedListNode* buildList(std::initializer_list<int> values) {
    SinglyLinkedListNode* head = nullptr;
    SinglyLinkedListNode* tail = nullptr;
    for (int v : values) {
        SinglyLinkedListNode* node = new SinglyLinkedListNode(v);
        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

// Helper to free a list.
void freeList(SinglyLinkedListNode* head) {
    while (head != nullptr) {
        SinglyLinkedListNode* next = head->next;
        delete head;
        head = next;
    }
}

// Helper to collect values into a vector.
std::vector<int> toVector(SinglyLinkedListNode* head) {
    std::vector<int> result;
    while (head != nullptr) {
        result.push_back(head->data);
        head = head->next;
    }
    return result;
}

int main() {
    // Test 1: Both empty.
    {
        SinglyLinkedListNode* result = mergeSortedLists(nullptr, nullptr);
        assert(result == nullptr);
    }

    // Test 2: One empty.
    {
        SinglyLinkedListNode* list = buildList({1, 2, 3});
        SinglyLinkedListNode* result = mergeSortedLists(list, nullptr);
        assert(toVector(result) == std::vector<int>({1, 2, 3}));
        freeList(result);
    }

    // Test 3: Basic merge.
    {
        SinglyLinkedListNode* l1 = buildList({1, 3, 5});
        SinglyLinkedListNode* l2 = buildList({2, 4, 6});
        SinglyLinkedListNode* result = mergeSortedLists(l1, l2);
        assert(toVector(result) == std::vector<int>({1, 2, 3, 4, 5, 6}));
        freeList(result);
    }

    // Test 4: Duplicate values.
    {
        SinglyLinkedListNode* l1 = buildList({1, 2, 2, 3});
        SinglyLinkedListNode* l2 = buildList({2, 2, 4});
        SinglyLinkedListNode* result = mergeSortedLists(l1, l2);
        assert(toVector(result) == std::vector<int>({1, 2, 2, 2, 2, 3, 4}));
        freeList(result);
    }

    // Test 5: One list completely smaller than the other.
    {
        SinglyLinkedListNode* l1 = buildList({1, 2});
        SinglyLinkedListNode* l2 = buildList({3, 4, 5});
        SinglyLinkedListNode* result = mergeSortedLists(l1, l2);
        assert(toVector(result) == std::vector<int>({1, 2, 3, 4, 5}));
        freeList(result);
    }

    // Test 6: Interleaved with equal first elements.
    {
        SinglyLinkedListNode* l1 = buildList({1, 3, 5});
        SinglyLinkedListNode* l2 = buildList({1, 2, 4});
        SinglyLinkedListNode* result = mergeSortedLists(l1, l2);
        assert(toVector(result) == std::vector<int>({1, 1, 2, 3, 4, 5}));
        freeList(result);
    }

    // Test 7: Single node each.
    {
        SinglyLinkedListNode* l1 = new SinglyLinkedListNode(7);
        SinglyLinkedListNode* l2 = new SinglyLinkedListNode(3);
        SinglyLinkedListNode* result = mergeSortedLists(l1, l2);
        assert(toVector(result) == std::vector<int>({3, 7}));
        freeList(result);
    }

    return 0;
}
