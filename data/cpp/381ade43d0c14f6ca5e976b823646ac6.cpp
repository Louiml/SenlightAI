Given a singly linked list where each node contains an integer value, write a C++ function `removeNodes(ListNode* head)` that removes every node whose value is strictly less than the value of any node that appears to its right in the original list. The function should return the head of the modified list, preserving the relative order of the remaining nodes. The input list is non-empty and may contain duplicate values. You must not allocate new nodes; only rearrange or delete existing nodes. For example, given `5 -> 2 -> 13 -> 3 -> 8`, the result should be `13 -> 8` because `5`, `2`, and `3` each have a larger value to their right, while `13` and `8` do not (since `8` is the rightmost). If the list is already non-increasing (e.g., `5 -> 4 -> 3`), no nodes are removed.
#include <cassert>

// Helper to create a linked list from a vector-like initializer list.
ListNode* createList(std::initializer_list<int> values) {
    ListNode* dummy = new ListNode(0);
    ListNode* tail = dummy;
    for (int v : values) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy->next;
}

// Helper to compare a list against an expected vector of values.
bool compareList(ListNode* head, std::initializer_list<int> expected) {
    std::vector<int> actual;
    ListNode* cur = head;
    while (cur != nullptr) {
        actual.push_back(cur->val);
        cur = cur->next;
    }
    std::vector<int> exp(expected);
    return actual == exp;
}

int main() {
    // Case 1: Provided example.
    ListNode* l1 = createList({5, 2, 13, 3, 8});
    l1 = removeNodes(l1);
    assert(compareList(l1, {13, 8}));

    // Case 2: Already non-increasing list, no removals.
    ListNode* l2 = createList({5, 4, 3});
    l2 = removeNodes(l2);
    assert(compareList(l2, {5, 4, 3}));

    // Case 3: Single node.
    ListNode* l3 = createList({7});
    l3 = removeNodes(l3);
    assert(compareList(l3, {7}));

    // Case 4: Duplicates, only strictly larger right removes.
    ListNode* l4 = createList({1, 1, 2});
    l4 = removeNodes(l4);
    assert(compareList(l4, {1, 2}));

    // Case 5: Strictly increasing list, only the last node remains.
    ListNode* l5 = createList({1, 2, 3, 4});
    l5 = removeNodes(l5);
    assert(compareList(l5, {4}));

    // Case 6: Decreasing then increasing.
    ListNode* l6 = createList({10, 9, 8, 11});
    l6 = removeNodes(l6);
    assert(compareList(l6, {11}));

    // Case 7: Empty list (not required but safe).
    ListNode* l7 = nullptr;
    l7 = removeNodes(l7);
    assert(l7 == nullptr);

    // Case 8: Negative and positive mix.
    ListNode* l8 = createList({-3, -1, -2, 0});
    l8 = removeNodes(l8);
    assert(compareList(l8, {0}));

    return 0;
}
#include <deque>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Removes every node that has a strictly larger value to its right.
// Returns the head of the modified list. The input list is modified in-place.
ListNode* removeNodes(ListNode* head) {
    if (head == nullptr) return nullptr;

    std::deque<ListNode*> candidates;
    ListNode* current = head;

    // Traverse the list and maintain a non-increasing deque of candidate nodes.
    while (current != nullptr) {
        // Remove all nodes from the back that have value strictly less than current's value.
        while (!candidates.empty() && current->val > candidates.back()->val) {
            candidates.pop_back();
        }
        candidates.push_back(current);
        current = current->next;
    }

    // Rebuild the list from the surviving nodes.
    ListNode* newHead = nullptr;
    ListNode* tail = nullptr;
    while (!candidates.empty()) {
        ListNode* node = candidates.front();
        candidates.pop_front();
        if (newHead == nullptr) {
            newHead = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    if (tail != nullptr) tail->next = nullptr;
    return newHead;
}
// The problem requires identifying and removing every node that has at least one larger value to its right. A direct approach scanning each node and checking all nodes to its right would be O(n²) and inefficient. Instead, observe that a node should be kept only if it is greater than or equal to the maximum value among all nodes to its right. One clean way is to traverse the list from left to right while maintaining a monotonic stack (specifically a non-increasing deque of pointers). Algorithm: initialize an empty deque. For each node in the original list, while the deque is not empty and the current node's value is strictly greater than the value at the back of the deque, pop the back (those nodes are dominated by the current node). Then push the current node onto the deque. After processing all nodes, the deque contains exactly the nodes that should remain, in original order, because any node that was popped was found to have a larger value to its right (the current node that caused the pop). Then rebuild the linked list by linking the nodes in the deque sequentially and setting the last node's `next` to `nullptr`. Edge cases: empty list? Problem says non-empty, but function can handle empty by returning `nullptr`. Duplicate values: a node is removed only if there is a strictly larger value to its right, so equal values are kept. For example, `1 -> 1 -> 2` gives `1 -> 2` (the first 1 is removed, the second 1 is kept). Time complexity is O(n) because each node is pushed and popped at most once. Space complexity is O(n) in the worst case for the deque (e.g., when the list is strictly decreasing, all nodes are kept). The solution avoids recursion to prevent stack overflow on long lists.
