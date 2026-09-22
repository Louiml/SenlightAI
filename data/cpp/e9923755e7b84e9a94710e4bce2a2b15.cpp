// Write a C++ function that takes a singly linked list of integers (represented by a `node` struct with `data` and `next` members) and reorders its nodes so that all nodes at odd positions (1st, 3rd, 5th, ...) appear first in their original relative order, followed by all nodes at even positions (2nd, 4th, 6th, ...) in their original relative order. The function must modify the list in place (no new nodes may be allocated) and must handle lists with 0, 1, or 2 nodes correctly. For example, a list `1 -> 2 -> 3 -> 4 -> 5` becomes `1 -> 3 -> 5 -> 2 -> 4`.
// The algorithm splits the list into two interleaved sequences: odd-indexed nodes and even-indexed nodes. Maintain three pointers: `odd` pointing to the current odd node (initially `head`), `even` pointing to the current even node (initially `head->next`), and `evenStart` to remember the head of the even sequence (needed to concatenate at the end). Iterate while `even` and `even->next` exist: first link `odd->next` to `even->next` (the next odd), advance `odd`; then if the new `odd` has a next, link `even->next` to that next (the next even), and advance `even`. After the loop, set `even->next = NULL` (to terminate the even chain) and `odd->next = evenStart` (to concatenate). Edge cases: empty list (return immediately), one node (even is NULL, so just return), and two nodes (loop not entered; then `even->next` already NULL, set `odd->next = evenStart` which restores original order, effectively no change). Complexity: O(n) time and O(1) extra space.
#include <cstddef>

struct node {
    int data;
    node* next;
    explicit node(int val) : data(val), next(nullptr) {}
};

// Reorder the list so odd-position nodes appear before even-position nodes.
void evenAfterOdd(node* head) {
    if (head == nullptr || head->next == nullptr) {
        return; // empty or single-node list
    }

    node* odd = head;
    node* even = head->next;
    node* evenStart = even;

    while (even != nullptr && even->next != nullptr) {
        odd->next = even->next;
        odd = odd->next;

        even->next = odd->next;
        even = even->next;
    }

    even->next = nullptr;
    odd->next = evenStart;
}
#include <cassert>
#include <vector>

// Helper to create a list from a vector, returns head.
node* makeList(const std::vector<int>& vals) {
    node* head = nullptr;
    node* tail = nullptr;
    for (int v : vals) {
        node* n = new node(v);
        if (head == nullptr) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }
    return head;
}

// Helper to convert list to vector for comparison.
std::vector<int> toVector(node* head) {
    std::vector<int> out;
    while (head) {
        out.push_back(head->data);
        head = head->next;
    }
    return out;
}

// Helper to free list memory.
void deleteList(node* head) {
    while (head) {
        node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Standard 5-element list
    node* l1 = makeList({1, 2, 3, 4, 5});
    evenAfterOdd(l1);
    assert(toVector(l1) == std::vector<int>({1, 3, 5, 2, 4}));
    deleteList(l1);

    // Empty list
    node* l2 = nullptr;
    evenAfterOdd(l2);
    assert(l2 == nullptr);

    // Single element list
    node* l3 = makeList({7});
    evenAfterOdd(l3);
    assert(toVector(l3) == std::vector<int>({7}));
    deleteList(l3);

    // Two element list (order unchanged)
    node* l4 = makeList({10, 20});
    evenAfterOdd(l4);
    assert(toVector(l4) == std::vector<int>({10, 20}));
    deleteList(l4);

    // Four elements: 1,2,3,4 -> 1,3,2,4
    node* l5 = makeList({1, 2, 3, 4});
    evenAfterOdd(l5);
    assert(toVector(l5) == std::vector<int>({1, 3, 2, 4}));
    deleteList(l5);

    // Six elements: 1,2,3,4,5,6 -> 1,3,5,2,4,6
    node* l6 = makeList({1, 2, 3, 4, 5, 6});
    evenAfterOdd(l6);
    assert(toVector(l6) == std::vector<int>({1, 3, 5, 2, 4, 6}));
    deleteList(l6);

    // Negative numbers and duplicates
    node* l7 = makeList({-1, -2, -3, -4});
    evenAfterOdd(l7);
    assert(toVector(l7) == std::vector<int>({-1, -3, -2, -4}));
    deleteList(l7);

    return 0;
}
