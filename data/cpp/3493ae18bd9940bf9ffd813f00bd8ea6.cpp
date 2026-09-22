// Write a C++ function `ListNode* removeMiddle(ListNode* head)` that takes the head of a singly linked list and removes the middle node. If the list has an even number of nodes, remove the node at index `n/2` (0-based), where `n` is the length of the list. If the list has only one node, return the list unchanged (do not delete the only node). The function should return the head of the modified list, properly deallocating the removed node's memory. You must not use any extra data structures beyond a few pointers, and must handle empty lists (return `nullptr`) and lists of length 1 or 2 correctly. The linked list nodes are defined as `struct ListNode { int val; ListNode* next; ListNode(int x) : val(x), next(nullptr) {} };`.

The solution uses the fast-and-slow pointer technique to locate the middle node. Initialize both `fast` and `slow` to the head. Move `fast` two steps and `slow` one step per iteration, but we also need to track the node just before `slow` to unlink the middle. A cleaner approach: maintain a `prev` pointer that starts at `head` and is updated to `slow` before `slow` advances. When `fast` reaches the end (or `fast->next` is null), `slow` points to the middle node. Special cases: if the list is empty or has one node, return the head unchanged. If the list has exactly two nodes, the "middle" is the second node (index 1 = n/2=1), so we must remove the second node. The fast-slow loop must correctly handle this: with two nodes, `fast` moves from head to the second node, then `fast->next` is null, so loop stops; `slow` is at the second node. Then we unlink `prev->next` to skip `slow`, delete `slow`, and return head. Edge cases include a list of length 1 (no removal) and a list of length 0 (return null). Time complexity is O(n) with a single pass, and space is O(1) beyond the list itself.

#include <cstddef>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Removes the middle node of a singly linked list.
// Returns the head of the modified list.
// If the list has 0 or 1 nodes, returns head unchanged.
// For even length, removes node at index n/2 (0-based).
ListNode* removeMiddle(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    ListNode* fast = head;
    ListNode* slow = head;
    ListNode* prev = nullptr;

    while (fast != nullptr && fast->next != nullptr) {
        fast = fast->next->next;
        prev = slow;
        slow = slow->next;
    }

    // At this point, slow points to the middle node, prev points to node before it.
    // Special case for a list of length 2: prev is head, slow is second node.
    if (prev != nullptr) {
        prev->next = slow->next;
    } else {
        // Should not happen because length >= 2 ensures prev gets set.
        head = slow->next;
    }

    delete slow;
    return head;
}

#include <cassert>

// Helper to build a list from a vector-like initializer list
ListNode* buildList(std::initializer_list<int> vals) {
    ListNode* dummy = new ListNode(0);
    ListNode* cur = dummy;
    for (int v : vals) {
        cur->next = new ListNode(v);
        cur = cur->next;
    }
    return dummy->next;
}

// Helper to delete a list
void deleteList(ListNode* head) {
    while (head) {
        ListNode* tmp = head->next;
        delete head;
        head = tmp;
    }
}

// Helper to compare list values
bool listEqual(ListNode* head, std::initializer_list<int> vals) {
    auto it = vals.begin();
    while (head && it != vals.end()) {
        if (head->val != *it) return false;
        head = head->next;
        ++it;
    }
    return head == nullptr && it == vals.end();
}

int main() {
    // Empty list
    ListNode* empty = nullptr;
    assert(removeMiddle(empty) == nullptr);

    // Single node: unchanged
    ListNode* one = new ListNode(5);
    assert(removeMiddle(one) == one);
    assert(listEqual(one, {5}));
    deleteList(one);

    // Two nodes: remove second (index 1 = n/2)
    ListNode* two = buildList({1, 2});
    ListNode* res2 = removeMiddle(two);
    assert(listEqual(res2, {1}));
    deleteList(res2);

    // Three nodes: remove middle (index 1)
    ListNode* three = buildList({1, 2, 3});
    ListNode* res3 = removeMiddle(three);
    assert(listEqual(res3, {1, 3}));
    deleteList(res3);

    // Four nodes: remove index 2 (index n/2 = 2)
    ListNode* four = buildList({1, 2, 3, 4});
    ListNode* res4 = removeMiddle(four);
    assert(listEqual(res4, {1, 2, 4}));
    deleteList(res4);

    // Five nodes: remove index 2 (middle)
    ListNode* five = buildList({1, 2, 3, 4, 5});
    ListNode* res5 = removeMiddle(five);
    assert(listEqual(res5, {1, 2, 4, 5}));
    deleteList(res5);

    // Single node after removal is fine, already checked.

    return 0;
}
