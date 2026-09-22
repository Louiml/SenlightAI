Write a C++ function that takes a singly linked list whose nodes contain integers, where the list begins with a node storing 0 and contains at least one additional 0 between groups of non-zero integers (and ends with a 0), and returns a new linked list where each group of consecutive non-zero integers between two zeros is replaced by a single node containing the sum of that group. The returned list should preserve the order of the groups and contain no zero values. The input list must not be modified. You may assume the input list is well-formed as described. The function should be a free function (not a class method) named `mergeZeros`, returning a pointer to the new head. Use the standard `ListNode` definition with `int val` and `ListNode* next` (constructors as given). Handle edge cases such as empty non-zero groups (i.e., consecutive zeros) by producing a node with sum 0 (since the group is empty). The input list is non-empty and contains at least the initial 0.
#include <cassert>

// Helper to create a list from a vector-like initializer list.
ListNode* createList(std::initializer_list<int> values) {
    ListNode* dummy = new ListNode(-1);
    ListNode* tail = dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    ListNode* head = dummy->next;
    delete dummy;
    return head;
}

// Helper to compare two lists.
bool listsEqual(ListNode* a, ListNode* b) {
    while (a != nullptr && b != nullptr) {
        if (a->val != b->val) return false;
        a = a->next;
        b = b->next;
    }
    return a == nullptr && b == nullptr;
}

// Helper to free a list.
void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Example 1: 0 -> 3 -> 1 -> 0 -> 4 -> 5 -> 2 -> 0
    ListNode* l1 = createList({0, 3, 1, 0, 4, 5, 2, 0});
    ListNode* r1 = mergeZeros(l1);
    ListNode* e1 = createList({4, 11});
    assert(listsEqual(r1, e1));
    freeList(l1); freeList(r1); freeList(e1);

    // Example 2: 0 -> 0 -> 0 (empty groups)
    ListNode* l2 = createList({0, 0, 0});
    ListNode* r2 = mergeZeros(l2);
    ListNode* e2 = createList({0, 0}); // two empty groups between three zeros
    assert(listsEqual(r2, e2));
    freeList(l2); freeList(r2); freeList(e2);

    // Example 3: 0 -> 7 -> 0 -> -2 -> 3 -> 0 -> 0 -> 5 -> 0
    ListNode* l3 = createList({0, 7, 0, -2, 3, 0, 0, 5, 0});
    ListNode* r3 = mergeZeros(l3); // sums: 7, 1, 0, 5
    ListNode* e3 = createList({7, 1, 0, 5});
    assert(listsEqual(r3, e3));
    freeList(l3); freeList(r3); freeList(e3);

    // Example 4: Single pair of zeros: 0 -> 0
    ListNode* l4 = createList({0, 0});
    ListNode* r4 = mergeZeros(l4);
    ListNode* e4 = createList({0}); // one empty group
    assert(listsEqual(r4, e4));
    freeList(l4); freeList(r4); freeList(e4);

    // Example 5: 0 -> 1 -> 2 -> 3 -> 0
    ListNode* l5 = createList({0, 1, 2, 3, 0});
    ListNode* r5 = mergeZeros(l5);
    ListNode* e5 = createList({6});
    assert(listsEqual(r5, e5));
    freeList(l5); freeList(r5); freeList(e5);

    return 0;
}
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Merges groups of non-zero nodes between zeros into their sum.
ListNode* mergeZeros(const ListNode* head) {
    ListNode* dummy = new ListNode(-1);   // sentinel for result list
    ListNode* tail = dummy;
    const ListNode* current = head->next; // skip initial 0

    while (current != nullptr) {
        int sum = 0;
        while (current != nullptr && current->val != 0) {
            sum += current->val;
            current = current->next;
        }
        // current is either nullptr (should not happen per spec) or points to a zero
        tail->next = new ListNode(sum);
        tail = tail->next;
        if (current != nullptr) {
            current = current->next; // move past the zero
        }
    }

    ListNode* result = dummy->next;
    delete dummy;
    return result;
}
// The main idea is to iterate through the input list starting from the node after the head (since the head is guaranteed to be 0). We traverse the list and accumulate sums of consecutive non-zero values until we hit a zero. When we hit a zero, that marks the end of the current group; we create a new node with the accumulated sum and append it to the result list. Then we continue from the node after that zero. Because the input is guaranteed to end with a zero, the loop will always find a terminating zero. Edge cases: if there are two zeros in a row (e.g., `0, 0, ...`), the sum of the empty group is 0, which is correctly produced. We use a dummy head to simplify appending and to avoid null checks. We must not modify the original list, so we only read from it. Time complexity is O(n) where n is the number of nodes in the input list, since each node is visited exactly once. Space complexity is O(1) auxiliary, excluding the new nodes created for the result (which are O(number of groups)).
