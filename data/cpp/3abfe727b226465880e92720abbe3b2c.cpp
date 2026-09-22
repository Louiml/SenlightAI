/*
Write a C++ function `bool hasCycleAndRemove(Node* head)` that detects whether a singly linked list contains a cycle using Floyd’s cycle-detection algorithm (tortoise and hare). If a cycle exists, the function must also remove it by setting the `next` pointer of the node just before the start of the cycle to `NULL`, and return `true`. If no cycle exists, return `false` and leave the list unchanged. The function should handle an empty list, a list with a single node (no self-cycle unless explicitly created), and lists where the cycle starts at the head node. You may assume the `Node` struct is provided as shown in the snippet, with `int data` and `Node* next`. The function must not use extra data structures (like maps) for detection, and must not modify the list when no cycle is present. After removing the cycle, the list must be a valid acyclic singly linked list from `head` with no dangling pointers.
*/

#include <cstddef>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Detects and removes a cycle in a singly linked list using Floyd's algorithm.
// Returns true if a cycle was found and removed; otherwise false.
bool hasCycleAndRemove(Node* head) {
    if (head == nullptr) {
        return false;
    }

    // Phase 1: Detect cycle using Floyd's tortoise and hare
    Node* slow = head;
    Node* fast = head;
    bool hasCycle = false;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            hasCycle = true;
            break;
        }
    }

    if (!hasCycle) {
        return false;
    }

    // Phase 2: Find the start of the cycle
    slow = head;
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }
    Node* startOfCycle = slow;

    // Phase 3: Find the last node of the cycle and break the link
    Node* temp = startOfCycle;
    while (temp->next != startOfCycle) {
        temp = temp->next;
    }
    temp->next = nullptr;

    return true;
}

#include <cassert>

int main() {
    // Test 1: Empty list
    assert(hasCycleAndRemove(nullptr) == false);

    // Test 2: Single node, no cycle
    Node* n1 = new Node(1);
    assert(hasCycleAndRemove(n1) == false);
    assert(n1->next == nullptr);
    delete n1;

    // Test 3: Single node with self-cycle
    Node* n2 = new Node(2);
    n2->next = n2;
    assert(hasCycleAndRemove(n2) == true);
    assert(n2->next == nullptr);
    delete n2;

    // Test 4: List with cycle starting at head
    Node* a = new Node(1);
    Node* b = new Node(2);
    Node* c = new Node(3);
    a->next = b; b->next = c; c->next = a; // cycle starts at a
    assert(hasCycleAndRemove(a) == true);
    // After removal, list should be 1 -> 2 -> 3 -> nullptr
    assert(a->next == b && b->next == c && c->next == nullptr);
    delete a; delete b; delete c;

    // Test 5: List with cycle starting in the middle
    Node* x = new Node(10);
    Node* y = new Node(20);
    Node* z = new Node(30);
    Node* w = new Node(40);
    x->next = y; y->next = z; z->next = w; w->next = y; // cycle starts at y
    assert(hasCycleAndRemove(x) == true);
    // After removal: 10 -> 20 -> 30 -> 40 -> nullptr
    assert(x->next == y && y->next == z && z->next == w && w->next == nullptr);
    delete x; delete y; delete z; delete w;

    // Test 6: No cycle in longer list
    Node* p = new Node(1);
    Node* q = new Node(2);
    Node* r = new Node(3);
    p->next = q; q->next = r; // acyclic
    assert(hasCycleAndRemove(p) == false);
    assert(p->next == q && q->next == r && r->next == nullptr);
    delete p; delete q; delete r;

    return 0;
}

// The solution uses Floyd’s algorithm: initialize `slow` and `fast` both at `head`. Move `slow` one step and `fast` two steps per iteration. If they meet, a cycle exists. The meeting point is guaranteed to be inside the cycle. To find the start of the cycle, reset one pointer (say `slow`) to `head` and keep the other at the meeting point. Move both one step at a time; their first meeting is the start node of the cycle. To remove the cycle, traverse from the start node until the node whose `next` equals the start node; that node is the last node of the cycle. Set its `next` to `NULL`. Edge cases: empty list (`head == NULL`) returns `false`; single node with no self‑cycle returns `false` after the loop terminates; a self‑cycle (node pointing to itself) is correctly detected and removed by setting `next` to `NULL`. Time complexity is O(n) (linear in the number of nodes) because both detection and removal traverse at most a constant multiple of the nodes. Space complexity is O(1) as only a few pointers are used.
