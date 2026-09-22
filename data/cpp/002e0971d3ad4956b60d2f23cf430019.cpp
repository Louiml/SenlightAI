Write a C++ function that takes the head of a singly linked list and swaps every two adjacent nodes, returning the new head. If the list has an odd number of nodes, the last node remains in place. The function must operate in-place (no copying of node values) and handle empty lists and single-node lists correctly. The linked list node structure is already defined with fields `val` and `next`, plus constructors. You may not use any standard library containers; only the given `ListNode` structure should be manipulated. The function signature is `ListNode* swapAdjacentPairs(ListNode* head)`.

// The solution uses an iterative approach with a dummy head node to simplify boundary handling. We maintain a pointer `prev` (initialized to the dummy node) that tracks the node before the current pair, and `cur` (initially the real head) that points to the first node of the current pair. For each iteration while both `cur` and `cur->next` exist, do the following: let `first = cur` and `second = cur->next`. Then rewire the links: set `first->next = second->next` (so the first node's next points past the pair), then `second->next = first` (so the second node points back to the first), and finally `prev->next = second` (so the previous section points to the new first of the pair). Then move `prev = first` (the old first becomes the new "prev" for the next pair), and `cur = first->next` (advance to the next pair's start). If the list has an odd length, the loop exits with `cur` pointing to the last (unswapped) node, and it remains attached correctly because it was already linked via the previous pair's `first->next`. Edge cases: empty list or single node → return head immediately. Time complexity is O(n) as each node is visited at most once. Space complexity is O(1) excluding the dummy node (which is just a local constant overhead). The approach ensures no memory leak by not allocating per-node; the dummy node is a local object, not heap-allocated.

#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Swap every two adjacent nodes in the linked list, returning the new head.
ListNode* swapAdjacentPairs(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    // Dummy node simplifies pointer rewiring at the start.
    ListNode dummy(0);
    ListNode* prev = &dummy;
    ListNode* cur = head;

    while (cur != nullptr && cur->next != nullptr) {
        ListNode* first = cur;
        ListNode* second = cur->next;

        // Rewire links for the pair.
        first->next = second->next;
        second->next = first;
        prev->next = second;

        // Advance to the next pair.
        prev = first;
        cur = first->next;
    }

    return dummy.next;
}

#include <cassert>

// Helper to create a list from an initializer list-like array.
ListNode* makeList(int arr[], int size) {
    if (size == 0) return nullptr;
    ListNode* head = new ListNode(arr[0]);
    ListNode* cur = head;
    for (int i = 1; i < size; ++i) {
        cur->next = new ListNode(arr[i]);
        cur = cur->next;
    }
    return head;
}

// Helper to check list equality.
bool listsEqual(ListNode* a, ListNode* b) {
    while (a != nullptr && b != nullptr) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

// Helper to delete the list.
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Even length list
    int arr1[] = {1, 2, 3, 4};
    ListNode* list1 = makeList(arr1, 4);
    ListNode* swapped1 = swapAdjacentPairs(list1);
    int expected1[] = {2, 1, 4, 3};
    ListNode* expList1 = makeList(expected1, 4);
    assert(listsEqual(swapped1, expList1));
    deleteList(swapped1);
    deleteList(expList1);

    // Test 2: Odd length list → last remains
    int arr2[] = {1, 2, 3};
    ListNode* list2 = makeList(arr2, 3);
    ListNode* swapped2 = swapAdjacentPairs(list2);
    int expected2[] = {2, 1, 3};
    ListNode* expList2 = makeList(expected2, 3);
    assert(listsEqual(swapped2, expList2));
    deleteList(swapped2);
    deleteList(expList2);

    // Test 3: Single node → unchanged
    int arr3[] = {5};
    ListNode* list3 = makeList(arr3, 1);
    ListNode* swapped3 = swapAdjacentPairs(list3);
    int expected3[] = {5};
    ListNode* expList3 = makeList(expected3, 1);
    assert(listsEqual(swapped3, expList3));
    deleteList(swapped3);
    deleteList(expList3);

    // Test 4: Empty list → null
    ListNode* list4 = nullptr;
    ListNode* swapped4 = swapAdjacentPairs(list4);
    assert(swapped4 == nullptr);

    // Test 5: Two nodes
    int arr5[] = {10, 20};
    ListNode* list5 = makeList(arr5, 2);
    ListNode* swapped5 = swapAdjacentPairs(list5);
    int expected5[] = {20, 10};
    ListNode* expList5 = makeList(expected5, 2);
    assert(listsEqual(swapped5, expList5));
    deleteList(swapped5);
    deleteList(expList5);

    return 0;
}
