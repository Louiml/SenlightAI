/*
Write a C++ function `swapKthFromBothEnds` that takes a singly linked list (represented by a `head` pointer to a node with `int data` and `next` pointer) and an integer `k`. The function should swap the k-th node from the beginning with the k-th node from the end. If `k` is invalid (less than or equal to 0, or greater than the list length), or if the two nodes are the same (i.e., the list length is odd and k is the exact middle), the function should leave the list unchanged. The function must modify the list in-place by adjusting pointers only—do not swap the `data` values or create new nodes. You may assume the list has at least one node and that `k` is a positive integer, but your code should handle the edge cases gracefully. The function signature should be `void swapKthFromBothEnds(Node*& head, int k)`.
*/
#include <iostream>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Swap k-th node from beginning with k-th node from end in a singly linked list.
void swapKthFromBothEnds(Node*& head, int k) {
    if (head == nullptr || k <= 0) return;

    // Compute length
    int len = 0;
    Node* temp = head;
    while (temp) {
        ++len;
        temp = temp->next;
    }

    // Invalid k or same middle node
    if (k > len || 2*k - 1 == len) return;

    // Find k-th node from beginning (x) and its predecessor
    Node* x_prev = nullptr;
    Node* x = head;
    for (int i = 1; i < k && x; ++i) {
        x_prev = x;
        x = x->next;
    }

    // Find k-th node from end (y) and its predecessor
    Node* y_prev = nullptr;
    Node* y = head;
    int steps = len - k;  // number of steps from head to reach y
    for (int i = 0; i < steps; ++i) {
        y_prev = y;
        y = y->next;
    }

    // Ensure we found both nodes (safety; should not happen due to validity check)
    if (x == nullptr || y == nullptr) return;

    // Update predecessors
    if (x_prev) x_prev->next = y;
    else head = y;  // x was head

    if (y_prev) y_prev->next = x;
    else head = x;  // y was head

    // Swap next pointers
    Node* temp_next = x->next;
    x->next = y->next;
    y->next = temp_next;
}
#include <cassert>

// Helper to build list from initializer list
Node* buildList(std::initializer_list<int> vals) {
    Node* head = nullptr;
    Node** curr = &head;
    for (int v : vals) {
        *curr = new Node(v);
        curr = &((*curr)->next);
    }
    return head;
}

// Helper to convert list to vector for comparison
std::vector<int> listToVector(Node* head) {
    std::vector<int> vec;
    while (head) {
        vec.push_back(head->data);
        head = head->next;
    }
    return vec;
}

// Helper to delete list
void deleteList(Node* head) {
    while (head) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    // Test 1: normal swap
    Node* head1 = buildList({1, 2, 3, 4, 5});
    swapKthFromBothEnds(head1, 2);
    assert((listToVector(head1) == std::vector<int>{1, 4, 3, 2, 5}));
    deleteList(head1);

    // Test 2: swap head and tail (k=1)
    Node* head2 = buildList({10, 20, 30});
    swapKthFromBothEnds(head2, 1);
    assert((listToVector(head2) == std::vector<int>{30, 20, 10}));
    deleteList(head2);

    // Test 3: k greater than length -> unchanged
    Node* head3 = buildList({1, 2, 3});
    swapKthFromBothEnds(head3, 5);
    assert((listToVector(head3) == std::vector<int>{1, 2, 3}));
    deleteList(head3);

    // Test 4: k <= 0 -> unchanged
    Node* head4 = buildList({1, 2, 3});
    swapKthFromBothEnds(head4, 0);
    assert((listToVector(head4) == std::vector<int>{1, 2, 3}));
    deleteList(head4);

    // Test 5: middle element in odd length -> unchanged
    Node* head5 = buildList({1, 2, 3, 4, 5});
    swapKthFromBothEnds(head5, 3);
    assert((listToVector(head5) == std::vector<int>{1, 2, 3, 4, 5}));
    deleteList(head5);

    // Test 6: adjacent nodes (k=2 in length 3) -> swap positions 2 and 2 (same? no, middle is same so unchanged)
    Node* head6 = buildList({1, 2, 3});
    swapKthFromBothEnds(head6, 2);
    assert((listToVector(head6) == std::vector<int>{1, 2, 3}));
    deleteList(head6);

    // Test 7: even length, k=2 in length 4 -> swap second and third
    Node* head7 = buildList({1, 2, 3, 4});
    swapKthFromBothEnds(head7, 2);
    assert((listToVector(head7) == std::vector<int>{1, 3, 2, 4}));
    deleteList(head7);

    // Test 8: single node, k=1 -> unchanged
    Node* head8 = buildList({42});
    swapKthFromBothEnds(head8, 1);
    assert((listToVector(head8) == std::vector<int>{42}));
    deleteList(head8);

    return 0;
}
// The main algorithm involves two passes: first compute the length `n` of the list. If `k <= 0` or `k > n`, or if `2*k - 1 == n` (meaning the k-th from start and k-th from end are the same middle node), return without changes. Otherwise, we need to locate:
// - The k-th node from start (`x`) and its predecessor (`x_prev`)
// - The (n-k+1)-th node from start (which is k-th from end) (`y`) and its predecessor (`y_prev`)
//
// We can do this in a single traversal by using two pointers. Start `x_prev = NULL`, `x = head`, and advance `x` exactly `k-1` steps to reach the k-th node (keeping track of `x_prev`). Then set `y_prev = NULL`, `y = head`. Advance `y` (and `y_prev`) until `y` reaches the (n-k+1)-th node. Alternatively, a simpler approach: first traverse to find `x` and `x_prev` by moving k-1 steps, then reset and traverse n-k steps to find `y` and `y_prev`. Because the list is singly linked, we must update pointers carefully: if `x_prev` is not null, set `x_prev->next = y`; if `y_prev` is not null, set `y_prev->next = x`. Then swap their `next` pointers: save `temp = x->next`, set `x->next = y->next`, set `y->next = temp`. Special care is needed if `x` and `y` are adjacent (i.e., `x_prev == y` or `y_prev == x`), but the swapping logic above works generally if we perform the pointer updates in the correct order. The time complexity is O(n) due to the two traversals; space complexity is O(1) as we only use a constant number of pointers.
