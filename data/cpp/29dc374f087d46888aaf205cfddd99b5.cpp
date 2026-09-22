Write a C++ function that takes two singly linked lists, each sorted in non-decreasing order, and returns a pointer to the first node from the first list whose data value appears at the same position in the second list (i.e., the first index i where both lists have the same value). If no such index exists or either list is empty, return `nullptr`. The lists may have different lengths; positions are compared from the head (index 0), and once one list ends, no further comparisons are possible. The function must not modify the lists, must handle duplicate values correctly (e.g., if both lists have 5 at index 2 and also at index 5, the first occurrence at index 2 is returned), and must work with lists containing negative or zero values. The solution should be standalone, using a `node` class with `int data` and `node* next` members, and must be consistent with the provided snippet's node definition.

// The algorithm traverses both linked lists simultaneously, moving one step forward in each list on every iteration, exactly as the given `intersection` function does, but corrected to use `&&` (logical AND) instead of `or` and to check for equality at each step before advancing. The key improvement over the original snippet is that the original used `while(a != NULL or b != NULL)` which continues even if one list becomes null (causing a potential null dereference when accessing `a->data` or `b->data`) and also advanced both pointers incorrectly. The correct approach: while both `a` and `b` are non-null, compare `a->data` and `b->data`. If equal, return `a` immediately. Otherwise, advance both pointers to the next nodes. If the loop ends without finding a match, return `nullptr`. Edge cases: (1) either list empty → return `nullptr`; (2) lists of different lengths → stop at the end of the shorter list; (3) no matching values at any aligned position → return `nullptr`; (4) the matching node could be anywhere, including the head; (5) duplicates are handled naturally because we return on the first equality encountered. Time complexity is O(min(n, m)) where n and m are the lengths of the lists, and space complexity is O(1) beyond the input lists.

#include <cstddef>

// Node structure for singly linked list
struct node {
    int data;
    node* next;
    node(int val) : data(val), next(nullptr) {}
};

// Returns pointer to first node in list 'a' whose data matches the data
// of the node at the same position in list 'b'. Returns nullptr if no match.
// Both lists are assumed to be sorted in non-decreasing order, but the
// algorithm works for unsorted lists as well since it only checks alignment.
node* firstMatchingAtSamePosition(const node* a, const node* b) {
    const node* pa = a;
    const node* pb = b;

    while (pa != nullptr && pb != nullptr) {
        if (pa->data == pb->data) {
            // Return a non-const pointer (cast away const since caller may modify)
            return const_cast<node*>(pa);
        }
        pa = pa->next;
        pb = pb->next;
    }
    return nullptr;
}

#include <cassert>

int main() {
    // Helper to build list from initializer list for testing
    node* buildList(std::initializer_list<int> vals) {
        node* head = nullptr;
        node** tail = &head;
        for (int v : vals) {
            *tail = new node(v);
            tail = &((*tail)->next);
        }
        return head;
    }

    // Helper to free list memory
    void freeList(node* head) {
        while (head) {
            node* temp = head->next;
            delete head;
            head = temp;
        }
    }

    // Test 1: Matching at same position (index 2)
    node* a1 = buildList({10, 20, 30, 40});
    node* b1 = buildList({15, 25, 30, 45});
    node* result1 = firstMatchingAtSamePosition(a1, b1);
    assert(result1 != nullptr && result1->data == 30);
    freeList(a1); freeList(b1);

    // Test 2: No match
    node* a2 = buildList({1, 2, 3});
    node* b2 = buildList({4, 5, 6});
    assert(firstMatchingAtSamePosition(a2, b2) == nullptr);
    freeList(a2); freeList(b2);

    // Test 3: Head match
    node* a3 = buildList({7, 8});
    node* b3 = buildList({7, 9});
    node* result3 = firstMatchingAtSamePosition(a3, b3);
    assert(result3 != nullptr && result3->data == 7);
    freeList(a3); freeList(b3);

    // Test 4: One list empty
    node* a4 = buildList({});
    node* b4 = buildList({1, 2});
    assert(firstMatchingAtSamePosition(a4, b4) == nullptr);
    freeList(a4); freeList(b4);

    // Test 5: Both empty
    node* a5 = buildList({});
    node* b5 = buildList({});
    assert(firstMatchingAtSamePosition(a5, b5) == nullptr);
    freeList(a5); freeList(b5);

    // Test 6: Different lengths, shorter list ends first
    node* a6 = buildList({1, 2, 3, 4, 5});
    node* b6 = buildList({1, 2, 3});
    node* result6 = firstMatchingAtSamePosition(a6, b6);
    assert(result6 != nullptr && result6->data == 1);
    freeList(a6); freeList(b6);

    // Test 7: Duplicate values, first occurrence returned
    node* a7 = buildList({5, 5, 5});
    node* b7 = buildList({1, 5, 5});
    node* result7 = firstMatchingAtSamePosition(a7, b7);
    assert(result7 != nullptr && result7->data == 5);
    freeList(a7); freeList(b7);

    // Test 8: Negative values
    node* a8 = buildList({-10, -5, 0});
    node* b8 = buildList({-10, -5, 0});
    node* result8 = firstMatchingAtSamePosition(a8, b8);
    assert(result8 != nullptr && result8->data == -10);
    freeList(a8); freeList(b8);

    return 0;
}
