// Write a C++ function `std::vector<int> reverseLinkedListValues(const ListNode* head)` that takes a pointer to the head of a singly-linked list (where each node stores an `int` value and a `next` pointer, and the list may be empty) and returns a `std::vector<int>` containing the values of the nodes in **reverse order** (from tail to head). The input list must not be modified, and the function should work correctly for lists of any length, including a single node and an empty list (where an empty vector is returned). Assume the `ListNode` structure is predefined exactly as in the snippet, and you may use any standard library facilities.
#include <cassert>
#include <vector>

int main() {
    // Empty list
    ListNode* empty = nullptr;
    assert(reverseLinkedListValues(empty) == std::vector<int>{});

    // Single node
    ListNode* single = new ListNode(42);
    assert(reverseLinkedListValues(single) == std::vector<int>{42});

    // Multiple nodes
    ListNode* n1 = new ListNode(1);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(3);
    n1->next = n2;
    n2->next = n3;
    assert(reverseLinkedListValues(n1) == std::vector<int>({3, 2, 1}));

    // Negative and zero values
    ListNode* m1 = new ListNode(-5);
    ListNode* m2 = new ListNode(0);
    ListNode* m3 = new ListNode(7);
    m1->next = m2;
    m2->next = m3;
    assert(reverseLinkedListValues(m1) == std::vector<int>({7, 0, -5}));

    // Larger chain
    ListNode* a = new ListNode(10);
    ListNode* b = new ListNode(20);
    ListNode* c = new ListNode(30);
    ListNode* d = new ListNode(40);
    a->next = b;
    b->next = c;
    c->next = d;
    assert(reverseLinkedListValues(a) == std::vector<int>({40, 30, 20, 10}));

    // Clean up to avoid leaks (not strictly needed for asserts)
    delete single;
    delete n1; delete n2; delete n3;
    delete m1; delete m2; delete m3;
    delete a; delete b; delete c; delete d;
    
    return 0;
}
#include <vector>
#include <algorithm>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Return the values of the singly-linked list in reverse order.
// The input list is not modified. An empty list yields an empty vector.
std::vector<int> reverseLinkedListValues(const ListNode* head) {
    std::vector<int> result;
    for (const ListNode* ptr = head; ptr != nullptr; ptr = ptr->next) {
        result.push_back(ptr->val);
    }
    std::reverse(result.begin(), result.end());
    return result;
}
// The simplest approach is to traverse the list from head to tail, pushing each node's value into a `vector<int>`; since that vector will be in forward order, we then reverse it (using `std::reverse`) to obtain the desired reverse order. This is correct because the list is singly-linked and we do not have a `prev` pointer; we must first collect all values. Edge cases: an empty list (`head == nullptr`) yields an immediately empty vector without any traversal; a single node yields a one-element vector that remains the same after reversal. The list is not modified because we only read `val` and follow `next` pointers. Time complexity is O(n) for traversal plus O(n) for the reversal, totaling O(n). Space complexity is O(n) because we store all values in the vector; the reversal is done in-place on that vector, so no extra auxiliary space beyond the output is used.
