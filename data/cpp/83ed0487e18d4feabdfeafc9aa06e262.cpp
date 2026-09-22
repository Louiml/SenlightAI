// Write a C++ function `bool hasCycle(ListNode *head)` that determines whether a singly-linked list contains a cycle. The list nodes are defined as `struct ListNode { int val; ListNode *next; ListNode(int x) : val(x), next(NULL) {} };`. The function must return `true` if there exists a node in the list whose `next` pointer eventually leads back to a previously visited node (forming a loop), and `false` otherwise. The list may be empty, may consist of a single node, may be a linear list with no cycle, or may contain a cycle that starts at any position (including a self-loop). The function must not modify the list, must not use extra memory proportional to the list length, and must handle extremely long lists efficiently. All `next` pointers are either `NULL` or point to a valid `ListNode` object; you may assume no malformed pointers.
#include <cassert>

int main() {
    // Test 1: Empty list
    assert(hasCycle(nullptr) == false);

    // Test 2: Single node, no cycle
    ListNode a(1);
    assert(hasCycle(&a) == false);

    // Test 3: Two nodes, no cycle
    ListNode b(2);
    a.next = &b;
    assert(hasCycle(&a) == false);

    // Test 4: Self-loop on single node
    ListNode c(3);
    c.next = &c;
    assert(hasCycle(&c) == true);

    // Test 5: Cycle at the end (1->2->3->2)
    ListNode d(4);
    ListNode e(5);
    ListNode f(6);
    d.next = &e;
    e.next = &f;
    f.next = &e;  // creates cycle
    assert(hasCycle(&d) == true);

    // Test 6: Cycle at the beginning (1->2->3->1)
    ListNode g(7);
    ListNode h(8);
    ListNode i(9);
    g.next = &h;
    h.next = &i;
    i.next = &g;  // cycle back to start
    assert(hasCycle(&g) == true);

    // Test 7: Linear list of 4 nodes
    ListNode j(10);
    ListNode k(11);
    ListNode l(12);
    ListNode m(13);
    j.next = &k;
    k.next = &l;
    l.next = &m;
    m.next = nullptr;
    assert(hasCycle(&j) == false);

    // Test 8: Long linear list (1000 nodes) - ensure no crash
    ListNode* head = new ListNode(0);
    ListNode* curr = head;
    for (int n = 1; n < 1000; ++n) {
        curr->next = new ListNode(n);
        curr = curr->next;
    }
    assert(hasCycle(head) == false);
    // Cleanup
    curr = head;
    while (curr != nullptr) {
        ListNode* next = curr->next;
        delete curr;
        curr = next;
    }

    return 0;
}
#include <cstddef>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Returns true if the singly-linked list contains a cycle; false otherwise.
bool hasCycle(const ListNode *head) {
    if (head == nullptr) {
        return false;
    }

    const ListNode *slow = head;
    const ListNode *fast = head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return true;
        }
    }

    return false;
}
// The standard solution uses Floyd’s cycle‑detection algorithm (tortoise and hare). Two pointers, `slow` and `fast`, start at `head`. `slow` advances one node per step, `fast` advances two nodes per step. If the list has a cycle, the two pointers will eventually meet inside the cycle; if the list ends (i.e., `fast` or `fast->next` becomes `NULL`), then there is no cycle. The loop condition checks `fast != nullptr && fast->next != nullptr && slow != nullptr`. The `slow != nullptr` check is technically redundant because `fast` would hit the end first if there were no cycle, but it is included for safety. Edge cases: an empty list (`head == nullptr`) immediately returns `false` because the loop condition fails; a single node with `next == nullptr` also returns `false`; a self‑loop (`node->next == node`) is detected because after one step both pointers point to the same node. The time complexity is \(O(n)\) in the worst case for a linear list (fast traverses the whole list) and \(O(n)\) for a cyclic list (meeting occurs within one cycle traversal), where \(n\) is the number of nodes. Space complexity is \(O(1)\) since only two pointers are used.
