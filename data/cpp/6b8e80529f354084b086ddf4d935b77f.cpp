Write a C++ function `Node* deepCopyList(Node* head)` that takes the head of a singly linked list where each node also contains a `random` pointer (which may point to any node in the list or be `nullptr`), and returns a deep copy of the entire list. The deep copy must have completely independent nodes, but the `random` pointers in the copy must mirror the structure of the original (i.e., if a node in the original points via `random` to a node that is at some relative position, the corresponding copied node's `random` must point to the corresponding copied node). Do not modify the original list. The input list may be empty. Define the `Node` struct with `int val; Node* next; Node* random;` and a constructor that initializes `val` and sets both pointers to `nullptr`.
// The classic efficient approach is a three‑pass in‑place interleaving technique. First, traverse the original list and for each node, create a new copy node, insert it immediately after the original node, and link the original next to the copy. This doubles the list structure so that each original node is followed by its copy. Second, traverse the list again, but this time moving two steps at a time. For each original node, set the `random` pointer of its copy (which is `orig->next`) to the copy of the original's random target (`orig->random->next`), if `orig->random` exists. Third, separate the two interleaved lists by restoring the original list's `next` pointers and linking the copied nodes together, returning the head of the newly formed copied list. Edge cases: an empty list returns `nullptr`; a node with `random == nullptr` leaves the copy's random as `nullptr`; the list may have nodes with `random` pointing to `nullptr` or to itself. The algorithm runs in O(n) time with O(1) extra space (aside from the new nodes themselves), and the original list is restored exactly as it was.
#include <cstddef>

struct Node {
    int val;
    Node* next;
    Node* random;
    Node(int _val) : val(_val), next(nullptr), random(nullptr) {}
};

// Deep copy a linked list with random pointers. Original list is unchanged.
Node* deepCopyList(Node* head) {
    if (!head) return nullptr;

    // Pass 1: interleave copies after originals
    Node* curr = head;
    while (curr) {
        Node* copy = new Node(curr->val);
        copy->next = curr->next;
        curr->next = copy;
        curr = copy->next;
    }

    // Pass 2: set random pointers on copies
    curr = head;
    while (curr) {
        if (curr->random) {
            curr->next->random = curr->random->next;
        }
        // else copy->random remains nullptr (already set)
        curr = curr->next->next;
    }

    // Pass 3: split the interleaved list
    Node* newHead = head->next;
    curr = head;
    while (curr) {
        Node* copy = curr->next;
        curr->next = copy->next;          // restore original next
        if (copy->next) {
            copy->next = copy->next->next; // point copy to next copy
        }
        curr = curr->next;                // move along original list
    }

    return newHead;
}
#include <cassert>

int main() {
    // Test 1: empty list
    assert(deepCopyList(nullptr) == nullptr);

    // Test 2: single node with random = nullptr
    Node a(1);
    Node* single = deepCopyList(&a);
    assert(single != &a);
    assert(single->val == 1);
    assert(single->next == nullptr);
    assert(single->random == nullptr);
    delete single;

    // Test 3: two nodes, random points to itself
    Node n1(10), n2(20);
    n1.next = &n2;
    n1.random = &n1; // self
    n2.random = &n1;
    Node* copy = deepCopyList(&n1);
    assert(copy != &n1);
    assert(copy->val == 10);
    assert(copy->next != nullptr);
    assert(copy->next->val == 20);
    assert(copy->random == copy);                // self in copy
    assert(copy->next->random == copy);          // points to copy of n1
    // Verify original unchanged
    assert(n1.next == &n2);
    assert(n1.random == &n1);
    assert(n2.random == &n1);
    delete copy->next;
    delete copy;

    // Test 4: three nodes with random pointers to null and middle
    Node x(1), y(2), z(3);
    x.next = &y; y.next = &z;
    x.random = nullptr;
    y.random = &x;
    z.random = &y;
    Node* c = deepCopyList(&x);
    assert(c->val == 1 && c->random == nullptr);
    assert(c->next->val == 2 && c->next->random == c);
    assert(c->next->next->val == 3 && c->next->next->random == c->next);
    assert(c->next->next->next == nullptr);
    // Original unchanged
    assert(x.random == nullptr && y.random == &x && z.random == &y);
    delete c->next->next; delete c->next; delete c;

    // Test 5: long list with mixed random pointers
    Node p(1), q(2), r(3), s(4);
    p.next = &q; q.next = &r; r.next = &s;
    p.random = &s;
    q.random = &p;
    r.random = nullptr;
    s.random = &q;
    Node* d = deepCopyList(&p);
    assert(d->random == d->next->next->next);
    assert(d->next->random == d);
    assert(d->next->next->random == nullptr);
    assert(d->next->next->next->random == d->next);
    assert(d->next->next->next->next == nullptr);
    delete d->next->next->next; delete d->next->next; delete d->next; delete d;

    return 0;
}
