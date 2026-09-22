Write a C++ function `ListNode* removeSmallerNodes(ListNode* head)` that takes the head of a singly-linked list where each node contains an integer value, and returns the head of a new list formed by removing every node whose value is smaller than the value of any node that appears later in the original list. The relative order of the remaining nodes must be preserved. For example, given `5 -> 2 -> 13 -> 3 -> 8`, the output should be `13 -> 8` because 2, 3, and 5 are smaller than 13 (which appears later), and 8 is larger than 3. If the list is empty or has one node, return the head unchanged. Assume the node structure is already defined as provided. Do not modify the original list; instead, reconstruct a new list (you may reuse the existing heap-allocated nodes, but you must not depend on the original list's `next` pointers after restructuring). The function should be `noexcept`.
The key insight is that a node should be kept only if it is greater than or equal to the maximum value seen among all nodes to its right. A natural way to enforce this from left to right is to use a monotonic stack that maintains values in non-increasing order. Traverse the list once: for each current node, pop from the stack while the stack's top value is strictly less than the current node’s value, because those popped nodes are dominated by a later larger value. Then push the current node onto the stack. After the traversal, the stack contains exactly the nodes that should survive, in reverse order (the top is the last surviving node). Rebuild the list by popping from the stack and reversing the `next` pointers. Edge cases: empty list, single node, all equal values (no removal), and strictly decreasing list (all nodes survive). Time complexity is O(n) because each node is pushed and popped at most once. Space complexity is O(n) in the worst case for the stack. The original nodes are reused, but no new memory is allocated.
#include <stack>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Remove nodes that have a strictly larger value later in the list.
// Returns the head of the resulting list, preserving relative order.
ListNode* removeSmallerNodes(ListNode* head) noexcept {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    std::stack<ListNode*> st;
    ListNode* cur = head;

    while (cur != nullptr) {
        while (!st.empty() && st.top()->val < cur->val) {
            st.pop();
        }
        st.push(cur);
        cur = cur->next;
    }

    ListNode* newHead = nullptr;
    while (!st.empty()) {
        ListNode* node = st.top();
        st.pop();
        node->next = newHead;
        newHead = node;
    }

    return newHead;
}
#include <cassert>

int main() {
    // Test 1: Provided example
    ListNode* n1 = new ListNode(5);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(13);
    ListNode* n4 = new ListNode(3);
    ListNode* n5 = new ListNode(8);
    n1->next = n2; n2->next = n3; n3->next = n4; n4->next = n5;
    ListNode* result = removeSmallerNodes(n1);
    assert(result->val == 13);
    assert(result->next->val == 8);
    assert(result->next->next == nullptr);

    // Test 2: Empty list
    ListNode* empty = nullptr;
    assert(removeSmallerNodes(empty) == nullptr);

    // Test 3: Single node
    ListNode* single = new ListNode(42);
    assert(removeSmallerNodes(single) == single);
    assert(single->next == nullptr);

    // Test 4: Decreasing list – all nodes survive
    ListNode* a = new ListNode(9);
    ListNode* b = new ListNode(7);
    ListNode* c = new ListNode(3);
    a->next = b; b->next = c;
    ListNode* res4 = removeSmallerNodes(a);
    assert(res4->val == 9);
    assert(res4->next->val == 7);
    assert(res4->next->next->val == 3);
    assert(res4->next->next->next == nullptr);

    // Test 5: All equal values – all survive
    ListNode* x = new ListNode(5);
    ListNode* y = new ListNode(5);
    ListNode* z = new ListNode(5);
    x->next = y; y->next = z;
    ListNode* res5 = removeSmallerNodes(x);
    assert(res5->val == 5);
    assert(res5->next->val == 5);
    assert(res5->next->next->val == 5);
    assert(res5->next->next->next == nullptr);

    // Test 6: Increasing list – only last survives
    ListNode* p = new ListNode(1);
    ListNode* q = new ListNode(2);
    ListNode* r = new ListNode(3);
    p->next = q; q->next = r;
    ListNode* res6 = removeSmallerNodes(p);
    assert(res6->val == 3);
    assert(res6->next == nullptr);

    // Cleanup (for completeness, though not required for asserts)
    delete res6;
    delete res5->next->next; delete res5->next; delete res5;
    delete res4->next->next; delete res4->next; delete res4;
    delete single;
    delete result->next; delete result;
}
