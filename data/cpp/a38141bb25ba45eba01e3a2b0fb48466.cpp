Write a C++ function `bool findTriplet(const ListNode* a, const ListNode* b, const ListNode* c, int target)` that takes three singly-linked lists (each containing positive integers) and a target integer. The function must determine whether there exists exactly one element from each list such that their sum equals the target. If such a triplet exists, the function should print the three values (separated by spaces) to standard output and return `true`; otherwise, it prints nothing and returns `false`. The lists may be of different lengths, may contain duplicate values, and are guaranteed to be non-empty. You must implement the function using only pointer traversal (no arrays or container conversion) and without modifying the original lists. Additionally, provide a helper `ListNode* createLinkedList(const int arr[], int n)` that builds a linked list from an array, and ensure your solution handles any ordering of values (not necessarily sorted) and works correctly when multiple triplets match (print only the first found in the nested traversal order: first by list `a`, then by `b`, then by `c`).
The core algorithm is a simple triple-nested loop: for each node in list `a`, iterate through every node in list `b`, and for each such pair, iterate through every node in list `c`, computing the sum of the three values. If the sum equals the target, print the three values and return `true` immediately. The traversal order naturally matches the required “first found” behavior. Edge cases: lists are non-empty, so no null-dereference risk; duplicates are handled naturally because each occurrence is visited; values are positive integers, so sums are always positive, but no special handling is needed. Time complexity is \(O(n \cdot m \cdot k)\) where \(n\), \(m\), \(k\) are the lengths of the three lists; space complexity is \(O(1)\) beyond the input lists themselves (using only a few pointers). Since the lists are provided as `const` pointers, we must declare parameters as `const ListNode*` to ensure we do not modify them, and we traverse them via local mutable pointers.
#include <iostream>

// Definition for a singly-linked list node.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Helper to create a linked list from an array (used by tests).
ListNode* createLinkedList(const int arr[], int n) {
    ListNode dummy(0);
    ListNode* current = &dummy;
    for (int i = 0; i < n; ++i) {
        current->next = new ListNode(arr[i]);
        current = current->next;
    }
    return dummy.next;
}

// Determine if there exists a triplet (one element from each list) summing to target.
// Prints the first found triplet as "a b c" and returns true; otherwise returns false.
bool findTriplet(const ListNode* a, const ListNode* b, const ListNode* c, int target) {
    for (const ListNode* pa = a; pa != nullptr; pa = pa->next) {
        for (const ListNode* pb = b; pb != nullptr; pb = pb->next) {
            for (const ListNode* pc = c; pc != nullptr; pc = pc->next) {
                if (pa->val + pb->val + pc->val == target) {
                    std::cout << pa->val << " " << pb->val << " " << pc->val << std::endl;
                    return true;
                }
            }
        }
    }
    return false;
}
#include <cassert>

int main() {
    // Test 1: Basic match
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5};
    int arr3[] = {6, 7};
    ListNode* a = createLinkedList(arr1, 3);
    ListNode* b = createLinkedList(arr2, 2);
    ListNode* c = createLinkedList(arr3, 2);
    // 1+4+6=11, 1+4+7=12, 1+5+6=12, 1+5+7=13, 2+4+6=12, ... 3+5+7=15
    assert(findTriplet(a, b, c, 11) == true);   // prints "1 4 6"
    // Reset output? Not needed for assert, but test another non-match
    assert(findTriplet(a, b, c, 99) == false);

    // Test 2: Duplicate values
    int arr4[] = {2, 2, 2};
    int arr5[] = {3, 3};
    int arr6[] = {5};
    ListNode* d = createLinkedList(arr4, 3);
    ListNode* e = createLinkedList(arr5, 2);
    ListNode* f = createLinkedList(arr6, 1);
    assert(findTriplet(d, e, f, 10) == true);   // 2+3+5=10
    assert(findTriplet(d, e, f, 11) == false);

    // Test 3: Single-element lists
    int arr7[] = {10};
    int arr8[] = {20};
    int arr9[] = {30};
    ListNode* g = createLinkedList(arr7, 1);
    ListNode* h = createLinkedList(arr8, 1);
    ListNode* i = createLinkedList(arr9, 1);
    assert(findTriplet(g, h, i, 60) == true);
    assert(findTriplet(g, h, i, 61) == false);

    // Test 4: Larger lists, target at end of traversal
    int arr10[] = {1, 2, 3, 4, 5};
    int arr11[] = {10, 20, 30};
    int arr12[] = {100, 200};
    ListNode* j = createLinkedList(arr10, 5);
    ListNode* k = createLinkedList(arr11, 3);
    ListNode* l = createLinkedList(arr12, 2);
    // 5+30+200=235
    assert(findTriplet(j, k, l, 235) == true);
    assert(findTriplet(j, k, l, 236) == false);

    // Test 5: All same values, target requires multiple same numbers
    int arr13[] = {7, 7};
    int arr14[] = {7};
    int arr15[] = {7, 7};
    ListNode* m = createLinkedList(arr13, 2);
    ListNode* n = createLinkedList(arr14, 1);
    ListNode* o = createLinkedList(arr15, 2);
    assert(findTriplet(m, n, o, 21) == true);
    assert(findTriplet(m, n, o, 22) == false);
}
