Write a C++ function `bool hasCycle(const ListNode* head)` that takes a pointer to the head of a singly linked list and returns `true` if the list contains a cycle (i.e., a node whose `next` pointer eventually leads back to an earlier node in the list), and `false` otherwise. The function must handle an empty list (return `false`) and a list with only one node that points to `nullptr` (return `false`). You may assume the list nodes are defined as `struct ListNode { int val; ListNode *next; ListNode(int x) : val(x), next(nullptr) {} };`. The function must not modify the list, must use the Floyd’s Tortoise and Hare algorithm (two pointers moving at different speeds), and must be const-correct by taking a pointer to const data. The solution should be in a standalone, self-contained file (no main function) and must compile with C++11 or later.

The task is to detect a cycle in a singly linked list using Floyd’s cycle-detection algorithm (also known as the "tortoise and hare" approach). Start with two pointers, `slow` and `fast`, both initially pointing to `head`. In each iteration, advance `slow` by one node and `fast` by two nodes. If the list has no cycle, `fast` will eventually reach `nullptr` (or `fast->next` becomes `nullptr`), and the loop terminates, returning `false`. If a cycle exists, the two pointers will eventually meet inside the cycle because the relative speed difference is one node per iteration. The check `if (slow == fast)` after moving both pointers catches the meeting point. Edge cases include an empty list (`head == nullptr`) and a single-node list with no cycle—both return `false` because the while condition fails immediately. The algorithm uses constant extra space (two pointers) and runs in O(n) time, where n is the number of nodes, because in the worst case (a linear list) the fast pointer traverses the list once; in a cyclic list, the meeting occurs within one cycle traversal. The function is marked `const` by accepting a `const ListNode*`, ensuring no modification of list data.

#include <cstddef>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Detect a cycle in a singly linked list using Floyd's algorithm.
bool hasCycle(const ListNode* head) {
    if (head == nullptr) {
        return false;
    }

    const ListNode* slow = head;
    const ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;          // move one step
        fast = fast->next->next;    // move two steps

        if (slow == fast) {
            return true;            // cycle detected
        }
    }

    return false;                   // reached end, no cycle
}

#include <cassert>

// ListNode is defined as above; we rely on the solution function.

int main() {
    // Test 1: empty list
    ListNode* empty = nullptr;
    assert(!hasCycle(empty));

    // Test 2: single node, no cycle
    ListNode a(1);
    assert(!hasCycle(&a));

    // Test 3: two-node list, no cycle
    ListNode b(2);
    a.next = &b;
    assert(!hasCycle(&a));

    // Test 4: two-node list with cycle (tail points to head)
    b.next = &a;
    assert(hasCycle(&a));

    // Test 5: four-node list, no cycle
    ListNode c(3), d(4);
    b.next = nullptr; // break previous cycle
    b.next = &c;
    c.next = &d;
    assert(!hasCycle(&a));

    // Test 6: four-node list with cycle at node c
    d.next = &c; // cycle: c->d->c
    assert(hasCycle(&a));

    // Test 7: three-node list with cycle at head itself
    ListNode e(5), f(6), g(7);
    e.next = &f;
    f.next = &g;
    g.next = &e; // cycle back to head
    assert(hasCycle(&e));

    // Test 8: three-node list, cycle at middle
    g.next = &f; // f->g->f
    assert(hasCycle(&e));

    // Test 9: long list no cycle (e.g., 100 nodes)
    ListNode nodes[100];
    for (int i = 0; i < 99; ++i) {
        nodes[i].next = &nodes[i+1];
    }
    nodes[99].next = nullptr;
    assert(!hasCycle(&nodes[0]));

    // Test 10: long list with cycle at node 50
    nodes[99].next = &nodes[50];
    assert(hasCycle(&nodes[0]));

    return 0;
}
