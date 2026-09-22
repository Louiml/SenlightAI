Write a C++ function named `compare_linked_lists` that takes two singly linked lists (represented by their head pointers, where each node contains an integer value and a pointer to the next node) and returns a boolean indicating whether the two lists are identical in both length and element values at each position. The function should stop early and return `false` as soon as a mismatch is found, and it must handle the case where one list is longer than the other (returning `false` in that case). You may assume the lists are non-cyclic. The function signature should be `bool compare_linked_lists(const Node* head1, const Node* head2);` where `Node` is a struct with `int val` and `Node* next`. Provide only the function implementation (no main), and include the necessary `#include` directives and a `Node` definition.

// The solution traverses both lists simultaneously using two pointers, `curr1` and `curr2`, initialized to the heads. At each step, we first check if either pointer is null while the other is not—this would indicate different lengths, so we return `false`. If both are non-null, we compare their values; if they differ, return `false`. Otherwise, advance both pointers. After the loop, if both pointers are null, the lists have the same length and all matched, so return `true`. Edge cases: both lists empty (returns `true`), one empty and one non-empty (returns `false`), lists with same values but different lengths (handled by the null checks). Time complexity is O(min(n, m)) for the mismatch case and O(n) for equal-length lists, where n and m are the sizes. Space complexity is O(1) since only pointers are used.

#include <cstddef>

struct Node {
    int val;
    Node* next;
    explicit Node(int v) : val(v), next(nullptr) {}
};

bool compare_linked_lists(const Node* head1, const Node* head2) {
    const Node* curr1 = head1;
    const Node* curr2 = head2;

    while (curr1 != nullptr && curr2 != nullptr) {
        if (curr1->val != curr2->val) {
            return false;
        }
        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    // If both are null, lists are equal in length and all values matched.
    // If one is null and the other isn't, lengths differ.
    return curr1 == nullptr && curr2 == nullptr;
}

#include <cassert>

int main() {
    // Test 1: Both empty lists are equal.
    assert(compare_linked_lists(nullptr, nullptr) == true);

    // Test 2: One empty, one non-empty.
    Node a(1);
    assert(compare_linked_lists(&a, nullptr) == false);
    assert(compare_linked_lists(nullptr, &a) == false);

    // Test 3: Single-node identical lists.
    Node b(5);
    assert(compare_linked_lists(&a, &a) == true);
    assert(compare_linked_lists(&a, &b) == false);

    // Test 4: Multi-node identical lists.
    Node c1(1), c2(2), c3(3);
    c1.next = &c2; c2.next = &c3;
    Node d1(1), d2(2), d3(3);
    d1.next = &d2; d2.next = &d3;
    assert(compare_linked_lists(&c1, &d1) == true);

    // Test 5: Same values but different lengths (extra node at end).
    Node e1(1), e2(2), e3(3), e4(4);
    e1.next = &e2; e2.next = &e3; e3.next = &e4;
    assert(compare_linked_lists(&c1, &e1) == false);

    // Test 6: Same length but one value differs.
    Node f1(1), f2(9), f3(3);
    f1.next = &f2; f2.next = &f3;
    assert(compare_linked_lists(&c1, &f1) == false);

    // Test 7: Lists with duplicate values but identical.
    Node g1(7), g2(7), g3(7);
    g1.next = &g2; g2.next = &g3;
    Node h1(7), h2(7), h3(7);
    h1.next = &h2; h2.next = &h3;
    assert(compare_linked_lists(&g1, &h1) == true);
}
