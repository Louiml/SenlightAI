/*
Write a C++ function `mergeTwoSortedLists` that takes two pointers to the heads of singly linked lists, where each list is sorted in non-decreasing order (e.g., each node's `val` is less than or equal to the next node's `val`). The function must merge the two lists into a single sorted linked list and return the head pointer of the merged list. The function must not allocate any new nodes; it must reuse the existing nodes from the two input lists by rearranging their `next` pointers. If either input list is `nullptr`, return the other list as is. The function should handle duplicate values correctly (e.g., merging `1->2->4` and `1->3->4` should produce `1->1->2->3->4->4`). The lists may be of different lengths. Provide a self-contained implementation without using the `Solution` class or any external libraries beyond standard C++ headers. For testing, you may assume a `ListNode` struct defined as `struct ListNode { int val; ListNode* next; ListNode(int x) : val(x), next(nullptr) {} };`.
*/
#include <cstddef>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Merges two sorted linked lists into one sorted linked list by reusing nodes.
// Returns the head of the merged list. If either list is empty, returns the other.
ListNode* mergeTwoSortedLists(ListNode* l1, ListNode* l2) {
    if (l1 == nullptr) return l2;
    if (l2 == nullptr) return l1;

    ListNode* head = nullptr;
    ListNode* left = nullptr;
    ListNode* right = nullptr;

    // Choose the head of the merged list as the smaller first node.
    if (l1->val < l2->val) {
        head = l1;
        left = l1->next;
        right = l2;
    } else {
        head = l2;
        left = l1;
        right = l2->next;
    }

    ListNode* tail = head;

    // Merge remaining nodes while both lists have nodes.
    while (left != nullptr && right != nullptr) {
        if (left->val < right->val) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }

    // Attach the remaining nodes of the non-empty list.
    if (left != nullptr) {
        tail->next = left;
    } else {
        tail->next = right;
    }

    return head;
}
#include <cassert>

// Helper to build a list from an initializer list for testing.
ListNode* buildList(std::initializer_list<int> values) {
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    for (int v : values) {
        ListNode* node = new ListNode(v);
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

// Helper to check if a list matches an expected sequence.
bool listEquals(ListNode* head, std::initializer_list<int> expected) {
    ListNode* cur = head;
    for (int v : expected) {
        if (cur == nullptr || cur->val != v) return false;
        cur = cur->next;
    }
    return cur == nullptr;
}

// Helper to free memory of a list.
void freeList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: Both lists empty
    assert(mergeTwoSortedLists(nullptr, nullptr) == nullptr);

    // Test 2: One list empty, the other non-empty
    ListNode* a1 = buildList({1, 3, 5});
    ListNode* result1 = mergeTwoSortedLists(a1, nullptr);
    assert(listEquals(result1, {1, 3, 5}));
    freeList(result1);

    // Test 3: Two equal-length lists with no duplicates
    ListNode* a2 = buildList({1, 2, 4});
    ListNode* b2 = buildList({1, 3, 4});
    ListNode* result2 = mergeTwoSortedLists(a2, b2);
    assert(listEquals(result2, {1, 1, 2, 3, 4, 4}));
    freeList(result2);

    // Test 4: Different lengths, one list longer
    ListNode* a3 = buildList({5});
    ListNode* b3 = buildList({1, 2, 3, 6});
    ListNode* result3 = mergeTwoSortedLists(a3, b3);
    assert(listEquals(result3, {1, 2, 3, 5, 6}));
    freeList(result3);

    // Test 5: All duplicates
    ListNode* a4 = buildList({2, 2, 2});
    ListNode* b4 = buildList({2, 2});
    ListNode* result4 = mergeTwoSortedLists(a4, b4);
    assert(listEquals(result4, {2, 2, 2, 2, 2}));
    freeList(result4);

    // Test 6: One list completely smaller than the other
    ListNode* a5 = buildList({-3, 0, 1});
    ListNode* b5 = buildList({10, 20});
    ListNode* result5 = mergeTwoSortedLists(a5, b5);
    assert(listEquals(result5, {-3, 0, 1, 10, 20}));
    freeList(result5);

    // Test 7: Negative numbers and zeros
    ListNode* a6 = buildList({-5, -1});
    ListNode* b6 = buildList({-10, -2, 0});
    ListNode* result6 = mergeTwoSortedLists(a6, b6);
    assert(listEquals(result6, {-10, -5, -2, -1, 0}));
    freeList(result6);

    return 0;
}
// The algorithm uses a two-pointer merge approach similar to merging two sorted arrays. We first handle the base cases: if either head is `nullptr`, return the other. To decide the merged head, we compare the first values of both lists: the smaller one becomes the head, and we set up two pointers `left` and `right` to the remaining nodes of the list that supplied the head and the other list, respectively. We also keep a `tail` pointer that tracks the last node of the merged list so far. In a loop, while both `left` and `right` are non-null, we compare their values; we append the smaller node to the tail, advance the corresponding pointer, and move the tail forward. When one list runs out, we attach the remaining nodes of the other list to the tail. This reuses existing nodes, so no new memory is allocated. Edge cases include one list being empty, one list having only one node, duplicate values (the `if` using `<` ensures stability if desired; with duplicates, it appends from `left` first, which is fine), and lists of very different lengths. Time complexity is O(n + m) where n and m are the lengths of the two lists, because each node is visited exactly once. Space complexity is O(1) auxiliary, since we only use a few pointers.
